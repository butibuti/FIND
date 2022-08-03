#include "stdafx_u.h"
#include "BurstManager.h"
#include "NextStageBlock.h"

void ButiEngine::BurstManager::OnUpdate()
{
}

void ButiEngine::BurstManager::OnSet()
{
}

void ButiEngine::BurstManager::OnRemove()
{
}

void ButiEngine::BurstManager::OnShowUI()
{
}

void ButiEngine::BurstManager::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::BurstManager::Clone()
{
	return ObjectFactory::Create<BurstManager>();
}

void ButiEngine::BurstManager::Burst()
{
	auto objects = GetManager().lock()->GetGameObjects(GetTagManager()->GetObjectTag("MapChip"));
	auto goals = GetManager().lock()->GetGameObjects(GetTagManager()->GetObjectTag("Goal"));
	auto invBlocks = GetManager().lock()->GetGameObjects(GetTagManager()->GetObjectTag("InvisibleBlock"));
	auto nextStageBlocks = GetManager().lock()->GetGameObjects(GetTagManager()->GetObjectTag("NextStageBlock"));

	{
		auto end = nextStageBlocks.end();
		for (auto itr = nextStageBlocks.begin(); itr != end; ++itr)
		{
			(*itr)->GetGameComponent<NextStageBlock>()->DisappearPreview();
		}
	}

	objects.insert(objects.end(), goals.begin(), goals.end());
	objects.insert(objects.end(), invBlocks.begin(), invBlocks.end());
	objects.insert(objects.end(), nextStageBlocks.begin(), nextStageBlocks.end());

	auto mapChipEnd = objects.end();
	for (auto itr = objects.begin(); itr != mapChipEnd; ++itr)
	{
		Vector3 dir = (*itr)->transform->GetWorldPosition() - Vector3Const::Zero;
		dir.Normalize();
		if (dir.GetLengthSqr() == 0)
		{
			dir.x = 1;
			dir.y = 1;
			dir.z = -1;
			dir.Normalize();
		}

		auto t = (*itr)->transform;
		auto anim = (*itr)->AddGameComponent<TransformAnimation>();

		anim->SetSpeed(1.0f / 60);
		anim->SetTargetTransform(t->Clone());
		anim->GetTargetTransform()->SetLocalScale(0.0f);
		anim->GetTargetTransform()->SetWorldPosition(dir * 100.0f);

		anim->SetEaseType(Easing::EasingType::EaseOutExpo);
	}
}
