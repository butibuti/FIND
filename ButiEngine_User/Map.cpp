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
#include "NextStageBlock.h"
#include"MapEditor.h"
#include"Glass.h"
ButiEngine::Value_ptr<ButiEngine::MapData> ButiEngine::Map::m_vlp_stageSelectMapData;
ButiEngine::Value_ptr<ButiEngine::Transform> ButiEngine::Map::m_vlp_playerTransform;
ButiEngine::Value_ptr<ButiEngine::Transform> ButiEngine::Map::m_vlp_eyeBlockTransform;

void ButiEngine::Map::OnUpdate()
{
	auto player = GetManager().lock()->GetGameObject("Player").lock();
	if (player && player->GetGameComponent<Player>()->IsGoal())
	{
		if (!m_vlp_stageEndTimer->IsOn())
		{
			SavePlayerData();
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

		std::string currentSceneName = sceneManager->GetCurrentScene()->GetSceneInformation()->GetSceneName();
		std::string nextSceneName;
		if (currentSceneName == "NewStageSelectScene")
		{
			auto playerPos = player->GetGameComponent<Player>()->GetMapPos();
			std::uint16_t mapNum = m_vlp_currentMapData->m_vec_mapDatas[playerPos.y][playerPos.z][playerPos.x];
			std::uint16_t nextStageNum = mapNum - GameSettings::MAP_CHIP_NEXT_STAGE_BLOCK;
			if (nextStageNum >= 100)
			{
				nextStageNum -= 100;
			}
			nextSceneName = "Stage_" + std::to_string(nextStageNum);
		}
		else
		{
			NextStageBlock::SetStatus(m_currentStageNum, NextStageBlockStatus::Cleared);
			nextSceneName = "NewStageSelectScene";
			if (NextStageBlock::IsAllCleard())
			{
				nextSceneName = "ThanksScene";
			}
		}
		sceneManager->RemoveScene(nextSceneName);
		sceneManager->LoadScene(nextSceneName);
		sceneManager->ChangeScene(nextSceneName);
	}
	if (abs(m_vlp_stageEndTimer->GetRemainFrame() - 80) <= 0.001)
	{
		GetManager().lock()->GetGameObject("MainCamera").lock()->GetGameComponent<CameraController>()->ZoomOut();
	}
	else if (abs(m_vlp_stageEndTimer->GetRemainFrame() - 60) <= 0.001)
	{
		gameObject.lock()->GetApplication().lock()->GetSoundManager()->PlaySE(SoundTag("Sound/Burst.wav"), 0.1f);

		GetManager().lock()->GetGameObject("BurstManager").lock()->GetGameComponent<BurstManager>()->Burst();
		GetManager().lock()->GetGameObject("BackGroundController").lock()->GetGameComponent<BackGround>()->Brast();

	}
	else if (abs(m_vlp_stageEndTimer->GetRemainFrame() - 20) <= 0.001)
	{
		//DestoroyMapChip();
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

	auto sceneName = GetManager().lock()->GetScene().lock()->GetSceneInformation()->GetSceneName();
	if (sceneName == "LevelEditor") {
		auto mapFilePath = "Scene/Stage_" + std::to_string(MapEditor::GetEditMapIndex()) + "/mapData.map";
		if (ResourceSystem::ExistResource(mapFilePath)) {
			auto mapData = ObjectFactory::Create<MapData>();
			InputCereal(mapData, mapFilePath);
			m_vec_vlp_mapDatas.push_back(mapData);
		}
		else {
			m_vec_vlp_mapDatas.push_back(ObjectFactory::Create<MapData>(0));
		}
	}
	else if (StringHelper::Contains(sceneName, "Select") && m_vlp_stageSelectMapData)
	{
		m_vec_vlp_mapDatas.push_back(m_vlp_stageSelectMapData);
	}
	else {
		auto mapFilePath = "Scene/" + sceneName + "/mapData.map";
		if (ResourceSystem::ExistResource(mapFilePath)) {
			auto mapData = ObjectFactory::Create<MapData>();
			InputCereal(mapData, mapFilePath);
			m_vec_vlp_mapDatas.push_back(mapData);

			if (StringHelper::Contains(sceneName, "Select"))
			{
				m_vlp_stageSelectMapData = mapData;
			}
		}
		else {
			m_vec_vlp_mapDatas.push_back(ObjectFactory::Create<MapData>(0));
		}
	}

	m_currentStageNum = 0;
	auto splitSceneName = StringHelper::Split(sceneName, "_");
	if (splitSceneName.size() > 1)
	{
		m_currentStageNum = atoi(splitSceneName[1].c_str());
	}
	
	m_startPlayerPos = Vector3Const::Zero;
	GetManager().lock()->AddObjectFromCereal("InvisibleBlockManager");
	PutBlock(0);
	m_mapStartColor = Vector4(0.0f, 0.0f, 0.2f, 1.0f);
	m_mapEndColor = Vector4(0.0f, 0.3f, 1.0f, 1.0f);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Map::Clone()
{
	auto output = ObjectFactory::Create<Map>();
	output->m_vec_vlp_mapDatas = m_vec_vlp_mapDatas;

	return output;
}

void ButiEngine::Map::PutBlock(std::uint16_t arg_stageNum)
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

	auto vec_mapDatas = m_vlp_currentMapData->m_vec_mapDatas;
	Vector3 scale(GameSettings::BLOCK_SIZE, GameSettings::BLOCK_SIZE, GameSettings::BLOCK_SIZE);
	Vector3 offset(vec_mapDatas[0][0].size() / 2, vec_mapDatas.size() / 2, vec_mapDatas[0].size() / 2);

	CreateRandom();

	for (std::uint16_t y = 0; y < vec_mapDatas.size(); y++)
	{
		for (std::uint16_t z = 0; z < vec_mapDatas[y].size(); z++)
		{
			for (std::uint16_t x = 0; x < vec_mapDatas[y][z].size(); x++)
			{
				Vector3 position(x, y, z);
				position -= offset;
				position *= scale;

				auto gameObject = ObjectFactory::Create<GameObject>();
				std::uint16_t mapNum = vec_mapDatas[y][z][x];
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
				else if (mapNum == GameSettings::MAP_CHIP_GLASS)
				{
					Vector3 targetPos = position;
					gameObject = GetManager().lock()->AddObjectFromCereal("Glass", ObjectFactory::Create<Transform>(position, Vector3Const::Zero, scale));
					position.y = m_vec_randomBlockPoss[z][x] - (vec_mapDatas.size() - y) * 3.5f;
					gameObject->transform->SetWorldPosition(position);
					AddTransformAnimation(gameObject, targetPos.y);
					gameObject->GetGameComponent<Shake>()->SetDefaultPos(targetPos);
					gameObject->GetGameComponent<GlassBlock>()->SetMapPos(Vector3(x, y, z));
					gameObject->GetGameComponent<GlassBlock>()->SetMap(GetThis<Map>());
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
					std::uint16_t id = mapNum - GameSettings::MAP_CHIP_INVISIBLEBLOCK;
					gameObject->GetGameComponent<InvisibleBlock>()->SetID(id);
					gameObject->GetGameComponent<InvisibleBlock>()->SetMapPos(Vector3(x, y, z));
					AddTransformAnimation(gameObject, targetPos.y);
					gameObject->GetGameComponent<Shake>()->SetDefaultPos(targetPos);
				}
				else if (mapNum == GameSettings::MAP_CHIP_PLAYER || (mapNum >= GameSettings::MAP_CHIP_PLAYER_ROTATE_90 && mapNum <= GameSettings::MAP_CHIP_PLAYER_DOWN_ROTATE_90) || (mapNum >= GameSettings::MAP_CHIP_PLAYER_AND_GOAL && mapNum < GameSettings::MAP_CHIP_NEXT_STAGE_BLOCK))
				{
					m_startPlayerPos = Vector3(x, y, z);
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
					if (mapNum >= GameSettings::MAP_CHIP_PLAYER_ROTATE_90 && mapNum <= GameSettings::MAP_CHIP_PLAYER_DOWN_ROTATE_90) {
						auto playerDir = (mapNum - GameSettings::MAP_CHIP_PLAYER_ROTATE_90 + 1);
						if (playerDir < 4) {
							auto rotation = playerDir * 90;
							gameObject->transform->RollLocalRotationY_Degrees(rotation);
							playerBehavior->SetStartRotation(rotation);
						}
						else if (playerDir < 6) {
							auto rotation = (playerDir - 4) * 90;
							gameObject->transform->RollLocalRotationY_Degrees(rotation);
							gameObject->transform->RollLocalRotationX_Degrees(-90);
							playerBehavior->SetStartRotation(rotation);
						}
						else {
							auto rotation = (playerDir - 6) * 90;
							gameObject->transform->RollLocalRotationY_Degrees(rotation);
							gameObject->transform->RollLocalRotationX_Degrees(90);
							playerBehavior->SetStartRotation(rotation);
						}
					}
					else
						if (mapNum >= GameSettings::MAP_CHIP_PLAYER_AND_GOAL) {

							//一の位がプレイヤーの向き、十の位がゴールの種類
							std::int16_t mapNum_tenthSpace = (mapNum - GameSettings::MAP_CHIP_PLAYER_AND_GOAL) / 10;
							std::int16_t mapNum_onceSpace = (mapNum - GameSettings::MAP_CHIP_PLAYER_AND_GOAL) % 10;

							if (mapNum_onceSpace < 4) {
								auto rotation = mapNum_onceSpace * 90;
								gameObject->transform->RollLocalRotationY_Degrees(rotation);
								playerBehavior->SetStartRotation(rotation);
							}
							else if (mapNum_onceSpace < 6) {
								auto rotation =( mapNum_onceSpace -4)*90;
								gameObject->transform->RollLocalRotationY_Degrees(rotation);
								gameObject->transform->RollLocalRotationX_Degrees(-90);
								playerBehavior->SetStartRotation(rotation);
							}
							else {
								auto rotation = (mapNum_onceSpace - 6) * 90;
								gameObject->transform->RollLocalRotationY_Degrees(rotation);
								gameObject->transform->RollLocalRotationX_Degrees(90);
								playerBehavior->SetStartRotation(rotation);
							}

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
				else if (mapNum >= GameSettings::MAP_CHIP_NEXT_STAGE_BLOCK && mapNum < GameSettings::MAP_CHIP_ALREADY_SEEN_NEXT_STAGE_BLOCK)
				{
					std::uint16_t stageNum = mapNum - GameSettings::MAP_CHIP_NEXT_STAGE_BLOCK;
					
					float targetPosY = position.y;
					position.y = m_vec_randomBlockPoss[z][x] - (vec_mapDatas.size() - y) * 3.5f;
					gameObject = GetManager().lock()->AddObjectFromCereal("NextStageBlock");
					gameObject->transform->SetWorldPosition(position);
					gameObject->transform->SetLocalScale(scale * 0.75f);
					auto nextStageBlockComponent = gameObject->GetGameComponent<NextStageBlock>();
					gameObject->GetGameComponent<NextStageBlock>()->SetStageNum(stageNum);
					nextStageBlockComponent->SetStatus(stageNum, NextStageBlockStatus::InActive);

					AddTransformAnimation(gameObject, targetPosY);
                }
				else if (mapNum >= GameSettings::MAP_CHIP_ALREADY_SEEN_NEXT_STAGE_BLOCK)
				{
					std::uint16_t stageNum = mapNum - GameSettings::MAP_CHIP_ALREADY_SEEN_NEXT_STAGE_BLOCK;

					float targetPosY = position.y;
					position.y = m_vec_randomBlockPoss[z][x] - (vec_mapDatas.size() - y) * 3.5f;
					gameObject = GetManager().lock()->AddObjectFromCereal("NextStageBlock");
					gameObject->transform->SetWorldPosition(position);
					gameObject->transform->SetLocalScale(scale * 0.75f);
					auto nextStageBlockComponent = gameObject->GetGameComponent<NextStageBlock>();
					gameObject->GetGameComponent<NextStageBlock>()->SetStageNum(stageNum);
					nextStageBlockComponent->SetStatus(stageNum, NextStageBlockStatus::Active);

					AddTransformAnimation(gameObject, targetPosY);
				}
					

				m_vec_vwp_mapObjectDatas[y][z][x] = gameObject;
			}
		}
	}
}

ButiEngine::Vector3 ButiEngine::Map::GetWorldPos(const Vector3& arg_mapPos)
{
	Vector3 output = arg_mapPos;

	auto vec_mapDatas = m_vlp_currentMapData->m_vec_mapDatas;
	Vector3 scale(GameSettings::BLOCK_SIZE, GameSettings::BLOCK_SIZE, GameSettings::BLOCK_SIZE);
	Vector3 offset(vec_mapDatas[0][0].size() / 2, vec_mapDatas.size() / 2, vec_mapDatas[0].size() / 2);

	output -= offset;
	output *= scale;

	return output;
}

void ButiEngine::Map::ChangeBlock(const Vector3& arg_mapPos, std::uint16_t arg_mapChipNum)
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
	auto vec_mapDatas = m_vlp_currentMapData->m_vec_mapDatas;
	for (std::uint16_t z = 0; z < vec_mapDatas[0].size(); z++)
	{
		std::vector<float> pos;
		for (std::uint16_t x = 0; x < vec_mapDatas[0][z].size(); x++)
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
	anim->GetTargetTransform()->SetLocalPositionY(arg_y);
	anim->GetTargetTransform()->RollLocalRotationX_Degrees(0.1f);

	anim->SetEaseType(Easing::EasingType::EaseInOutQuint);
}

void ButiEngine::Map::SavePlayerData()
{
	auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();

	std::string currentSceneName = sceneManager->GetCurrentScene()->GetSceneInformation()->GetSceneName();
	std::string nextSceneName;
	if (currentSceneName != "NewStageSelectScene")
	{
		return;
	}

	m_vlp_stageSelectMapData->m_vec_mapDatas[m_startPlayerPos.y][m_startPlayerPos.z][m_startPlayerPos.x] = 0;

	auto player = GetManager().lock()->GetGameObject("Player").lock()->GetGameComponent<Player>();
	Vector3 playerPos = player->GetMapPos();

}

ButiEngine::MapData::MapData(std::uint16_t arg_stageNum)
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
				{2,2,2,2,2,2,2,2,2,2,2},
				{2,2,2,2,2,2,2,2,2,2,2},
				{2,2,2,2,2,2,2,2,2,2,2},
			},
			{
				{405,0,0,0,0,0,0,0,0,0,300},
				{0,0,0,0,0,0,0,0,0,0,0},
				{406,0,0,0,0,0,0,0,0,0,301},
				{0,0,0,0,0,0,0,0,0,0,0},
				{407,0,0,0,0,0,0,0,0,0,302},
				{0,0,0,0,0,1,0,0,0,0,0},
				{408,0,0,0,0,0,0,0,0,0,303},
				{0,0,0,0,0,0,0,0,0,0,0},
				{409,0,0,0,0,0,0,0,0,0,304},
				{0,0,0,0,100,100,100,0,0,0,0},
				{410,0,0,0,0,0,0,0,0,0,311},
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
				{0,0,0,0,0,0,0,0,0,0,0},
				{0,0,0,100,0,0,0,100,0,0,0},
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
				{0,0,0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,0,0,0,0,0,0},
			},
		};
	}
}
