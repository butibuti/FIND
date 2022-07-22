#include "stdafx_u.h"
#include "TitleBlock.h"

void ButiEngine::TitleBlock::OnUpdate()
{
	m_theta += m_thetaPase;
	Vector3 pos = gameObject.lock()->transform->GetWorldPosition();
	pos.y = std::sinf(m_theta) * 0.05f + m_defaultY;
	gameObject.lock()->transform->RollLocalRotationY_Degrees(m_anglePase);
	gameObject.lock()->transform->SetWorldPosition(pos);
}

void ButiEngine::TitleBlock::OnSet()
{
}

void ButiEngine::TitleBlock::Start()
{
	m_defaultY = gameObject.lock()->transform->GetWorldPosition().y;
	float angle = ButiRandom::GetRandom(0.0f, 360.0f, 100);
	gameObject.lock()->transform->RollLocalRotationY_Degrees(angle);
	m_theta = ButiRandom::GetRandom(0.0f, 360.0f, 100);

	m_anglePase = ButiRandom::GetRandom(0.3f, 0.7f, 100);
	m_thetaPase = ButiRandom::GetRandom(0.03f, 0.07f, 100);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::TitleBlock::Clone()
{
	return ObjectFactory::Create<TitleBlock>();
}
