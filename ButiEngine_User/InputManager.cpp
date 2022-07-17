#include "stdafx_u.h"
#include "InputManager.h"

const float ButiEngine::InputManager::m_DEADZONE = 0.25f;

void ButiEngine::InputManager::OnUpdate()
{
    m_currentLeftStick = GameDevice::GetInput()->GetLeftStick();
}

void ButiEngine::InputManager::OnSet()
{
}

void ButiEngine::InputManager::Start()
{
    m_currentLeftStick = Vector2();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::InputManager::Clone()
{
    return ObjectFactory::Create<InputManager>();
}

bool ButiEngine::InputManager::IsTriggerDecisionKey()
{
    bool key = GameDevice::GetInput()->TriggerKey(Keys::Space);
    bool button = GameDevice::GetInput()->GetAnyButtonTrigger();

    return (key || button);
}

bool ButiEngine::InputManager::IsPushRightKey()
{
    bool key = GameDevice::GetInput()->CheckKey(Keys::D);
    bool button = GameDevice::GetInput()->GetPadButton(PadButtons::XBOX_RIGHT);
    bool stick = m_currentLeftStick.x > m_DEADZONE;

    return (key || button || stick);
}

bool ButiEngine::InputManager::IsPushLeftKey()
{
    bool key = GameDevice::GetInput()->CheckKey(Keys::A);
    bool button = GameDevice::GetInput()->GetPadButton(PadButtons::XBOX_LEFT);
    bool stick = m_currentLeftStick.x < -m_DEADZONE;

    return (key || button || stick);
}

bool ButiEngine::InputManager::IsPushFrontKey()
{
    bool key = GameDevice::GetInput()->CheckKey(Keys::W);
    bool button = GameDevice::GetInput()->GetPadButton(PadButtons::XBOX_UP);
    bool stick = m_currentLeftStick.y > m_DEADZONE;

    return (key || button || stick);
}

bool ButiEngine::InputManager::IsPushBackKey()
{
    bool key = GameDevice::GetInput()->CheckKey(Keys::S);
    bool button = GameDevice::GetInput()->GetPadButton(PadButtons::XBOX_DOWN);
    bool stick = m_currentLeftStick.y < -m_DEADZONE;

    return (key || button || stick);
}

bool ButiEngine::InputManager::IsTriggerCameraRotateRightKey()
{
    bool key = GameDevice::GetInput()->TriggerKey(Keys::E);
    bool button = GameDevice::GetInput()->GetPadButtonTrigger(PadButtons::XBOX_BUTTON_RIGHT);

    return (key || button);
}

bool ButiEngine::InputManager::IsTriggerCameraRotateLeftKey()
{
    bool key = GameDevice::GetInput()->TriggerKey(Keys::Q);
    bool button = GameDevice::GetInput()->GetPadButtonTrigger(PadButtons::XBOX_BUTTON_LEFT);

    return (key || button);
}
