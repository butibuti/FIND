#include "stdafx_u.h"
#include "Player.h"
#include "Map.h"
#include "EasyGoal.h"
#include "DefaultGoal.h"
#include "InvisibleBlockManager.h"
#include "GameSettings.h"
#include "Ripple.h"
#include "StartPlayerDirecting.h"
#include "InvisibleBlock.h"
#include "Shake.h"
#include "InputManager.h"
#include "CameraMesh.h"

void ButiEngine::Player::OnUpdate()
{
	auto directing = gameObject.lock()->GetGameComponent<StartPlayerDirecting>();
	if (gameObject.lock()->transform->GetWorldPosition().y < -40.0f)
	{
		m_vlp_fallTimer->Stop();
		m_isFall = false;
		m_isFallStart = false;
		m_mapPos = m_vwp_mapComponent.lock()->GetPlayerPos();
		auto rotation = gameObject.lock()->transform->GetLocalRotation().GetEulerOneValue_local();
		rotation.x = 0;
		rotation.y = MathHelper::ToRadian(m_startRotation);
		rotation.z = 0;
		gameObject.lock()->transform->SetLocalRotation(Matrix4x4().CreateFromEuler_local(rotation));
		gameObject.lock()->transform->SetWorldPosition(directing->GetSpawnPos());
		directing->Animation();
	}

	if (!directing->IsStart())
	{
		return;
	}
	if (!m_isGoal)
	{
		Contoroll();
	}
	if (m_vlp_timer->Update())
	{
		m_vlp_timer->Stop();
		Expansion();

		int rand = ButiRandom::GetRandom(0, 2, 1);
		gameObject.lock()->GetApplication().lock()->GetSoundManager()->PlaySE(m_moveSounds[rand], 0.1f);

		//”g–ä
		auto pos = gameObject.lock()->transform->GetWorldPosition();
		pos.y -= 0.3f;
		GetManager().lock()->AddObjectFromCereal("Ripple", ObjectFactory::Create<Transform>(pos, Vector3(90, 0, 0), 0.0f));

		CheckLookBlock();
		//ƒtƒ‰ƒbƒVƒ…
		GetManager().lock()->GetGameObject("CameraMesh").lock()->GetGameComponent<CameraMesh>()->Flash();
		m_vwp_invisibleBlockManagerComponent.lock()->CheckSeen();
		CheckExistUnderBlock(m_mapPos);

		CheckGoal();
	}
	Shrink();
	Fall();
}

void ButiEngine::Player::OnSet()
{
}

void ButiEngine::Player::OnRemove()
{
}

void ButiEngine::Player::OnShowUI()
{
}

