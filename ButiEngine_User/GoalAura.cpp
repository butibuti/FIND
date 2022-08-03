#include "stdafx_u.h"
#include "GoalAura.h"

void ButiEngine::GoalAura::OnUpdate()
{
	if (m_vlp_timer->Update())
	{
		m_vlp_timer->Stop();
		gameObject.lock()->SetIsRemove(true);
		return;
	}

	float alpha = MathHelper::Lerp(m_startAlpha, m_targetAlpha, Easing::EaseOutExpo(m_vlp_timer->GetPercent()));
	gameObject.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>()->Get().color.w = alpha;
}

void ButiEngine::GoalAura::OnSet()
{
}

void ButiEngine::GoalAura::Start()
{
	m_vlp_timer = ObjectFactory::Create<RelativeTimer>(m_animFrame);
	m_vlp_timer->Start();
	AnimInitialize();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::GoalAura::Clone()
{
	return ObjectFactory::Create<GoalAura>();
}

void ButiEngine::GoalAura::AnimInitialize()
{
	auto t = gameObject.lock()->transform;


	auto anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
	anim->SetSpeed(1.0f / m_animFrame);
	anim->SetTargetTransform(t->Clone());
	anim->GetTargetTransform()->SetLocalScale(m_targetScale);

	anim->SetEaseType(m_easingType);
}
