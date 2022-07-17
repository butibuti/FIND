#include "stdafx_u.h"
#include "TitleBlock.h"

void ButiEngine::TitleBlock::OnUpdate()
{
	angle += 0.05f;
	theta += 0.05f;
	Vector3 pos = gameObject.lock()->transform->GetWorldPosition();
	pos.y = std::sinf(theta) * 0.05f + defaultY;
	gameObject.lock()->transform->RollLocalRotationY_Degrees(0.5f);
	gameObject.lock()->transform->SetWorldPosition(pos);
}

void ButiEngine::TitleBlock::OnSet()
{
}

void ButiEngine::TitleBlock::Start()
{
	defaultY = gameObject.lock()->transform->GetWorldPosition().y;
	angle = ButiRandom::GetRandom(0.0f, 360.0f, 100);
	theta = ButiRandom::GetRandom(0.0f, 360.0f, 100);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::TitleBlock::Clone()
{
	return ObjectFactory::Create<TitleBlock>();
}
