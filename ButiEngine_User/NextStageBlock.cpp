#include "stdafx_u.h"
#include "NextStageBlock.h"

void ButiEngine::NextStageBlock::OnUpdate()
{
}

void ButiEngine::NextStageBlock::OnSet()
{
}

void ButiEngine::NextStageBlock::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::NextStageBlock::Clone()
{
	return ObjectFactory::Create<NextStageBlock>();
}
