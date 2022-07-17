#include "stdafx_u.h"
#include "Shake.h"

void ButiEngine::Shake::OnUpdate()
{
	if (!start)
	{
		return;
	}

	float x = ButiRandom::GetRandom(-amplitude, amplitude, 100);
	float y = ButiRandom::GetRandom(-amplitude, amplitude, 100);
	float z = ButiRandom::GetRandom(-amplitude, amplitude, 100);

	Vector3 newPos = defaultPos + Vector3(x, y, z);

	gameObject.lock()->transform->SetWorldPosition(newPos);
}

void ButiEngine::Shake::OnSet()
{
}

void ButiEngine::Shake::Start()
{
	start = false;
	amplitude = 0.0f;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Shake::Clone()
{
	return ObjectFactory::Create<Shake>();
}

void ButiEngine::Shake::ShakeStart(float arg_amplitude)
{
	amplitude = arg_amplitude;
	start = true;
}

void ButiEngine::Shake::ShakeStop()
{
	gameObject.lock()->transform->SetWorldPosition(defaultPos);
	amplitude = 0.0f;
	start = false;
}
