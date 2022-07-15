#include "stdafx_u.h"
#include "ConfigManager.h"
static bool  isConfig = false;
void ButiEngine::ConfigManager::OnUpdate()
{
    if (GameDevice::GetInput()->TriggerKey(Keys::C)) {
        gameObject.lock()->GetGameObjectManager().lock()->AddObjectFromCereal("GameObject", nullptr);
        gameObject.lock()->GetGameObjectManager().lock()->AddObjectFromCereal("GameObject", nullptr);
        gameObject.lock()->GetGameObjectManager().lock()->AddObjectFromCereal("GameObject", nullptr);
        gameObject.lock()->GetGameObjectManager().lock()->AddObjectFromCereal("GameObject", nullptr);
    }
}

void ButiEngine::ConfigManager::OnSet()
{
}

void ButiEngine::ConfigManager::Start()
{
    isConfig = true;
	//gameObject.lock()->GetGameObjectManager().lock()->AddObjectFromCereal("GameObject", nullptr);
}
void ButiEngine::ConfigManager::End()
{
    isConfig = false;
}

ButiEngine::Value_ptr< ButiEngine::GameComponent> ButiEngine::ConfigManager::Clone()
{
    return ObjectFactory::Create<ConfigManager>();
}

bool ButiEngine::ConfigManager::IsConfig()
{
    return isConfig;
}
