#include "stdafx_u.h"
#include "Map.h"

void ButiEngine::Map::OnUpdate()
{
	//auto player = GetManager().lock()->GetGameObject("Player").lock();
	//if (player && player->GetBehavior<PlayerBehavior>()->GetGoal())
	//{
	//	if (!stageEndTimer->IsOn())
	//	{
	//		stageEndTimer->Start();
	//		ShakeStop();
	//		ShakeStart(0.06f);
	//		GetManager().lock()->GetGameObject("MainCamera").lock()->GetGameComponent<CameraController>()->ZoomIn();
	//		return;
	//	}
	//}

	//if (stageEndTimer->Update())
	//{
	//	stageEndTimer->Stop();
	//	auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
	//	std::string sceneName = StageSelectManagerComponent::GetNextSceneName();
	//	sceneManager->RemoveScene(sceneName);
	//	sceneManager->LoadScene(sceneName);
	//	sceneManager->ChangeScene(sceneName);

	//	if (sceneName == "StageSelectScene") {

	//	}
	//}
	//if (abs(stageEndTimer->GetRemainFrame() - 80) <= 0.001)
	//{
	//	GetManager().lock()->GetGameObject("MainCamera").lock()->GetGameComponent<CameraController>()->ZoomOut();
	//}
	//else if (abs(stageEndTimer->GetRemainFrame() - 60) <= 0.001)
	//{
	//	auto seTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Burst.wav");
	//	gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->Play(seTag, 0.1f);
	//	GetManager().lock()->GetGameObject("BurstManager").lock()->GetGameComponent<BurstManagerComponent>()->Burst();
	//	GetManager().lock()->GetGameObject("BackGroundController").lock()->GetGameComponent<BackGround>()->Brast();

	//}
	//else if (abs(stageEndTimer->GetRemainFrame() - 20) <= 0.001)
	//{
	//	DestoroyMapChip();
	//}
}

void ButiEngine::Map::OnSet()
{
	//currentMapData = std::make_shared<MapData>();
}

