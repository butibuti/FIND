#include "stdafx_u.h"
#include "NumberManager.h"

void ButiEngine::NumberManager::OnUpdate()
{
}

void ButiEngine::NumberManager::OnSet()
{
}

void ButiEngine::NumberManager::Start()
{
	//number1 = GetManager().lock()->GetGameObject("Number1");
	//number10 = GetManager().lock()->GetGameObject("Number10");
	//StageSelectManagerComponent::stageNum = 0;
	//SetNumber(StageSelectManagerComponent::GetStageNum());
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::NumberManager::Clone()
{
	return ObjectFactory::Create<NumberManager>();
}

void ButiEngine::NumberManager::SetNumber(int arg_number)
{
	//number = arg_number + 1;

	//int one = number % 10;
	//int ten = number / 10;

	//auto meshDraw1 = number1.lock()->GetGameComponent<MeshDrawComponent>();
	//auto meshDraw10 = number10.lock()->GetGameComponent<MeshDrawComponent>();

	//if (number < 10)
	//{
	//	number1.lock()->transform->SetWorldPosition(Vector3(0, 100, 30));
	//	meshDraw10->UnRegist();
	//}
	//else
	//{
	//	number1.lock()->transform->SetWorldPosition(Vector3(150, 100, 30));
	//	number10.lock()->GetGameComponent<MeshDrawComponent>()->Regist();
	//	std::string name10 = "number_" + std::to_string(ten);
	//	meshDraw10->SetMaterialTag(gameObject.lock()->GetResourceContainer()->GetMaterialTag(name10));
	//	meshDraw10->ReRegist();
	//}

	//std::string name1 = "number_" + std::to_string(one);
	//meshDraw1->SetMaterialTag(gameObject.lock()->GetResourceContainer()->GetMaterialTag(name1));
	//meshDraw1->ReRegist();
}

void ButiEngine::NumberManager::TranslateY(float y)
{
	//number1.lock()->transform->TranslateY(y);
	//number10.lock()->transform->TranslateY(y);
}

void ButiEngine::NumberManager::SetScale(Vector3 scale)
{
	//number1.lock()->transform->SetLocalScale(scale);
	//number10.lock()->transform->SetLocalScale(scale);
}

void ButiEngine::NumberManager::Remove()
{
	//number1.lock()->SetIsRemove(true);
	//number10.lock()->SetIsRemove(true);
}
