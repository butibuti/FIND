#include "stdafx_u.h"
#include "Pause.h"
#include"ConfigManager.h"

static bool isPause=false;
void ButiEngine::PauseManager::OnUpdate()
{
    if (GameDevice::GetInput()->TriggerKey(Keys::P)) {
        if (!ConfigManager::IsConfig()) {
            gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneManager().lock()->PopSubScene();
        }
    }
    if (GameDevice::GetInput()->TriggerKey(Keys::C)) {
        if (!ConfigManager::IsConfig()) {
            gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneManager().lock()->LoadScene("Config");
            gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneManager().lock()->PushSubScene("Config");
        }
    }
}

void ButiEngine::PauseManager::OnSet()
{
}

void ButiEngine::PauseManager::Start()
{
    isPause = true;
}

void ButiEngine::PauseManager::End()
{
    isPause = false;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::PauseManager::Clone()
{
    return ObjectFactory::Create<PauseManager>();
}

bool ButiEngine::PauseManager::IsPause()
{
    return isPause;
}
