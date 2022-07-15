#include "TestBehavior.h"
#include"Header/Common/CollisionPrimitive.h"
#include"ConfigManager.h"
#include"Pause.h"
float g_vibe_l = 0.0f, g_vibe_r = 0.0f;
void ButiEngine::TestBehavior::OnUpdate()
{
	Vector3 force=Vector3(GameDevice::GetInput()->GetLeftStick().x,0, GameDevice::GetInput()->GetLeftStick().y).Normalize()*0.05;
	if (GameDevice::GetInput()->CheckKey(Keys::E)) {
		force.y += 0.1f;
	}
	if (GameDevice::GetInput()->CheckKey(Keys::D)) {
		force.y -= 0.1f;
	}

	gameObject.lock()->transform->Translate(force);

	if (GameDevice::GetInput()->TriggerKey(Keys::S)) {
		auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
		std::string sceneName = "Renewal_Test";
		sceneManager->RemoveScene(sceneName);
		sceneManager->LoadScene(sceneName);
		sceneManager->ChangeScene(sceneName);
	}

}


void ButiEngine::TestBehavior::OnSet()
{
}

void ButiEngine::TestBehavior::Start()
{
	

}
void ButiEngine::TestBehavior::OnRemove()
{
	
}

void ButiEngine::TestBehavior::OnShowUI()
{
}

void ButiEngine::TestBehavior::ShowGUI()
{

}

ButiEngine::Vector3 ButiEngine::TestBehavior::GetWorldPosition()
{
	return gameObject.lock()->transform->GetWorldPosition();
}

void ButiEngine::TestBehavior::AdditionMass(const float arg_mass)
{

}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::TestBehavior::Clone()
{
	auto clone = ObjectFactory::Create<TestBehavior>();
	return clone;
}