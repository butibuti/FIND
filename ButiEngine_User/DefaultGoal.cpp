#include "stdafx_u.h"
#include "DefaultGoal.h"
#include "Map.h"
#include "BackGround.h"


void ButiEngine::DefaultGoal::OnUpdate()
{
}

void ButiEngine::DefaultGoal::OnSet()
{
}

void ButiEngine::DefaultGoal::OnRemove()
{
}

void ButiEngine::DefaultGoal::OnShowUI()
{
}

void ButiEngine::DefaultGoal::Start()
{
	//active = false;
	//gameObject.lock()->GetGameComponent<MeshDrawComponent>()->UnRegist();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::DefaultGoal::Clone()
{
	return ObjectFactory::Create<DefaultGoal>();
}

void ButiEngine::DefaultGoal::Seen()
{
	//if (active)
	//{
	//	return;
	//}
	//auto meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent_Static>();
	//meshDraw->Regist();
	//active = true;


	//auto seTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/potion.wav");

	//gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->Play(seTag, 0.1f);
	////seTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/bubble.wav");

	////gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->Play(seTag, 0.1f);

	////マテリアル変更
	//meshDraw->SetMaterialTag(gameObject.lock()->GetResourceContainer()->GetMaterialTag("goalMaterial"));
	//meshDraw->ReRegist();

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

	//GetManager().lock()->GetGameObject("MainCamera").lock()->GetGameComponent<CameraController>()->ZoomInOut();
	//auto backGround = GetManager().lock()->GetGameObject("BackGroundController").lock()->GetGameComponent<BackGround>();
	//backGround->SetEndColor(Vector4(1.0f, 1.0f, 0.5f, 1.0f));
	//backGround->SetStartColor(Vector4(0.7f, 0.5f, 0.0f, 1.0f));
	//backGround->SetIsClear(true);
}
