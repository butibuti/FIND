#include "stdafx_u.h"
#include "DefaultGoal.h"
#include "Map.h"
#include "BackGround.h"
#include "CameraController.h"
#include "GameSettings.h"


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
	m_isActive = false;
	gameObject.lock()->GetGameComponent<MeshDrawComponent>()->UnRegist();

	gameObject.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>()->Get().color = GameSettings::ACTIVE_GOAL_COLOR;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::DefaultGoal::Clone()
{
	return ObjectFactory::Create<DefaultGoal>();
}

void ButiEngine::DefaultGoal::Seen()
{
	if (m_isActive)
	{
		return;
	}
	auto meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent>();
	meshDraw->Regist();
	m_isActive = true;

	gameObject.lock()->GetApplication().lock()->GetSoundManager()->PlaySE(SoundTag("Sound/potion.wav"), 0.1f);

	auto t = gameObject.lock()->transform;
	auto pos = t->GetWorldPosition();
	auto rot = t->GetWorldRotation();
	auto scale = t->GetLocalScale();

	GetManager().lock()->AddObjectFromCereal("GoalAura", ObjectFactory::Create<Transform>(pos, rot, scale));

	auto map = GetManager().lock()->GetGameObject("Map");
	auto mapComponent = map.lock()->GetGameComponent< Map>();
	mapComponent->SetMapEndColor(Vector4(1.0f, 1.0f, 0.5f, 1.0f));
	mapComponent->SetMapStartColor(Vector4(0.7f, 0.5f, 0.0f, 1.0f));
	mapComponent->ShakeStart(0.01f);

	GetManager().lock()->GetGameObject("MainCamera").lock()->GetGameComponent<CameraController>()->ZoomInOut();
	auto backGround = GetManager().lock()->GetGameObject("BackGroundController").lock()->GetGameComponent<BackGround>();
	backGround->SetEndColor(Vector4(1.0f, 1.0f, 0.5f, 1.0f));
	backGround->SetStartColor(Vector4(0.7f, 0.5f, 0.0f, 1.0f));
	backGround->SetIsClear(true);
}
