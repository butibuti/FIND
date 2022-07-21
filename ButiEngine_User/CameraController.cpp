#include "stdafx_u.h"
#include "CameraController.h"
#include "Player.h"
#include "InputManager.h"

void ButiEngine::CameraController::OnUpdate()
{
    if (InputManager::IsTriggerCameraRotateRightKey()) {
        auto anim = m_vwp_cameraAxis.lock()->GetGameComponent<TransformAnimation>();
        if (!anim) {
            anim = m_vwp_cameraAxis.lock()->AddGameComponent<TransformAnimation>();
            anim->SetTargetTransform(m_vwp_cameraAxis.lock()->transform->Clone());
            anim->GetTargetTransform()->RollLocalRotationY_Degrees(90);
            anim->SetSpeed(1.0f / 10.0f);

            m_vwp_playerComponent.lock()->RollCameraDirection(1);

        }
    }
    if (InputManager::IsTriggerCameraRotateLeftKey()) {
        auto anim = m_vwp_cameraAxis.lock()->GetGameComponent<TransformAnimation>();
        if (!anim) {
            anim = m_vwp_cameraAxis.lock()->AddGameComponent<TransformAnimation>();
            anim->SetTargetTransform(m_vwp_cameraAxis.lock()->transform->Clone());
            anim->GetTargetTransform()->RollLocalRotationY_Degrees(-90);
            anim->SetSpeed(1.0f / 10.0f);

            m_vwp_playerComponent.lock()->RollCameraDirection(-1);
        }
    }
}

void ButiEngine::CameraController::OnSet()
{
}

void ButiEngine::CameraController::OnRemove()
{
}

void ButiEngine::CameraController::OnShowUI()
{
}

void ButiEngine::CameraController::Start()
{
    m_vwp_cameraAxis = gameObject.lock()->GetGameObjectManager().lock()->AddObjectFromCereal("CameraAxis");

    auto mainCamera = GetCamera("main");
    mainCamera->vlp_transform->SetBaseTransform(gameObject.lock()->transform, true);
    GetCamera("BloomSource")->vlp_transform->SetBaseTransform(mainCamera->vlp_transform, true);
    gameObject.lock()->transform->SetBaseTransform(m_vwp_cameraAxis.lock()->transform, true);

    m_vwp_playerComponent = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("Player").lock()->GetGameComponent<Player>();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::CameraController::Clone()
{
    auto output = ObjectFactory::Create<CameraController>();

    output->m_pase = m_pase;
    return output;
}

void ButiEngine::CameraController::ZoomInOut()
{
    auto t = gameObject.lock()->transform;

    auto anim = gameObject.lock()->GetGameComponent<TransformAnimation>();
    if (!anim)
    {
        anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
        anim->SetTargetTransform(t->Clone());
        anim->GetTargetTransform()->SetWorldPosition(t->GetWorldPosition() - t->GetFront() * 2.5f);
        anim->SetSpeed(1.0f / 50.0f);
        anim->SetEaseType(Easing::EasingType::EaseOutBack);
    }
}

void ButiEngine::CameraController::ZoomIn()
{
    auto anim = gameObject.lock()->GetGameComponent<TransformAnimation>();
    if (!anim && gameObject.lock()->transform->GetLocalPosition().z < -10) {
        anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
        anim->SetTargetTransform(gameObject.lock()->transform->Clone());
        anim->GetTargetTransform()->SetWorldPosition(gameObject.lock()->transform->GetWorldPosition() + gameObject.lock()->transform->GetFront() * 5);
        anim->SetSpeed(1.0f / 39.0f);
        anim->SetEaseType(Easing::EasingType::EaseOutBack);
    }
}

void ButiEngine::CameraController::ZoomOut()
{
    auto anim = gameObject.lock()->GetGameComponent<TransformAnimation>();
    if (!anim && gameObject.lock()->transform->GetLocalPosition().z > -30) {
        anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
        anim->SetTargetTransform(gameObject.lock()->transform->Clone());
        anim->GetTargetTransform()->SetWorldPosition(gameObject.lock()->transform->GetWorldPosition() - gameObject.lock()->transform->GetFront() * 30);
        anim->SetSpeed(1.0f / 20.0f);
        anim->SetEaseType(Easing::EasingType::EaseInExpo);
    }
}
