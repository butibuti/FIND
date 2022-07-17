#include "stdafx_u.h"
#include "EasyGoal.h"
#include "Map.h"
#include "BackGround.h"

void ButiEngine::EasyGoal::OnUpdate()
{
}

void ButiEngine::EasyGoal::OnSet()
{
}

void ButiEngine::EasyGoal::OnRemove()
{
}

void ButiEngine::EasyGoal::OnShowUI()
{
}

void ButiEngine::EasyGoal::Start()
{
	active = false;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::EasyGoal::Clone()
{
	return ObjectFactory::Create<EasyGoal>();
}

void ButiEngine::EasyGoal::Seen()
{
	//if (active)
	//{
	//	return;
	//}

	////マテリアル変更
	//auto meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent_Static>();
	//meshDraw->SetMaterialTag(gameObject.lock()->GetResourceContainer()->GetMaterialTag("goalMaterial"));
	//meshDraw->ReRegist();
	//active = true;

	//auto t = gameObject.lock()->transform;
	//auto pos = t->GetWorldPosition();
	//auto rot = t->GetWorldRotation();
	//auto scale = t->GetLocalScale();

	//GetManager().lock()->AddObjectFromCereal("GoalAura", ObjectFactory::Create<Transform>(pos, rot, scale));
	//auto map = GetManager().lock()->GetGameObject("Map");
	//auto mapComponent = map.lock()->GetGameComponent< MapComponent>();
	//mapComponent->SetMapEndColor(Vector4(1.0f, 1.0f, 0.5f, 1.0f));
	//mapComponent->SetMapStartColor(Vector4(0.7f, 0.5f, 0.0f, 1.0f));
	//mapComponent->ShakeStart(0.01f);



	//auto seTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/potion.wav");

	//gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->Play(seTag, 0.1f);

	//GetManager().lock()->GetGameObject("MainCamera").lock()->GetGameComponent<CameraController>()->ZoomInOut();
	//auto backGround = GetManager().lock()->GetGameObject("BackGroundController").lock()->GetGameComponent<BackGround>();
	//backGround->SetEndColor(Vector4(1.0f, 1.0f, 0.5f, 1.0f));
	//backGround->SetStartColor(Vector4(0.7f, 0.5f, 0.0f, 1.0f));
	//backGround->SetIsClear(true);
}
