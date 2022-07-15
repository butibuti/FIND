#include"stdafx.h"
#ifdef _EDITORBUILD
#ifndef _IMGUIEDITOR

#include"Header/ApplicationCreater.h"
#include"RuntimeServer.h"
#include"ButiEventSystem/ButiEventSystem/TaskSystem.h"
#include <grpc++\grpc++.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include "ProtBuf/EngineEditorCommunicate.grpc.pb.h"

namespace ButiEngine {
Vector3	ToVector3(const Vector3_message& arg_message) {
	return Vector3(arg_message.x(), arg_message.y(), arg_message.z());
}
Value_ptr<Transform> ToTransform(const Transform_message& arg_message) {
	return ObjectFactory::Create<Transform>(ToVector3(arg_message.position()), ToVector3(arg_message.rotation()), ToVector3(arg_message.scaling()));
}
std::vector<std::string> ToStringVector(const google::protobuf::internal::RepeatedStringTypeTraits::RepeatedFieldType& arg_strings) {
	std::vector<std::string> output;
	output.reserve(arg_strings.size());
	for (std::int32_t i = 0; i < arg_strings.size(); i++) {
		output.push_back(arg_strings.Get(i));
	}
	return output;
}
template<typename T>
std::vector<T> ToTagVector(const google::protobuf::internal::RepeatedStringTypeTraits::RepeatedFieldType& arg_strings) {
	std::vector<T> output;
	output.reserve(arg_strings.size());
	for (std::int32_t i = 0; i < arg_strings.size(); i++) {
		output.push_back(T( arg_strings.Get(i)));
	}
	return output;
}
const std::int32_t streamProgress = 0x200000;
class Service final : public EngineCommunicate::Service {
public:
	Service() {
	}
	std::int32_t Run() {
#ifndef _IMGUIEDITOR
		{
			std::unique_lock lock(mtx_app);
			cv_app.wait(lock, [this] {return vlp_app; });
		}
#else
		ApplicationInit();
#endif // !_IMGUIEDITOR

		std::int32_t retCode = -1;
		{
			std::lock_guard lock(mtx_app);
			if (vlp_app) {
				retCode = vlp_app->Run();
				vlp_app->Exit();
				vlp_app = nullptr;
			}
		}

		return retCode;
	}
	void ApplicationInit() {
		std::lock_guard lock(mtx_app);
		auto init = InputApplicationInitData();
		vlp_app = CreateApplicationInstance(init);

		GameDevice::Initialize();
		GameDevice::GetInput()->Initialize(vlp_app);
		GameDevice::SetReturnCode(0);
		GameDevice::SetIsEnd(false);
		vlp_app->PreLoadResources();
		vlp_app->InitLoadResources();
		vlp_app->GetSceneManager()->LoadScene_Init(init.initSceneName);
		vlp_app->GetGraphicDevice()->SetClearColor(Vector4(0, 0, 0, 1));
	}
	grpc::Status SetAppInitData(grpc::ServerContext* context, const AppInitData* request, Integer* reply) override {
		return grpc::Status::OK;
	}
	grpc::Status SceneActive(grpc::ServerContext* context, const Boolean* request, Boolean* reply)override {
		auto ret = vlp_app->GetSceneManager()->SetActive(request->value());
		reply->set_value(ret);
		return grpc::Status::OK;
	}
	grpc::Status ApplicationReload(grpc::ServerContext* context, const Integer* request, Integer* reply)override {

		reply->set_value(0);
		return grpc::Status::OK;
	}
	grpc::Status ApplicationStartUp(grpc::ServerContext* context, const Integer* request, Integer* reply)override {
		if (vlp_app) {
			return grpc::Status::OK;
		}
#ifndef _IMGUIEDITOR

		ApplicationInit();
		cv_app.notify_all();
#endif // !_IMGUIEDITOR

		reply->set_value(0);
		return grpc::Status::OK;
	}
	grpc::Status ApplicationShutDown(grpc::ServerContext* context, const Integer* request, Integer* reply)override {
		if (!vlp_app) {
			return grpc::Status::OK;
		}
		GameDevice::SetIsEnd(true);
		GameDevice::SetReturnCode(request->value());
		reply->set_value(0);
		return grpc::Status::OK;
	}
	grpc::Status SceneReload(grpc::ServerContext* context, const Integer* request, Integer* reply)override {
		vlp_app->GetSceneManager()->ReloadButtonPerform();
		reply->set_value(0);
		return grpc::Status::OK;
	}
	grpc::Status SceneSave(grpc::ServerContext* context, const Integer* request, Integer* reply)override {
		vlp_app->GetSceneManager()->SaveButtonPerform();
		reply->set_value(0);
		return grpc::Status::OK;
	}
	grpc::Status SceneChange(grpc::ServerContext* context, const String* request, Integer* reply)override {
		vlp_app->GetSceneManager()->SceneChangeButtonPerform(request->value());
		reply->set_value(0);
		return grpc::Status::OK;
	}
	grpc::Status GetFPS(grpc::ServerContext* context, const Integer* request, FPSData* reply)override {
		reply->set_average(vlp_app->GetFrameInformation().averageFPS);
		reply->set_current(vlp_app->GetFrameInformation().currentFPS);
		reply->set_drawmillsec(vlp_app->GetFrameInformation().drawMillSec);
		reply->set_updatemillsec( vlp_app->GetFrameInformation().updateMillSec);
		return grpc::Status::OK;
	}
	grpc::Status SetRenderTargetView(grpc::ServerContext* context, const RenderTargetViewed* request, Boolean* reply)override {
		auto serchTag = TextureTag(request->name());
		if (!vlp_app->GetResourceContainer()->Exist(serchTag)) {
			return grpc::Status::OK;
		}
		auto outputTexture = vlp_app->GetResourceContainer()->GetTexture(serchTag).lock();
		outputTexture->SetEditorViewed(request->isviewed());
		reply->set_value(outputTexture->GetEditorViewed());
		return grpc::Status::OK;
	}
	grpc::Status GetDefaultRenderTargetImageName(grpc::ServerContext* context, const Integer* request, String* response)override {
		response->set_value(":/_editorScreen/" + std::to_string(vlp_app->GetAppInitData()->windowWidth / 2) + "/" + std::to_string(vlp_app->GetAppInitData()->windowHeight / 2));
		return grpc::Status::OK;
	}

