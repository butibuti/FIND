#include "stdafx_u.h"
#include "SeenObject.h"
#include "GoalAura.h"
#include "Player.h"

void ButiEngine::SeenObject::OnUpdate()
{
	if (m_vwp_player.lock()->IsGoal())
	{
		SetIsRemove(true);
		return;
	}
	if (!m_vlp_auraIntervalTimer) { return; }

	if (m_vlp_auraIntervalTimer->Update())
	{
		auto aura = GetManager().lock()->AddObjectFromCereal("GoalAura", gameObject.lock()->transform->Clone());
		auto auraComponent = aura.lock()->GetGameComponent<GoalAura>();
		auraComponent->SetColor(ButiColor::Grey());
		auraComponent->SetTargetScale(1.5f);
		auraComponent->SetAnimFrame(60);
	}
}

void ButiEngine::SeenObject::OnSet()
{
	m_vwp_player = GetManager().lock()->GetGameObject("Player").lock()->GetGameComponent<Player>();
	m_vlp_auraIntervalTimer = ObjectFactory::Create<RelativeTimer>(40);
	m_vlp_auraIntervalTimer->Start();

	auto aura = GetManager().lock()->AddObjectFromCereal("GoalAura", gameObject.lock()->transform->Clone());
	auto auraComponent = aura.lock()->GetGameComponent<GoalAura>();
	auraComponent->SetColor(ButiColor::Grey());
	auraComponent->SetTargetScale(1.5f);
	auraComponent->SetAnimFrame(60);
}

void ButiEngine::SeenObject::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::SeenObject::Clone()
{
	return ObjectFactory::Create<SeenObject>();
}
