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
#include "EyeBlock.h"
#include "SeenObject.h"
#include "CameraController.h"
#include "Header/GameObjects/DefaultGameComponent/PositionAnimationComponent.h"

bool ButiEngine::Player::g_canPutEyeBlock = false;
bool g_canAlreadyPutEyeBlock = false;
void ButiEngine::Player::OnUpdate()
{
	Shrink();
	if (m_vwp_eyeBlockComponent.lock())
	{
		m_vwp_eyeBlockComponent.lock()->Shrink();
	}
	if (m_isGoal) { return; }

	auto directing = gameObject.lock()->GetGameComponent<StartPlayerDirecting>();
	if (gameObject.lock()->transform->GetWorldPosition().y < -40.0f)
	{
		m_vlp_fallTimer->Stop();

		if (m_vwp_eyeBlockComponent.lock())
		{
			gameObject.lock()->GetGameComponent<MeshDrawComponent>(1)->Regist();
			m_vwp_eyeBlockComponent.lock()->Dead(false);
			m_vwp_eyeBlock = Value_weak_ptr<GameObject>();
			m_vwp_eyeBlockComponent = Value_weak_ptr<EyeBlock>();
		}

		m_isFall = false;
		m_isFallStart = false;
		m_mapPos = m_vwp_mapComponent.lock()->GetStartPlayerPos();
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
	if (m_vlp_flashMeshTimer->Update()) {
		m_vlp_flashMeshTimer->Stop();
		auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
		if (anim)
		{
			FlashMeshSet(anim->GetTargetTransform(), CheckLookDirection(anim->GetTargetTransform()), m_nextMapPos);
		}
		else
		{
			auto tAnim = gameObject.lock()->GetGameComponent<TransformAnimation>();
			FlashMeshSet(tAnim->GetTargetTransform(), CheckLookDirection(tAnim->GetTargetTransform()), m_nextMapPos);
		}
		if (m_vwp_eyeBlockComponent.lock())
		{
			auto a = m_mapPos;
			m_vwp_eyeBlockComponent.lock()->FlashMeshSet(m_nextMapPos);
		}
	}
	if (m_vlp_timer->Update())
	{
		m_vlp_timer->Stop();

		m_isMoveUp = false;
		m_isMoveDown = false;

		int rand = ButiRandom::GetRandom(0, 2, 1);
		gameObject.lock()->GetApplication().lock()->GetSoundManager()->PlaySE(m_moveSounds[rand], 0.1f);

		//波紋
		auto pos = gameObject.lock()->transform->GetWorldPosition();
		pos.y -= 0.3f;
		GetManager().lock()->AddObjectFromCereal("Ripple", ObjectFactory::Create<Transform>(pos, Vector3(90, 0, 0), 0.0f));

		CheckLookBlock();
		//フラッシュ
		if (!m_vwp_eyeBlockComponent.lock())
		{
			m_vwp_rightCameraMesh.lock()->GetGameComponent<CameraMesh>()->Flash();
		}
		m_vwp_leftCameraMesh.lock()->GetGameComponent<CameraMesh>()->Flash();

		if (m_vwp_eyeBlockComponent.lock())
		{
			if (m_mapPos == m_vwp_eyeBlockComponent.lock()->GetMapPos())
			{
				gameObject.lock()->GetGameComponent<MeshDrawComponent>(1)->Regist();
				gameObject.lock()->GetApplication().lock()->GetSoundManager()->PlaySE(SoundTag("Sound/select.wav"), 0.075f);
				m_vwp_eyeBlockComponent.lock()->Dead();
				m_vwp_eyeBlock = Value_weak_ptr<GameObject>();
				m_vwp_eyeBlockComponent = Value_weak_ptr<EyeBlock>();
			}
			else
			{
				m_vwp_eyeBlockComponent.lock()->CheckLookBlock();
				m_vwp_eyeBlockComponent.lock()->Flash();
			}
		}
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
		if (m_vwp_eyeBlockComponent.lock())
		{
			m_vwp_eyeBlockComponent.lock()->Expansion();
		}
	}
	Fall();

	if (IsRollFinish())
	{
		if (m_isTouchNextStageBlock && InputManager::IsTriggerDecisionKey())
		{
			Goal();
		}
		auto vec_mapDatas = m_vwp_mapComponent.lock()->GetCurrentMapData().lock()->m_vec_mapDatas;
		auto mapObjectData = m_vwp_mapComponent.lock()->GetMapObjectData();
		auto object = mapObjectData[m_mapPos.y][m_mapPos.z][m_mapPos.x];
		if (CanPutEyeBlock(object) && !m_vwp_eyeBlockComponent.lock() && InputManager::IsTriggerPutEyeBlockKey())
		{
			m_vwp_eyeBlock = GetManager().lock()->AddObjectFromCereal("EyeBlock", gameObject.lock()->transform->Clone());
			m_vwp_eyeBlockComponent = m_vwp_eyeBlock.lock()->GetGameComponent<EyeBlock>();
			m_vwp_eyeBlockComponent.lock()->SetMapPos(m_mapPos);

			gameObject.lock()->GetGameComponent<MeshDrawComponent>(1)->UnRegist();

			m_vwp_rightCameraMesh.lock()->transform->SetLocalScale(0.0f);

			gameObject.lock()->GetApplication().lock()->GetSoundManager()->PlaySE(SoundTag("PutEye.wav"), 0.25f);
		}
	}

	if (m_isMoveUp)
	{
		OnMoveUp();
	}
	else if (m_isMoveDown)
	{
		OnMoveDown();
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
	m_mapPos = m_vwp_mapComponent.lock()->GetStartPlayerPos();
	m_offset = m_mapPos - m_startPos;

	m_vlp_timer = ObjectFactory::Create<RelativeTimer>(12);
	m_vlp_flashMeshTimer = ObjectFactory::Create<RelativeTimer>(11);
	m_vlp_expantionTimer = ObjectFactory::Create<RelativeTimer>(10);
	m_vlp_fallTimer = ObjectFactory::Create<RelativeTimer>(24);
	m_vlp_fallTimer->Stop();
	m_vwp_invisibleBlockManagerComponent = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("InvisibleBlockManager").lock()->GetGameComponent<InvisibleBlockManager>();

	m_vwp_rightCameraMesh = GetManager().lock()->AddObjectFromCereal("CameraMesh", ObjectFactory::Create<Transform>(Vector3(0, 0, 10000.0f)));
	m_vwp_leftCameraMesh = GetManager().lock()->AddObjectFromCereal("CameraMesh", ObjectFactory::Create<Transform>(Vector3(0, 0, 10000.0f)));

	m_vwp_rightCameraMesh.lock()->SetObjectName("RightCameraMesh");
	m_vwp_leftCameraMesh.lock()->SetObjectName("LeftCameraMesh");

	m_vwp_rightCameraMesh.lock()->transform->SetLocalScale(0.0f);
	m_vwp_leftCameraMesh.lock()->transform->SetLocalScale(0.0f);

	m_vwp_rightCameraMesh.lock()->GetGameComponent<CameraMesh>()->SetColor(ButiColor::White());
	if (g_canPutEyeBlock)
	{
		m_vwp_rightCameraMesh.lock()->GetGameComponent<CameraMesh>()->SetColor(Vector4(0.0f, 1.0f, 1.0f, 1.0f));
		gameObject.lock()->GetGameComponent<MeshDrawComponent>(1)->GetCBuffer<ButiRendering::ObjectInformation>()->Get().color = Vector4(0.0f, 1.0f, 1.0f, 1.0f);
	}
	m_vwp_leftCameraMesh.lock()->GetGameComponent<CameraMesh>()->SetColor(ButiColor::White());

	m_vlp_rightEyeTransform = ObjectFactory::Create<Transform>(Vector3(0.2f, 0.0f, 0.0f));
	m_vlp_rightEyeTransform->SetBaseTransform(gameObject.lock()->transform, true);

	m_vlp_leftEyeTransform = ObjectFactory::Create<Transform>(Vector3(-0.2f, 0.0f, 0.0f));
	m_vlp_leftEyeTransform->SetBaseTransform(gameObject.lock()->transform, true);

	if (m_vwp_eyeBlockComponent.lock())
	{
		gameObject.lock()->GetGameComponent<MeshDrawComponent>(1)->UnRegist();
	}
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Player::Clone()
{
	return ObjectFactory::Create<Player>();
}

void ButiEngine::Player::SetEyeBlock(Value_weak_ptr<GameObject> arg_eyeBlock)
{
	m_vwp_eyeBlock = arg_eyeBlock;
	m_vwp_eyeBlockComponent = m_vwp_eyeBlock.lock()->GetGameComponent<EyeBlock>();
}

bool ButiEngine::Player::IsRollFinish()
{
	return !m_vlp_timer->IsOn();
}

void ButiEngine::Player::CheckLookBlock()
{
	m_vwp_invisibleBlockManagerComponent.lock()->Reset();
	m_lookDirection= CheckLookDirection(gameObject.lock()->transform);

	if (m_vwp_lookObject.lock())
	{
		auto seenObjectComponent = m_vwp_lookObject.lock()->GetGameComponent<SeenObject>();
		if (seenObjectComponent)
		{
			seenObjectComponent->RemoveObserverCount();
		}
	}

	if (m_lookDirection == LookDirection::Right)
	{
		m_vwp_lookObject = GetRightBlock(m_mapPos);
	}
	else if (m_lookDirection == LookDirection::Left)
	{
		m_vwp_lookObject = GetLeftBlock(m_mapPos);
	}
	else if (m_lookDirection == LookDirection::Up)
	{
		m_vwp_lookObject = GetUpBlock(m_mapPos);
	}
	else if (m_lookDirection == LookDirection::Down)
	{
		std::int8_t tmp = 0;
		m_vwp_lookObject = GetDownBlock(m_mapPos, tmp);
	}
	else if (m_lookDirection == LookDirection::Front)
	{
		m_vwp_lookObject = GetFrontBlock(m_mapPos);
	}
	else if (m_lookDirection == LookDirection::Back)
	{
		m_vwp_lookObject = GetBackBlock(m_mapPos);
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

void ButiEngine::Player::SetCanPutEyeBlock(const bool arg_canPutEyeBlock)
{
	g_canPutEyeBlock = arg_canPutEyeBlock;
	g_canAlreadyPutEyeBlock |= arg_canPutEyeBlock;
}

bool ButiEngine::Player::GetCanAlreadyPutEyeBlock()
{
	return g_canAlreadyPutEyeBlock;
}

void ButiEngine::Player::SetCanAlreadyPutEyeBlock(const bool arg_canAlreadyPutEyeBlock)
{
	g_canAlreadyPutEyeBlock = arg_canAlreadyPutEyeBlock;
}

void ButiEngine::Player::FlashMeshSet(Value_ptr<Transform> arg_vlp_transform, const LookDirection arg_dir, const Vector3& arg_pos)
{
	Value_weak_ptr<GameObject> lookObject;

	Vector3 pos = arg_vlp_transform->GetWorldPosition();
	Vector3 rightPos = pos + (m_vlp_rightEyeTransform->GetLocalPosition() * arg_vlp_transform->GetLocalRotation());
	Vector3 leftPos = pos + (m_vlp_leftEyeTransform->GetLocalPosition() * arg_vlp_transform->GetLocalRotation());

	Vector3 bPos_right = rightPos;
	Vector3 bPos_left = leftPos;

	if (arg_dir == LookDirection::Right)
	{
		bPos_right.x += 100;
		bPos_left.x += 100;
		lookObject = GetRightBlock(arg_pos);
	}
	else if (arg_dir == LookDirection::Left)
	{
		bPos_right.x -= 100;
		bPos_left.x -= 100;
		lookObject = GetLeftBlock(arg_pos);
	}
	else if (arg_dir == LookDirection::Up)
	{
		bPos_right.y += 100;
		bPos_left.y += 100;
		lookObject = GetUpBlock(arg_pos);
	}
	else if (arg_dir == LookDirection::Down)
	{
		bPos_right.y -= 100;
		bPos_left.y -= 100;
		std::int8_t tmp = 0;
		lookObject = GetDownBlock(arg_pos, tmp);
	}
	else if (arg_dir == LookDirection::Front)
	{
		bPos_right.z += 100;
		bPos_left.z += 100;
		lookObject = GetFrontBlock(arg_pos);
	}
	else if (arg_dir == LookDirection::Back)
	{
		bPos_right.z -= 100;
		bPos_left.z -= 100;
		lookObject = GetBackBlock(arg_pos);
	}

	if (lookObject.lock())
	{
		bPos_right = lookObject.lock()->transform->GetWorldPosition();
		bPos_left = lookObject.lock()->transform->GetWorldPosition();
	}

	Vector3 midPoint_right = Vector3((rightPos.x + bPos_right.x) * 0.5f, (rightPos.y + bPos_right.y) * 0.5f, (rightPos.z + bPos_right.z) * 0.5f);
	Vector3 midPoint_left = Vector3((leftPos.x + bPos_left.x) * 0.5f, (leftPos.y + bPos_left.y) * 0.5f, (leftPos.z + bPos_left.z) * 0.5f);

	Vector3 cameraMeshScale = Vector3Const::Zero;
	if (arg_dir == LookDirection::Right || arg_dir == LookDirection::Left)
	{
		midPoint_right.y = rightPos.y;
		midPoint_right.z = rightPos.z;

		midPoint_left.y = leftPos.y;
		midPoint_left.z = leftPos.z;

		cameraMeshScale = Vector3(leftPos.Distance(midPoint_left) * 2.0f, 0.1f, 0.1f);
	}
	else if (arg_dir == LookDirection::Up || arg_dir == LookDirection::Down)
	{
		midPoint_right.x = rightPos.x;
		midPoint_right.z = rightPos.z;

		midPoint_left.x = leftPos.x;
		midPoint_left.z = leftPos.z;

		cameraMeshScale = Vector3(0.1f, leftPos.Distance(midPoint_left) * 2.0f, 0.1f);
	}
	else if (arg_dir == LookDirection::Front || arg_dir == LookDirection::Back)
	{
		midPoint_right.x = rightPos.x;
		midPoint_right.y = rightPos.y;

		midPoint_left.x = leftPos.x;
		midPoint_left.y = leftPos.y;

		cameraMeshScale = Vector3(0.1f, 0.1f, leftPos.Distance(midPoint_left) * 2.0f);
	}

	m_vwp_rightCameraMesh.lock()->transform->SetWorldPosition(midPoint_right);
	m_vwp_leftCameraMesh.lock()->transform->SetWorldPosition(midPoint_left);

	if (!m_vwp_eyeBlockComponent.lock())
	{
		m_vwp_rightCameraMesh.lock()->transform->SetLocalScale(cameraMeshScale);
	}
	m_vwp_leftCameraMesh.lock()->transform->SetLocalScale(cameraMeshScale);
}


void ButiEngine::Player::Goal()
{
	if (m_isGoal) { return; }

	gameObject.lock()->GetApplication().lock()->GetSoundManager()->PlaySE(SoundTag("Sound/TouchGoal.wav"), 0.1f);
	m_isGoal = true;
}

void ButiEngine::Player::CheckTouchNextStageBlock()
{
	auto& vec_mapDatas = m_vwp_mapComponent.lock()->GetCurrentMapData().lock()->m_vec_mapDatas;
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
	auto& vec_mapDatas = m_vwp_mapComponent.lock()->GetCurrentMapData().lock()->m_vec_mapDatas;
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

ButiEngine::LookDirection ButiEngine::Player::CheckLookDirection(Value_ptr<Transform> arg_vlp_transform)
{
	Vector3 dir = arg_vlp_transform->GetFront().Round();

	if (dir.x == 1.0f)
	{
		return LookDirection::Right;
	}
	else if (dir.x == -1.0f)
	{
		return LookDirection::Left;
	}
	else if (dir.y == 1.0f)
	{
		return LookDirection::Up;
	}
	else if (dir.y == -1.0f)
	{
		return LookDirection::Down;
	}
	else if (dir.z == 1.0f)
	{
		return LookDirection::Front;
	}
	else if (dir.z == -1.0f)
	{
		return LookDirection::Back;
	}
}


void ButiEngine::Player::Contoroll()
{
	if (m_isMoveUp || m_isMoveDown) { return; }
	auto cAnim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	auto tAnim = gameObject.lock()->GetGameComponent<TransformAnimation>();
	if (cAnim || tAnim) { return; }
	bool isInputed=false;
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
		isInputed = true;
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
		isInputed = true;
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
		isInputed = true;
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
		isInputed = true;
	}
	if (isInputed ) {
		auto vlp_anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
		if(vlp_anim){
			m_vwp_rightCameraMesh.lock()->transform->SetLocalScale(0.0f);
			m_vwp_leftCameraMesh.lock()->transform->SetLocalScale(0.0f);
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

	//auto t = gameObject.lock()->transform;
	//auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();

	//if (!anim)
	//{
	//	m_vlp_timer->Reset();
	//	m_vlp_timer->Start();
	//	m_vlp_flashMeshTimer->Reset();
	//	m_vlp_flashMeshTimer->Start();
	//	m_vlp_expantionTimer->Reset();
	//	m_vlp_expantionTimer->Start();
	//	anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
	//	anim->SetSpeed(1.0f / 10);
	//	anim->SetTargetTransform(t->Clone());
	//	anim->GetTargetTransform()->Translate(Vector3(m_length, m_length, 0));
	//	TargetTransformRotation_WorldZAxis_m180(m_lookDirection, anim);
	//	anim->SetEaseType(Easing::EasingType::Liner);
	//	anim->SetXEaseType(Easing::EasingType::CubeRotateMin180);
	//	anim->SetYEaseType(Easing::EasingType::CubeRotate180);

	//	m_mapPos.x++;
	//	m_mapPos.y++;
	//}

	MoveUp(AfterMoveDirection::Right);
}
void ButiEngine::Player::MoveRight(std::uint16_t arg_frame)
{
	auto tAnim = gameObject.lock()->GetGameComponent<TransformAnimation>();
	if (tAnim) { return; }

	auto t = gameObject.lock()->transform;
	auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	if (!anim)
	{
		if (!m_isMoveUp)
		{
			m_vlp_timer->Reset();
			m_vlp_timer->Start();
			m_vlp_flashMeshTimer->Reset();
			m_vlp_flashMeshTimer->Start();
			m_vlp_expantionTimer->Reset();
			m_vlp_expantionTimer->Start();
		}
		anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
		anim->SetSpeed(1.0f / arg_frame);
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
	//auto t = gameObject.lock()->transform;
	//auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	//if (!anim)
	//{
	//	m_vlp_timer->Reset();
	//	m_vlp_timer->Start();
	//	m_vlp_flashMeshTimer->Reset();
	//	m_vlp_flashMeshTimer->Start();
	//	m_vlp_expantionTimer->Reset();
	//	m_vlp_expantionTimer->Start();
	//	anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
	//	anim->SetSpeed(1.0f / 10);
	//	anim->SetTargetTransform(t->Clone());
	//	anim->GetTargetTransform()->Translate(Vector3(m_length, -m_length, 0));

	//	TargetTransformRotation_WorldZAxis_m180(m_lookDirection, anim);
	//	anim->SetEaseType(Easing::EasingType::Liner);
	//	anim->SetXEaseType(Easing::EasingType::CubeRotate180);
	//	anim->SetYEaseType(Easing::EasingType::CubeRotate180);

	//	if (m_isFall) { return; }
	//	m_mapPos.x++;
	//	m_mapPos.y--;
	//}
	auto cAnim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	auto tAnim = gameObject.lock()->GetGameComponent<TransformAnimation>();
	if (cAnim || tAnim) { return; }
	m_isMoveDown = true;
	MoveRight(5);
}

void ButiEngine::Player::MoveLeftUp()
{
	//auto t = gameObject.lock()->transform;
	//auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	//if (!anim)
	//{
	//	m_vlp_timer->Reset();
	//	m_vlp_timer->Start();
	//	m_vlp_flashMeshTimer->Reset();
	//	m_vlp_flashMeshTimer->Start();
	//	m_vlp_expantionTimer->Reset();
	//	m_vlp_expantionTimer->Start();
	//	anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
	//	anim->SetSpeed(1.0f / 10);
	//	anim->SetTargetTransform(t->Clone());
	//	anim->GetTargetTransform()->Translate(Vector3(-m_length, m_length, 0));

	//	TargetTransformRotation_WorldZAxis_180(m_lookDirection, anim);
	//	anim->SetEaseType(Easing::EasingType::Liner);
	//	anim->SetXEaseType(Easing::EasingType::CubeRotate180);
	//	anim->SetYEaseType(Easing::EasingType::CubeRotate180);

	//	m_mapPos.x--;
	//	m_mapPos.y++;
	//}

	MoveUp(AfterMoveDirection::Left);
}

void ButiEngine::Player::MoveLeft(std::uint16_t arg_frame)
{
	auto tAnim = gameObject.lock()->GetGameComponent<TransformAnimation>();
	if (tAnim) { return; }

	auto t = gameObject.lock()->transform;
	auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	if (!anim)
	{
		if (!m_isMoveUp)
		{
			m_vlp_timer->Reset();
			m_vlp_timer->Start();
			m_vlp_flashMeshTimer->Reset();
			m_vlp_flashMeshTimer->Start();
			m_vlp_expantionTimer->Reset();
			m_vlp_expantionTimer->Start();
		}
		anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
		anim->SetSpeed(1.0f / arg_frame);
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
	//auto t = gameObject.lock()->transform;
	//auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	//if (!anim)
	//{
	//	m_vlp_timer->Reset();
	//	m_vlp_timer->Start();
	//	m_vlp_flashMeshTimer->Reset();
	//	m_vlp_flashMeshTimer->Start();
	//	m_vlp_expantionTimer->Reset();
	//	m_vlp_expantionTimer->Start();
	//	anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
	//	anim->SetSpeed(1.0f / 10);
	//	anim->SetTargetTransform(t->Clone());
	//	anim->GetTargetTransform()->Translate(Vector3(-m_length, -m_length, 0));

	//	TargetTransformRotation_WorldZAxis_180(m_lookDirection, anim);
	//	anim->SetEaseType(Easing::EasingType::Liner);
	//	anim->SetXEaseType(Easing::EasingType::CubeRotateMin180);
	//	anim->SetYEaseType(Easing::EasingType::CubeRotate180);

	//	if (m_isFall) { return; }
	//	m_mapPos.x--;
	//	m_mapPos.y--;
	//}
	auto cAnim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	auto tAnim = gameObject.lock()->GetGameComponent<TransformAnimation>();
	if (cAnim || tAnim) { return; }
	m_isMoveDown = true;
	MoveLeft(5);
}

void ButiEngine::Player::MoveUpFront()
{
	//auto t = gameObject.lock()->transform;
	//auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	//if (!anim)
	//{
	//	m_vlp_timer->Reset();
	//	m_vlp_timer->Start();
	//	m_vlp_flashMeshTimer->Reset();
	//	m_vlp_flashMeshTimer->Start();
	//	m_vlp_expantionTimer->Reset();
	//	m_vlp_expantionTimer->Start();
	//	anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
	//	anim->SetSpeed(1.0f / 10);
	//	anim->SetTargetTransform(t->Clone());
	//	anim->GetTargetTransform()->Translate(Vector3(0, m_length, m_length));

	//	TargetTransformRotation_WorldXAxis_180(m_lookDirection, anim);
	//	anim->SetEaseType(Easing::EasingType::Liner);
	//	anim->SetYEaseType(Easing::EasingType::CubeRotate180);
	//	anim->SetZEaseType(Easing::EasingType::CubeRotateMin180);

	//	m_mapPos.y++;
	//	m_mapPos.z++;
	//}

	MoveUp(AfterMoveDirection::Front);
}

void ButiEngine::Player::MoveFront(std::uint16_t arg_frame)
{
	auto tAnim = gameObject.lock()->GetGameComponent<TransformAnimation>();
	if (tAnim) { return; }

	auto t = gameObject.lock()->transform;
	auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	if (!anim)
	{
		if (!m_isMoveUp)
		{
			m_vlp_timer->Reset();
			m_vlp_timer->Start();
			m_vlp_flashMeshTimer->Reset();
			m_vlp_flashMeshTimer->Start();
			m_vlp_expantionTimer->Reset();
			m_vlp_expantionTimer->Start();
		}
		anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
		anim->SetSpeed(1.0f / arg_frame);
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
	//auto t = gameObject.lock()->transform;
	//auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	//if (!anim)
	//{
	//	m_vlp_timer->Reset();
	//	m_vlp_timer->Start();
	//	m_vlp_flashMeshTimer->Reset();
	//	m_vlp_flashMeshTimer->Start();
	//	m_vlp_expantionTimer->Reset();
	//	m_vlp_expantionTimer->Start();
	//	anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
	//	anim->SetSpeed(1.0f / 10);
	//	anim->SetTargetTransform(t->Clone());
	//	anim->GetTargetTransform()->Translate(Vector3(0, -m_length, m_length));

	//	TargetTransformRotation_WorldXAxis_180(m_lookDirection, anim);
	//	anim->SetEaseType(Easing::EasingType::Liner);
	//	anim->SetYEaseType(Easing::EasingType::CubeRotate180);
	//	anim->SetZEaseType(Easing::EasingType::CubeRotate180);

	//	if (m_isFall) { return; }
	//	m_mapPos.y--;
	//	m_mapPos.z++;
	//}
	auto cAnim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	auto tAnim = gameObject.lock()->GetGameComponent<TransformAnimation>();
	if (cAnim || tAnim) { return; }
	m_isMoveDown = true;
	MoveFront(5);
}

void ButiEngine::Player::MoveUpBack()
{
	//auto t = gameObject.lock()->transform;
	//auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	//if (!anim)
	//{
	//	m_vlp_timer->Reset();
	//	m_vlp_timer->Start();
	//	m_vlp_flashMeshTimer->Reset();
	//	m_vlp_flashMeshTimer->Start();
	//	m_vlp_expantionTimer->Reset();
	//	m_vlp_expantionTimer->Start();
	//	anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
	//	anim->SetSpeed(1.0f / 10);
	//	anim->SetTargetTransform(t->Clone());
	//	anim->GetTargetTransform()->Translate(Vector3(0, m_length, -m_length));

	//	TargetTransformRotation_WorldXAxis_m180(m_lookDirection, anim);
	//	anim->SetEaseType(Easing::EasingType::Liner);
	//	anim->SetYEaseType(Easing::EasingType::CubeRotate180);
	//	anim->SetZEaseType(Easing::EasingType::CubeRotate180);

	//	m_mapPos.y++;
	//	m_mapPos.z--;
	//}

	MoveUp(AfterMoveDirection::Back);
}

void ButiEngine::Player::MoveBack(std::uint16_t arg_frame)
{
	auto tAnim = gameObject.lock()->GetGameComponent<TransformAnimation>();
	if (tAnim) { return; }

	auto t = gameObject.lock()->transform;
	auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	if (!anim)
	{
		if (!m_isMoveUp)
		{
			m_vlp_timer->Reset();
			m_vlp_timer->Start();
			m_vlp_flashMeshTimer->Reset();
			m_vlp_flashMeshTimer->Start();
			m_vlp_expantionTimer->Reset();
			m_vlp_expantionTimer->Start();
		}
		anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
		anim->SetSpeed(1.0f / arg_frame);
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
	//auto t = gameObject.lock()->transform;
	//auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	//if (!anim)
	//{
	//	m_vlp_timer->Reset();
	//	m_vlp_timer->Start();
	//	m_vlp_flashMeshTimer->Reset();
	//	m_vlp_flashMeshTimer->Start();
	//	m_vlp_expantionTimer->Reset();
	//	m_vlp_expantionTimer->Start();
	//	anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
	//	anim->SetSpeed(1.0f / 10);
	//	anim->SetTargetTransform(t->Clone());
	//	anim->GetTargetTransform()->Translate(Vector3(0, -m_length, -m_length));

	//	TargetTransformRotation_WorldXAxis_m180(m_lookDirection, anim);
	//	anim->SetEaseType(Easing::EasingType::Liner);
	//	anim->SetYEaseType(Easing::EasingType::CubeRotate180);
	//	anim->SetZEaseType(Easing::EasingType::CubeRotateMin180);

	//	if (m_isFall) { return; }
	//	m_mapPos.y--;
	//	m_mapPos.z--;
	//}

	auto cAnim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	auto tAnim = gameObject.lock()->GetGameComponent<TransformAnimation>();
	if (cAnim || tAnim) { return; }
	m_isMoveDown = true;
	MoveBack(5);
}

void ButiEngine::Player::MoveUp(const AfterMoveDirection arg_dir)
{
	auto t = gameObject.lock()->transform;
	auto cAnim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	auto tAnim = gameObject.lock()->GetGameComponent<TransformAnimation>();
	if (!cAnim && !tAnim)
	{
		m_isMoveUp = true;
		m_afterMoveDirection = arg_dir;

		m_vlp_timer->Reset();
		m_vlp_timer->Start();
		m_vlp_flashMeshTimer->Reset();
		m_vlp_flashMeshTimer->Start();
		m_vlp_expantionTimer->Reset();
		m_vlp_expantionTimer->Start();
		tAnim = gameObject.lock()->AddGameComponent<TransformAnimation>();
		tAnim->SetSpeed(1.0f / 4);
		tAnim->SetTargetTransform(t->Clone());
		tAnim->GetTargetTransform()->TranslateY(m_length);

		tAnim->SetEaseType(Easing::EasingType::Liner);

		if (m_isFall) { return; }
		m_mapPos.y++;
	}
}

void ButiEngine::Player::MoveDown()
{
	auto t = gameObject.lock()->transform;
	auto cAnim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	auto tAnim = gameObject.lock()->GetGameComponent<TransformAnimation>();
	if (!cAnim && !tAnim)
	{
		tAnim = gameObject.lock()->AddGameComponent<TransformAnimation>();
		tAnim->SetSpeed(1.0f / 5);
		tAnim->SetTargetTransform(t->Clone());
		tAnim->GetTargetTransform()->TranslateY(-m_length);

		tAnim->SetEaseType(Easing::EasingType::Liner);

		if (m_isFall) { return; }
		m_mapPos.y--;
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
			Vector3 pos = m_mapPos;
			pos.y = i;
			if (IsCantThroughBlock(pos))
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

		m_vwp_rightCameraMesh.lock()->transform->SetLocalScale(0.0f);
		m_vwp_leftCameraMesh.lock()->transform->SetLocalScale(0.0f);
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
			FlashMeshSet(gameObject.lock()->transform, m_lookDirection, m_mapPos);
			if (m_vwp_eyeBlockComponent.lock())
			{
				m_vwp_eyeBlockComponent.lock()->FlashMeshSet(m_mapPos);
				m_vwp_eyeBlockComponent.lock()->CheckLookBlock();
				m_vwp_eyeBlockComponent.lock()->Flash();
			}
			else
			{
				m_vwp_rightCameraMesh.lock()->GetGameComponent<CameraMesh>()->Flash();
			}
			m_vwp_leftCameraMesh.lock()->GetGameComponent<CameraMesh>()->Flash();
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
	auto& mapData = m_vwp_mapComponent.lock()->GetCurrentMapData().lock()->m_vec_mapDatas;

	if (arg_movePos.x >= mapData[0][0].size() ||
		arg_movePos.y >= mapData.size() ||
		arg_movePos.z >= mapData[0].size() ||
		arg_movePos.x < 0 || arg_movePos.y < 0 || arg_movePos.z < 0)
	{
		output = MoveDirection::No;
		return output;
	}

	if (IsCantThroughBlock(arg_movePos))
	{
		if (arg_movePos.y + 1 >= mapData.size() ||
			IsCantThroughBlock(Vector3(arg_movePos.x, arg_movePos.y + 1, arg_movePos.z)) ||
			IsCantThroughBlock(Vector3(m_mapPos.x, m_mapPos.y + 1, m_mapPos.z)))
		{
			output = MoveDirection::No;
		}
		else
		{
			output = MoveDirection::Up;
		}
	}
	else if (IsCantThroughBlock(Vector3(arg_movePos.x, arg_movePos.y - 1, arg_movePos.z)))
	{
		output = MoveDirection::Normal;
	}
	else if (arg_movePos.y - 2 >= 0 && IsCantThroughBlock(Vector3(arg_movePos.x, arg_movePos.y - 2, arg_movePos.z)))
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
	auto& mapData = m_vwp_mapComponent.lock()->GetCurrentMapData().lock()->m_vec_mapDatas;
	if (IsCantThroughBlock(Vector3(arg_movePos.x, arg_movePos.y - 1, arg_movePos.z)))
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
		m_afterFallPos.Round();

		m_mapPos.y -= diff;
		m_mapPos.y += 1;
	}
	else
	{
		GetManager().lock()->GetGameObject("MainCamera").lock()->GetGameComponent<CameraController>()->RemoveChaseComponent();
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

bool ButiEngine::Player::IsCantThroughBlock(const Vector3& arg_mapPos)
{
	auto mapData = m_vwp_mapComponent.lock()->GetCurrentMapData().lock()->m_vec_mapDatas;
	auto mapObjectData = m_vwp_mapComponent.lock()->GetMapObjectData();

	std::uint16_t mapChipNum = mapData[arg_mapPos.y][arg_mapPos.z][arg_mapPos.x];

	if (mapChipNum == GameSettings::MAP_CHIP_BLOCK) { return true; }
	if (mapChipNum == GameSettings::MAP_CHIP_GLASS) { return true; }
	if (mapChipNum >= GameSettings::MAP_CHIP_INVISIBLEBLOCK && mapChipNum < GameSettings::MAP_CHIP_PLAYER_AND_GOAL)
	{
		auto object = mapObjectData[arg_mapPos.y][arg_mapPos.z][arg_mapPos.x];
		auto invisibleBlockComponent = object.lock()->GetGameComponent<InvisibleBlock>();
		if (invisibleBlockComponent && invisibleBlockComponent->IsBlock())
		{
			return true;
		}
	}
	return false;
}

void ButiEngine::Player::OnMoveUp()
{
	auto tAnim = gameObject.lock()->GetGameComponent<TransformAnimation>();
	if (tAnim) { return; }
	
	switch (m_afterMoveDirection)
	{
	case AfterMoveDirection::Right:
		MoveRight(6);
		break;
	case AfterMoveDirection::Left:
		MoveLeft(6);
		break;
	case AfterMoveDirection::Front:
		MoveFront(6);
		break;
	case AfterMoveDirection::Back:
		MoveBack(6);
		break;
	}
}

void ButiEngine::Player::OnMoveDown()
{
	auto cAnim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	if (cAnim) { return; }
	MoveDown();
}

bool ButiEngine::Player::CanPutEyeBlock(Value_weak_ptr<GameObject> arg_object)
{
	if (!g_canPutEyeBlock) { return false; }
	if (!arg_object.lock()) { return true; }

	auto name = arg_object.lock()->GetGameObjectName();
	if (StringHelper::Contains(name, "Goal") || StringHelper::Contains(name, "NextStageBlock"))
	{
		return false;
	}
	return true;
}