	grpc::Status GetRenderTargetImage(grpc::ServerContext* context, const String* request,grpc::ServerWriter< FileData>* writer)override {
		auto serchTag = TextureTag(request->value());
		if (!vlp_app->GetResourceContainer()->Exist(serchTag)) {
			return grpc::Status::OK;
		}
		auto outputTexture = vlp_app->GetResourceContainer()->GetTexture(serchTag).lock();
		FileData temp;
		temp.set_eof(false);
		auto data = outputTexture->GetRawData();
		if (!data) {
			temp.set_eof(true);
			writer->Write(temp);
			return grpc::Status::OK;
		}
		for (std::int32_t i = 0, texSize = outputTexture->GetDataSize(); i < texSize; i +=streamProgress) {
			if (i + streamProgress > texSize) {
				temp.set_data(&data[i], texSize-i);
				temp.set_eof(true);
			}
			else {
				temp.set_data(&data[i], streamProgress);
			}
			writer->Write(temp);
		}
		return grpc::Status::OK;
	}

	grpc::Status GetRenderTargetImageStream(grpc::ServerContext* context, const ButiEngine::String* request, grpc::ServerWriter< ButiEngine::FileData>* writer)override {
		auto serchTag = TextureTag(request->value());
		if (!vlp_app->GetResourceContainer()->Exist(serchTag)) {
			return grpc::Status::OK;
		}
		auto outputTexture = vlp_app->GetResourceContainer()->GetTexture(serchTag).lock();
		FileData temp;

		while (!GameDevice::GetIsEnd()) {
			temp.set_eof(false);
			outputTexture->Wait();
			auto data = outputTexture->GetRawData();
			if (!outputTexture->GetEditorViewed()) {
				temp.set_eof(true);
				temp.set_streamend(true);
				writer->Write(temp);
				
				break;
			}
			if (!data) {
				continue;
			}
			for (std::int32_t i = 0, texSize = outputTexture->GetDataSize(); i < texSize; i += streamProgress) {
				if (i + streamProgress > texSize) {
					temp.set_data(&data[i], texSize - i);
					temp.set_eof(true);
				}
				else {
					temp.set_data(&data[i], streamProgress);
				}
				//Sleep(1000);
				writer->Write(temp);
				temp.clear_data();
			}
		}

		return grpc::Status::OK;
	}
	grpc::Status GetRenderTargetInformation(grpc::ServerContext* context, const String* request, RenderTargetInformation* response) override{
		auto serchTag = TextureTag(request->value());
		if (!vlp_app->GetResourceContainer()->Exist(serchTag)) {
			return grpc::Status::OK;
		}
		auto outputTexture = vlp_app->GetResourceContainer()->GetTexture(serchTag).lock();
		response->set_format(outputTexture->GetFormat());
		response->set_size(outputTexture->GetDataSize());
		response->set_stride(outputTexture->GetStride());
		response->set_width(outputTexture->GetSize().x);
		response->set_height(outputTexture->GetSize().y);
		return grpc::Status::OK;
	}
	grpc::Status StreamOutputMessage(grpc::ServerContext* context, const Integer* request, grpc::ServerWriter< OutputMessage>* writer)override {

		OutputMessage message;
		while (!GameDevice::GetIsEnd()) {
			GUI::WaitMessage();
			auto& messages = GUI::GetGUIMessage();
			for (auto itr =messages .begin(), end = messages.end(); itr != end;itr++) {
				if (itr->type == GUI::MessageType::Message_End) {
					message.set_messagetype((OutputMessage_MessageType)itr->type);
					writer->Write(message);
					return grpc::Status::OK;
				}
				message.set_content(itr->content);
				message.set_r(itr->color.x);
				message.set_g(itr->color.y);
				message.set_b(itr->color.z);
				message.set_a(itr->color.w);
				message.set_messagetype((OutputMessage_MessageType) itr->type);
				writer->Write(message);
			}
		}

		return grpc::Status::OK;
	}
	grpc::Status StreamOutputStop(grpc::ServerContext* context, const Integer* request, Integer* response)override {
		GUI::MessageStreamStop();
		return grpc::Status::OK;
	}
	grpc::Status SetWindowActive(grpc::ServerContext* context, const  Boolean* request, Boolean* response)override {
		auto ret=vlp_app->GetWindow()->SetCurrentSelect(request->value());
		response->set_value(ret);
		return grpc::Status::OK;
	}
	google::protobuf::Arena diffuseArena, emissiveArena, ambientArena, specularArena;
	grpc::Status SetEditorWindowHandle(grpc::ServerContext* context, const LongInteger* request, Integer* response)override {
		GlobalSettings::SetEditorWindowHandle(request->value());
		return grpc::Status::OK;
	}
	grpc::Status CreateGameObject(grpc::ServerContext* context, const GameObjectCreate_message* request, String* response)override {
		auto addGameObject= vlp_app->GetSceneManager()->GetCurrentScene()->GetGameObjectManager()->AddObjectFromCereal(request->gameobjectname(), ToTransform(request->transform()));
		response->set_value(addGameObject.lock()->GetGameObjectName());
		return grpc::Status::OK;
	}
	grpc::Status SetBaseTransform(::grpc::ServerContext* context, const ::ButiEngine::String* request, ::ButiEngine::Integer* response)override {
		auto manager = vlp_app->GetSceneManager()->GetCurrentScene()->GetGameObjectManager();
		auto parent = manager->GetGameObject(request->value());
		auto current = manager->GetSelectedUI();
		if (parent.lock()&&current.lock()) {
			current.lock()->transform->SetBaseTransform(parent.lock()->transform, true);
		}
		return grpc::Status::OK;
	}
	grpc::Status GUIEdit(::grpc::ServerContext* context, const ::ButiEngine::GUIAction* request, ::ButiEngine::Integer* response)override {
		GUI::IGUIAction* act =nullptr;
		switch (request->inputype())
		{
		case GUIType::GUIType_Integer:
			act = new GUI::GUIAction(request->id(), request->value_int());
			break;
		case GUIType::GUIType_Float:
			act = new GUI::GUIAction(request->id(), request->value_float().x());
			break;
		case GUIType::GUIType_Vector2:
			act = new GUI::GUIAction(request->id(), Vector2(request->value_float().x(), request->value_float().y()));
			break;
		case GUIType::GUIType_Vector3:
			act = new GUI::GUIAction(request->id(), Vector3(request->value_float().x(), request->value_float().y(), request->value_float().z()));
			break;
		case GUIType::GUIType_Vector4:
			act = new GUI::GUIAction(request->id(), Vector4(request->value_float().x(), request->value_float().y(), request->value_float().z(), request->value_float().w()));
			break;
		case GUIType::GUIType_String:
			act = new GUI::GUIAction(request->id(), request->value_string());
			break;
		default:
			break;
		}
		GUI::OccurGUIAction(act);
		return grpc::Status::OK;
	}
	grpc::Status SelectGameObject(::grpc::ServerContext* context, const ::ButiEngine::String* request, ::ButiEngine::GameObjectGUIInformation* response)override {
		auto manager = vlp_app->GetSceneManager()->GetCurrentScene()->GetGameObjectManager();
		auto obj = manager->GetGameObject(request->value());
		if (obj.lock()) {
			manager->SetSelectedGameObject(obj);
		}
		return grpc::Status::OK;
	}
	grpc::Status GetGameObjectList(grpc::ServerContext* context, const GameObjectListRequest* request, StringArray* response) {
		vlp_app->GetSceneManager()->GetCurrentScene()->GetGameObjectManager()->CreateViewGameObjectVector(request->serchstr(), ToTagVector<GameObjectTag>(request->serchtags()));
		auto list = vlp_app->GetSceneManager()->GetCurrentScene()->GetGameObjectManager()->GetGameObjectList();
		for (auto itr = list.begin(), end = list.end(); itr != end; itr++) {
			response->add_values(*itr);
		}
		return grpc::Status::OK;
	}
	grpc::Status RenameGameObject(grpc::ServerContext* context, const Rename_message* request, String* response)override {
		vlp_app->GetSceneManager()->GetCurrentScene()->GetGameObjectManager()->GetGameObject(request->before());
		return grpc::Status::OK;
	}
	grpc::Status DeleteGameObject(grpc::ServerContext* context, const String* request, Integer* response)override {
		vlp_app->GetSceneManager()->GetCurrentScene()->GetGameObjectManager()->GetGameObject(request->value()).lock()->SetIsRemove(true);
		return grpc::Status::OK;
	}
	
