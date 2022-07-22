#include "stdafx_u.h"
#include "NumberManager.h"
#include "StageSelectManager.h"

void ButiEngine::NumberManager::OnUpdate()
{
}

void ButiEngine::NumberManager::OnSet()
{
}

void ButiEngine::NumberManager::Start()
{
	m_vwp_number1 = GetManager().lock()->GetGameObject("Number1");
	m_vwp_number10 = GetManager().lock()->GetGameObject("Number10");
	StageSelectManager::m_stageNum = 0;
	SetNumber(StageSelectManager::GetStageNum());
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::NumberManager::Clone()
{
	return ObjectFactory::Create<NumberManager>();
}

void ButiEngine::NumberManager::SetNumber(std::uint8_t arg_number)
{
	m_number = arg_number + 1;

	int one = m_number % 10;
	int ten = m_number / 10;

	auto meshDraw1 = m_vwp_number1.lock()->GetGameComponent<MeshDrawComponent>();
	auto meshDraw10 = m_vwp_number10.lock()->GetGameComponent<MeshDrawComponent>();

	if (m_number < 10)
	{
		m_vwp_number1.lock()->transform->SetWorldPosition(Vector3(0, 100, 30));
		meshDraw10->UnRegist();
	}
	else
	{
		m_vwp_number1.lock()->transform->SetWorldPosition(Vector3(150, 100, 30));
		m_vwp_number10.lock()->GetGameComponent<MeshDrawComponent>()->Regist();

		std::string name10 = "Material/Number_" + std::to_string(ten) + ".mat";
		meshDraw10->SetMaterialTag(MaterialTag(name10), 0);
		meshDraw10->ReRegist();
	}

	std::string name1 = "Material/Number_" + std::to_string(one) + ".mat";
	meshDraw1->SetMaterialTag(MaterialTag(name1), 0);
	meshDraw1->ReRegist();
}

void ButiEngine::NumberManager::TranslateY(float arg_y)
{
	m_vwp_number1.lock()->transform->TranslateY(arg_y);
	m_vwp_number10.lock()->transform->TranslateY(arg_y);
}

void ButiEngine::NumberManager::SetScale(const Vector3& arg_scale)
{
	m_vwp_number1.lock()->transform->SetLocalScale(arg_scale);
	m_vwp_number10.lock()->transform->SetLocalScale(arg_scale);
}

void ButiEngine::NumberManager::Remove()
{
	m_vwp_number1.lock()->SetIsRemove(true);
	m_vwp_number10.lock()->SetIsRemove(true);
}
