#include "stdafx_u.h"
#include "EyeBlock.h"
#include "Player.h"
#include "Map.h"
#include "GameSettings.h"
#include "CameraMesh.h"
#include "EasyGoal.h"
#include "DefaultGoal.h"
#include "InvisibleBlock.h"
#include "NextStageBlock.h"
#include "SeenObject.h"

void ButiEngine::EyeBlock::OnUpdate()
{
}

void ButiEngine::EyeBlock::OnSet()
{
}

void ButiEngine::EyeBlock::Start()
{
	SetLookDirection();

	m_vwp_cameraMesh = GetManager().lock()->AddObjectFromCereal("CameraMesh", ObjectFactory::Create<Transform>(Vector3(0, 0, -0.1f)));
	m_vwp_cameraMeshComponent = m_vwp_cameraMesh.lock()->GetGameComponent<CameraMesh>();

	m_vwp_mapComponent = GetManager().lock()->GetGameObject("Map").lock()->GetGameComponent<Map>();

	m_scale = 1.0f;

	m_vwp_playerComponent = GetManager().lock()->GetGameObject("Player").lock()->GetGameComponent<Player>();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::EyeBlock::Clone()
{
	return ObjectFactory::Create<EyeBlock>();
}

void ButiEngine::EyeBlock::Dead()
{
	m_vwp_cameraMesh.lock()->SetIsRemove(true);
	gameObject.lock()->SetIsRemove(true);
}

void ButiEngine::EyeBlock::CheckLookBlock()
{
	if (m_vwp_lookObject.lock())
	{
		auto seenObjectComponent = m_vwp_lookObject.lock()->GetGameComponent<SeenObject>();
		if (seenObjectComponent)
		{
			seenObjectComponent->RemoveObserverCount();
		}
	}

	Vector3 lookObjectPos = Vector3(10000, 0, 0);
	if (m_lookDirection == LookDirection::Right)
	{
		m_vwp_lookObject = GetRightBlock(lookObjectPos);
	}
	else if (m_lookDirection == LookDirection::Left)
	{
		m_vwp_lookObject = GetLeftBlock(lookObjectPos);
	}
	else if (m_lookDirection == LookDirection::Up)
	{
		m_vwp_lookObject = GetUpBlock(lookObjectPos);
	}
	else if (m_lookDirection == LookDirection::Down)
	{
		m_vwp_lookObject = GetDownBlock(lookObjectPos);
	}
	else if (m_lookDirection == LookDirection::Front)
	{
		m_vwp_lookObject = GetFrontBlock(lookObjectPos);
	}
	else if (m_lookDirection == LookDirection::Back)
	{
		m_vwp_lookObject = GetBackBlock(lookObjectPos);
	}

	if (!m_vwp_lookObject.lock())
	{
		return;
	}

	auto seenObjectComponent = m_vwp_lookObject.lock()->GetGameComponent<SeenObject>();
	if (!seenObjectComponent)
	{
		seenObjectComponent = m_vwp_lookObject.lock()->AddGameComponent<SeenObject>();
	}
	seenObjectComponent->AddObserverCount();

	if (m_vwp_lookObject.lock()->HasGameObjectTag("Goal"))
	{
		auto eGoalComp = m_vwp_lookObject.lock()->GetGameComponent<EasyGoal>();
		auto dGoalComp = m_vwp_lookObject.lock()->GetGameComponent<DefaultGoal>();
		if (eGoalComp)
		{
			eGoalComp->Seen();
		}
		else if (dGoalComp)
		{
			dGoalComp->Seen();
		}
	}
	else if (m_vwp_lookObject.lock()->HasGameObjectTag("InvisibleBlock"))
	{
		auto invBlockComp = m_vwp_lookObject.lock()->GetGameComponent<InvisibleBlock>();
		if (invBlockComp)
		{
			invBlockComp->Seen();
		}
	}
	else if (m_vwp_lookObject.lock()->HasGameObjectTag("NextStageBlock"))
	{
		auto nextStageBlockComp = m_vwp_lookObject.lock()->GetGameComponent<NextStageBlock>();
		if (nextStageBlockComp)
		{
			nextStageBlockComp->Seen();
		}
	}
}

void ButiEngine::EyeBlock::FlashMeshSet(const Vector3& arg_playerPos)
{
	Vector3 lookObjectPos = Vector3(10000, 0, 0);

	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	Vector3 bPos = pos;
	m_playerPos = arg_playerPos;

	if (m_lookDirection == LookDirection::Right)
	{
		bPos.x += 100;
		GetRightBlock(lookObjectPos);
	}
	else if (m_lookDirection == LookDirection::Left)
	{
		bPos.x -= 100;
		GetLeftBlock(lookObjectPos);
	}
	else if (m_lookDirection == LookDirection::Up)
	{
		bPos.y += 100;
		GetUpBlock(lookObjectPos);
	}
	else if (m_lookDirection == LookDirection::Down)
	{
		bPos.y -= 100;
		GetDownBlock(lookObjectPos);
	}
	else if (m_lookDirection == LookDirection::Front)
	{
		bPos.z += 100;
		GetFrontBlock(lookObjectPos);
	}
	else if (m_lookDirection == LookDirection::Back)
	{
		bPos.z -= 100;
		GetBackBlock(lookObjectPos);
	}

	if (lookObjectPos.x != 10000)
	{
		bPos = lookObjectPos;
	}
	Vector3 midPoint = Vector3((pos.x + bPos.x) * 0.5f, (pos.y + bPos.y) * 0.5f, (pos.z + bPos.z) * 0.5f);

	m_vwp_cameraMesh.lock()->transform->SetWorldPosition(midPoint);

	Vector3 cameraMeshScale = Vector3Const::Zero;
	if (m_lookDirection == LookDirection::Right || m_lookDirection == LookDirection::Left)
	{
		cameraMeshScale = Vector3(pos.Distance(bPos), 0.1f, 0.1f);
	}
	else if (m_lookDirection == LookDirection::Up || m_lookDirection == LookDirection::Down)
	{
		cameraMeshScale = Vector3(0.1f, pos.Distance(bPos), 0.1f);
	}
	else if (m_lookDirection == LookDirection::Front || m_lookDirection == LookDirection::Back)
	{
		cameraMeshScale = Vector3(0.1f, 0.1f, pos.Distance(bPos));
	}
	m_vwp_cameraMesh.lock()->transform->SetLocalScale(cameraMeshScale);
}

void ButiEngine::EyeBlock::Flash()
{
	m_vwp_cameraMeshComponent.lock()->Flash();
}

void ButiEngine::EyeBlock::Expansion()
{
	m_scale = 1.5f;
	gameObject.lock()->transform->SetLocalScale(m_scale);
}

void ButiEngine::EyeBlock::Shrink()
{
	if (m_scale <= 1.0f)
	{
		return;
	}
	m_scale -= 0.1f;
	if (m_scale < 1.0f)
	{
		m_scale = 1.0f;
	}
	gameObject.lock()->transform->SetLocalScale(m_scale);
}

void ButiEngine::EyeBlock::SetLookDirection()
{
	Vector3 dir = gameObject.lock()->transform->GetFront().Round();

	if (dir.x == 1.0f)
	{
		m_lookDirection = LookDirection::Right;
	}
	else if (dir.x == -1.0f)
	{
		m_lookDirection = LookDirection::Left;
	}
	else if (dir.y == 1.0f)
	{
		m_lookDirection = LookDirection::Up;
	}
	else if (dir.y == -1.0f)
	{
		m_lookDirection = LookDirection::Down;
	}
	else if (dir.z == 1.0f)
	{
		m_lookDirection = LookDirection::Front;
	}
	else if (dir.z == -1.0f)
	{
		m_lookDirection = LookDirection::Back;
	}
}

ButiEngine::Value_weak_ptr<ButiEngine::GameObject> ButiEngine::EyeBlock::GetRightBlock(Vector3& ref_output_pos)
{
	auto mapData = m_vwp_mapComponent.lock()->GetCurrentMapData().lock()->m_vec_mapDatas;
	for (std::uint16_t i = m_mapPos.x + 1; i < mapData[m_mapPos.y][m_mapPos.z].size(); i++)
	{
		Vector3 pos = m_mapPos;
		pos.x = i;

		if (pos == m_playerPos)
		{
			ref_output_pos = m_vwp_mapComponent.lock()->GetWorldPos(pos);
			return Value_weak_ptr<GameObject>();
		}
		else if (IsBlock(mapData[pos.y][pos.z][pos.x]))
		{
			ref_output_pos = m_vwp_mapComponent.lock()->GetWorldPos(pos);
			auto mapObjectData = m_vwp_mapComponent.lock()->GetMapObjectData();
			return mapObjectData[pos.y][pos.z][pos.x];
		}
	}
	return Value_weak_ptr<GameObject>();
}

ButiEngine::Value_weak_ptr<ButiEngine::GameObject> ButiEngine::EyeBlock::GetLeftBlock(Vector3& ref_output_pos)
{
	auto mapData = m_vwp_mapComponent.lock()->GetCurrentMapData().lock()->m_vec_mapDatas;
	for (std::int8_t i = m_mapPos.x - 1; i >= 0; i--)
	{
		Vector3 pos = m_mapPos;
		pos.x = i;

		if (pos == m_playerPos)
		{
			ref_output_pos = m_vwp_mapComponent.lock()->GetWorldPos(pos);
			return Value_weak_ptr<GameObject>();
		}
		else if (IsBlock(mapData[pos.y][pos.z][pos.x]))
		{
			ref_output_pos = m_vwp_mapComponent.lock()->GetWorldPos(pos);
			auto mapObjectData = m_vwp_mapComponent.lock()->GetMapObjectData();
			return mapObjectData[pos.y][pos.z][pos.x];
		}
	}
	return Value_weak_ptr<GameObject>();
}

ButiEngine::Value_weak_ptr<ButiEngine::GameObject> ButiEngine::EyeBlock::GetUpBlock(Vector3& ref_output_pos)
{
	auto mapData = m_vwp_mapComponent.lock()->GetCurrentMapData().lock()->m_vec_mapDatas;
	for (std::uint16_t i = m_mapPos.y + 1; i < mapData.size(); i++)
	{
		Vector3 pos = m_mapPos;
		pos.y = i;

		if (pos == m_playerPos)
		{
			ref_output_pos = m_vwp_mapComponent.lock()->GetWorldPos(pos);
			return Value_weak_ptr<GameObject>();
		}
		else if (IsBlock(mapData[pos.y][pos.z][pos.x]))
		{
			ref_output_pos = m_vwp_mapComponent.lock()->GetWorldPos(pos);
			auto mapObjectData = m_vwp_mapComponent.lock()->GetMapObjectData();
			return mapObjectData[pos.y][pos.z][pos.x];
		}
	}
	return Value_weak_ptr<GameObject>();
}

ButiEngine::Value_weak_ptr<ButiEngine::GameObject> ButiEngine::EyeBlock::GetDownBlock(Vector3& ref_output_pos)
{
	auto mapData = m_vwp_mapComponent.lock()->GetCurrentMapData().lock()->m_vec_mapDatas;
	for (std::int8_t i = m_mapPos.y - 1; i >= 0; i--)
	{
		Vector3 pos = m_mapPos;
		pos.y = i;

		if (pos == m_playerPos)
		{
			ref_output_pos = m_vwp_mapComponent.lock()->GetWorldPos(pos);
			return Value_weak_ptr<GameObject>();
		}
		else if (IsBlock(mapData[pos.y][pos.z][pos.x]))
		{
			ref_output_pos = m_vwp_mapComponent.lock()->GetWorldPos(pos);
			auto mapObjectData = m_vwp_mapComponent.lock()->GetMapObjectData();
			return mapObjectData[pos.y][pos.z][pos.x];
		}
	}
	return Value_weak_ptr<GameObject>();
}

ButiEngine::Value_weak_ptr<ButiEngine::GameObject> ButiEngine::EyeBlock::GetFrontBlock(Vector3& ref_output_pos)
{
	auto mapData = m_vwp_mapComponent.lock()->GetCurrentMapData().lock()->m_vec_mapDatas;
	for (std::uint16_t i = m_mapPos.z + 1; i < mapData[m_mapPos.y].size(); i++)
	{
		Vector3 pos = m_mapPos;
		pos.z = i;

		if (pos == m_playerPos)
		{
			ref_output_pos = m_vwp_mapComponent.lock()->GetWorldPos(pos);
			return Value_weak_ptr<GameObject>();
		}
		else if (IsBlock(mapData[pos.y][pos.z][pos.x]))
		{
			ref_output_pos = m_vwp_mapComponent.lock()->GetWorldPos(pos);
			auto mapObjectData = m_vwp_mapComponent.lock()->GetMapObjectData();
			return mapObjectData[pos.y][pos.z][pos.x];
		}
	}
	return Value_weak_ptr<GameObject>();
}

ButiEngine::Value_weak_ptr<ButiEngine::GameObject> ButiEngine::EyeBlock::GetBackBlock(Vector3& ref_output_pos)
{
	auto mapData = m_vwp_mapComponent.lock()->GetCurrentMapData().lock()->m_vec_mapDatas;
	for (std::int8_t i = m_mapPos.z - 1; i >= 0; i--)
	{
		Vector3 pos = m_mapPos;
		pos.z = i;

		if (pos == m_playerPos)
		{
			ref_output_pos = m_vwp_mapComponent.lock()->GetWorldPos(pos);
			return Value_weak_ptr<GameObject>();
		}
		else if (IsBlock(mapData[pos.y][pos.z][pos.x]))
		{
			ref_output_pos = m_vwp_mapComponent.lock()->GetWorldPos(pos);
			auto mapObjectData = m_vwp_mapComponent.lock()->GetMapObjectData();
			return mapObjectData[pos.y][pos.z][pos.x];
		}
	}
	return Value_weak_ptr<GameObject>();
}

bool ButiEngine::EyeBlock::IsBlock(std::uint16_t arg_mapChipNum)
{
	if (arg_mapChipNum == GameSettings::MAP_CHIP_BLOCK) { return true; }
	if (arg_mapChipNum == GameSettings::MAP_CHIP_TUTORIALGOAL) { return true; }
	if (arg_mapChipNum == GameSettings::MAP_CHIP_EASYGOAL) { return true; }
	if (arg_mapChipNum == GameSettings::MAP_CHIP_DEFAULTGOAL) { return true; }
	if (arg_mapChipNum >= GameSettings::MAP_CHIP_INVISIBLEBLOCK) { return true; }
	if (arg_mapChipNum >= GameSettings::MAP_CHIP_NEXT_STAGE_BLOCK) { return true; }
	return false;
}