	grpc::Status GetLoadedResources(grpc::ServerContext* context, const Integer* request, ResourceLoadInformation* response)override {
		auto names = vlp_app->GetResourceContainer()->GetLoadedTextureNames();
		for (auto itr = names.begin(), end = names.end(); itr != end; itr++) {
			response->add_textures(*itr);
		}
		names = vlp_app->GetResourceContainer()->GetLoadedMotionNames();
		for (auto itr = names.begin(), end = names.end(); itr != end; itr++) {
			response->add_motions(*itr);
		}
		names = vlp_app->GetResourceContainer()->GetLoadedModelNames();
		for (auto itr = names.begin(), end = names.end(); itr != end; itr++) {
			response->add_models(*itr);
		}
		names = vlp_app->GetResourceContainer()->GetLoadedSoundNames();
		for (auto itr = names.begin(), end = names.end(); itr != end; itr++) {
			response->add_sounds(*itr);
		}
		names = vlp_app->GetResourceContainer()->GetLoadedMeshNames();
		for (auto itr = names.begin(), end = names.end(); itr != end; itr++) {
			response->add_meshes(*itr);
		}
		names = vlp_app->GetResourceContainer()->GetLoadedFontNames();
		for (auto itr = names.begin(), end = names.end(); itr != end; itr++) {
			response->add_fonts(*itr);
		}
		names = vlp_app->GetResourceContainer()->GetLoadedScriptNames();
		for (auto itr = names.begin(), end = names.end(); itr != end; itr++) {
			response->add_scripts(*itr);
		}
		names = vlp_app->GetResourceContainer()->GetLoadedVertexShaderNames();
		for (auto itr = names.begin(), end = names.end(); itr != end; itr++) {
			response->add_vertexshaders(*itr);
		}
		names = vlp_app->GetResourceContainer()->GetLoadedPixelShaderNames();
		for (auto itr = names.begin(), end = names.end(); itr != end; itr++) {
			response->add_pixelshaders(*itr);
		}
		names = vlp_app->GetResourceContainer()->GetLoadedGeometryShaderNames();
		for (auto itr = names.begin(), end = names.end(); itr != end; itr++) {
			response->add_geometryshaders(*itr);
		}

		auto& shaderLoadedInfos = vlp_app->GetResourceContainer()->GetLoadedShaderInfos();
		for (auto itr = shaderLoadedInfos.begin(), end = shaderLoadedInfos.end(); itr != end; itr++) {
			auto additionalInfo = response->add_shaderloadinfo();
			additionalInfo->set_shadername(itr->shaderName);
			additionalInfo->set_vertexshadername(itr->vertexShaderTag.GetID());
			additionalInfo->set_pixelshadername(itr->pixelShaderTag.GetID());
			additionalInfo->set_geometryshadername( itr->geometryShaderTag.GetID());			
		}

		auto& materialLoadedInfos = vlp_app->GetResourceContainer()->GetLoadedMaterialInfos();
		for (auto itr = materialLoadedInfos.begin(), end = materialLoadedInfos.end(); itr != end; itr++) {

			auto additionalInfo = response->add_materialloadinfo();

			additionalInfo->set_materialname(itr->materialName);
			additionalInfo->set_filepath(itr->fileName);
			
			Vector4_message* diffuseTemp =google::protobuf::Arena::CreateMessage<Vector4_message>(nullptr),
			* ambientTemp = google::protobuf::Arena::CreateMessage<Vector4_message>(nullptr),
			* emissiveTemp = google::protobuf::Arena::CreateMessage<Vector4_message>(nullptr),
			* specularTemp = google::protobuf::Arena::CreateMessage<Vector4_message>(nullptr);
			ambientTemp->set_x(itr->var.ambient.x); ambientTemp->set_y(itr->var.ambient.y); ambientTemp->set_z(itr->var.ambient.z); ambientTemp->set_w(itr->var.ambient.w);
			emissiveTemp->set_x(itr->var.emissive.x); emissiveTemp->set_y(itr->var.emissive.y); emissiveTemp->set_z(itr->var.emissive.z); emissiveTemp->set_w(itr->var.emissive.w);
			diffuseTemp->set_x(itr->var.diffuse.x); diffuseTemp->set_y(itr->var.diffuse.y); diffuseTemp->set_z(itr->var.diffuse.z); diffuseTemp->set_w(itr->var.diffuse.w);
			specularTemp->set_x(itr->var.specular.x); specularTemp->set_y(itr->var.specular.y); specularTemp->set_z(itr->var.specular.z); specularTemp->set_w(itr->var.specular.w);
			additionalInfo->set_allocated_ambient(ambientTemp);
			additionalInfo->set_allocated_emissive(emissiveTemp);
			additionalInfo->set_allocated_diffuse(diffuseTemp);
			additionalInfo->set_allocated_specular(specularTemp);
			for (auto texItr = itr->vec_texture.begin(), texEnd = itr->vec_texture.end(); texItr != texEnd; texItr++) {
				additionalInfo->add_textures(texItr->GetID());
			}
		}

		return grpc::Status::OK;
	}
	grpc::Status LoadedResourcesToFile(grpc::ServerContext* context, const ResourceLoadInformation* request, Integer* response)override{
		AssetLoadData outputData;

		std::int32_t size = request->textures_size();
		outputData.vec_filePathAndDirectory_tex.reserve(size);
		for (std::int32_t i = 0; i < size; i++) {
			outputData.vec_filePathAndDirectory_tex.push_back(request->textures().Get(i));
		}
		size = request->models_size();
		outputData.vec_filePathAndDirectory_model.reserve(size);
		for (std::int32_t i = 0; i < size; i++) {
			outputData.vec_filePathAndDirectory_model.push_back(request->models().Get(i));
		}
		size = request->motions_size();
		outputData.vec_filePathAndDirectory_motion.reserve(size);
		for (std::int32_t i = 0; i < size; i++) {
			outputData.vec_filePathAndDirectory_motion.push_back(request->motions().Get(i));
		}
		size = request->fonts_size();
		outputData.vec_filePathAndDirectory_font.reserve(size);
		for (std::int32_t i = 0; i < size; i++) {
			outputData.vec_filePathAndDirectory_font.push_back(request->fonts().Get(i));
		}
		size = request->sounds_size();
		outputData.vec_filePathAndDirectory_sound.reserve(size);
		for (std::int32_t i = 0; i < size; i++) {
			outputData.vec_filePathAndDirectory_sound.push_back(request->sounds().Get(i));
		}
		size = request->scripts_size();
		outputData.vec_filePathAndDirectory_script.reserve(size);
		for (std::int32_t i = 0; i < size; i++) {
			outputData.vec_filePathAndDirectory_script.push_back(request->scripts().Get(i));
		}
		size = request->vertexshaders_size();
		outputData.vec_filePathAndDirectory_vs.reserve(size);
		for (std::int32_t i = 0; i < size; i++) {
			outputData.vec_filePathAndDirectory_vs.push_back(request->vertexshaders().Get(i));
		}
		size = request->pixelshaders_size();
		outputData.vec_filePathAndDirectory_ps.reserve(size);
		for (std::int32_t i = 0; i < size; i++) {
			outputData.vec_filePathAndDirectory_ps.push_back(request->pixelshaders().Get(i));
		}
		size = request->geometryshaders_size();
		outputData.vec_filePathAndDirectory_gs.reserve(size);
		for (std::int32_t i = 0; i < size; i++) {
			outputData.vec_filePathAndDirectory_gs.push_back(request->geometryshaders().Get(i));
		}

		size = request->materialloadinfo_size();
		outputData.vec_materialLoadInfos.reserve(size);
		for (std::int32_t i = 0; i < size; i++) {
			MaterialLoadInfo additional;
			auto& info = request->materialloadinfo().Get(i);
			additional.materialName = info.materialname();
			additional.fileName= info.filepath();
			additional.var.diffuse = Vector4(info.diffuse().x(), info.diffuse().y(), info.diffuse().z(), info.diffuse().w());
			additional.var.ambient = Vector4(info.ambient().x(), info.ambient().y(), info.ambient().z(), info.ambient().w());
			additional.var.emissive= Vector4(info.emissive().x(), info.emissive().y(), info.emissive().z(), info.emissive().w());
			additional.var.specular= Vector4(info.specular().x(), info.specular().y(), info.specular().z(), info.specular().w());
			std::int32_t texSize = info.textures_size();
			for (std::int32_t j = 0; j < texSize; j++) {
				additional.vec_texture.push_back(TextureTag( info.textures().Get(j)));
			}
			outputData.vec_materialLoadInfos.push_back(additional);
		}

		size = request->shaderloadinfo_size();
		outputData.vec_shaderNames.reserve(size);
		for (std::int32_t i = 0; i < size; i++) {
			ShaderAssetData additional;
			auto& info = request->shaderloadinfo().Get(i);
			additional.shaderName = info.shadername();
			additional.vertexShaderTag = VertexShaderTag(info.vertexshadername());
			additional.pixelShaderTag= PixelShaderTag(info.pixelshadername());
			additional.geometryShaderTag= GeometryShaderTag(info.geometryshadername());
			outputData.vec_shaderNames.push_back(additional);
		}
		OutputCereal(outputData, request->filepath());

		return grpc::Status::OK;
	}

