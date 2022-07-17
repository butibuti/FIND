#include "stdafx_u.h"
#include "CameraMesh.h"

void ButiEngine::CameraMesh::OnUpdate()
{
	//alpha -= 0.1f;
	//if (alpha < 0)
	//{
	//	alpha = 0;
	//}

	//auto meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent>();
	//auto lightBuff = meshDraw->GetCBuffer<LightVariable>("LightBuffer");
	//lightBuff->Get().lightDir.w = alpha;
}

void ButiEngine::CameraMesh::OnSet()
{
}

void ButiEngine::CameraMesh::Start()
{
	alpha = 0.0f;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::CameraMesh::Clone()
{
	return ObjectFactory::Create<CameraMesh>();
}

void ButiEngine::CameraMesh::Flash()
{
	alpha = 1.0f;
}
