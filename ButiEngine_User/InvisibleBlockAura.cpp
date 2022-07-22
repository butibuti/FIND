#include "stdafx_u.h"
#include "InvisibleBlockAura.h"

void ButiEngine::InvisibleBlockAura::OnUpdate()
{
	if (m_vlp_timer->Update())
	{
		m_vlp_timer->Stop();
		gameObject.lock()->SetIsRemove(true);
		return;
	}

	float alpha = 1.0f - Easing::EaseOutExpo(m_vlp_timer->GetPercent());
	gameObject.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>()->Get().color.w = alpha;
}

void ButiEngine::InvisibleBlockAura::OnSet()
{
}

void ButiEngine::InvisibleBlockAura::Start()
{
	m_vlp_timer = ObjectFactory::Create<RelativeTimer>(60);
	m_vlp_timer->Start();

	AnimInitialize();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::InvisibleBlockAura::Clone()
{
	return ObjectFactory::Create<InvisibleBlockAura>();
}

void ButiEngine::InvisibleBlockAura::AnimInitialize()
{
	auto t = gameObject.lock()->transform;

	auto anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
	anim->SetSpeed(1.0f / 60);
	anim->SetTargetTransform(t->Clone());
	anim->GetTargetTransform()->SetLocalScale(2.0f);
	anim->GetTargetTransform()->RollLocalRotationX_Degrees(0.1f);

	anim->SetEaseType(Easing::EasingType::EaseOutQuart);
}