void ButiEngine::Map::Start()
{
	//stageEndTimer = ObjectFactory::Create<RelativeTimer>(120);
	//vec_mapData.clear();
	//randomBlockPos.clear();
	//auto mapFilePath = "Scene/" + GetManager().lock()->GetScene().lock()->GetSceneInformation()->GetSceneName() + "/mapInfo.map";
	//if (Util::IsFileExistence(GlobalSettings::GetResourceDirectory() + mapFilePath)) {

	//	auto input = std::make_shared<MapData>();
	//	InputCereal(input, mapFilePath);
	//	vec_mapData.push_back(*input);
	//}
	//else {
	//	vec_mapData.push_back(MapData(0));
	//}

	//playerPos = Vector3::Zero;
	//currentStageNum = 0;
	//auto invManager = GetManager().lock()->AddObjectFromCereal("InvisibleBlockManager");
	//PutBlock(currentStageNum);
	//mapStartColor = Vector4(0.0f, 0.0f, 0.2f, 1.0f);
	//mapEndColor = Vector4(0.0f, 0.3f, 1.0f, 1.0f);

	//auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
	//std::string sceneName = "Stage" + std::to_string(StageSelectManagerComponent::stageNum - 1) + "Scene";
	//sceneManager->RemoveScene(sceneName);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Map::Clone()
{
	auto output = ObjectFactory::Create<Map>();
	output->vec_mapData = vec_mapData;

	return output;
}

void ButiEngine::Map::PutBlock(int stageNum)
{
	//auto invisibleBlockManager = GetManager().lock()->GetGameObject("InvisibleBlockManager").lock()->GetGameComponent<InvisibleBlockManagerComponent>();
	//DestoroyMapChip();

	//*currentMapData = vec_mapData[stageNum];

	//mapObjectData.clear();
	//auto mapSize = currentMapData->GetSize();
	//mapObjectData.resize(mapSize.y);
	//for (int i = 0; i < mapSize.y; i++) {
	//	mapObjectData[i].resize(mapSize.z);
	//	for (int j = 0; j < mapSize.z; j++) {
	//		mapObjectData[i][j].resize(mapSize.x);
	//	}
	//}

	//std::vector<std::vector<std::vector<int>>> mapData = currentMapData->mapData;
	//Vector3 scale(GameSettings::BlockSize, GameSettings::BlockSize, GameSettings::BlockSize);
	//Vector3 offset(mapData[0][0].size() / 2, mapData.size() / 2, mapData[0].size() / 2);

	//CreateRandom();

	//for (unsigned int y = 0; y < mapData.size(); y++)
	//{
	//	for (unsigned int z = 0; z < mapData[y].size(); z++)
	//	{
	//		for (unsigned int x = 0; x < mapData[y][z].size(); x++)
	//		{
	//			Vector3 position(x, y, z);
	//			position -= offset;
	//			position *= GameSettings::BlockSize;
	//			std::weak_ptr<GameObject> gameObject = std::shared_ptr<GameObject>();
	//			int mapNum = mapData[y][z][x];
	//			if (mapNum == 0) {
	//				mapObjectData[y][z][x] = gameObject.lock();
	//				continue;
	//			}
	//			else if (mapNum == GameSettings::block)
	//			{
	//				Vector3 targetPos = position;
	//				gameObject = GetManager().lock()->AddObjectFromCereal("Block", ObjectFactory::Create<Transform>(position, Vector3::Zero, scale));
	//				position.y = randomBlockPos[z][x] - (mapData.size() - y) * 3.5f;
	//				gameObject.lock()->transform->SetWorldPosition(position);
	//				AddTransformAnimation(gameObject, targetPos.y);
	//				gameObject.lock()->GetGameComponent<ShakeComponent>()->SetDefaultPos(targetPos);
	//			}
	//			else if (mapNum == GameSettings::tutorialGoal)
	//			{
	//				float targetPosY = position.y;
	//				position.y = randomBlockPos[z][x] - (mapData.size() - y) * 3.5f;
	//				gameObject = GetManager().lock()->AddObjectFromCereal("TutorialGoal");
	//				gameObject.lock()->transform->SetWorldPosition(position);
	//				AddTransformAnimation(gameObject, targetPosY);
	//			}
	//			else if (mapNum == GameSettings::easyGoal)
	//			{
	//				float targetPosY = position.y;
	//				position.y = randomBlockPos[z][x] - (mapData.size() - y) * 3.5f;
	//				gameObject = GetManager().lock()->AddObjectFromCereal("EasyGoal");
	//				gameObject.lock()->transform->SetWorldPosition(position);
	//				AddTransformAnimation(gameObject, targetPosY);
	//			}
	//			else if (mapNum == GameSettings::defaultGoal)
	//			{
	//				float targetPosY = position.y;
	//				position.y = randomBlockPos[z][x] - (mapData.size() - y) * 3.5f;
	//				gameObject = GetManager().lock()->AddObjectFromCereal("DefaultGoal");
	//				gameObject.lock()->transform->SetWorldPosition(position);
	//				AddTransformAnimation(gameObject, targetPosY);
	//			}
	//			else if (mapNum >= GameSettings::invisibleBlock && mapNum < GameSettings::playerAndGoal)
	//			{
	//				Vector3 targetPos = position;
	//				position.y = randomBlockPos[z][x] - (mapData.size() - y) * 3.5f;
	//				gameObject = GetManager().lock()->AddObjectFromCereal("InvisibleBlock");
	//				gameObject.lock()->transform->SetWorldPosition(position);
	//				int id = mapNum - GameSettings::invisibleBlock;
	//				gameObject.lock()->GetGameComponent<InvisibleBlockComponent>()->SetID(id);
	//				gameObject.lock()->GetGameComponent<InvisibleBlockComponent>()->SetMapPos(Vector3(x, y, z));
	//				AddTransformAnimation(gameObject, targetPos.y);
	//				gameObject.lock()->GetGameComponent<ShakeComponent>()->SetDefaultPos(targetPos);
	//			}
	//			else
	//				if (mapNum == GameSettings::player || (mapNum >= GameSettings::playerRotate_90 && mapNum <= GameSettings::playerRotate_min90) || mapNum > GameSettings::playerAndGoal)
	//				{
	//					playerPos = Vector3(x, y, z);
	//					Vector3 spawnPos = position;
	//					spawnPos.y += 30.0f;
	//					gameObject = GetManager().lock()->AddObjectFromCereal("Player", ObjectFactory::Create<Transform>(position, Vector3::Zero, scale));
	//					gameObject.lock()->transform->SetWorldPosition(spawnPos);
	//					auto playerBehavior = gameObject.lock()->GetBehavior<PlayerBehavior>();
	//					playerBehavior->SetStartPos(position);
	//					auto directing = gameObject.lock()->GetGameComponent<StartPlayerDirectingComponent>();
	//					directing->SetSpawnPos(spawnPos);
	//					directing->SetStartPos(position);

	//					auto cameraMesh = GetManager().lock()->AddObjectFromCereal("CameraMesh", ObjectFactory::Create<Transform>(Vector3(0, 0, -0.1f), Vector3::Zero, scale));
	//					auto camera = GetCamera("playerCamera");
	//					camera.lock()->shp_transform->SetLocalPosition().z = 0.5f;
	//					camera.lock()->shp_transform->SetBaseTransform(gameObject.lock()->transform, true);
	//					if (mapNum >= GameSettings::playerRotate_90 && mapNum <= GameSettings::playerRotate_min90) {
	//						auto rotation = (mapNum - GameSettings::playerRotate_90 + 1) * 90;
	//						gameObject.lock()->transform->RollLocalRotationY_Degrees(rotation);
	//						playerBehavior->SetStartRotation(rotation);

	//						//gameObject = std::weak_ptr<GameObject>();
	//					}
	//					else
	//						if (mapNum >= GameSettings::playerAndGoal) {

	//							int mapNum_tenthSpace = (mapNum - GameSettings::playerAndGoal) / 10;
	//							int mapNum_onceSpace = (mapNum - GameSettings::playerAndGoal) % 10;


	//							auto rotation = mapNum_onceSpace * 90;
	//							gameObject.lock()->transform->RollLocalRotationY_Degrees(rotation);
	//							playerBehavior->SetStartRotation(rotation);

	//							if (mapNum_tenthSpace == GameSettings::tutorialGoal)
	//							{
	//								float targetPosY = position.y;
	//								position.y = randomBlockPos[z][x] - (mapData.size() - y) * 3.5f;
	//								gameObject = GetManager().lock()->AddObjectFromCereal("TutorialGoal");
	//								gameObject.lock()->transform->SetWorldPosition(position);
	//								AddTransformAnimation(gameObject, targetPosY);
	//							}
	//							else if (mapNum_tenthSpace == GameSettings::easyGoal)
	//							{
	//								float targetPosY = position.y;
	//								position.y = randomBlockPos[z][x] - (mapData.size() - y) * 3.5f;
	//								gameObject = GetManager().lock()->AddObjectFromCereal("EasyGoal");
	//								gameObject.lock()->transform->SetWorldPosition(position);
	//								AddTransformAnimation(gameObject, targetPosY);
	//							}
	//							else if (mapNum_tenthSpace == GameSettings::defaultGoal)
	//							{
	//								float targetPosY = position.y;
	//								position.y = randomBlockPos[z][x] - (mapData.size() - y) * 3.5f;
	//								gameObject = GetManager().lock()->AddObjectFromCereal("DefaultGoal");
	//								gameObject.lock()->transform->SetWorldPosition(position);
	//								AddTransformAnimation(gameObject, targetPosY);
	//							}
	//						}
	//				}

	//			mapObjectData[y][z][x] = gameObject.lock();
	//		}
	//	}
	//}
}

void ButiEngine::Map::ChangeBlock(Vector3 mapPos, int mapChipNum)
{
	//if (mapPos.x < 0 || mapPos.x >= currentMapData->mapData[0][0].size() ||
	//	mapPos.y < 0 || mapPos.y >= currentMapData->mapData.size() ||
	//	mapPos.z < 0 || mapPos.z >= currentMapData->mapData[0].size())
	//{
	//	return;
	//}
	//currentMapData->mapData[mapPos.y][mapPos.z][mapPos.x] = mapChipNum;
}

void ButiEngine::Map::ShakeStart(float arg_amplitude)
{
	//auto endY = mapObjectData.end();
	//for (auto itrY = mapObjectData.begin(); itrY != endY; ++itrY)
	//{
	//	auto endZ = itrY->end();
	//	for (auto itrZ = itrY->begin(); itrZ != endZ; ++itrZ)
	//	{
	//		auto endX = itrZ->end();
	//		for (auto itrX = itrZ->begin(); itrX != endX; ++itrX)
	//		{
	//			if (!(*itrX) || (*itrX)->GetGameObjectName() == "Player")
	//			{
	//				continue;
	//			}
	//			auto shake = (*itrX)->GetGameComponent<ShakeComponent>();
	//			if (shake)
	//			{
	//				shake->Start(arg_amplitude);
	//			}
	//		}
	//	}
	//}
}

void ButiEngine::Map::ShakeStop()
{
	//auto endY = mapObjectData.end();
	//for (auto itrY = mapObjectData.begin(); itrY != endY; ++itrY)
	//{
	//	auto endZ = itrY->end();
	//	for (auto itrZ = itrY->begin(); itrZ != endZ; ++itrZ)
	//	{
	//		auto endX = itrZ->end();
	//		for (auto itrX = itrZ->begin(); itrX != endX; ++itrX)
	//		{
	//			if (!(*itrX) || (*itrX)->GetGameObjectName() == "Player")
	//			{
	//				continue;
	//			}
	//			auto shake = (*itrX)->GetGameComponent<ShakeComponent>();
	//			if (shake)
	//			{
	//				shake->Stop();
	//			}
	//		}
	//	}
	//}
}

void ButiEngine::Map::DestoroyMapChip()
{
	//{
	//	auto tag = GetTagManager()->GetObjectTag("MapChip");
	//	auto manager = GetManager().lock();
	//	std::vector<std::shared_ptr<GameObject>> gameObjects = manager->GetGameObjects(tag);

	//	for (auto itr = gameObjects.begin(); itr != gameObjects.end(); ++itr)
	//	{
	//		(*itr)->SetIsRemove(true);
	//	}
	//}

	//{
	//	auto tag = GetTagManager()->GetObjectTag("InvisibleBlock");
	//	auto manager = GetManager().lock();
	//	std::vector<std::shared_ptr<GameObject>> gameObjects = manager->GetGameObjects(tag);

	//	for (auto itr = gameObjects.begin(); itr != gameObjects.end(); ++itr)
	//	{
	//		(*itr)->SetIsRemove(true);
	//	}
	//}

	//{
	//	auto tag = GetTagManager()->GetObjectTag("Goal");
	//	auto manager = GetManager().lock();
	//	std::vector<std::shared_ptr<GameObject>> gameObjects = manager->GetGameObjects(tag);

	//	for (auto itr = gameObjects.begin(); itr != gameObjects.end(); ++itr)
	//	{
	//		(*itr)->SetIsRemove(true);
	//	}
	//}
}

void ButiEngine::Map::CreateRandom()
{
	//std::vector<std::vector<std::vector<int>>> mapData = currentMapData->mapData;
	//for (unsigned int z = 0; z < mapData[0].size(); z++)
	//{
	//	std::vector<float> pos;
	//	for (unsigned int x = 0; x < mapData[0][z].size(); x++)
	//	{
	//		pos.push_back(ButiRandom::GetRandom(-30.0f, -15.0f, 100));
	//	}
	//	randomBlockPos.push_back(pos);
	//}
}

void ButiEngine::Map::AddTransformAnimation(std::weak_ptr<ButiEngine::GameObject> gameObject, float y)
{
	//auto t = gameObject.lock()->transform;

	//float d = y - t->GetWorldPosition().y;

	//auto anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
	//anim->SetSpeed(1.0f / (d * 2));
	//anim->SetTargetTransform(t->Clone());
	//anim->GetTargetTransform()->TranslateY(d);
	//anim->GetTargetTransform()->RollLocalRotationX_Degrees(0.1f);

	//anim->SetEaseType(Easing::EasingType::EaseInOutQuint);
}

void ButiEngine::OutputCereal(const std::shared_ptr<MapData>& v, const std::string& path)
{

	//std::stringstream stream;


	//cereal::BinaryOutputArchive binOutArchive(stream);
	//binOutArchive(v);

	//std::ofstream outputFile(GlobalSettings::GetResourceDirectory() + path, std::ios::binary);

	//outputFile << stream.str();

	//outputFile.close();
	//stream.clear();
}

void ButiEngine::InputCereal(std::shared_ptr<MapData>& v, const std::string& path)
{
	//std::stringstream stream;

	//std::ifstream inputFile(GlobalSettings::GetResourceDirectory() + path, std::ios::binary);

	//stream << inputFile.rdbuf();

	//cereal::BinaryInputArchive binInputArchive(stream);


	//binInputArchive(v);
}
