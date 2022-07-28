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
#include "NextStageBlock.h"

void ButiEngine::Player::OnUpdate()
{
	if (m_isGoal) { return; }

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
	Contoroll();
	if (m_vlp_timer->Update())
	{
		m_vlp_timer->Stop();

		int rand = ButiRandom::GetRandom(0, 2, 1);
		gameObject.lock()->GetApplication().lock()->GetSoundManager()->PlaySE(m_moveSounds[rand], 0.1f);

		//波紋
		auto pos = gameObject.lock()->transform->GetWorldPosition();
		pos.y -= 0.3f;
		GetManager().lock()->AddObjectFromCereal("Ripple", ObjectFactory::Create<Transform>(pos, Vector3(90, 0, 0), 0.0f));

		CheckLookBlock();
		//フラッシュ
		GetManager().lock()->GetGameObject("CameraMesh").lock()->GetGameComponent<CameraMesh>()->Flash();
		m_vwp_invisibleBlockManagerComponent.lock()->CheckSeen();
		CheckExistUnderBlock(m_mapPos);
		if (!m_isFallStart)
		{
			CheckTouchNextStageBlock();
			CheckGoal();
		}
	}
	if (m_vlp_expantionTimer->Update())
	{
		m_vlp_expantionTimer->Stop();
		Expansion();
	}
	Shrink();
	Fall();

	if (m_isTouchNextStageBlock && InputManager::IsTriggerDecisionKey())
	{
		Goal();
	}
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
	for (std::uint16_t i = 0; i < 3; i++) {

		m_moveSounds[i] = SoundTag("Sound/Move_" + std::to_string(i) + ".wav");
	}

	m_isGoal = false;
	m_isFall = false;
	m_isFallStart = false;
	m_isTouchNextStageBlock = false;
	m_afterFallPos = Vector3Const::Zero;
	m_length = 1.0f;
	m_vwp_mapComponent = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("Map").lock()->GetGameComponent<Map>();
	m_mapPos = m_vwp_mapComponent.lock()->GetPlayerPos();
	m_offset = m_mapPos - m_startPos;

	m_vlp_timer = ObjectFactory::Create<RelativeTimer>(12);
	m_vlp_expantionTimer = ObjectFactory::Create<RelativeTimer>(10);
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
		std::int8_t tmp = 0;
		lookObject = GetDownBlock(m_mapPos, tmp);
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
	else if (lookObject.lock()->HasGameObjectTag("NextStageBlock"))
	{
		auto nextStageBlockComp = lookObject.lock()->GetGameComponent<NextStageBlock>();
		if (nextStageBlockComp)
		{
			nextStageBlockComp->Seen();
		}
	}
}

void ButiEngine::Player::RollCameraDirection(const std::uint16_t arg_rotateDir)
{
	m_cameraDirection = (CameraDirection)((std::uint16_t)m_cameraDirection + arg_rotateDir);

	if (m_cameraDirection > CameraDirection::Right) {
		m_cameraDirection = (CameraDirection)((std::uint16_t)m_cameraDirection % 4);
	}
	else if (m_cameraDirection < CameraDirection::Front) {
		m_cameraDirection = (CameraDirection)((std::uint16_t)m_cameraDirection + 4);
	}
}


void ButiEngine::Player::Goal()
{
	if (m_isGoal) { return; }

	gameObject.lock()->GetApplication().lock()->GetSoundManager()->PlaySE(SoundTag("Sound/TouchGoal.wav"), 0.1f);
	m_isGoal = true;
}

