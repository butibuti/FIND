#include "stdafx_u.h"
#include "ThanksSceneManager.h"

void ButiEngine::ThanksSceneManager::OnUpdate()
{
    //if (!isClicked && (GameDevice::GetInput()->TriggerKey(Keys::Space) || GameDevice::GetInput()->GetAnyButtonTrigger())) {
    //    isClicked = true;


    //    auto seTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/thanks.wav");
    //    gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->Play(seTag, 0.1f);
    //}


    //buffer->Get().lightDir.w = power;
    //if (isClicked) {
    //    t -= 0.02f;


    //    buffer->Get().lightDir.x = t;
    //    buffer->Get().lightDir.y = t;
    //    buffer->Get().lightDir.z = t;

    //    if (t <= -0.5) {

    //        auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
    //        std::string sceneName = "TitleScene";
    //        sceneManager->RemoveScene(sceneName);
    //        sceneManager->LoadScene(sceneName);
    //        sceneManager->ChangeScene(sceneName);
    //    }

    //}
}

void ButiEngine::ThanksSceneManager::OnSet()
{
}

void ButiEngine::ThanksSceneManager::Start()
{
    //buffer = gameObject.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<LightVariable>("LightBuffer");

    //auto finalScreen = GetManager().lock()->GetGameObject("FinalScreen");
    //auto anim = finalScreen.lock()->AddGameComponent<TransformAnimation>();

    //anim->SetSpeed(1 / 60.0f);
    //anim->SetTargetTransform(finalScreen.lock()->transform->Clone());
    //anim->GetTargetTransform()->SetLocalScale(Vector3(1980, 1080, 1));
    //anim->SetEaseType(Easing::EasingType::EaseOutCirc);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::ThanksSceneManager::Clone()
{
	return ObjectFactory::Create<ThanksSceneManager>();
}
