#include "stdafx_u.h"
#include "NextStageBlock.h"
#include "StageSelectManager.h"

std::vector<bool> ButiEngine::NextStageBlock::m_vec_isActives;

void ButiEngine::NextStageBlock::OnUpdate()
{
}

void ButiEngine::NextStageBlock::OnSet()
{
}

void ButiEngine::NextStageBlock::Start()
{
	m_isActive = m_vec_isActives[m_stageNum];

	if (!m_isActive)
	{
		gameObject.lock()->GetGameComponent<MeshDrawComponent>()->UnRegist();
	}
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::NextStageBlock::Clone()
{
	return ObjectFactory::Create<NextStageBlock>();
}

void ButiEngine::NextStageBlock::Seen()
{
	if (m_isActive)
	{
		return;
	}
	auto meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent>();
	meshDraw->Regist();
	m_isActive = true;
	m_vec_isActives[m_stageNum] = m_isActive;

	gameObject.lock()->GetApplication().lock()->GetSoundManager()->PlaySE(SoundTag("Sound/potion.wav"), 0.1f);

	auto t = gameObject.lock()->transform;
	auto pos = t->GetWorldPosition();
	auto rot = t->GetWorldRotation();
	auto scale = t->GetLocalScale();

	GetManager().lock()->AddObjectFromCereal("GoalAura", ObjectFactory::Create<Transform>(pos, rot, scale));
}

void ButiEngine::NextStageBlock::InitializeIsActives()
{
	m_vec_isActives.clear();

	std::uint16_t maxStageNum = StageSelectManager::GetMaxStageNum();

	for (std::uint16_t i = 0; i < maxStageNum; i++)
	{
		m_vec_isActives.push_back(false);
	}
}
