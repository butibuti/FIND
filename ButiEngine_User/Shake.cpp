#include "stdafx_u.h"
#include "Shake.h"

void ButiEngine::Shake::OnUpdate()
{
	if (!m_isStart)
	{
		return;
	}

	float x = ButiRandom::GetRandom(-m_amplitude, m_amplitude, 100);
	float y = ButiRandom::GetRandom(-m_amplitude, m_amplitude, 100);
	float z = ButiRandom::GetRandom(-m_amplitude, m_amplitude, 100);

	Vector3 newPos = m_defaultPos + Vector3(x, y, z);

	gameObject.lock()->transform->SetWorldPosition(newPos);
}

void ButiEngine::Shake::OnSet()
{
}

void ButiEngine::Shake::Start()
{
	m_isStart = false;
	m_amplitude = 0.0f;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Shake::Clone()
{
	return ObjectFactory::Create<Shake>();
}

void ButiEngine::Shake::ShakeStart(const float arg_amplitude)
{
	m_amplitude = arg_amplitude;
	m_isStart = true;
}

void ButiEngine::Shake::ShakeStop()
{
	gameObject.lock()->transform->SetWorldPosition(m_defaultPos);
	m_amplitude = 0.0f;
	m_isStart = false;
}
