#include "stdafx_u.h"
#include "CameraController.h"
#include "Player.h"
#include "InputManager.h"

void ButiEngine::CameraController::OnUpdate()
{
    //if (InputManager::IsTriggerCameraRotateRightKey()) {
    //    auto anim = shp_cameraAxis->GetGameComponent<TransformAnimation>();
    //    if (!anim) {
    //        anim = shp_cameraAxis->AddGameComponent<TransformAnimation>();
    //        anim->SetTargetTransform(shp_cameraAxis->transform->Clone());
    //        anim->GetTargetTransform()->RollLocalRotationY_Degrees(90);
    //        anim->SetSpeed(1.0f / 10.0f);

    //        shp_player->RollCameraDirection(1);

    //    }
    //}
    //if (InputManager::IsTriggerCameraRotateLeftKey()) {
    //    auto anim = shp_cameraAxis->GetGameComponent<TransformAnimation>();
    //    if (!anim) {
    //        anim = shp_cameraAxis->AddGameComponent<TransformAnimation>();
    //        anim->SetTargetTransform(shp_cameraAxis->transform->Clone());
    //        anim->GetTargetTransform()->RollLocalRotationY_Degrees(-90);
    //        anim->SetSpeed(1.0f / 10.0f);

    //        shp_player->RollCameraDirection(-1);
    //    }
    //}
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
    //shp_cameraAxis = gameObject.lock()->GetGameObjectManager().lock()->AddObjectFromCereal("cameraAxis").lock();
    //gameObject.lock()->transform->SetBaseTransform(shp_cameraAxis->transform, true);
    //shp_player = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("Player").lock()->GetBehavior<PlayerBehavior>();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::CameraController::Clone()
{
    auto output = ObjectFactory::Create<CameraController>();

    output->pase = pase;
    return output;
}

void ButiEngine::CameraController::ZoomInOut()
{
    //auto t = gameObject.lock()->transform;

    //auto anim = gameObject.lock()->GetGameComponent<TransformAnimation>();
    //if (!anim)
    //{
    //    anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
    //    anim->SetTargetTransform(t->Clone());
    //    anim->GetTargetTransform()->SetWorldPosition(t->GetWorldPosition() - t->GetFront() * 2.5f);
    //    anim->SetSpeed(1.0f / 50.0f);
    //    anim->SetEaseType(Easing::EasingType::EaseOutBack);
    //}
}

void ButiEngine::CameraController::ZoomIn()
{
    //auto anim = gameObject.lock()->GetGameComponent<TransformAnimation>();
    //if (!anim && gameObject.lock()->transform->GetLocalPosition().z < -10) {
    //    anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
    //    anim->SetTargetTransform(gameObject.lock()->transform->Clone());
    //    anim->GetTargetTransform()->SetWorldPosition(gameObject.lock()->transform->GetWorldPosition() + gameObject.lock()->transform->GetFront() * 5);
    //    anim->SetSpeed(1.0f / 39.0f);
    //    anim->SetEaseType(Easing::EasingType::EaseOutBack);
    //}
}

void ButiEngine::CameraController::ZoomOut()
{
    //auto anim = gameObject.lock()->GetGameComponent<TransformAnimation>();
    //if (!anim && gameObject.lock()->transform->GetLocalPosition().z > -30) {
    //    anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
    //    anim->SetTargetTransform(gameObject.lock()->transform->Clone());
    //    anim->GetTargetTransform()->SetWorldPosition(gameObject.lock()->transform->GetWorldPosition() - gameObject.lock()->transform->GetFront() * 30);
    //    anim->SetSpeed(1.0f / 20.0f);
    //    anim->SetEaseType(Easing::EasingType::EaseInExpo);
    //}
}
