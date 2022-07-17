#include "stdafx_u.h"
#include "Player.h"
#include "MapComponent.h"
#include "EasyGoal.h"
#include "DefaultGoal.h"
#include "InvisibleBlockManagerComponent.h"
#include "GameSettings.h"
#include "RippleComponent.h"
#include "StartPlayerDirectingComponent.h"
#include "InvisibleBlockComponent.h"
#include "ShakeComponent.h"
#include "InputManager.h"

void ButiEngine::Player::OnUpdate()
{
	//auto directing = gameObject.lock()->GetGameComponent<StartPlayerDirectingComponent>();
	//if (gameObject.lock()->transform->GetWorldPosition().y < -40.0f)
	//{
	//	fallTimer->Stop();
	//	fall = false;
	//	fallStart = false;
	//	mapPos = shp_map->GetPlayerPos();
	//	auto rotation = gameObject.lock()->transform->GetLocalRotation().GetEulerOneValue_local();
	//	rotation.x = 0;
	//	rotation.y = MathHelper::ToRadian(startRotation);
	//	rotation.z = 0;
	//	gameObject.lock()->transform->SetLocalRotation(Matrix4x4().CreateFromEuler_local(rotation));
	//	gameObject.lock()->transform->SetWorldPosition(directing->GetSpawnPos());
	//	directing->Animation();
	//}

	//if (!directing->IsStart())
	//{
	//	return;
	//}
	//if (!goal)
	//{
	//	Contoroll();
	//}
	//if (timer->Update())
	//{
	//	timer->Stop();
	//	Expansion();

	//	int rand = ButiRandom::GetRandom(0, 2, 1);

	//	gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->Play(moveSounds[rand], 0.1f);

	//	//”g–ä
	//	auto pos = gameObject.lock()->transform->GetWorldPosition();
	//	pos.y -= 0.3f;
	//	GetManager().lock()->AddObjectFromCereal("Ripple", ObjectFactory::Create<Transform>(pos, Vector3(90, 0, 0), 0.0f));

	//	CheckLookBlock();
	//	//ƒtƒ‰ƒbƒVƒ…
	//	GetManager().lock()->GetGameObject("CameraMesh").lock()->GetGameComponent<CameraMeshComponent>()->Flash();
	//	shp_invisibleBlockManager->CheckSeen();
	//	CheckExistUnderBlock(mapPos);
	//}
	//Shrink();
	//Fall();
}

void ButiEngine::Player::OnSet()
{
	//auto collisionLambda = std::function<void(Value_weak_ptr<GameObject>&)>([this](Value_weak_ptr<GameObject>& arg_vwp_other)->void
	//	{
	//		if (!IsRollFinish() || goal)
	//		{
	//			return;
	//		}
	//		GameObjectTag tag = arg_vwp_other.lock()->GetGameObjectTag();
	//		std::string tagStr = GetTagManager()->GetTagName(tag);
	//		if (tagStr != "Goal")
	//		{
	//			return;
	//		}

	//		std::vector<std::vector<std::vector<int>>>& mapData = shp_map->GetCurrentMapData()->mapData;
	//		int mapNum = mapData[mapPos.y][mapPos.z][mapPos.x];
	//		if (mapNum >= GameSettings::playerAndGoal)
	//		{
	//			mapNum = (mapNum - GameSettings::playerAndGoal) / 10;
	//		}

	//		if (mapNum == GameSettings::tutorialGoal)
	//		{
	//			goal = true;
	//		}
	//		else if (mapNum == GameSettings::easyGoal && arg_other.lock()->GetGameComponent<EasyGoalComponent>()->IsActive())
	//		{
	//			goal = true;
	//		}
	//		else if (mapNum == GameSettings::defaultGoal && arg_other.lock()->GetGameComponent<DefaultGoalComponent>()->IsActive())
	//		{
	//			goal = true;
	//		}

	//		if (goal)
	//		{
	//			auto seTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/TouchGoal.wav");
	//			gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->Play(seTag, 0.1f);
	//		}
	//	});

	//gameObject.lock()->AddCollisionStayReaction(collisionLambda);
}

void ButiEngine::Player::OnRemove()
{
}

void ButiEngine::Player::OnShowUI()
{
}

