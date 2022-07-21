#include "stdafx_u.h"
#include "CameraMesh.h"

void ButiEngine::CameraMesh::OnUpdate()
{
	m_alpha -= 0.1f;
	if (m_alpha < 0)
	{
		m_alpha = 0;
	}

	gameObject.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>()->Get().color.w = m_alpha;
}

void ButiEngine::CameraMesh::OnSet()
{
}

void ButiEngine::CameraMesh::Start()
{
	m_alpha = 0.0f;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::CameraMesh::Clone()
{
	return ObjectFactory::Create<CameraMesh>();
}

void ButiEngine::CameraMesh::Flash()
{
	m_alpha = 1.0f;
}
