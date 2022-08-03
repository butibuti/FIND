#include "stdafx_u.h"
#include "FloatMotion.h"

void ButiEngine::FloatMotion::OnUpdate()
{
	m_theta += m_thetaPase;
	Vector3 pos = gameObject.lock()->transform->GetWorldPosition();
	pos.y = std::sinf(m_theta) * 0.05f + m_defaultY;
	gameObject.lock()->transform->RollLocalRotationY_Degrees(m_anglePase);
	gameObject.lock()->transform->SetWorldPosition(pos);
}

void ButiEngine::FloatMotion::OnSet()
{
	m_defaultY = gameObject.lock()->transform->GetWorldPosition().y;
	m_theta = 0.0f;

	m_anglePase = 0.3f;
	m_thetaPase = 0.03f;
}

void ButiEngine::FloatMotion::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FloatMotion::Clone()
{
	return ObjectFactory::Create<FloatMotion>();
}
