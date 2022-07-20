#include "stdafx_u.h"
#include "ArrowEffect.h"

void ButiEngine::Arroweffect::OnUpdate()
{
	if (m_vlp_timer->Update())
	{
		gameObject.lock()->SetIsRemove(true);
	}

	float progress = m_vlp_timer->GetPercent();
	float scale = 500.0f + Easing::Liner(progress) * 1500.0f;
	gameObject.lock()->transform->SetLocalScale(Vector3(scale, scale, 1));

	//auto meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent>();
	//auto lightBuff = meshDraw->GetCBuffer<LightVariable>("LightBuffer");
	//float alpha = 0.3f - Easing::Liner(progress) * 0.3f;
	//if (alpha < 0.0f)
	//{
	//	alpha = 0.0f;
	//}
	//lightBuff->Get().lightDir.w = alpha;
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
