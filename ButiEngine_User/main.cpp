
#include"Header/ApplicationCreater.h"
#include"Header/GameObjects/DefaultGameComponent/ModelDrawComponent.h"
#include"Header/GameObjects/DefaultGameComponent/SimpleBoneAnimator.Component.h"
#include"Header/Device//ModelFileConverter.h"
#include"ButiFont/ButiFont.h"
#include"Server/RuntimeServer.h"
#include"ButiEventSystem/ButiEventSystem/TaskSystem.h"
#include"cereal/include/cereal/cereal.hpp"
#include"cereal/include/cereal/archives/json.hpp"
#include"StageSelectManager.h"
#define _CRTDBG_MAP_ALLOC

using namespace::ButiEngine;

#ifdef DEBUG

#ifdef _EDITORBUILD
std::int32_t main(const std::int32_t argCount, const char* args[])
#else

std::int32_t main() 

#endif
{
	std::cout << "debug" << std::endl;
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#else

std::int32_t APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR arg, std::int32_t)
{
#endif
	//StageSelectManager::SetMaxStageNum();
	std::int32_t returnCode = 0;

#ifdef _EDITORBUILD

	//引数あり実行ならリソースのディレクトリを変更する
#ifdef DEBUG
	if (argCount > 1) {
		GlobalSettings::SetResourceDirectory(args[1]);
	}
#else

	auto argments = Util::WStringToString(std::wstring(GetCommandLine()));
	auto splitedArgments = StringHelper::Split(argments, " ");
	if (splitedArgments.size() > 1)
		GlobalSettings::SetResourceDirectory(splitedArgments[1]);
#endif

#ifdef _IMGUIEDITOR
	ButiTaskSystem::Start();
	GameAssetData::ApplicationInitData init;
	InputCereal(init, "Application/ButiEngine.ini");
	auto vlp_app = CreateApplicationInstance(init);

	GameDevice::Initialize();
	GameDevice::GetInput()->Initialize(vlp_app);
	GameDevice::SetReturnCode(0);
	GameDevice::SetIsEnd(false);
	vlp_app->PreLoadResources();
	vlp_app->InitLoadResources();
	vlp_app->GetSceneManager()->LoadScene_Init(init.initSceneName);
	vlp_app->GetGraphicDevice()->SetClearColor(ButiColor::DeepOrange());
	returnCode = vlp_app->Run();
	vlp_app->Exit();

	ButiTaskSystem::Dispose();
#else

	auto server = CreateRuntimeServer();

	returnCode = server->Start();
#endif


#else
	auto init = InputApplicationInitData();
#ifdef DEBUG
#else
	init.hInstance = hInstance;
#endif // !DEBUG

	
	auto app = CreateApplicationInstance(init);

	GameDevice::Initialize();
	//GameDevice::GetInput()->SetCursorHide(true);
	GameDevice::GetInput()->Initialize(app);
	app->PreLoadResources();
	app->InitLoadResources();

	app->GetSceneManager()->LoadScene_Init(init.initSceneName);


	app->GetGraphicDevice()->SetClearColor(ButiColor::DeepOrange());
	returnCode = app->Run();
	app->Exit();
#endif // _EDITORBUILD

	return returnCode;
}