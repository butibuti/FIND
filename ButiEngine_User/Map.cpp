#include "stdafx_u.h"
#include "Map.h"
#include "StageSelectManager.h"
#include "GameSettings.h"
#include "Shake.h"
#include "InvisibleBlock.h"
#include "InvisibleBlockManager.h"
#include "Player.h"
#include "StartPlayerDirecting.h"
#include "CameraController.h"
#include "BurstManager.h"
#include "BackGround.h"

void ButiEngine::Map::OnUpdate()
{
	auto player = GetManager().lock()->GetGameObject("Player").lock();
	if (player && player->GetGameComponent<Player>()->IsGoal())
	{
		if (!m_vlp_stageEndTimer->IsOn())
		{
			m_vlp_stageEndTimer->Start();
			ShakeStop();
			ShakeStart(0.06f);
			GetManager().lock()->GetGameObject("MainCamera").lock()->GetGameComponent<CameraController>()->ZoomIn();
			return;
		}
	}

	if (m_vlp_stageEndTimer->Update())
	{
		m_vlp_stageEndTimer->Stop();
		auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
		std::string sceneName = StageSelectManager::GetNextSceneName();
		sceneManager->RemoveScene(sceneName);
		sceneManager->LoadScene(sceneName);
		sceneManager->ChangeScene(sceneName);

		if (sceneName == "StageSelectScene") {

		}
	}
	if (abs(m_vlp_stageEndTimer->GetRemainFrame() - 80) <= 0.001)
	{
		GetManager().lock()->GetGameObject("MainCamera").lock()->GetGameComponent<CameraController>()->ZoomOut();
	}
	else if (abs(m_vlp_stageEndTimer->GetRemainFrame() - 60) <= 0.001)
	{
		//auto seTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Burst.wav");
		//gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->Play(seTag, 0.1f);

		GetManager().lock()->GetGameObject("BurstManager").lock()->GetGameComponent<BurstManager>()->Burst();
		GetManager().lock()->GetGameObject("BackGroundController").lock()->GetGameComponent<BackGround>()->Brast();

	}
	else if (abs(m_vlp_stageEndTimer->GetRemainFrame() - 20) <= 0.001)
	{
		DestoroyMapChip();
	}
}

void ButiEngine::Map::OnSet()
{
	m_vlp_currentMapData = ObjectFactory::Create<MapData>();
}