void ButiEngine::Player::Start()
{
	for (std::uint8_t i = 0; i < 3; i++) {

		m_moveSounds[i] = SoundTag("Sound/Move_" + std::to_string(i) + ".wav");
	}

	m_isGoal = false;
	m_isFall = false;
	m_isFallStart = false;
	m_afterFallPos = Vector3Const::Zero;
	m_length = 1.0f;
	m_vwp_mapComponent = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("Map").lock()->GetGameComponent<Map>();
	m_mapPos = m_vwp_mapComponent.lock()->GetPlayerPos();
	m_offset = m_mapPos - m_startPos;

	m_vlp_timer = ObjectFactory::Create<RelativeTimer>(11);
	m_vlp_fallTimer = ObjectFactory::Create<RelativeTimer>(24);
	m_vlp_fallTimer->Stop();
	m_vwp_invisibleBlockManagerComponent = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("InvisibleBlockManager").lock()->GetGameComponent<InvisibleBlockManager>();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Player::Clone()
{
	return ObjectFactory::Create<Player>();
}

bool ButiEngine::Player::IsRollFinish()
{
	return !m_vlp_timer->IsOn();
}

void ButiEngine::Player::CheckLookBlock()
{
	m_vwp_invisibleBlockManagerComponent.lock()->Reset();
	CheckLookDirection();
	Value_weak_ptr<GameObject> lookObject;

	Vector3 pos = gameObject.lock()->transform->GetWorldPosition();
	Vector3 bPos = pos;

	if (m_lookDirection == LookDirection::Right)
	{
		bPos.x += 100;
		lookObject = GetRightBlock(m_mapPos);
	}
	else if (m_lookDirection == LookDirection::Left)
	{
		bPos.x -= 100;
		lookObject = GetLeftBlock(m_mapPos);
	}
	else if (m_lookDirection == LookDirection::Up)
	{
		bPos.y += 100;
		lookObject = GetUpBlock(m_mapPos);
	}
	else if (m_lookDirection == LookDirection::Down)
	{
		bPos.y -= 100;
		lookObject = GetDownBlock(m_mapPos);
	}
	else if (m_lookDirection == LookDirection::Front)
	{
		bPos.z += 100;
		lookObject = GetFrontBlock(m_mapPos);
	}
	else if (m_lookDirection == LookDirection::Back)
	{
		bPos.z -= 100;
		lookObject = GetBackBlock(m_mapPos);
	}

	if (lookObject.lock())
	{
		bPos = lookObject.lock()->transform->GetWorldPosition();
	}
	Vector3 midPoint = Vector3((pos.x + bPos.x) * 0.5f, (pos.y + bPos.y) * 0.5f, (pos.z + bPos.z) * 0.5f);

	auto cameraMesh = GetManager().lock()->GetGameObject("CameraMesh");
	cameraMesh.lock()->transform->SetWorldPosition(midPoint);

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
	cameraMesh.lock()->transform->SetLocalScale(cameraMeshScale);

	if (!lookObject.lock())
	{
		return;
	}

	if (lookObject.lock()->HasGameObjectTag("Goal"))
	{
		auto eGoalComp = lookObject.lock()->GetGameComponent<EasyGoal>();
		auto dGoalComp = lookObject.lock()->GetGameComponent<DefaultGoal>();
		if (eGoalComp)
		{
			eGoalComp->Seen();
		}
		else if (dGoalComp)
		{
			dGoalComp->Seen();
		}
	}
	else if (lookObject.lock()->HasGameObjectTag("InvisibleBlock"))
	{
		auto invBlockComp = lookObject.lock()->GetGameComponent<InvisibleBlock>();
		if (invBlockComp)
		{
			invBlockComp->Seen();
		}
	}
}

void ButiEngine::Player::RollCameraDirection(const std::uint8_t arg_rotateDir)
{
	m_cameraDirection = (CameraDirection)((std::uint8_t)m_cameraDirection + arg_rotateDir);

	if (m_cameraDirection > CameraDirection::Right) {
		m_cameraDirection = (CameraDirection)((std::uint8_t)m_cameraDirection % 4);
	}
	else if (m_cameraDirection < CameraDirection::Front) {
		m_cameraDirection = (CameraDirection)((std::uint8_t)m_cameraDirection + 4);
	}
}

void ButiEngine::Player::CheckGoal()
{
	if (m_isGoal)
	{
		return;
	}

	std::vector<std::vector<std::vector<std::uint8_t>>>& vec_mapDatas = m_vwp_mapComponent.lock()->GetCurrentMapData().lock()->m_vec_mapDatas;
	std::uint8_t mapNum = vec_mapDatas[m_mapPos.y][m_mapPos.z][m_mapPos.x];
	auto hitObject = m_vwp_mapComponent.lock()->GetMapObjectData()[m_mapPos.y][m_mapPos.z][m_mapPos.x];

	if (!hitObject.lock() || !hitObject.lock()->HasGameObjectTag("Goal"))
	{
		return;
	}

	if (mapNum >= GameSettings::MAP_CHIP_PLAYER_AND_GOAL)
	{
		mapNum = (mapNum - GameSettings::MAP_CHIP_PLAYER_AND_GOAL) / 10;
	}

	if (mapNum == GameSettings::MAP_CHIP_TUTORIALGOAL)
	{
		m_isGoal = true;
	}
	else if (mapNum == GameSettings::MAP_CHIP_EASYGOAL && hitObject.lock()->GetGameComponent<EasyGoal>()->IsActive())
	{
		m_isGoal = true;
	}
	else if (mapNum == GameSettings::MAP_CHIP_DEFAULTGOAL && hitObject.lock()->GetGameComponent<DefaultGoal>()->IsActive())
	{
		m_isGoal = true;
	}

	if (m_isGoal)
	{
		gameObject.lock()->GetApplication().lock()->GetSoundManager()->PlaySE(SoundTag("Sound/TouchGoal.wav"), 0.1f);
	}
}

void ButiEngine::Player::CheckLookDirection()
{
	Vector3 front = gameObject.lock()->transform->GetFront() * 10.0f;
	Vector3 dir = front - gameObject.lock()->transform->GetWorldPosition();

	float x = abs(dir.x);
	float y = abs(dir.y);
	float z = abs(dir.z);

	if (x > y && x > z)
	{
		if (dir.x > 0)
		{
			m_lookDirection = LookDirection::Right;
		}
		if (dir.x < 0)
		{
			m_lookDirection = LookDirection::Left;
		}
	}

	if (y > x && y > z)
	{
		if (dir.y > 0)
		{
			m_lookDirection = LookDirection::Up;
		}
		if (dir.y < 0)
		{
			m_lookDirection = LookDirection::Down;
		}
	}

	if (z > x && z > y)
	{
		if (dir.z > 0)
		{
			m_lookDirection = LookDirection::Front;
		}
		if (dir.z < 0)
		{
			m_lookDirection = LookDirection::Back;
		}
	}
}

void ButiEngine::Player::Contoroll()
{
	auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	if (anim) { return; }
	if (InputManager::IsPushRightKey())
	{
		switch (m_cameraDirection)
		{
		case CameraDirection::Front:
		{
			OnPushRight();
		}
		break;
		case CameraDirection::Right:
		{
			OnPushFront();
		}
		break;
		case CameraDirection::Back:
		{
			OnPushLeft();
		}
		break;
		case CameraDirection::Left:
		{
			OnPushBack();
		}
		break;
		default:
			break;
		}
	}
	else if (InputManager::IsPushLeftKey())
	{

		switch (m_cameraDirection)
		{
		case CameraDirection::Front:
		{
			OnPushLeft();
		}
		break;
		case CameraDirection::Right:
		{
			OnPushBack();
		}
		break;
		case CameraDirection::Back:
		{
			OnPushRight();
		}
		break;
		case CameraDirection::Left:
		{
			OnPushFront();
		}
		break;
		default:
			break;
		}
	}
	if (InputManager::IsPushFrontKey())
	{
		switch (m_cameraDirection)
		{
		case CameraDirection::Front:
		{
			OnPushFront();
		}
		break;
		case CameraDirection::Right:
		{
			OnPushLeft();
		}
		break;
		case CameraDirection::Back:
		{
			OnPushBack();
		}
		break;
		case CameraDirection::Left:
		{
			OnPushRight();
		}
		break;
		default:
			break;
		}

	}
	else if (InputManager::IsPushBackKey())
	{
		switch (m_cameraDirection)
		{
		case CameraDirection::Front:
		{
			OnPushBack();
		}
		break;
		case CameraDirection::Right:
		{
			OnPushRight();
		}
		break;
		case CameraDirection::Back:
		{
			OnPushFront();
		}
		break;
		case CameraDirection::Left:
		{
			OnPushLeft();
		}
		break;
		default:
			break;
		}
	}
}

void ButiEngine::Player::OnPushRight()
{
	if (m_vlp_fallTimer->IsOn()) { return; }
	if (!m_isFall)
	{
		MoveDirection dir = CheckMoveDirection(Vector3(m_mapPos.x + 1, m_mapPos.y, m_mapPos.z));
		if (dir == MoveDirection::Up) { MoveRightUp(); }
		else if (dir == MoveDirection::Normal) { MoveRight(); }
		else if (dir == MoveDirection::Down) { MoveRightDown(); }
		else if (dir == MoveDirection::Fall) { MoveRightDown(); }
	}


	auto cubeAnim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	if (cubeAnim) {
		m_nextMapPos = cubeAnim->GetTargetTransform()->GetWorldPosition().Ceil();
		m_nextMapPos += m_offset;
	}
}

void ButiEngine::Player::OnPushLeft()
{
	if (m_vlp_fallTimer->IsOn()) { return; }
	if (!m_isFall)
	{
		MoveDirection dir = CheckMoveDirection(Vector3(m_mapPos.x - 1, m_mapPos.y, m_mapPos.z));
		if (dir == MoveDirection::Up) { MoveLeftUp(); }
		else if (dir == MoveDirection::Normal) { MoveLeft(); }
		else if (dir == MoveDirection::Down) { MoveLeftDown(); }
		else if (dir == MoveDirection::Fall) { MoveLeftDown(); }
	}


	auto cubeAnim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	if (cubeAnim) {
		m_nextMapPos = cubeAnim->GetTargetTransform()->GetWorldPosition().Ceil();
		m_nextMapPos += m_offset;
	}
}

void ButiEngine::Player::OnPushFront()
{
	if (m_vlp_fallTimer->IsOn()) { return; }
	if (!m_isFall)
	{
		MoveDirection dir = CheckMoveDirection(Vector3(m_mapPos.x, m_mapPos.y, m_mapPos.z + 1));
		if (dir == MoveDirection::Up) { MoveUpFront(); }
		else if (dir == MoveDirection::Normal) { MoveFront(); }
		else if (dir == MoveDirection::Down) { MoveDownFront(); }
		else if (dir == MoveDirection::Fall) { MoveDownFront(); }
	}

	auto cubeAnim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	if (cubeAnim) {
		m_nextMapPos = cubeAnim->GetTargetTransform()->GetWorldPosition().Ceil();
		m_nextMapPos += m_offset;
	}
}

void ButiEngine::Player::OnPushBack()
{
	if (m_vlp_fallTimer->IsOn()) { return; }
	if (!m_isFall)
	{
		MoveDirection dir = CheckMoveDirection(Vector3(m_mapPos.x, m_mapPos.y, m_mapPos.z - 1));
		if (dir == MoveDirection::Up) { MoveUpBack(); }
		else if (dir == MoveDirection::Normal) { MoveBack(); }
		else if (dir == MoveDirection::Down) { MoveDownBack(); }
		else if (dir == MoveDirection::Fall) { MoveDownBack(); }
	}

	auto cubeAnim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	if (cubeAnim) {
		m_nextMapPos = cubeAnim->GetTargetTransform()->GetWorldPosition().Ceil();
		m_nextMapPos += m_offset;
	}
}

void ButiEngine::Player::Expansion()
{
	m_scale = 1.5f;
	gameObject.lock()->transform->SetLocalScale(m_scale);
}

void ButiEngine::Player::Shrink()
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

void ButiEngine::Player::MoveRightUp()
{
	auto t = gameObject.lock()->transform;
	auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	if (!anim)
	{
		m_vlp_timer->Reset();
		m_vlp_timer->Start();
		anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
		anim->SetSpeed(1.0f / 10);
		anim->SetTargetTransform(t->Clone());
		anim->GetTargetTransform()->Translate(Vector3(m_length, m_length, 0));

		anim->GetTargetTransform()->RollWorldRotationZ_Degrees(-179.9999f);
		anim->SetEaseType(Easing::EasingType::Liner);
		anim->SetXEaseType(Easing::EasingType::CubeRotateMin180);
		anim->SetYEaseType(Easing::EasingType::CubeRotate180);

		m_mapPos.x++;
		m_mapPos.y++;
	}
}

void ButiEngine::Player::MoveRight()
{
	auto t = gameObject.lock()->transform;
	auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	if (!anim)
	{
		m_vlp_timer->Reset();
		m_vlp_timer->Start();
		anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
		anim->SetSpeed(1.0f / 10);
		anim->SetTargetTransform(t->Clone());
		anim->GetTargetTransform()->TranslateX(m_length);

		anim->GetTargetTransform()->RollWorldRotationZ_Degrees(-90.0f);
		anim->SetEaseType(Easing::EasingType::Liner);
		anim->SetYEaseType(Easing::EasingType::CubeRotate90);

		m_mapPos.x++;
	}
}

void ButiEngine::Player::MoveRightDown()
{
	auto t = gameObject.lock()->transform;
	auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	if (!anim)
	{
		m_vlp_timer->Reset();
		m_vlp_timer->Start();
		anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
		anim->SetSpeed(1.0f / 10);
		anim->SetTargetTransform(t->Clone());
		anim->GetTargetTransform()->Translate(Vector3(m_length, -m_length, 0));

		anim->GetTargetTransform()->RollWorldRotationZ_Degrees(-179.9999f);
		anim->SetEaseType(Easing::EasingType::Liner);
		anim->SetXEaseType(Easing::EasingType::CubeRotate180);
		anim->SetYEaseType(Easing::EasingType::CubeRotate180);

		if (m_isFall) { return; }
		m_mapPos.x++;
		m_mapPos.y--;
	}
}

void ButiEngine::Player::MoveLeftUp()
{
	auto t = gameObject.lock()->transform;
	auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	if (!anim)
	{
		m_vlp_timer->Reset();
		m_vlp_timer->Start();
		anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
		anim->SetSpeed(1.0f / 10);
		anim->SetTargetTransform(t->Clone());
		anim->GetTargetTransform()->Translate(Vector3(-m_length, m_length, 0));

		anim->GetTargetTransform()->RollWorldRotationZ_Degrees(179.9999f);
		anim->SetEaseType(Easing::EasingType::Liner);
		anim->SetXEaseType(Easing::EasingType::CubeRotate180);
		anim->SetYEaseType(Easing::EasingType::CubeRotate180);

		m_mapPos.x--;
		m_mapPos.y++;
	}
}

void ButiEngine::Player::MoveLeft()
{
	auto t = gameObject.lock()->transform;
	auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	if (!anim)
	{
		m_vlp_timer->Reset();
		m_vlp_timer->Start();
		anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
		anim->SetSpeed(1.0f / 10);
		anim->SetTargetTransform(t->Clone());
		anim->GetTargetTransform()->TranslateX(-m_length);

		anim->GetTargetTransform()->RollWorldRotationZ_Degrees(90.0f);
		anim->SetEaseType(Easing::EasingType::Liner);
		anim->SetYEaseType(Easing::EasingType::CubeRotate90);

		m_mapPos.x--;
	}
}

void ButiEngine::Player::MoveLeftDown()
{
	auto t = gameObject.lock()->transform;
	auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	if (!anim)
	{
		m_vlp_timer->Reset();
		m_vlp_timer->Start();
		anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
		anim->SetSpeed(1.0f / 10);
		anim->SetTargetTransform(t->Clone());
		anim->GetTargetTransform()->Translate(Vector3(-m_length, -m_length, 0));

		anim->GetTargetTransform()->RollWorldRotationZ_Degrees(179.9999f);
		anim->SetEaseType(Easing::EasingType::Liner);
		anim->SetXEaseType(Easing::EasingType::CubeRotateMin180);
		anim->SetYEaseType(Easing::EasingType::CubeRotate180);

		if (m_isFall) { return; }
		m_mapPos.x--;
		m_mapPos.y--;
	}
}

void ButiEngine::Player::MoveUpFront()
{
	auto t = gameObject.lock()->transform;
	auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	if (!anim)
	{
		m_vlp_timer->Reset();
		m_vlp_timer->Start();
		anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
		anim->SetSpeed(1.0f / 10);
		anim->SetTargetTransform(t->Clone());
		anim->GetTargetTransform()->Translate(Vector3(0, m_length, m_length));

		anim->GetTargetTransform()->RollWorldRotationX_Degrees(179.9999f);
		anim->SetEaseType(Easing::EasingType::Liner);
		anim->SetYEaseType(Easing::EasingType::CubeRotate180);
		anim->SetZEaseType(Easing::EasingType::CubeRotateMin180);

		m_mapPos.y++;
		m_mapPos.z++;
	}
}

void ButiEngine::Player::MoveFront()
{
	auto t = gameObject.lock()->transform;
	auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	if (!anim)
	{
		m_vlp_timer->Reset();
		m_vlp_timer->Start();
		anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
		anim->SetSpeed(1.0f / 10);
		anim->SetTargetTransform(t->Clone());
		anim->GetTargetTransform()->TranslateZ(m_length);

		anim->GetTargetTransform()->RollWorldRotationX_Degrees(90.0f);
		anim->SetEaseType(Easing::EasingType::Liner);
		anim->SetYEaseType(Easing::EasingType::CubeRotate90);

		m_mapPos.z++;
	}
}

void ButiEngine::Player::MoveDownFront()
{
	auto t = gameObject.lock()->transform;
	auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	if (!anim)
	{
		m_vlp_timer->Reset();
		m_vlp_timer->Start();
		anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
		anim->SetSpeed(1.0f / 10);
		anim->SetTargetTransform(t->Clone());
		anim->GetTargetTransform()->Translate(Vector3(0, -m_length, m_length));

		anim->GetTargetTransform()->RollWorldRotationX_Degrees(179.9999f);
		anim->SetEaseType(Easing::EasingType::Liner);
		anim->SetYEaseType(Easing::EasingType::CubeRotate180);
		anim->SetZEaseType(Easing::EasingType::CubeRotate180);

		if (m_isFall) { return; }
		m_mapPos.y--;
		m_mapPos.z++;
	}
}

void ButiEngine::Player::MoveUpBack()
{
	auto t = gameObject.lock()->transform;
	auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	if (!anim)
	{
		m_vlp_timer->Reset();
		m_vlp_timer->Start();
		anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
		anim->SetSpeed(1.0f / 10);
		anim->SetTargetTransform(t->Clone());
		anim->GetTargetTransform()->Translate(Vector3(0, m_length, -m_length));

		anim->GetTargetTransform()->RollWorldRotationX_Degrees(-179.9999f);
		anim->SetEaseType(Easing::EasingType::Liner);
		anim->SetYEaseType(Easing::EasingType::CubeRotate180);
		anim->SetZEaseType(Easing::EasingType::CubeRotate180);

		m_mapPos.y++;
		m_mapPos.z--;
	}
}

void ButiEngine::Player::MoveBack()
{
	auto t = gameObject.lock()->transform;
	auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	if (!anim)
	{
		m_vlp_timer->Reset();
		m_vlp_timer->Start();
		anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
		anim->SetSpeed(1.0f / 10);
		anim->SetTargetTransform(t->Clone());
		anim->GetTargetTransform()->TranslateZ(-m_length);

		anim->GetTargetTransform()->RollWorldRotationX_Degrees(-90.0f);
		anim->SetEaseType(Easing::EasingType::Liner);
		anim->SetYEaseType(Easing::EasingType::CubeRotate90);

		m_mapPos.z--;
	}
}

void ButiEngine::Player::MoveDownBack()
{
	auto t = gameObject.lock()->transform;
	auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	if (!anim)
	{
		m_vlp_timer->Reset();
		m_vlp_timer->Start();
		anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
		anim->SetSpeed(1.0f / 10);
		anim->SetTargetTransform(t->Clone());
		anim->GetTargetTransform()->Translate(Vector3(0, -m_length, -m_length));

		anim->GetTargetTransform()->RollWorldRotationX_Degrees(-179.9999f);
		anim->SetEaseType(Easing::EasingType::Liner);
		anim->SetYEaseType(Easing::EasingType::CubeRotate180);
		anim->SetZEaseType(Easing::EasingType::CubeRotateMin180);

		if (m_isFall) { return; }
		m_mapPos.y--;
		m_mapPos.z--;
	}
}

ButiEngine::Value_weak_ptr<ButiEngine::GameObject> ButiEngine::Player::GetRightBlock(const Vector3& arg_mapPos)
{
	auto mapData = m_vwp_mapComponent.lock()->GetCurrentMapData().lock()->m_vec_mapDatas;
	for (std::uint8_t i = m_mapPos.x + 1; i < mapData[m_mapPos.y][m_mapPos.z].size(); i++)
	{
		if (IsBlock(mapData[m_mapPos.y][m_mapPos.z][i]))
		{
			auto mapObjectData = m_vwp_mapComponent.lock()->GetMapObjectData();
			return mapObjectData[m_mapPos.y][m_mapPos.z][i];
		}
	}
	return Value_weak_ptr<GameObject>();
}

ButiEngine::Value_weak_ptr<ButiEngine::GameObject> ButiEngine::Player::GetLeftBlock(const Vector3& arg_mapPos)
{
	auto mapData = m_vwp_mapComponent.lock()->GetCurrentMapData().lock()->m_vec_mapDatas;
	for (std::int8_t i = m_mapPos.x - 1; i >= 0; i--)
	{
		if (IsBlock(mapData[m_mapPos.y][m_mapPos.z][i]))
		{
			auto mapObjectData = m_vwp_mapComponent.lock()->GetMapObjectData();
			return mapObjectData[m_mapPos.y][m_mapPos.z][i];
		}
	}
	return Value_weak_ptr<GameObject>();
}

ButiEngine::Value_weak_ptr<ButiEngine::GameObject> ButiEngine::Player::GetUpBlock(const Vector3& arg_mapPos)
{
	auto mapData = m_vwp_mapComponent.lock()->GetCurrentMapData().lock()->m_vec_mapDatas;
	for (std::uint8_t i = m_mapPos.y + 1; i < mapData.size(); i++)
	{
		if (IsBlock(mapData[i][m_mapPos.z][m_mapPos.x]))
		{
			auto mapObjectData = m_vwp_mapComponent.lock()->GetMapObjectData();
			return mapObjectData[i][m_mapPos.z][m_mapPos.x];
		}
	}
	return Value_weak_ptr<GameObject>();
}

ButiEngine::Value_weak_ptr<ButiEngine::GameObject> ButiEngine::Player::GetDownBlock(const Vector3& arg_mapPos)
{
	auto mapData = m_vwp_mapComponent.lock()->GetCurrentMapData().lock()->m_vec_mapDatas;
	for (std::int8_t i = m_mapPos.y - 1; i >= 0; i--)
	{
		if (IsBlock(mapData[i][m_mapPos.z][m_mapPos.x]))
		{
			auto mapObjectData = m_vwp_mapComponent.lock()->GetMapObjectData();
			return mapObjectData[i][m_mapPos.z][m_mapPos.x];
		}
	}
	return Value_weak_ptr<GameObject>();
}

ButiEngine::Value_weak_ptr<ButiEngine::GameObject> ButiEngine::Player::GetFrontBlock(const Vector3& arg_mapPos)
{
	auto mapData = m_vwp_mapComponent.lock()->GetCurrentMapData().lock()->m_vec_mapDatas;
	for (std::uint8_t i = m_mapPos.z + 1; i < mapData[m_mapPos.y].size(); i++)
	{
		if (IsBlock(mapData[m_mapPos.y][i][m_mapPos.x]))
		{
			auto mapObjectData = m_vwp_mapComponent.lock()->GetMapObjectData();
			return mapObjectData[m_mapPos.y][i][m_mapPos.x];
		}
	}
	return Value_weak_ptr<GameObject>();
}

ButiEngine::Value_weak_ptr<ButiEngine::GameObject> ButiEngine::Player::GetBackBlock(const Vector3& arg_mapPos)
{
	auto mapData = m_vwp_mapComponent.lock()->GetCurrentMapData().lock()->m_vec_mapDatas;
	for (std::int8_t i = m_mapPos.z - 1; i >= 0; i--)
	{
		if (IsBlock(mapData[m_mapPos.y][i][m_mapPos.x]))
		{
			auto mapObjectData = m_vwp_mapComponent.lock()->GetMapObjectData();
			return mapObjectData[m_mapPos.y][i][m_mapPos.x];
		}
	}
	return Value_weak_ptr<GameObject>();
}

void ButiEngine::Player::Fall()
{
	auto shakeComponent = gameObject.lock()->GetGameComponent<Shake>();
	if (!m_isFall && m_isFallStart && !m_vlp_fallTimer->IsOn())
	{
		m_vlp_fallTimer->Reset();
		m_vlp_fallTimer->Start();
		shakeComponent->SetDefaultPos(gameObject.lock()->transform->GetWorldPosition());
		shakeComponent->ShakeStart(0.06f);
		m_isFallStart = false;
	}

	if (!m_isFall && m_vlp_fallTimer->Update())
	{
		m_vlp_fallTimer->Stop();
		m_isFall = true;
	}

	if (!m_isFall && m_vlp_fallTimer->GetRemainFrame() == 6)
	{
		shakeComponent->ShakeStop();
	}

	if (m_isFall)
	{
		gameObject.lock()->transform->TranslateY(-3.0f);
		if (gameObject.lock()->transform->GetWorldPosition().y <= m_afterFallPos.y)
		{
			gameObject.lock()->transform->SetWorldPosition(m_afterFallPos);
			m_isFall = false;
		}
	}
}

ButiEngine::MoveDirection ButiEngine::Player::CheckMoveDirection(const Vector3& arg_movePos)
{
	MoveDirection output;
	std::vector<std::vector<std::vector<std::uint8_t>>>& mapData = m_vwp_mapComponent.lock()->GetCurrentMapData().lock()->m_vec_mapDatas;

	if (arg_movePos.x >= mapData[0][0].size() ||
		arg_movePos.y >= mapData.size() ||
		arg_movePos.z >= mapData[0].size() ||
		arg_movePos.x < 0 || arg_movePos.y < 0 || arg_movePos.z < 0)
	{
		output = MoveDirection::No;
		return output;
	}

	if (mapData[arg_movePos.y][arg_movePos.z][arg_movePos.x] == GameSettings::MAP_CHIP_BLOCK)
	{
		if (arg_movePos.y + 1 >= mapData.size() ||
			mapData[arg_movePos.y + 1][arg_movePos.z][arg_movePos.x] == GameSettings::MAP_CHIP_BLOCK ||
			mapData[m_mapPos.y + 1][m_mapPos.z][m_mapPos.x] == GameSettings::MAP_CHIP_BLOCK)
		{
			output = MoveDirection::No;
		}
		else
		{
			output = MoveDirection::Up;
		}
	}
	else if (mapData[arg_movePos.y - 1][arg_movePos.z][arg_movePos.x] == GameSettings::MAP_CHIP_BLOCK)
	{
		output = MoveDirection::Normal;
	}
	else if (arg_movePos.y - 2 >= 0 && mapData[arg_movePos.y - 2][arg_movePos.z][arg_movePos.x] == GameSettings::MAP_CHIP_BLOCK)
	{
		output = MoveDirection::Down;
	}
	else
	{
		output = MoveDirection::No;
	}

	return output;
}

void ButiEngine::Player::CheckExistUnderBlock(const Vector3& arg_movePos)
{
	if (m_vlp_fallTimer->IsOn())
	{
		return;
	}
	std::vector<std::vector<std::vector<std::uint8_t>>>& mapData = m_vwp_mapComponent.lock()->GetCurrentMapData().lock()->m_vec_mapDatas;
	if (mapData[arg_movePos.y - 1][arg_movePos.z][arg_movePos.x] == GameSettings::MAP_CHIP_BLOCK)
	{
		return;
	}
	m_isFallStart = true;
	m_afterFallPos = gameObject.lock()->transform->GetWorldPosition();
	m_afterFallPos.y = -500.0f;
}

bool ButiEngine::Player::IsBlock(std::uint8_t arg_mapChipNum)
{
	if (arg_mapChipNum == GameSettings::MAP_CHIP_BLOCK) { return true; }
	if (arg_mapChipNum == GameSettings::MAP_CHIP_TUTORIALGOAL) { return true; }
	if (arg_mapChipNum == GameSettings::MAP_CHIP_EASYGOAL) { return true; }
	if (arg_mapChipNum == GameSettings::MAP_CHIP_DEFAULTGOAL) { return true; }
	if (arg_mapChipNum >= GameSettings::MAP_CHIP_INVISIBLEBLOCK) { return true; }
	return false;
}
