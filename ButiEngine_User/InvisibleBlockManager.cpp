#include "stdafx_u.h"
#include "InvisibleBlockManager.h"
#include "InvisibleBlock.h"
#include "Player.h"

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
	std::vector<std::uint16_t> IDs;
	auto blocks = GetManager().lock()->GetGameObjects(GetTagManager()->GetObjectTag("InvisibleBlock"));
	auto end = blocks.end();
	for (auto itr = blocks.begin(); itr != end; ++itr)
	{
		auto invBlockComp = (*itr)->GetGameComponent<InvisibleBlock>();
		if (invBlockComp->IsSeen())
		{
			std::uint16_t id = invBlockComp->GetID();
			if (std::find(IDs.begin(), IDs.end(), id) == IDs.end())
			{
				IDs.push_back(id);
			}
		}
	}

	auto player = GetManager().lock()->GetGameObject("Player").lock()->GetGameComponent<Player>();

	auto idEnd = IDs.end();
	for (auto idItr = IDs.begin(); idItr != idEnd; ++idItr)
	{
		for (auto itr = blocks.begin(); itr != end; ++itr)
		{
			auto invBlockComp = (*itr)->GetGameComponent<InvisibleBlock>();

			if (invBlockComp->GetID() == *idItr && invBlockComp->GetMapPos() != player->GetMapPos())
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