void ButiEngine::Map::Start()
{
	m_vlp_stageEndTimer = ObjectFactory::Create<RelativeTimer>(120);
	m_vec_vlp_mapDatas.clear();
	m_vec_randomBlockPoss.clear();

	auto mapFilePath = "Scene/" + GetManager().lock()->GetScene().lock()->GetSceneInformation()->GetSceneName() + "/mapInfo.map";
	//if (Util::ExistFile(GlobalSettings::GetResourceDirectory() + mapFilePath)) {
	//	auto mapData= ObjectFactory::Create<MapData>();
	//	InputCereal(*mapData, mapFilePath);
	//	m_vec_vlp_mapDatas.push_back(mapData);
	//}

	m_vec_vlp_mapDatas.push_back(ObjectFactory::Create<MapData>(0));

	m_playerPos = Vector3Const::Zero;
	m_currentStageNum = 0;
	auto invManager = GetManager().lock()->AddObjectFromCereal("InvisibleBlockManager");
	PutBlock(m_currentStageNum);
	m_mapStartColor = Vector4(0.0f, 0.0f, 0.2f, 1.0f);
	m_mapEndColor = Vector4(0.0f, 0.3f, 1.0f, 1.0f);

	auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
	std::string sceneName = "Stage" + std::to_string(StageSelectManager::m_stageNum - 1) + "Scene";
	sceneManager->RemoveScene(sceneName);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Map::Clone()
{
	auto output = ObjectFactory::Create<Map>();
	output->m_vec_vlp_mapDatas = m_vec_vlp_mapDatas;

	return output;
}

void ButiEngine::Map::PutBlock(std::uint8_t arg_stageNum)
{
	auto invisibleBlockManager = GetManager().lock()->GetGameObject("InvisibleBlockManager").lock()->GetGameComponent<InvisibleBlockManager>();
	DestoroyMapChip();

	m_vlp_currentMapData = m_vec_vlp_mapDatas[arg_stageNum];

	m_vec_vwp_mapObjectDatas.clear();
	auto mapSize = m_vlp_currentMapData->GetSize();
	m_vec_vwp_mapObjectDatas.resize(mapSize.y);
	for (int i = 0; i < mapSize.y; i++) {
		m_vec_vwp_mapObjectDatas[i].resize(mapSize.z);
		for (int j = 0; j < mapSize.z; j++) {
			m_vec_vwp_mapObjectDatas[i][j].resize(mapSize.x);
		}
	}

	std::vector<std::vector<std::vector<std::uint8_t>>> vec_mapDatas = m_vlp_currentMapData->m_vec_mapDatas;
	Vector3 scale(GameSettings::BLOCK_SIZE, GameSettings::BLOCK_SIZE, GameSettings::BLOCK_SIZE);
	Vector3 offset(vec_mapDatas[0][0].size() / 2, vec_mapDatas.size() / 2, vec_mapDatas[0].size() / 2);

	CreateRandom();

	for (std::uint8_t y = 0; y < vec_mapDatas.size(); y++)
	{
		for (std::uint8_t z = 0; z < vec_mapDatas[y].size(); z++)
		{
			for (std::uint8_t x = 0; x < vec_mapDatas[y][z].size(); x++)
			{
				Vector3 position(x, y, z);
				position -= offset;
				position *= scale;

				auto gameObject = ObjectFactory::Create<GameObject>();
				std::uint8_t mapNum = vec_mapDatas[y][z][x];
				if (mapNum == 0) {
					m_vec_vwp_mapObjectDatas[y][z][x] = nullptr;
					continue;
				}
				else if (mapNum == GameSettings::MAP_CHIP_BLOCK)
				{
					Vector3 targetPos = position;
					gameObject = GetManager().lock()->AddObjectFromCereal("Block", ObjectFactory::Create<Transform>(position, Vector3Const::Zero, scale));
					position.y = m_vec_randomBlockPoss[z][x] - (vec_mapDatas.size() - y) * 3.5f;
					gameObject->transform->SetWorldPosition(position);
					AddTransformAnimation(gameObject, targetPos.y);
					gameObject->GetGameComponent<Shake>()->SetDefaultPos(targetPos);
				}
				else if (mapNum == GameSettings::MAP_CHIP_TUTORIALGOAL)
				{
					float targetPosY = position.y;
					position.y = m_vec_randomBlockPoss[z][x] - (vec_mapDatas.size() - y) * 3.5f;
					gameObject = GetManager().lock()->AddObjectFromCereal("TutorialGoal");
					gameObject->transform->SetWorldPosition(position);
					AddTransformAnimation(gameObject, targetPosY);
				}
				else if (mapNum == GameSettings::MAP_CHIP_EASYGOAL)
				{
					float targetPosY = position.y;
					position.y = m_vec_randomBlockPoss[z][x] - (vec_mapDatas.size() - y) * 3.5f;
					gameObject = GetManager().lock()->AddObjectFromCereal("EasyGoal");
					gameObject->transform->SetWorldPosition(position);
					AddTransformAnimation(gameObject, targetPosY);
				}
				else if (mapNum == GameSettings::MAP_CHIP_DEFAULTGOAL)
				{
					float targetPosY = position.y;
					position.y = m_vec_randomBlockPoss[z][x] - (vec_mapDatas.size() - y) * 3.5f;
					gameObject = GetManager().lock()->AddObjectFromCereal("DefaultGoal");
					gameObject->transform->SetWorldPosition(position);
					AddTransformAnimation(gameObject, targetPosY);
				}
				else if (mapNum >= GameSettings::MAP_CHIP_INVISIBLEBLOCK && mapNum < GameSettings::MAP_CHIP_PLAYER_AND_GOAL)
				{
					Vector3 targetPos = position;
					position.y = m_vec_randomBlockPoss[z][x] - (vec_mapDatas.size() - y) * 3.5f;
					gameObject = GetManager().lock()->AddObjectFromCereal("InvisibleBlock");
					gameObject->transform->SetWorldPosition(position);
					std::uint8_t id = mapNum - GameSettings::MAP_CHIP_INVISIBLEBLOCK;
					gameObject->GetGameComponent<InvisibleBlock>()->SetID(id);
					gameObject->GetGameComponent<InvisibleBlock>()->SetMapPos(Vector3(x, y, z));
					AddTransformAnimation(gameObject, targetPos.y);
					gameObject->GetGameComponent<Shake>()->SetDefaultPos(targetPos);
				}
				else
					if (mapNum == GameSettings::MAP_CHIP_PLAYER || (mapNum >= GameSettings::MAP_CHIP_PLAYER_ROTATE_90 && mapNum <= GameSettings::MAP_CHIP_PLAYER_ROTATE_MIN_90) || mapNum > GameSettings::MAP_CHIP_PLAYER_AND_GOAL)
					{
						m_playerPos = Vector3(x, y, z);
						Vector3 spawnPos = position;
						spawnPos.y += 30.0f;
						gameObject = GetManager().lock()->AddObjectFromCereal("Player", ObjectFactory::Create<Transform>(position, Vector3Const::Zero, scale));
						gameObject->transform->SetWorldPosition(spawnPos);
						auto playerBehavior = gameObject->GetGameComponent<Player>();
						playerBehavior->SetStartPos(position);
						auto directing = gameObject->GetGameComponent<StartPlayerDirecting>();
						directing->SetSpawnPos(spawnPos);
						directing->SetStartPos(position);

						auto cameraMesh = GetManager().lock()->AddObjectFromCereal("CameraMesh", ObjectFactory::Create<Transform>(Vector3(0, 0, -0.1f), Vector3Const::Zero, scale));
						if (mapNum >= GameSettings::MAP_CHIP_PLAYER_ROTATE_90 && mapNum <= GameSettings::MAP_CHIP_PLAYER_ROTATE_MIN_90) {
							auto rotation = (mapNum - GameSettings::MAP_CHIP_PLAYER_ROTATE_90 + 1) * 90;
							gameObject->transform->RollLocalRotationY_Degrees(rotation);
							playerBehavior->SetStartRotation(rotation);
						}
						else
							if (mapNum >= GameSettings::MAP_CHIP_PLAYER_AND_GOAL) {

								int mapNum_tenthSpace = (mapNum - GameSettings::MAP_CHIP_PLAYER_AND_GOAL) / 10;
								int mapNum_onceSpace = (mapNum - GameSettings::MAP_CHIP_PLAYER_AND_GOAL) % 10;


								auto rotation = mapNum_onceSpace * 90;
								gameObject->transform->RollLocalRotationY_Degrees(rotation);
								playerBehavior->SetStartRotation(rotation);

								if (mapNum_tenthSpace == GameSettings::MAP_CHIP_TUTORIALGOAL)
								{
									float targetPosY = position.y;
									position.y = m_vec_randomBlockPoss[z][x] - (vec_mapDatas.size() - y) * 3.5f;
									gameObject = GetManager().lock()->AddObjectFromCereal("TutorialGoal");
									gameObject->transform->SetWorldPosition(position);
									AddTransformAnimation(gameObject, targetPosY);
								}
								else if (mapNum_tenthSpace == GameSettings::MAP_CHIP_EASYGOAL)
								{
									float targetPosY = position.y;
									position.y = m_vec_randomBlockPoss[z][x] - (vec_mapDatas.size() - y) * 3.5f;
									gameObject = GetManager().lock()->AddObjectFromCereal("EasyGoal");
									gameObject->transform->SetWorldPosition(position);
									AddTransformAnimation(gameObject, targetPosY);
								}
								else if (mapNum_tenthSpace == GameSettings::MAP_CHIP_DEFAULTGOAL)
								{
									float targetPosY = position.y;
									position.y = m_vec_randomBlockPoss[z][x] - (vec_mapDatas.size() - y) * 3.5f;
									gameObject = GetManager().lock()->AddObjectFromCereal("DefaultGoal");
									gameObject->transform->SetWorldPosition(position);
									AddTransformAnimation(gameObject, targetPosY);
								}
							}
					}

				m_vec_vwp_mapObjectDatas[y][z][x] = gameObject;
			}
		}
	}
}

void ButiEngine::Map::ChangeBlock(const Vector3& arg_mapPos, std::uint8_t arg_mapChipNum)
{
	if (arg_mapPos.x < 0 || arg_mapPos.x >= m_vlp_currentMapData->m_vec_mapDatas[0][0].size() ||
		arg_mapPos.y < 0 || arg_mapPos.y >= m_vlp_currentMapData->m_vec_mapDatas.size() ||
		arg_mapPos.z < 0 || arg_mapPos.z >= m_vlp_currentMapData->m_vec_mapDatas[0].size())
	{
		return;
	}
	m_vlp_currentMapData->m_vec_mapDatas[arg_mapPos.y][arg_mapPos.z][arg_mapPos.x] = arg_mapChipNum;
}

void ButiEngine::Map::ShakeStart(float arg_amplitude)
{
	auto endY = m_vec_vwp_mapObjectDatas.end();
	for (auto itrY = m_vec_vwp_mapObjectDatas.begin(); itrY != endY; ++itrY)
	{
		auto endZ = itrY->end();
		for (auto itrZ = itrY->begin(); itrZ != endZ; ++itrZ)
		{
			auto endX = itrZ->end();
			for (auto itrX = itrZ->begin(); itrX != endX; ++itrX)
			{
				if (!(*itrX).lock() || (*itrX).lock()->GetGameObjectName() == "Player")
				{
					continue;
				}
				auto shake = (*itrX).lock()->GetGameComponent<Shake>();
				if (shake)
				{
					shake->ShakeStart(arg_amplitude);
				}
			}
		}
	}
}

void ButiEngine::Map::ShakeStop()
{
	auto endY = m_vec_vwp_mapObjectDatas.end();
	for (auto itrY = m_vec_vwp_mapObjectDatas.begin(); itrY != endY; ++itrY)
	{
		auto endZ = itrY->end();
		for (auto itrZ = itrY->begin(); itrZ != endZ; ++itrZ)
		{
			auto endX = itrZ->end();
			for (auto itrX = itrZ->begin(); itrX != endX; ++itrX)
			{
				if (!(*itrX).lock() || (*itrX).lock()->GetGameObjectName() == "Player")
				{
					continue;
				}
				auto shake = (*itrX).lock()->GetGameComponent<Shake>();
				if (shake)
				{
					shake->ShakeStop();
				}
			}
		}
	}
}

void ButiEngine::Map::DestoroyMapChip()
{
	{
		auto tag = GetTagManager()->GetObjectTag("MapChip");
		auto manager = GetManager().lock();
		std::vector<Value_ptr<GameObject>> vec_vlp_gameObjects = manager->GetGameObjects(tag);

		for (auto itr = vec_vlp_gameObjects.begin(); itr != vec_vlp_gameObjects.end(); ++itr)
		{
			(*itr)->SetIsRemove(true);
		}
	}

	{
		auto tag = GetTagManager()->GetObjectTag("InvisibleBlock");
		auto manager = GetManager().lock();
		std::vector<Value_ptr<GameObject>> vec_vlp_gameObjects = manager->GetGameObjects(tag);

		for (auto itr = vec_vlp_gameObjects.begin(); itr != vec_vlp_gameObjects.end(); ++itr)
		{
			(*itr)->SetIsRemove(true);
		}
	}

	{
		auto tag = GetTagManager()->GetObjectTag("Goal");
		auto manager = GetManager().lock();
		std::vector<Value_ptr<GameObject>> vec_vlp_gameObjects = manager->GetGameObjects(tag);

		for (auto itr = vec_vlp_gameObjects.begin(); itr != vec_vlp_gameObjects.end(); ++itr)
		{
			(*itr)->SetIsRemove(true);
		}
	}
}

void ButiEngine::Map::CreateRandom()
{
	std::vector<std::vector<std::vector<std::uint8_t>>> vec_mapDatas = m_vlp_currentMapData->m_vec_mapDatas;
	for (std::uint8_t z = 0; z < vec_mapDatas[0].size(); z++)
	{
		std::vector<float> pos;
		for (std::uint8_t x = 0; x < vec_mapDatas[0][z].size(); x++)
		{
			pos.push_back(ButiRandom::GetRandom(-30.0f, -15.0f, 100));
		}
		m_vec_randomBlockPoss.push_back(pos);
	}
}

void ButiEngine::Map::AddTransformAnimation(Value_weak_ptr<ButiEngine::GameObject> arg_gameObject, float arg_y)
{
	auto transform = arg_gameObject.lock()->transform;

	float d = arg_y - transform->GetWorldPosition().y;

	auto anim = arg_gameObject.lock()->AddGameComponent<TransformAnimation>();
	anim->SetSpeed(1.0f / (d * 2));
	anim->SetTargetTransform(transform->Clone());
	anim->GetTargetTransform()->TranslateY(d);
	anim->GetTargetTransform()->RollLocalRotationX_Degrees(0.1f);

	anim->SetEaseType(Easing::EasingType::EaseInOutQuint);
}

ButiEngine::MapData::MapData(std::uint8_t arg_stageNum)
{
	if (arg_stageNum == 0)
	{
		m_vec_mapDatas =
		{
			{
				{2,2,2,2,2,2,2,2,2,2,2},
				{2,2,2,2,2,2,2,2,2,2,2},
				{2,2,2,2,2,2,2,2,2,2,2},
				{2,2,2,2,2,2,2,2,2,2,2},
				{2,2,2,2,2,2,2,2,2,2,2},
				{2,2,2,2,2,2,2,2,2,2,2},
				{2,2,2,2,2,2,2,2,2,2,2},
				{2,2,2,2,2,2,2,2,2,2,2},
				{2,2,2,2,0,2,2,2,2,2,2},
				{2,2,2,2,2,2,2,2,2,2,2},
				{2,2,2,2,2,2,2,2,2,2,2},
			},
			{
				{0,0,0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,100,0,0,0,0},
				{0,0,0,0,0,2,0,0,0,0,0},
				{0,0,2,2,0,0,0,0,101,0,0},
				{0,0,0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,1,4,0,0,0,0},
				{0,0,101,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0,0,0},
				{0,0,101,0,0,2,2,2,2,2,0},
				{0,0,0,0,0,100,100,100,0,0,0},
				{0,0,0,0,0,0,0,0,0,0,5},
			},
			{
				{0,0,0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0,0,0},
				{0,0,0,2,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0,2,0},
				{0,0,0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0,0,0},
			},
			{
				{0,0,0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0,2,0},
				{0,0,0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0,0,0},
			},
		};
	}
}
