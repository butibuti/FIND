#include "stdafx_u.h"
#include "CubeAnimation.h"

void ButiEngine::CubeAnimation::OnUpdate()
{
}

void ButiEngine::CubeAnimation::OnSet()
{
}

void ButiEngine::CubeAnimation::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::CubeAnimation::Clone()
{
	return ObjectFactory::Create<CubeAnimation>();
}