	grpc::Status GetLoadedMeshes(::grpc::ServerContext* context, const ::ButiEngine::Integer* request, ::ButiEngine::StringArray* response)override {
		auto names=  vlp_app->GetResourceContainer()->GetMeshTags();
		for (auto itr = names.begin(), end = names.end(); itr != end; itr++) {
			response->add_values(itr->GetID());
		}
		return grpc::Status::OK;
	}
	grpc::Status GetLoadedTextures(::grpc::ServerContext* context, const ::ButiEngine::Integer* request, ::ButiEngine::StringArray* response)override {
		auto names = vlp_app->GetResourceContainer()->GetTextureTags();
		for (auto itr = names.begin(), end = names.end(); itr != end; itr++) {
			response->add_values(itr->GetID());
		}
		return grpc::Status::OK;
	}
	grpc::Status GetLoadedMaterials(::grpc::ServerContext* context, const ::ButiEngine::Integer* request, ::ButiEngine::MaterialLoadInfoArray* response)override {
		auto datas = vlp_app->GetResourceContainer()->GetLoadedMaterialInfos();
		for (auto itr = datas.begin(), end = datas.end(); itr != end; itr++) {
			auto additional=  response->add_values();
			additional->set_materialname(itr->materialName);
			additional->set_filepath(itr->fileName); Vector4_message* diffuseTemp = google::protobuf::Arena::CreateMessage<Vector4_message>(nullptr),
				* ambientTemp = google::protobuf::Arena::CreateMessage<Vector4_message>(nullptr),
				* emissiveTemp = google::protobuf::Arena::CreateMessage<Vector4_message>(nullptr),
				* specularTemp = google::protobuf::Arena::CreateMessage<Vector4_message>(nullptr);
			ambientTemp->set_x(itr->var.ambient.x); ambientTemp->set_y(itr->var.ambient.y); ambientTemp->set_z(itr->var.ambient.z); ambientTemp->set_w(itr->var.ambient.w);
			emissiveTemp->set_x(itr->var.emissive.x); emissiveTemp->set_y(itr->var.emissive.y); emissiveTemp->set_z(itr->var.emissive.z); emissiveTemp->set_w(itr->var.emissive.w);
			diffuseTemp->set_x(itr->var.diffuse.x); diffuseTemp->set_y(itr->var.diffuse.y); diffuseTemp->set_z(itr->var.diffuse.z); diffuseTemp->set_w(itr->var.diffuse.w);
			specularTemp->set_x(itr->var.specular.x); specularTemp->set_y(itr->var.specular.y); specularTemp->set_z(itr->var.specular.z); specularTemp->set_w(itr->var.specular.w);
			additional->set_allocated_ambient(ambientTemp);
			additional->set_allocated_diffuse(diffuseTemp);
			additional->set_allocated_emissive(emissiveTemp);
			additional->set_allocated_specular(specularTemp);

			for (auto texItr = itr->vec_texture.begin(), texEnd = itr->vec_texture.end(); texItr != texEnd; texItr++) {
				additional->add_textures(texItr->GetID());
			}

		}
		return grpc::Status::OK;
	}

