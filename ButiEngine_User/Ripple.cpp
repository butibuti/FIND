#include "stdafx_u.h"
#include "Ripple.h"

void ButiEngine::Ripple::OnUpdate()
{
	if (m_vlp_timer->Update())
	{
		m_vlp_timer->Stop();
		gameObject.lock()->SetIsRemove(true);
		return;
	}

	float progress = m_vlp_timer->GetPercent();

	float alpha = 1.0f - Easing::EaseOutExpo(progress);
	gameObject.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>()->Get().color.w = alpha;

	scale = Easing::EaseOutExpo(progress) * 3.0f;
	gameObject.lock()->transform->SetLocalScale(scale);
	gameObject.lock()->transform->TranslateY(-0.01f);
}

void ButiEngine::Ripple::OnSet()
{
}

void ButiEngine::Ripple::Start()
{
	m_vlp_timer = ObjectFactory::Create<RelativeTimer>(60);
	m_vlp_timer->Start();

	scale = 0.0f;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Ripple::Clone()
{
	return ObjectFactory::Create<Ripple>();
}
