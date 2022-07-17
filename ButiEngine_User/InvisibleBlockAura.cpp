#include "stdafx_u.h"
#include "InvisibleBlockAura.h"

void ButiEngine::InvisibleBlockAura::OnUpdate()
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

void ButiEngine::InvisibleBlockAura::OnSet()
{
}

void ButiEngine::InvisibleBlockAura::Start()
{
	progressFrame = 0;
	life = 60;
	AnimInitialize();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::InvisibleBlockAura::Clone()
{
	return ObjectFactory::Create<InvisibleBlockAura>();
}

void ButiEngine::InvisibleBlockAura::AnimInitialize()
{
	//auto t = gameObject.lock()->transform;

	//auto anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
	//anim->SetSpeed(1.0f / life);
	//anim->SetTargetTransform(t->Clone());
	//anim->GetTargetTransform()->SetLocalScale(2.0f);
	//anim->GetTargetTransform()->RollLocalRotationX_Degrees(0.1f);

	//anim->SetEaseType(Easing::EasingType::EaseOutQuart);
}