	grpc::Status LoadTexture(grpc::ServerContext* context, const StringArray* request, Integer* response)override{
		std::vector<std::string> vec_path;
		vec_path.reserve( request->values_size());
		for (std::int32_t i = 0; i < request->values_size();i++) {
			vec_path.push_back(request->values().Get(i));
		}
		vlp_app->GetResourceContainer()->LoadTexture(vec_path);
		return grpc::Status::OK;
	}
	grpc::Status LoadModel(grpc::ServerContext* context, const StringArray* request, Integer* response)override
	{
		std::vector<std::string> vec_path;
		vec_path.reserve(request->values_size());
		for (std::int32_t i = 0; i < request->values_size(); i++) {
			vec_path.push_back(request->values().Get(i));
		}
		vlp_app->GetResourceContainer()->LoadModel(vec_path);
		return grpc::Status::OK;
	}
	grpc::Status LoadMaterial(grpc::ServerContext* context, const MaterialLoadInfoArray* request, Integer* response)override {
		std::vector<MaterialLoadInfo> vec_info;
		std::int32_t size = request->values_size();
		vec_info.reserve(size);
		for (std::int32_t i = 0; i < size; i++) {
			auto& reqInfo = request->values().Get(i);
			MaterialLoadInfo info;
			info.materialName= reqInfo.materialname();
			info.fileName = reqInfo.filepath();
			info.var.diffuse = Vector4(reqInfo.diffuse().x(), reqInfo.diffuse().y(), reqInfo.diffuse().z(), reqInfo.diffuse().w());
			info.var.ambient = Vector4(reqInfo.ambient().x(), reqInfo.ambient().y(), reqInfo.ambient().z(), reqInfo.ambient().w());
			info.var.emissive = Vector4(reqInfo.emissive().x(), reqInfo.emissive().y(), reqInfo.emissive().z(), reqInfo.emissive().w());
			info.var.specular = Vector4(reqInfo.specular().x(), reqInfo.specular().y(), reqInfo.specular().z(), reqInfo.specular().w());

			for (std::int32_t j = 0, textureSize = reqInfo.textures_size(); j < textureSize; j++) {
				info.vec_texture.push_back(TextureTag(reqInfo.textures().Get(j)));
			}

			vec_info.push_back(info);
		}
		vlp_app->GetResourceContainer()->LoadMaterial(vec_info);
		return grpc::Status::OK;
	}
	grpc::Status LoadShader(grpc::ServerContext* context, const ShaderLoadInfoArray* request, Integer* response)override {
		std::vector<ShaderAssetData> vec_info;
		std::int32_t size = request->values_size();
		vec_info.reserve(size);
		for (std::int32_t i = 0; i < size; i++) {
			auto& reqInfo = request->values().Get(i);
			ShaderAssetData info;
			info.shaderName = reqInfo.shadername();
			info.vertexShaderTag = VertexShaderTag(reqInfo.vertexshadername());
			info.pixelShaderTag = PixelShaderTag(reqInfo.pixelshadername());
			if (reqInfo.geometryshadername().size()) {
				info.geometryShaderTag= GeometryShaderTag(reqInfo.geometryshadername());
			}
			vec_info.push_back(info);
		}
		vlp_app->GetResourceContainer()->LoadShader(vec_info);
		return grpc::Status::OK;
	}
	grpc::Status LoadVertexShader(grpc::ServerContext* context, const StringArray* request, Integer* response)override {
		std::vector<std::string> vec_path;
		std::int32_t size = request->values_size();
		vec_path.reserve(size);
		for (std::int32_t i = 0; i < size; i++) {
			vec_path.push_back(request->values().Get(i));
		}
		vlp_app->GetResourceContainer()->LoadVertexShader(vec_path);
		return grpc::Status::OK;
	}
	grpc::Status LoadPixelShader(grpc::ServerContext* context, const StringArray* request, Integer* response)override {
		std::vector<std::string> vec_path;
		std::int32_t size = request->values_size();
		vec_path.reserve(size);
		for (std::int32_t i = 0; i < size; i++) {
			vec_path.push_back(request->values().Get(i));
		}
		vlp_app->GetResourceContainer()->LoadPixelShader(vec_path);
		return grpc::Status::OK;
	}
	grpc::Status LoadGeometryShader(grpc::ServerContext* context, const StringArray* request, Integer* response)override {
		std::vector<std::string> vec_path;
		std::int32_t size = request->values_size();
		vec_path.reserve(size);
		for (std::int32_t i = 0; i < size; i++) {
			vec_path.push_back(request->values().Get(i));
		}
		vlp_app->GetResourceContainer()->LoadGeometryShader(vec_path);
		return grpc::Status::OK;
	}
	grpc::Status LoadSound(grpc::ServerContext* context, const StringArray* request, Integer* response)override {
		std::vector<std::string> vec_path;
		std::int32_t size = request->values_size();
		vec_path.reserve(size);
		for (std::int32_t i = 0; i < size; i++) {
			vec_path.push_back(request->values().Get(i));
		}
		vlp_app->GetResourceContainer()->LoadSound(vec_path);
		return grpc::Status::OK;
	}
	grpc::Status LoadFont(grpc::ServerContext* context, const StringArray* request, Integer* response)override {
		std::vector<std::string> vec_path;
		std::int32_t size = request->values_size();
		vec_path.reserve(size);
		for (std::int32_t i = 0; i < size; i++) {
			vec_path.push_back(request->values().Get(i));
		}
		vlp_app->GetResourceContainer()->LoadFont(vec_path);
		return grpc::Status::OK;
	}
	grpc::Status LoadMotion(grpc::ServerContext* context, const StringArray* request, Integer* response)override {
		std::vector<std::string> vec_path;
		std::int32_t size = request->values_size();
		vec_path.reserve(size);
		for (std::int32_t i = 0; i < size; i++) {
			vec_path.push_back(request->values().Get(i));
		}
		vlp_app->GetResourceContainer()->LoadMotion(vec_path);
		return grpc::Status::OK;
	}
	grpc::Status LoadScript(grpc::ServerContext* context, const StringArray* request, Integer* response)override {
		std::vector<std::string> vec_path;
		std::int32_t size = request->values_size();
		vec_path.reserve(size);
		for (std::int32_t i = 0; i < size; i++) {
			vec_path.push_back(request->values().Get(i));
		}
		vlp_app->GetResourceContainer()->LoadScript(vec_path);
		return grpc::Status::OK;
	}
	grpc::Status UnLoadTexture(grpc::ServerContext* context, const StringArray* request, Integer* response)override {
		std::int32_t size = request->values_size();
		for (std::int32_t i = 0; i < size; i++) {
			vlp_app->GetResourceContainer()->UnLoad (TextureTag( request->values().Get(i)));
		}
		return grpc::Status::OK;
	}
	grpc::Status UnLoadModel(grpc::ServerContext* context, const StringArray* request, Integer* response)override {
		std::int32_t size = request->values_size();
		for (std::int32_t i = 0; i < size; i++) {
			vlp_app->GetResourceContainer()->UnLoad(ModelTag(request->values().Get(i)));
		}
		return grpc::Status::OK;
	}
	grpc::Status UnLoadMaterial(grpc::ServerContext* context, const StringArray* request, Integer* response)override {
		std::int32_t size = request->values_size();
		for (std::int32_t i = 0; i < size; i++) {
			vlp_app->GetResourceContainer()->UnLoad(MaterialTag (request->values().Get(i)));
		}
		return grpc::Status::OK;
	}
	grpc::Status UnLoadSound(grpc::ServerContext* context, const StringArray* request, Integer* response)override {
		std::int32_t size = request->values_size();
		for (std::int32_t i = 0; i < size; i++) {
			vlp_app->GetResourceContainer()->UnLoad(SoundTag(request->values().Get(i)));
		}
		return grpc::Status::OK;
	}
	grpc::Status UnLoadFont(grpc::ServerContext* context, const StringArray* request, Integer* response)override {
		std::int32_t size = request->values_size();
		for (std::int32_t i = 0; i < size; i++) {
			vlp_app->GetResourceContainer()->UnLoad(FontTag(request->values().Get(i)));
		}
		return grpc::Status::OK;
	}
	grpc::Status UnLoadMotion(grpc::ServerContext* context, const StringArray* request, Integer* response)override{
		std::int32_t size = request->values_size();
		for (std::int32_t i = 0; i < size; i++) {
			vlp_app->GetResourceContainer()->UnLoad (MotionTag( request->values().Get(i)));
		}
		return grpc::Status::OK;
	}
	grpc::Status UnLoadScript(grpc::ServerContext* context, const StringArray* request, Integer* response)override{
		std::int32_t size = request->values_size();
		for (std::int32_t i = 0; i < size; i++) {
			vlp_app->GetResourceContainer()->UnLoad (ScriptTag( request->values().Get(i)));
		}
		return grpc::Status::OK;
	}
	grpc::Status UnLoadShader(grpc::ServerContext* context, const StringArray* request, Integer* response)override{
		std::int32_t size = request->values_size();
		for (std::int32_t i = 0; i < size; i++) {
			vlp_app->GetResourceContainer()->UnLoad (ShaderTag( request->values().Get(i)));
		}
		return grpc::Status::OK;
	}
	grpc::Status UnLoadVertexShader(grpc::ServerContext* context, const StringArray* request, Integer* response)override {
		std::int32_t size = request->values_size();
		for (std::int32_t i = 0; i < size; i++) {
			vlp_app->GetResourceContainer()->UnLoad(VertexShaderTag(request->values().Get(i)));
		}
		return grpc::Status::OK;
	}
	grpc::Status UnLoadPixelShader(grpc::ServerContext* context, const StringArray* request, Integer* response)override {
		std::int32_t size = request->values_size();
		for (std::int32_t i = 0; i < size; i++) {
			vlp_app->GetResourceContainer()->UnLoad(PixelShaderTag(request->values().Get(i)));
		}
		return grpc::Status::OK;
	}
	grpc::Status UnLoadGeometryShader(grpc::ServerContext* context, const StringArray* request, Integer* response)override {
		std::int32_t size = request->values_size();
		for (std::int32_t i = 0; i < size; i++) {
			vlp_app->GetResourceContainer()->UnLoad(GeometryShaderTag(request->values().Get(i)));
		}
		return grpc::Status::OK;
	}

