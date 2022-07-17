#include "stdafx_u.h"
#include "GoalAura.h"

void ButiEngine::GoalAura::OnUpdate()
{
	//if (progressFrame > life)
	//{
	//	gameObject.lock()->SetIsRemove(true);
	//	return;
	//}

	//progressFrame++;
	//auto meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent>();
	//auto lightBuff = meshDraw->GetCBuffer<LightVariable>("LightBuffer");
	//lightBuff->Get().lightDir.w = 1.0f - Easing::EaseOutExpo(float(progressFrame) / life);
}

void ButiEngine::GoalAura::OnSet()
{
}

void ButiEngine::GoalAura::Start()
{
	progressFrame = 0;
	life = 60;
	AnimInitialize();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::GoalAura::Clone()
{
	return ObjectFactory::Create<GoalAura>();
}

void ButiEngine::GoalAura::AnimInitialize()
{
	//auto t = gameObject.lock()->transform;


	//auto anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
	//anim->SetSpeed(1.0f / life);
	//anim->SetTargetTransform(t->Clone());
	//anim->GetTargetTransform()->SetLocalScale(30.0f);
	//anim->GetTargetTransform()->RollLocalRotationX_Degrees(0.1f);

	//anim->SetEaseType(Easing::EasingType::EaseOutQuart);
}
