#include "stdafx_u.h"
#include "NextStageBlock.h"
#include "StageSelectManager.h"
#include "StagePreviewParent.h"
#include "GameSettings.h"

std::vector<ButiEngine::NextStageBlockStatus> ButiEngine::NextStageBlock::m_vec_statuss;

void ButiEngine::NextStageBlock::OnUpdate()
{
}

void ButiEngine::NextStageBlock::OnSet()
{
}

void ButiEngine::NextStageBlock::Start()
{
	m_vwp_stagePreviewParent = GetManager().lock()->AddObjectFromCereal("StagePreviewParent");
	m_vwp_stagePreviewParent.lock()->transform->SetBaseTransform(gameObject.lock()->transform);
	m_vwp_stagePreviewParent.lock()->transform->SetLocalPosition(0.0f);
	m_vwp_stagePreviewParent.lock()->GetGameComponent<StagePreviewParent>()->CreatePreview(m_stageNum);

	m_isActive = true;
	if (m_vec_statuss[min(m_stageNum, m_vec_statuss.size() - 1)] == NextStageBlockStatus::InActive)
	{
		m_isActive = false;
		gameObject.lock()->GetGameComponent<MeshDrawComponent>()->UnRegist();
	}
	else if (m_vec_statuss[min(m_stageNum, m_vec_statuss.size() - 1)] == NextStageBlockStatus::Cleared)
	{
		gameObject.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>()->Get().color = GameSettings::ACTIVE_GOAL_COLOR;
	}
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::NextStageBlock::Clone()
{
	return ObjectFactory::Create<NextStageBlock>();
}

void ButiEngine::NextStageBlock::AppearPreview()
{
	m_vwp_stagePreviewParent.lock()->GetGameComponent<StagePreviewParent>()->Appear();
}

void ButiEngine::NextStageBlock::DisappearPreview()
{
	m_vwp_stagePreviewParent.lock()->GetGameComponent<StagePreviewParent>()->Disappear();
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
	m_vec_statuss[m_stageNum] = NextStageBlockStatus::Active;

	gameObject.lock()->GetApplication().lock()->GetSoundManager()->PlaySE(SoundTag("Sound/potion.wav"), 0.1f);

	auto t = gameObject.lock()->transform;
	auto pos = t->GetWorldPosition();
	auto rot = t->GetWorldRotation();
	auto scale = t->GetLocalScale();

	GetManager().lock()->AddObjectFromCereal("GoalAura", ObjectFactory::Create<Transform>(pos, rot, scale));
}

void ButiEngine::NextStageBlock::InitializeStatus()
{
	//すべてを非アクティブ状態で埋める
	m_vec_statuss.clear();

	std::uint16_t maxStageNum = StageSelectManager::GetMaxStageNum();

	for (std::uint16_t i = 0; i < maxStageNum + 1; i++)
	{
		m_vec_statuss.push_back(NextStageBlockStatus::InActive);
	}
}

bool ButiEngine::NextStageBlock::IsAllCleared()
{
	auto end = m_vec_statuss.end();
	for (auto itr = m_vec_statuss.begin(); itr != end; ++itr)
	{
		//クリア済みでないものがひとつでも見つかったらfalseを返す
		if ((*itr) != NextStageBlockStatus::Cleared)
		{
			return false;
		}
	}
	return true;
}