	void SetApplication(Value_ptr<IApplication> arg_vlp_app) { vlp_app = arg_vlp_app; }
private:
	Value_ptr<IApplication> vlp_app;
	std::mutex mtx_app;
	std::condition_variable cv_app;
};
class RuntimeServer :public IRuntimeServer{
public:
	RuntimeServer() {
		ButiTaskSystem::Start(8);
		vlp_service = make_value<Service>();
		//vlp_service->ApplicationInit();
	}
	std::int32_t Start()override {
		ButiTaskSystem::PushTask(std::function<void()>([this]()->void { this->RunServer(); }));
		std::int32_t ret = vlp_service->Run();
		ServerShutDown();
		ButiTaskSystem::Dispose();
		return ret;
	}
	void Stop()override {

	}

	void RunServer() {
		std::string server_address("0.0.0.0:50051");

		grpc::EnableDefaultHealthCheckService(true);
		grpc::reflection::InitProtoReflectionServerBuilderPlugin();
		grpc::ServerBuilder builder;
		// Listen on the given address without any authentication mechanism.
		builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
		// Register "service" as the instance through which we'll communicate with
		// clients. In this case it corresponds to an *synchronous* service.
		builder.RegisterService(vlp_service.get());
		unq_server = (builder.BuildAndStart());
		std::cout << "Server listening on " << server_address << std::endl;
		unq_server->Wait();
	}

	void ServerShutDown() {
		unq_server->Shutdown();
	}
private:
	Value_ptr< Service> vlp_service;
	std::unique_ptr<grpc::Server> unq_server;
};
}

ButiEngine::Value_ptr<ButiEngine::IRuntimeServer> ButiEngine::CreateRuntimeServer()
{
	return make_value<RuntimeServer>();
}

#endif // _IMGUIEDITOR
#endif // _EDITORBUILD