void ButiEngine::Player::CheckTouchNextStageBlock()
{
	std::vector<std::vector<std::vector<std::uint16_t>>>& vec_mapDatas = m_vwp_mapComponent.lock()->GetCurrentMapData().lock()->m_vec_mapDatas;
	std::uint16_t mapNum = vec_mapDatas[m_mapPos.y][m_mapPos.z][m_mapPos.x];
	auto hitObject = m_vwp_mapComponent.lock()->GetMapObjectData()[m_mapPos.y][m_mapPos.z][m_mapPos.x];

	if (!hitObject.lock() || !hitObject.lock()->HasGameObjectTag("NextStageBlock"))
	{
		m_isTouchNextStageBlock = false;
		if (m_vwp_nextStageBlockComponent.lock())
		{
			m_vwp_nextStageBlockComponent.lock()->DisappearPreview();
		}
		m_vwp_nextStageBlockComponent = nullptr;
		return;
	}

	if (mapNum >= GameSettings::MAP_CHIP_NEXT_STAGE_BLOCK && hitObject.lock()->GetGameComponent<NextStageBlock>()->IsActive())
	{
		m_isTouchNextStageBlock = true;
		if (m_vwp_nextStageBlockComponent.lock())
		{
			m_vwp_nextStageBlockComponent.lock()->DisappearPreview();
		}
		m_vwp_nextStageBlockComponent = hitObject.lock()->GetGameComponent<NextStageBlock>();
		m_vwp_nextStageBlockComponent.lock()->AppearPreview();
	}
	else
	{
		m_isTouchNextStageBlock = false;
		if (m_vwp_nextStageBlockComponent.lock())
		{
			m_vwp_nextStageBlockComponent.lock()->DisappearPreview();
		}
		m_vwp_nextStageBlockComponent = nullptr;
	}

}

void ButiEngine::Player::CheckGoal()
{
	std::vector<std::vector<std::vector<std::uint16_t>>>& vec_mapDatas = m_vwp_mapComponent.lock()->GetCurrentMapData().lock()->m_vec_mapDatas;
	std::uint16_t mapNum = vec_mapDatas[m_mapPos.y][m_mapPos.z][m_mapPos.x];
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
		Goal();
	}
	else if (mapNum == GameSettings::MAP_CHIP_EASYGOAL && hitObject.lock()->GetGameComponent<EasyGoal>()->IsActive())
	{
		Goal();
	}
	else if (mapNum == GameSettings::MAP_CHIP_DEFAULTGOAL && hitObject.lock()->GetGameComponent<DefaultGoal>()->IsActive())
	{
		Goal();
	}
}

void ButiEngine::Player::CheckLookDirection()
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
		m_nextMapPos = cubeAnim->GetTargetTransform()->GetWorldPosition().Round();
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
		m_nextMapPos = cubeAnim->GetTargetTransform()->GetWorldPosition().Round();
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
		m_nextMapPos = cubeAnim->GetTargetTransform()->GetWorldPosition().Round();
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
		m_nextMapPos = cubeAnim->GetTargetTransform()->GetWorldPosition().Round();
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