void ButiEngine::Player::Start()
{
	//for (int i = 0; i < 3; i++) {

	//	auto seTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Move_" + std::to_string(i) + ".wav");

	//	moveSounds[i] = seTag;
	//}

	//gameObject.lock()->SetObjectName("Player");
	//goal = false;
	//fall = false;
	//fallStart = false;
	//afterFallPos = Vector3::Zero;
	//length = 1.0f;
	//shp_map = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("Map").lock()->GetGameComponent<MapComponent>();
	//mapPos = shp_map->GetPlayerPos();
	//offset = mapPos - startPos;
	//timer = ObjectFactory::Create<RelativeTimer>(10);
	//fallTimer = ObjectFactory::Create<RelativeTimer>(24);
	//fallTimer->Stop();
	//shp_invisibleBlockManager = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("InvisibleBlockManager").lock()->GetGameComponent<InvisibleBlockManagerComponent>();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Player::Clone()
{
	return ObjectFactory::Create<Player>();
}

bool ButiEngine::Player::IsRollFinish()
{
	//return !timer->IsOn();
	return false;
}

void ButiEngine::Player::CheckLookBlock()
{
	//shp_invisibleBlockManager->Reset();
	//CheckLookDirection();
	//std::weak_ptr<GameObject> hitObj;

	//Vector3 pos = gameObject.lock()->transform->GetWorldPosition();
	//Vector3 bPos = pos;

	//if (lookDirection == LookDirection::Right)
	//{
	//	bPos.x += 100;
	//	hitObj = GetRightBlock(mapPos);
	//}
	//else if (lookDirection == LookDirection::Left)
	//{
	//	bPos.x -= 100;
	//	hitObj = GetLeftBlock(mapPos);
	//}
	//else if (lookDirection == LookDirection::Up)
	//{
	//	bPos.y += 100;
	//	hitObj = GetUpBlock(mapPos);
	//}
	//else if (lookDirection == LookDirection::Down)
	//{
	//	bPos.y -= 100;
	//	hitObj = GetDownBlock(mapPos);
	//}
	//else if (lookDirection == LookDirection::Front)
	//{
	//	bPos.z += 100;
	//	hitObj = GetFrontBlock(mapPos);
	//}
	//else if (lookDirection == LookDirection::Back)
	//{
	//	bPos.z -= 100;
	//	hitObj = GetBackBlock(mapPos);
	//}

	//if (hitObj.lock())
	//{
	//	bPos = hitObj.lock()->transform->GetWorldPosition();
	//}
	//Vector3 midPoint = Vector3((pos.x + bPos.x) * 0.5f, (pos.y + bPos.y) * 0.5f, (pos.z + bPos.z) * 0.5f);

	//auto cameraMesh = GetManager().lock()->GetGameObject("CameraMesh");
	//cameraMesh.lock()->transform->SetWorldPosition(midPoint);

	//Vector3 cameraMeshScale = Vector3::Zero;
	//if (lookDirection == LookDirection::Right || lookDirection == LookDirection::Left)
	//{
	//	cameraMeshScale = Vector3(pos.Distance(bPos), 0.1f, 0.1f);
	//}
	//else if (lookDirection == LookDirection::Up || lookDirection == LookDirection::Down)
	//{
	//	cameraMeshScale = Vector3(0.1f, pos.Distance(bPos), 0.1f);
	//}
	//else if (lookDirection == LookDirection::Front || lookDirection == LookDirection::Back)
	//{
	//	cameraMeshScale = Vector3(0.1f, 0.1f, pos.Distance(bPos));
	//}
	//cameraMesh.lock()->transform->SetLocalScale(cameraMeshScale);

	//if (!hitObj.lock())
	//{
	//	return;
	//}

	//auto objTag = hitObj.lock()->GetGameObjectTag();
	//if (objTag == GetTagManager()->GetObjectTag("Goal"))
	//{
	//	auto eGoalComp = hitObj.lock()->GetGameComponent<EasyGoalComponent>();
	//	auto dGoalComp = hitObj.lock()->GetGameComponent<DefaultGoalComponent>();
	//	if (eGoalComp)
	//	{
	//		eGoalComp->Seen();
	//	}
	//	else if (dGoalComp)
	//	{
	//		dGoalComp->Seen();
	//	}
	//}
	//else if (objTag == GetTagManager()->GetObjectTag("InvisibleBlock"))
	//{
	//	auto invBlockComp = hitObj.lock()->GetGameComponent<InvisibleBlockComponent>();
	//	if (invBlockComp)
	//	{
	//		invBlockComp->Seen();
	//	}
	//}
}

void ButiEngine::Player::RollCameraDirection(const int rotateDir)
{
	//cameraDirection = (CameraDirection)((int)cameraDirection + rotateDir);

	//if (cameraDirection > CameraDirection::Right) {
	//	cameraDirection = (CameraDirection)((int)cameraDirection % 4);
	//}
	//else if (cameraDirection < CameraDirection::Front) {
	//	cameraDirection = (CameraDirection)((int)cameraDirection + 4);
	//}
}

void ButiEngine::Player::CheckLookDirection()
{
	//Vector3 front = gameObject.lock()->transform->GetFront() * 10.0f;
	//Vector3 dir = front - gameObject.lock()->transform->GetWorldPosition();

	//float x = abs(dir.x);
	//float y = abs(dir.y);
	//float z = abs(dir.z);

	//if (x > y && x > z)
	//{
	//	if (dir.x > 0)
	//	{
	//		lookDirection = LookDirection::Right;
	//	}
	//	if (dir.x < 0)
	//	{
	//		lookDirection = LookDirection::Left;
	//	}
	//}

	//if (y > x && y > z)
	//{
	//	if (dir.y > 0)
	//	{
	//		lookDirection = LookDirection::Up;
	//	}
	//	if (dir.y < 0)
	//	{
	//		lookDirection = LookDirection::Down;
	//	}
	//}

	//if (z > x && z > y)
	//{
	//	if (dir.z > 0)
	//	{
	//		lookDirection = LookDirection::Front;
	//	}
	//	if (dir.z < 0)
	//	{
	//		lookDirection = LookDirection::Back;
	//	}
	//}
}

void ButiEngine::Player::Contoroll()
{
	auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	if (anim) { return; }
	if (InputManager::IsPushRightKey())
	{
		switch (cameraDirection)
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
	if (InputManager::IsPushLeftKey())
	{

		switch (cameraDirection)
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
		switch (cameraDirection)
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
	if (InputManager::IsPushBackKey())
	{
		switch (cameraDirection)
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
	if (!fall)
	{
		MoveDirection dir = CheckMoveDirection(Vector3(mapPos.x + 1, mapPos.y, mapPos.z));
		if (dir == MoveDirection::Up) { MoveRightUp(); }
		else if (dir == MoveDirection::Normal) { MoveRight(); }
		else if (dir == MoveDirection::Down) { MoveRightDown(); }
		else if (dir == MoveDirection::Fall) { MoveRightDown(); }
	}


	auto cubeAnim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	if (cubeAnim) {
		nextMapPos = cubeAnim->GetTargetTransform()->GetWorldPosition().Ceil();
		nextMapPos += offset;
	}
}

void ButiEngine::Player::OnPushLeft()
{
	if (!fall)
	{
		MoveDirection dir = CheckMoveDirection(Vector3(mapPos.x - 1, mapPos.y, mapPos.z));
		if (dir == MoveDirection::Up) { MoveLeftUp(); }
		else if (dir == MoveDirection::Normal) { MoveLeft(); }
		else if (dir == MoveDirection::Down) { MoveLeftDown(); }
		else if (dir == MoveDirection::Fall) { MoveLeftDown(); }
	}


	auto cubeAnim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	if (cubeAnim) {
		nextMapPos = cubeAnim->GetTargetTransform()->GetWorldPosition().Ceil();
		nextMapPos += offset;
	}
}

void ButiEngine::Player::OnPushFront()
{
	if (!fall)
	{
		MoveDirection dir = CheckMoveDirection(Vector3(mapPos.x, mapPos.y, mapPos.z + 1));
		if (dir == MoveDirection::Up) { MoveUpFront(); }
		else if (dir == MoveDirection::Normal) { MoveFront(); }
		else if (dir == MoveDirection::Down) { MoveDownFront(); }
		else if (dir == MoveDirection::Fall) { MoveDownFront(); }
	}

	auto cubeAnim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	if (cubeAnim) {
		nextMapPos = cubeAnim->GetTargetTransform()->GetWorldPosition().Ceil();
		nextMapPos += offset;
	}
}

void ButiEngine::Player::OnPushBack()
{
	if (!fall)
	{
		MoveDirection dir = CheckMoveDirection(Vector3(mapPos.x, mapPos.y, mapPos.z - 1));
		if (dir == MoveDirection::Up) { MoveUpBack(); }
		else if (dir == MoveDirection::Normal) { MoveBack(); }
		else if (dir == MoveDirection::Down) { MoveDownBack(); }
		else if (dir == MoveDirection::Fall) { MoveDownBack(); }
	}

	auto cubeAnim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	if (cubeAnim) {
		nextMapPos = cubeAnim->GetTargetTransform()->GetWorldPosition().Ceil();
		nextMapPos += offset;
	}
}

void ButiEngine::Player::Expansion()
{
	scale = 1.5f;
	gameObject.lock()->transform->SetLocalScale(scale);
}

void ButiEngine::Player::Shrink()
{
	if (scale <= 1.0f)
	{
		return;
	}
	scale -= 0.1f;
	if (scale < 1.0f)
	{
		scale = 1.0f;
	}
	gameObject.lock()->transform->SetLocalScale(scale);
}

void ButiEngine::Player::MoveRightUp()
{
	//auto t = gameObject.lock()->transform;
	//auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	//if (!anim)
	//{
	//	timer->Reset();
	//	timer->Start();
	//	anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
	//	anim->SetSpeed(1.0f / 10);
	//	anim->SetTargetTransform(t->Clone());
	//	anim->GetTargetTransform()->Translate(Vector3(length, length, 0));//

	//	anim->GetTargetTransform()->RollWorldRotationZ_Degrees(-179.99f);//
	//	anim->SetEaseType(Easing::EasingType::Liner);
	//	anim->SetXEaseType(Easing::EasingType::CubeRotateMin180);
	//	anim->SetYEaseType(Easing::EasingType::CubeRotate180);

	//	mapPos.x++;
	//	mapPos.y++;
	//}
}

void ButiEngine::Player::MoveRight()
{
	//auto t = gameObject.lock()->transform;
	//auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	//if (!anim)
	//{
	//	timer->Reset();
	//	timer->Start();
	//	anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
	//	anim->SetSpeed(1.0f / 10);
	//	anim->SetTargetTransform(t->Clone());
	//	anim->GetTargetTransform()->TranslateX(length);

	//	anim->GetTargetTransform()->RollWorldRotationZ_Degrees(-90.0f);
	//	anim->SetEaseType(Easing::EasingType::Liner);
	//	anim->SetYEaseType(Easing::EasingType::CubeRotate90);

	//	mapPos.x++;
	//}
}

void ButiEngine::Player::MoveRightDown()
{
	//auto t = gameObject.lock()->transform;
	//auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	//if (!anim)
	//{
	//	timer->Reset();
	//	timer->Start();
	//	anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
	//	anim->SetSpeed(1.0f / 10);
	//	anim->SetTargetTransform(t->Clone());
	//	anim->GetTargetTransform()->Translate(Vector3(length, -length, 0));

	//	anim->GetTargetTransform()->RollWorldRotationZ_Degrees(-179.99f);
	//	anim->SetEaseType(Easing::EasingType::Liner);
	//	anim->SetXEaseType(Easing::EasingType::CubeRotate180);
	//	anim->SetYEaseType(Easing::EasingType::CubeRotate180);

	//	if (fall) { return; }
	//	mapPos.x++;
	//	mapPos.y--;
	//}
}

void ButiEngine::Player::MoveLeftUp()
{
	//auto t = gameObject.lock()->transform;
	//auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	//if (!anim)
	//{
	//	timer->Reset();
	//	timer->Start();
	//	anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
	//	anim->SetSpeed(1.0f / 10);
	//	anim->SetTargetTransform(t->Clone());
	//	anim->GetTargetTransform()->Translate(Vector3(-length, length, 0));

	//	anim->GetTargetTransform()->RollWorldRotationZ_Degrees(179.99f);
	//	anim->SetEaseType(Easing::EasingType::Liner);
	//	anim->SetXEaseType(Easing::EasingType::CubeRotate180);
	//	anim->SetYEaseType(Easing::EasingType::CubeRotate180);

	//	mapPos.x--;
	//	mapPos.y++;
	//}
}

void ButiEngine::Player::MoveLeft()
{
	//auto t = gameObject.lock()->transform;
	//auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	//if (!anim)
	//{
	//	timer->Reset();
	//	timer->Start();
	//	anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
	//	anim->SetSpeed(1.0f / 10);
	//	anim->SetTargetTransform(t->Clone());
	//	anim->GetTargetTransform()->TranslateX(-length);

	//	anim->GetTargetTransform()->RollWorldRotationZ_Degrees(90.0f);
	//	anim->SetEaseType(Easing::EasingType::Liner);
	//	anim->SetYEaseType(Easing::EasingType::CubeRotate90);

	//	mapPos.x--;
	//}
}

void ButiEngine::Player::MoveLeftDown()
{
	//auto t = gameObject.lock()->transform;
	//auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	//if (!anim)
	//{
	//	timer->Reset();
	//	timer->Start();
	//	anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
	//	anim->SetSpeed(1.0f / 10);
	//	anim->SetTargetTransform(t->Clone());
	//	anim->GetTargetTransform()->Translate(Vector3(-length, -length, 0));

	//	anim->GetTargetTransform()->RollWorldRotationZ_Degrees(179.99f);
	//	anim->SetEaseType(Easing::EasingType::Liner);
	//	anim->SetXEaseType(Easing::EasingType::CubeRotateMin180);
	//	anim->SetYEaseType(Easing::EasingType::CubeRotate180);

	//	if (fall) { return; }
	//	mapPos.x--;
	//	mapPos.y--;
	//}
}

void ButiEngine::Player::MoveUpFront()
{
	//auto t = gameObject.lock()->transform;
	//auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	//if (!anim)
	//{
	//	timer->Reset();
	//	timer->Start();
	//	anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
	//	anim->SetSpeed(1.0f / 10);
	//	anim->SetTargetTransform(t->Clone());
	//	anim->GetTargetTransform()->Translate(Vector3(0, length, length));

	//	anim->GetTargetTransform()->RollWorldRotationX_Degrees(179.99f);
	//	anim->SetEaseType(Easing::EasingType::Liner);
	//	anim->SetYEaseType(Easing::EasingType::CubeRotate180);
	//	anim->SetZEaseType(Easing::EasingType::CubeRotateMin180);

	//	mapPos.y++;
	//	mapPos.z++;
	//}
}

void ButiEngine::Player::MoveFront()
{
	//auto t = gameObject.lock()->transform;
	//auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	//if (!anim)
	//{
	//	timer->Reset();
	//	timer->Start();
	//	anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
	//	anim->SetSpeed(1.0f / 10);
	//	anim->SetTargetTransform(t->Clone());
	//	anim->GetTargetTransform()->TranslateZ(length);

	//	anim->GetTargetTransform()->RollWorldRotationX_Degrees(90.0f);
	//	anim->SetEaseType(Easing::EasingType::Liner);
	//	anim->SetYEaseType(Easing::EasingType::CubeRotate90);

	//	mapPos.z++;
	//}
}

void ButiEngine::Player::MoveDownFront()
{
	//auto t = gameObject.lock()->transform;
	//auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	//if (!anim)
	//{
	//	timer->Reset();
	//	timer->Start();
	//	anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
	//	anim->SetSpeed(1.0f / 10);
	//	anim->SetTargetTransform(t->Clone());
	//	anim->GetTargetTransform()->Translate(Vector3(0, -length, length));

	//	anim->GetTargetTransform()->RollWorldRotationX_Degrees(179.99f);
	//	anim->SetEaseType(Easing::EasingType::Liner);
	//	anim->SetYEaseType(Easing::EasingType::CubeRotate180);
	//	anim->SetZEaseType(Easing::EasingType::CubeRotate180);

	//	if (fall) { return; }
	//	mapPos.y--;
	//	mapPos.z++;
	//}
}

void ButiEngine::Player::MoveUpBack()
{
	//auto t = gameObject.lock()->transform;
	//auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	//if (!anim)
	//{
	//	timer->Reset();
	//	timer->Start();
	//	anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
	//	anim->SetSpeed(1.0f / 10);
	//	anim->SetTargetTransform(t->Clone());
	//	anim->GetTargetTransform()->Translate(Vector3(0, length, -length));

	//	anim->GetTargetTransform()->RollWorldRotationX_Degrees(-179.99f);
	//	anim->SetEaseType(Easing::EasingType::Liner);
	//	anim->SetYEaseType(Easing::EasingType::CubeRotate180);
	//	anim->SetZEaseType(Easing::EasingType::CubeRotate180);

	//	mapPos.y++;
	//	mapPos.z--;
	//}
}

void ButiEngine::Player::MoveBack()
{
	//auto t = gameObject.lock()->transform;
	//auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	//if (!anim)
	//{
	//	timer->Reset();
	//	timer->Start();
	//	anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
	//	anim->SetSpeed(1.0f / 10);
	//	anim->SetTargetTransform(t->Clone());
	//	anim->GetTargetTransform()->TranslateZ(-length);

	//	anim->GetTargetTransform()->RollWorldRotationX_Degrees(-90.0f);
	//	anim->SetEaseType(Easing::EasingType::Liner);
	//	anim->SetYEaseType(Easing::EasingType::CubeRotate90);

	//	mapPos.z--;
	//}
}

void ButiEngine::Player::MoveDownBack()
{
	//auto t = gameObject.lock()->transform;
	//auto anim = gameObject.lock()->GetGameComponent<CubeTransformAnimation>();
	//if (!anim)
	//{
	//	timer->Reset();
	//	timer->Start();
	//	anim = gameObject.lock()->AddGameComponent<CubeTransformAnimation>();
	//	anim->SetSpeed(1.0f / 10);
	//	anim->SetTargetTransform(t->Clone());
	//	anim->GetTargetTransform()->Translate(Vector3(0, -length, -length));

	//	anim->GetTargetTransform()->RollWorldRotationX_Degrees(-179.99f);
	//	anim->SetEaseType(Easing::EasingType::Liner);
	//	anim->SetYEaseType(Easing::EasingType::CubeRotate180);
	//	anim->SetZEaseType(Easing::EasingType::CubeRotateMin180);

	//	if (fall) { return; }
	//	mapPos.y--;
	//	mapPos.z--;
	//}
}

ButiEngine::Value_weak_ptr<ButiEngine::GameObject> ButiEngine::Player::GetRightBlock(Vector3 mapPos)
{
	//auto mapData = shp_map->GetCurrentMapData()->mapData;
	//for (unsigned int i = mapPos.x + 1; i < mapData[mapPos.y][mapPos.z].size(); i++)
	//{
	//	if (IsBlock(mapData[mapPos.y][mapPos.z][i]))
	//	{
	//		auto mapObjectData = shp_map->GetMapObjectData();
	//		return mapObjectData[mapPos.y][mapPos.z][i];
	//	}
	//}
	return Value_weak_ptr<GameObject>();
}

ButiEngine::Value_weak_ptr<ButiEngine::GameObject> ButiEngine::Player::GetLeftBlock(Vector3 mapPos)
{
	//auto mapData = shp_map->GetCurrentMapData()->mapData;
	//for (int i = mapPos.x - 1; i >= 0; i--)
	//{
	//	if (IsBlock(mapData[mapPos.y][mapPos.z][i]))
	//	{
	//		auto mapObjectData = shp_map->GetMapObjectData();
	//		return mapObjectData[mapPos.y][mapPos.z][i];
	//	}
	//}
	return Value_weak_ptr<GameObject>();
}

ButiEngine::Value_weak_ptr<ButiEngine::GameObject> ButiEngine::Player::GetUpBlock(Vector3 mapPos)
{
	//auto mapData = shp_map->GetCurrentMapData()->mapData;
	//for (unsigned int i = mapPos.y + 1; i < mapData.size(); i++)
	//{
	//	if (IsBlock(mapData[i][mapPos.z][mapPos.x]))
	//	{
	//		auto mapObjectData = shp_map->GetMapObjectData();
	//		return mapObjectData[i][mapPos.z][mapPos.x];
	//	}
	//}
	return Value_weak_ptr<GameObject>();
}

ButiEngine::Value_weak_ptr<ButiEngine::GameObject> ButiEngine::Player::GetDownBlock(Vector3 mapPos)
{
	//auto mapData = shp_map->GetCurrentMapData()->mapData;
	//for (int i = mapPos.y - 1; i >= 0; i--)
	//{
	//	if (IsBlock(mapData[i][mapPos.z][mapPos.x]))
	//	{
	//		auto mapObjectData = shp_map->GetMapObjectData();
	//		return mapObjectData[i][mapPos.z][mapPos.x];
	//	}
	//}
	return Value_weak_ptr<GameObject>();
}

ButiEngine::Value_weak_ptr<ButiEngine::GameObject> ButiEngine::Player::GetFrontBlock(Vector3 mapPos)
{
	//auto mapData = shp_map->GetCurrentMapData()->mapData;
	//for (unsigned int i = mapPos.z + 1; i < mapData[mapPos.y].size(); i++)
	//{
	//	if (IsBlock(mapData[mapPos.y][i][mapPos.x]))
	//	{
	//		auto mapObjectData = shp_map->GetMapObjectData();
	//		return mapObjectData[mapPos.y][i][mapPos.x];
	//	}
	//}
	return Value_weak_ptr<GameObject>();
}

ButiEngine::Value_weak_ptr<ButiEngine::GameObject> ButiEngine::Player::GetBackBlock(Vector3 mapPos)
{
	//auto mapData = shp_map->GetCurrentMapData()->mapData;
	//for (int i = mapPos.z - 1; i >= 0; i--)
	//{
	//	if (IsBlock(mapData[mapPos.y][i][mapPos.x]))
	//	{
	//		auto mapObjectData = shp_map->GetMapObjectData();
	//		return mapObjectData[mapPos.y][i][mapPos.x];
	//	}
	//}
	return Value_weak_ptr<GameObject>();
}

void ButiEngine::Player::Fall()
{
	//auto shake = gameObject.lock()->GetGameComponent<ShakeComponent>();
	//if (!fall && fallStart && !fallTimer->IsOn())
	//{
	//	fallTimer->Reset();
	//	fallTimer->Start();
	//	shake->SetDefaultPos(gameObject.lock()->transform->GetWorldPosition());
	//	shake->Start(0.06f);
	//	fallStart = false;
	//}

	//if (!fall && fallTimer->Update())
	//{
	//	fallTimer->Stop();
	//	fall = true;
	//}

	//if (!fall && fallTimer->GetRemainFrame() == 6)
	//{
	//	shake->Stop();
	//}

	//if (fall)
	//{
	//	gameObject.lock()->transform->TranslateY(-3.0f);
	//	if (gameObject.lock()->transform->GetWorldPosition().y <= afterFallPos.y)
	//	{
	//		gameObject.lock()->transform->SetWorldPosition(afterFallPos);
	//		fall = false;
	//	}
	//}
}

ButiEngine::MoveDirection ButiEngine::Player::CheckMoveDirection(Vector3 movePos)
{
	//MoveDirection output;
	//std::vector<std::vector<std::vector<int>>>& mapData = shp_map->GetCurrentMapData()->mapData;

	//if (movePos.x >= mapData[0][0].size() ||
	//	movePos.y >= mapData.size() ||
	//	movePos.z >= mapData[0].size() ||
	//	movePos.x < 0 || movePos.y < 0 || movePos.z < 0)
	//{
	//	output = MoveDirection::No;
	//	return output;
	//}

	//if (mapData[movePos.y][movePos.z][movePos.x] == GameSettings::MAP_CHIP_BLOCK)
	//{
	//	if (movePos.y + 1 >= mapData.size() ||
	//		mapData[movePos.y + 1][movePos.z][movePos.x] == GameSettings::MAP_CHIP_BLOCK ||
	//		mapData[mapPos.y + 1][mapPos.z][mapPos.x] == GameSettings::MAP_CHIP_BLOCK)
	//	{
	//		output = MoveDirection::No;
	//	}
	//	else
	//	{
	//		output = MoveDirection::Up;
	//	}
	//}
	//else if (mapData[movePos.y - 1][movePos.z][movePos.x] == GameSettings::MAP_CHIP_BLOCK)
	//{
	//	output = MoveDirection::Normal;
	//}
	//else if (movePos.y - 2 >= 0 && mapData[movePos.y - 2][movePos.z][movePos.x] == GameSettings::MAP_CHIP_BLOCK)
	//{
	//	output = MoveDirection::Down;
	//}
	//else
	//{
	//	output = MoveDirection::No;
	//}

	//return output;

	return MoveDirection::Up;
}

void ButiEngine::Player::CheckExistUnderBlock(Vector3 movePos)
{
	//if (fallTimer->IsOn())
	//{
	//	return;
	//}
	//std::vector<std::vector<std::vector<int>>>& mapData = shp_map->GetCurrentMapData()->mapData;
	//if (mapData[movePos.y - 1][movePos.z][movePos.x] == GameSettings::block)
	//{
	//	return;
	//}
	//fallStart = true;
	//afterFallPos = gameObject.lock()->transform->GetWorldPosition();
	//afterFallPos.y = -500.0f;
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
