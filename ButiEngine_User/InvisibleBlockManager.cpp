#include "stdafx_u.h"
#include "InvisibleBlockManager.h"
#include "InvisibleBlock.h"

void ButiEngine::InvisibleBlockManager::OnUpdate()
{
}

void ButiEngine::InvisibleBlockManager::OnSet()
{
}

void ButiEngine::InvisibleBlockManager::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::InvisibleBlockManager::Clone()
{
	return ObjectFactory::Create<InvisibleBlockManager>();
}

void ButiEngine::InvisibleBlockManager::CheckSeen()
{
	std::vector<int> IDs;
	auto blocks = GetManager().lock()->GetGameObjects(GetTagManager()->GetObjectTag("InvisibleBlock"));
	auto end = blocks.end();
	for (auto itr = blocks.begin(); itr != end; ++itr)
	{
		auto invBlockComp = (*itr)->GetGameComponent<InvisibleBlock>();
		if (invBlockComp->GetSeen())
		{
			int id = invBlockComp->GetID();
			if (std::find(IDs.begin(), IDs.end(), id) == IDs.end())
			{
				IDs.push_back(id);
			}
		}
	}
	auto idEnd = IDs.end();
	for (auto idItr = IDs.begin(); idItr != idEnd; ++idItr)
	{
		for (auto itr = blocks.begin(); itr != end; ++itr)
		{
			auto invBlockComp = (*itr)->GetGameComponent<InvisibleBlock>();

			if (invBlockComp->GetID() == *idItr)
			{
				invBlockComp->Active();
			}
		}
	}
}

void ButiEngine::InvisibleBlockManager::Reset()
{
	auto blocks = GetManager().lock()->GetGameObjects(GetTagManager()->GetObjectTag("InvisibleBlock"));
	auto end = blocks.end();
	for (auto itr = blocks.begin(); itr != end; ++itr)
	{
		auto invBlockComp = (*itr)->GetGameComponent<InvisibleBlock>();
		invBlockComp->UnActive();
		invBlockComp->SetSeen(false);
	}
}