namespace ButiEngine {
void TargetTransformRotation_WorldZAxis_m90(const LookDirection arg_dir, Value_ptr<CubeTransformAnimation> arg_vlp_anim) {

	auto up = arg_vlp_anim->GetTargetTransform()->GetUp().Round();
	Matrix4x4 rotation;
	switch (arg_dir)
	{
	case  LookDirection::Front:
	{
		if (up == Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(-90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(180)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
	}
	break;
	case  LookDirection::Back:
	{
		if (up == Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(90)) * Matrix4x4::RollY(MathHelper::ToRadian(180)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(-90)) * Matrix4x4::RollY(MathHelper::ToRadian(180)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(180));
		}
		else if (up == -Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(180)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
	}
	break;
	case  LookDirection::Right:
	{
		if (up == Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(-90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
		else if (up == -Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
		else if (up == Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
		else if (up == -Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(180)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
	}
	break;
	case  LookDirection::Left:{
		if (up == Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(-90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
		else if (up == -Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
		else if (up == Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(180)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
		else if (up == -Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
	}
							 break;
	case  LookDirection::Up:
	{
		if (up == Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(180)) * Matrix4x4::RollY(MathHelper::ToRadian(90)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(90)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(90)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
		else if (up == -Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(90)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
	}
	break;
	case  LookDirection::Down: {
		if (up == Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(-90)) * Matrix4x4::RollX(MathHelper::ToRadian(180));
		}
		else if (up == -Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(-90)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(-90)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
		else if (up == -Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(-90)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
	}
							 break;
	}

	arg_vlp_anim->GetTargetTransform()->SetLocalRotation(rotation);
}
void TargetTransformRotation_WorldZAxis_90(const LookDirection arg_dir, Value_ptr<CubeTransformAnimation> arg_vlp_anim) {

	auto up = arg_vlp_anim->GetTargetTransform()->GetUp().Round();
	Matrix4x4 rotation;
	switch (arg_dir)
	{
	case  LookDirection::Front:
	{
		if (up == Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(-90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(180)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
	}
	break;
	case  LookDirection::Back:
	{
		if (up == Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(-90)) * Matrix4x4::RollY(MathHelper::ToRadian(180)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(90)) * Matrix4x4::RollY(MathHelper::ToRadian(180)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(180)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(180));
		}
	}
	break;
	case  LookDirection::Right:
	{
		if (up == Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
		else if (up == -Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(-90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
		else if (up == Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(180)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
		else if (up == -Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
	}
	break;
	case  LookDirection::Left: {
		if (up == Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
		else if (up == -Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(-90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
		else if (up == Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
		else if (up == -Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(180)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
	}
							 break;
	case  LookDirection::Up:
	{
		if (up == Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(-90)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(-90)) * Matrix4x4::RollX(MathHelper::ToRadian(180));
		}
		else if (up == Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(-90)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
		else if (up == -Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(-90)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
	}
	break;
	case  LookDirection::Down: {
		if (up == Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(90)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(90)) * Matrix4x4::RollX(MathHelper::ToRadian(180));
		}
		else if (up == Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(90)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
		else if (up == -Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(90)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
	}
							 break;
	}

	arg_vlp_anim->GetTargetTransform()->SetLocalRotation(rotation);
}

void TargetTransformRotation_WorldZAxis_180(const LookDirection arg_dir, Value_ptr<CubeTransformAnimation> arg_vlp_anim) {

	auto up = arg_vlp_anim->GetTargetTransform()->GetUp().Round();
	arg_vlp_anim->GetInitTransform()->RollWorldRotationZ_Degrees(0.1f);
	Matrix4x4 rotation;
	switch (arg_dir)
	{
	case  LookDirection::Front:
	{
		if (up == Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(180)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(-90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
	}
	break;
	case  LookDirection::Back:
	{
		if (up == Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(180)) * Matrix4x4::RollY(MathHelper::ToRadian(180)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(180)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(-90)) * Matrix4x4::RollY(MathHelper::ToRadian(180)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(90)) * Matrix4x4::RollY(MathHelper::ToRadian(180)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
	}
	break;
	case  LookDirection::Right:
	{
		if (up == Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(-90)) * Matrix4x4::RollX(MathHelper::ToRadian(180));
		}
		else if (up == -Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(-90)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(-90)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
		else if (up == -Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(90)) * Matrix4x4::RollY(MathHelper::ToRadian(-90)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
	}
	break;
	case  LookDirection::Left: {

		if (up == Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(90)) * Matrix4x4::RollX(MathHelper::ToRadian(180));
		}
		else if (up == -Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(90)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(90)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
		else if (up == -Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(90)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
	}
							 break;
	case  LookDirection::Up:
	{
		if (up == Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
		else if (up == -Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(-90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
		else if (up == Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
		else if (up == -Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(180)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
	}
	break;
	case  LookDirection::Down: {

		if (up == Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
		else if (up == -Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(-90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
		else if (up == Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(180)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
		else if (up == -Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
	}
							 break;
	}

	arg_vlp_anim->GetTargetTransform()->SetLocalRotation(rotation);
}
void TargetTransformRotation_WorldZAxis_m180(const LookDirection arg_dir, Value_ptr<CubeTransformAnimation> arg_vlp_anim) {

	auto up = arg_vlp_anim->GetTargetTransform()->GetUp().Round();
	arg_vlp_anim->GetInitTransform()->RollWorldRotationZ_Degrees(-0.1f);
	Matrix4x4 rotation;
	switch (arg_dir)
	{
	case  LookDirection::Front:
	{
		if (up == Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(180)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(-90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
	}
	break;
	case  LookDirection::Back:
	{
		if (up == Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(180)) * Matrix4x4::RollY(MathHelper::ToRadian(180)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(180)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(-90)) * Matrix4x4::RollY(MathHelper::ToRadian(180)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(90)) * Matrix4x4::RollY(MathHelper::ToRadian(180)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
	}
	break;
	case  LookDirection::Right:
	{
		if (up == Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(-90)) * Matrix4x4::RollX(MathHelper::ToRadian(180));
		}
		else if (up == -Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(-90)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(-90)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
		else if (up == -Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(90)) * Matrix4x4::RollY(MathHelper::ToRadian(-90)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
	}
	break;
	case  LookDirection::Left: {

		if (up == Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(90)) * Matrix4x4::RollX(MathHelper::ToRadian(180));
		}
		else if (up == -Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(90)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(90)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
		else if (up == -Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(90)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
	}
							 break;
	case  LookDirection::Up:
	{
		if (up == Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
		else if (up == -Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(-90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
		else if (up == Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
		else if (up == -Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(180)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
	}
	break;
	case  LookDirection::Down: {

		if (up == Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
		else if (up == -Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(-90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
		else if (up == Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(180)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
		else if (up == -Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
	}
							 break;
	}

	arg_vlp_anim->GetTargetTransform()->SetLocalRotation(rotation);
}

void TargetTransformRotation_WorldXAxis_90(const LookDirection arg_dir, Value_ptr<CubeTransformAnimation> arg_vlp_anim) {

	auto up = arg_vlp_anim->GetTargetTransform()->GetUp().Round();
	Matrix4x4 rotation;
	switch (arg_dir)
	{
	case  LookDirection::Front:
	{
		if (up == Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
		else if (up == -Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(180)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
		else if (up == Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(-90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
		else if (up == -Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(90))* Matrix4x4::RollY(MathHelper::ToRadian(0))* Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
	}
	break;
	case  LookDirection::Back:
	{
		if (up == Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(180)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
		else if (up == -Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
		else if (up == Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(-90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
		else if (up == -Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
	}
	break;
	case  LookDirection::Right:
	{
		if (up == Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(90)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
		else if (up == -Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(90)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
		else if (up == Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(180)) * Matrix4x4::RollY(MathHelper::ToRadian(90)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(90)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
	}
	break;
	case  LookDirection::Left: {
		if (up == Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(-90)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
		else if (up == -Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(-90)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
		else if (up == Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(-90)) * Matrix4x4::RollX(MathHelper::ToRadian(180));
		}
		else if (up == -Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(-90)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
	}
							 break;
	case  LookDirection::Up:
	{
		if (up == Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(-90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(180)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
	}
	break;
	case  LookDirection::Down: {
		if (up == Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(90)) * Matrix4x4::RollY(MathHelper::ToRadian(180)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(-90)) * Matrix4x4::RollY(MathHelper::ToRadian(180)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(180));
		}
		else if (up == -Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(180)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
	}
							 break;
	}

	arg_vlp_anim->GetTargetTransform()->SetLocalRotation(rotation);
}
void TargetTransformRotation_WorldXAxis_m90(const LookDirection arg_dir, Value_ptr<CubeTransformAnimation> arg_vlp_anim) {

	auto up = arg_vlp_anim->GetTargetTransform()->GetUp().Round();
	Matrix4x4 rotation;
	switch (arg_dir)
	{
	case  LookDirection::Front:
	{
		if (up == Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
		else if (up == -Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(180)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
		else if (up == Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(-90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
		else if (up == -Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
	}
	break;
	case  LookDirection::Back:
	{
		if (up == Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(180)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
		else if (up == -Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
		else if (up == Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(-90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
		else if (up == -Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
	}
	break;
	case  LookDirection::Right:
	{
		if (up == Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(90)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
		else if (up == -Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(90)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
		else if (up == Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(90)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(90)) * Matrix4x4::RollX(MathHelper::ToRadian(180));
		}
	}
	break;
	case  LookDirection::Left: {
		if (up == Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(-90)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
		else if (up == -Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(-90)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
		else if (up == Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(-90)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(-90)) * Matrix4x4::RollX(MathHelper::ToRadian(180));
		}
	}
							 break;
	case  LookDirection::Up:
	{
		if (up == Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(90)) * Matrix4x4::RollY(MathHelper::ToRadian(180)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(-90)) * Matrix4x4::RollY(MathHelper::ToRadian(180)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(180)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(180)) * Matrix4x4::RollY(MathHelper::ToRadian(180)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
	}
	break;
	case  LookDirection::Down: {
		if (up == Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(-90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(180)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
	}
							 break;
	}

	arg_vlp_anim->GetTargetTransform()->SetLocalRotation(rotation);
}

void TargetTransformRotation_WorldXAxis_180(const LookDirection arg_dir, Value_ptr<CubeTransformAnimation> arg_vlp_anim) {

	auto up = arg_vlp_anim->GetTargetTransform()->GetUp().Round();
	arg_vlp_anim->GetInitTransform()->RollWorldRotationX_Degrees(0.1f);
	Matrix4x4 rotation;
	switch (arg_dir)
	{
	case  LookDirection::Front:
	{
		if (up == Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(180));
		}
		else if (up == -Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(180)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(90)) * Matrix4x4::RollY(MathHelper::ToRadian(180)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(-90)) * Matrix4x4::RollY(MathHelper::ToRadian(180)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
	}
	break;
	case  LookDirection::Back:
	{
		if (up == Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(180)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(-90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
	}
	break;
	case  LookDirection::Right:
	{
		if (up == Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(180)) * Matrix4x4::RollY(MathHelper::ToRadian(90)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(90)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(90)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
		else if (up == -Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(90)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
	}
	break;
	case  LookDirection::Left: {

		if (up == Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(-90)) * Matrix4x4::RollX(MathHelper::ToRadian(180));
		}
		else if (up == -Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(-90)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(90)) * Matrix4x4::RollY(MathHelper::ToRadian(-90)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(-90)) * Matrix4x4::RollY(MathHelper::ToRadian(-90)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
	}
							 break;
	case  LookDirection::Up:
	{
		if (up == Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(-90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
		else if (up == -Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
		else if (up == Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(180)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
		else if (up == -Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
	}
	break;
	case  LookDirection::Down: {
		if (up == Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(-90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
		else if (up == -Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
		else if (up == Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
		else if (up == -Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(180)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
	}
							 break;
	}

	arg_vlp_anim->GetTargetTransform()->SetLocalRotation(rotation);
}
void TargetTransformRotation_WorldXAxis_m180(const LookDirection arg_dir, Value_ptr<CubeTransformAnimation> arg_vlp_anim) {

	auto up = arg_vlp_anim->GetTargetTransform()->GetUp().Round();
	arg_vlp_anim->GetInitTransform()->RollWorldRotationX_Degrees(-0.1f);
	Matrix4x4 rotation;

	switch (arg_dir)
	{
	case  LookDirection::Front:
	{
		if (up == Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(180));
		}
		else if (up == -Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(180)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(90)) * Matrix4x4::RollY(MathHelper::ToRadian(180)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(-90)) * Matrix4x4::RollY(MathHelper::ToRadian(180)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
	}
	break;
	case  LookDirection::Back:
	{
		if (up == Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(180)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(-90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
	}
	break;
	case  LookDirection::Right:
	{
		if (up == Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(180)) * Matrix4x4::RollY(MathHelper::ToRadian(90)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(90)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(90)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
		else if (up == -Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(90)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
	}
	break;
	case  LookDirection::Left: {

		if (up == Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(-90)) * Matrix4x4::RollX(MathHelper::ToRadian(180));
		}
		else if (up == -Vector3Const::YAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(-90)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(90)) * Matrix4x4::RollY(MathHelper::ToRadian(-90)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
		else if (up == -Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(-90)) * Matrix4x4::RollY(MathHelper::ToRadian(-90)) * Matrix4x4::RollX(MathHelper::ToRadian(0));
		}
	}
							 break;
	case  LookDirection::Up:
	{
		if (up == Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(-90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
		else if (up == -Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
		else if (up == Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(180)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
		else if (up == -Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(90));
		}
	}
	break;
	case  LookDirection::Down: {
		if (up == Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(-90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
		else if (up == -Vector3Const::XAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(90)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
		else if (up == Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(0)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
		else if (up == -Vector3Const::ZAxis)
		{
			rotation = Matrix4x4::RollZ(MathHelper::ToRadian(180)) * Matrix4x4::RollY(MathHelper::ToRadian(0)) * Matrix4x4::RollX(MathHelper::ToRadian(-90));
		}
	}
							 break;
	}

	arg_vlp_anim->GetTargetTransform()->SetLocalRotation(rotation);
}

}

void ButiEngine::Player::MoveRightUp()
{

	auto t = gameObject.lock()->transform;
	auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();

	if (!anim)
	{
		m_vlp_timer->Reset();
		m_vlp_timer->Start();
		m_vlp_expantionTimer->Reset();
		m_vlp_expantionTimer->Start();
		anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
		anim->SetSpeed(1.0f / 10);
		anim->SetTargetTransform(t->Clone());
		anim->GetTargetTransform()->Translate(Vector3(m_length, m_length, 0));
		TargetTransformRotation_WorldZAxis_m180(m_lookDirection, anim);
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
		m_vlp_expantionTimer->Reset();
		m_vlp_expantionTimer->Start();
		anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
		anim->SetSpeed(1.0f / 10);
		anim->SetTargetTransform(t->Clone());
		anim->GetTargetTransform()->TranslateX(m_length);
		TargetTransformRotation_WorldZAxis_m90(m_lookDirection, anim);
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
		m_vlp_expantionTimer->Reset();
		m_vlp_expantionTimer->Start();
		anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
		anim->SetSpeed(1.0f / 10);
		anim->SetTargetTransform(t->Clone());
		anim->GetTargetTransform()->Translate(Vector3(m_length, -m_length, 0));

		TargetTransformRotation_WorldZAxis_m180(m_lookDirection, anim);
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
		m_vlp_expantionTimer->Reset();
		m_vlp_expantionTimer->Start();
		anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
		anim->SetSpeed(1.0f / 10);
		anim->SetTargetTransform(t->Clone());
		anim->GetTargetTransform()->Translate(Vector3(-m_length, m_length, 0));

		TargetTransformRotation_WorldZAxis_180(m_lookDirection, anim);
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
		m_vlp_expantionTimer->Reset();
		m_vlp_expantionTimer->Start();
		anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
		anim->SetSpeed(1.0f / 10);
		anim->SetTargetTransform(t->Clone());
		anim->GetTargetTransform()->TranslateX(-m_length);

		TargetTransformRotation_WorldZAxis_90(m_lookDirection, anim);
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
		m_vlp_expantionTimer->Reset();
		m_vlp_expantionTimer->Start();
		anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
		anim->SetSpeed(1.0f / 10);
		anim->SetTargetTransform(t->Clone());
		anim->GetTargetTransform()->Translate(Vector3(-m_length, -m_length, 0));

		TargetTransformRotation_WorldZAxis_180(m_lookDirection, anim);
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
		m_vlp_expantionTimer->Reset();
		m_vlp_expantionTimer->Start();
		anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
		anim->SetSpeed(1.0f / 10);
		anim->SetTargetTransform(t->Clone());
		anim->GetTargetTransform()->Translate(Vector3(0, m_length, m_length));

		TargetTransformRotation_WorldXAxis_180(m_lookDirection, anim);
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
		m_vlp_expantionTimer->Reset();
		m_vlp_expantionTimer->Start();
		anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
		anim->SetSpeed(1.0f / 10);
		anim->SetTargetTransform(t->Clone());
		anim->GetTargetTransform()->TranslateZ(m_length);

		TargetTransformRotation_WorldXAxis_90(m_lookDirection, anim);
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
		m_vlp_expantionTimer->Reset();
		m_vlp_expantionTimer->Start();
		anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
		anim->SetSpeed(1.0f / 10);
		anim->SetTargetTransform(t->Clone());
		anim->GetTargetTransform()->Translate(Vector3(0, -m_length, m_length));

		TargetTransformRotation_WorldXAxis_180(m_lookDirection, anim);
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
		m_vlp_expantionTimer->Reset();
		m_vlp_expantionTimer->Start();
		anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
		anim->SetSpeed(1.0f / 10);
		anim->SetTargetTransform(t->Clone());
		anim->GetTargetTransform()->Translate(Vector3(0, m_length, -m_length));

		TargetTransformRotation_WorldXAxis_m180(m_lookDirection, anim);
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
		m_vlp_expantionTimer->Reset();
		m_vlp_expantionTimer->Start();
		anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
		anim->SetSpeed(1.0f / 10);
		anim->SetTargetTransform(t->Clone());
		anim->GetTargetTransform()->TranslateZ(-m_length);

		TargetTransformRotation_WorldXAxis_m90(m_lookDirection, anim);
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
		m_vlp_expantionTimer->Reset();
		m_vlp_expantionTimer->Start();
		anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
		anim->SetSpeed(1.0f / 10);
		anim->SetTargetTransform(t->Clone());
		anim->GetTargetTransform()->Translate(Vector3(0, -m_length, -m_length));

		TargetTransformRotation_WorldXAxis_m180(m_lookDirection, anim);
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
	for (std::uint16_t i = m_mapPos.x + 1; i < mapData[m_mapPos.y][m_mapPos.z].size(); i++)
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
	for (std::uint16_t i = m_mapPos.y + 1; i < mapData.size(); i++)
	{
		if (IsBlock(mapData[i][m_mapPos.z][m_mapPos.x]))
		{
			auto mapObjectData = m_vwp_mapComponent.lock()->GetMapObjectData();
			return mapObjectData[i][m_mapPos.z][m_mapPos.x];
		}
	}
	return Value_weak_ptr<GameObject>();
}

ButiEngine::Value_weak_ptr<ButiEngine::GameObject> ButiEngine::Player::GetDownBlock(const Vector3& arg_mapPos, std::int8_t& ref_output_diff, const bool arg_isCantThrough)
{
	auto mapData = m_vwp_mapComponent.lock()->GetCurrentMapData().lock()->m_vec_mapDatas;
	for (std::int8_t i = m_mapPos.y - 1; i >= 0; i--)
	{
		if (arg_isCantThrough)
		{
			if (IsCantThroughBlock(mapData[i][m_mapPos.z][m_mapPos.x]))
			{
				ref_output_diff = abs(m_mapPos.y - i);
				auto mapObjectData = m_vwp_mapComponent.lock()->GetMapObjectData();
				return mapObjectData[i][m_mapPos.z][m_mapPos.x];
			}
		}
		else if (IsBlock(mapData[i][m_mapPos.z][m_mapPos.x]))
		{
			ref_output_diff = abs(m_mapPos.y - i);
			auto mapObjectData = m_vwp_mapComponent.lock()->GetMapObjectData();
			return mapObjectData[i][m_mapPos.z][m_mapPos.x];
		}
	}
	return Value_weak_ptr<GameObject>();
}

ButiEngine::Value_weak_ptr<ButiEngine::GameObject> ButiEngine::Player::GetFrontBlock(const Vector3& arg_mapPos)
{
	auto mapData = m_vwp_mapComponent.lock()->GetCurrentMapData().lock()->m_vec_mapDatas;
	for (std::uint16_t i = m_mapPos.z + 1; i < mapData[m_mapPos.y].size(); i++)
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

			Expansion();

			int rand = ButiRandom::GetRandom(0, 2, 1);
			gameObject.lock()->GetApplication().lock()->GetSoundManager()->PlaySE(m_moveSounds[rand], 0.1f);

			//波紋
			auto pos = gameObject.lock()->transform->GetWorldPosition();
			pos.y -= 0.3f;
			GetManager().lock()->AddObjectFromCereal("Ripple", ObjectFactory::Create<Transform>(pos, Vector3(90, 0, 0), 0.0f));

			CheckLookBlock();
			//フラッシュ
			GetManager().lock()->GetGameObject("CameraMesh").lock()->GetGameComponent<CameraMesh>()->Flash();
			m_vwp_invisibleBlockManagerComponent.lock()->CheckSeen();
			CheckExistUnderBlock(m_mapPos);
			CheckTouchNextStageBlock();
			CheckGoal();
		}
	}
}

ButiEngine::MoveDirection ButiEngine::Player::CheckMoveDirection(const Vector3& arg_movePos)
{
	MoveDirection output;
	std::vector<std::vector<std::vector<std::uint16_t>>>& mapData = m_vwp_mapComponent.lock()->GetCurrentMapData().lock()->m_vec_mapDatas;

	if (arg_movePos.x >= mapData[0][0].size() ||
		arg_movePos.y >= mapData.size() ||
		arg_movePos.z >= mapData[0].size() ||
		arg_movePos.x < 0 || arg_movePos.y < 0 || arg_movePos.z < 0)
	{
		output = MoveDirection::No;
		return output;
	}

	if (IsCantThroughBlock(mapData[arg_movePos.y][arg_movePos.z][arg_movePos.x]))
	{
		if (arg_movePos.y + 1 >= mapData.size() ||
			IsCantThroughBlock(mapData[arg_movePos.y + 1][arg_movePos.z][arg_movePos.x]) ||
			IsCantThroughBlock(mapData[m_mapPos.y + 1][m_mapPos.z][m_mapPos.x]))
		{
			output = MoveDirection::No;
		}
		else
		{
			output = MoveDirection::Up;
		}
	}
	else if (IsCantThroughBlock(mapData[arg_movePos.y - 1][arg_movePos.z][arg_movePos.x]))
	{
		output = MoveDirection::Normal;
	}
	else if (arg_movePos.y - 2 >= 0 && IsCantThroughBlock(mapData[arg_movePos.y - 2][arg_movePos.z][arg_movePos.x]))
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
	std::vector<std::vector<std::vector<std::uint16_t>>>& mapData = m_vwp_mapComponent.lock()->GetCurrentMapData().lock()->m_vec_mapDatas;
	if (IsCantThroughBlock(mapData[arg_movePos.y - 1][arg_movePos.z][arg_movePos.x]))
	{
		return;
	}

	std::int8_t diff = 0;
	auto downBlock = GetDownBlock(arg_movePos, diff, true);
	m_isFallStart = true;
	if (downBlock.lock())
	{
		Vector3 downBlockPos = downBlock.lock()->transform->GetLocalPosition();
		m_afterFallPos = downBlockPos;
		m_afterFallPos.y += GameSettings::BLOCK_SIZE;

		m_mapPos.y -= diff;
		m_mapPos.y += 1;
	}
	else
	{
		m_afterFallPos = gameObject.lock()->transform->GetLocalPosition();
		m_afterFallPos.y = -500.0f;
	}
}

bool ButiEngine::Player::IsBlock(std::uint16_t arg_mapChipNum)
{
	if (arg_mapChipNum == GameSettings::MAP_CHIP_BLOCK) { return true; }
	if (arg_mapChipNum == GameSettings::MAP_CHIP_TUTORIALGOAL) { return true; }
	if (arg_mapChipNum == GameSettings::MAP_CHIP_EASYGOAL) { return true; }
	if (arg_mapChipNum == GameSettings::MAP_CHIP_DEFAULTGOAL) { return true; }
	if (arg_mapChipNum >= GameSettings::MAP_CHIP_INVISIBLEBLOCK) { return true; }
	if (arg_mapChipNum >= GameSettings::MAP_CHIP_NEXT_STAGE_BLOCK) { return true; }
	return false;
}

bool ButiEngine::Player::IsCantThroughBlock(std::uint16_t arg_mapChipNum)
{
	if (arg_mapChipNum == GameSettings::MAP_CHIP_BLOCK) { return true; }
	if (arg_mapChipNum == GameSettings::MAP_CHIP_GLASS) { return true; }
	return false;
}
