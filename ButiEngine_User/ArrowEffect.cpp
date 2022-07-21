#include "stdafx_u.h"
#include "ArrowEffect.h"

void ButiEngine::Arroweffect::OnUpdate()
{
	if (m_vlp_timer->Update())
	{
		m_vlp_timer->Stop();
		gameObject.lock()->SetIsRemove(true);
		return;
	}

	float progress = m_vlp_timer->GetPercent();
	float scale = 500.0f + Easing::Liner(progress) * 1500.0f;
	gameObject.lock()->transform->SetLocalScale(Vector3(scale, scale, 1));
 
	float alpha = 1.0f - Easing::Liner(progress);
	if (alpha < 0.0f)
	{
		alpha = 0.0f;
	}
	gameObject.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>()->Get().color.w = alpha;
}

void ButiEngine::Arroweffect::OnSet()
{
}

void ButiEngine::Arroweffect::OnRemove()
{
}

void ButiEngine::Arroweffect::OnShowUI()
{
}

void ButiEngine::Arroweffect::Start()
{
	m_vlp_timer = ObjectFactory::Create<RelativeTimer>(30);
	m_vlp_timer->Start();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Arroweffect::Clone()
{
	return ObjectFactory::Create<Arroweffect>();
}
