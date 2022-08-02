#include "stdafx_u.h"
#include "ThanksSceneManager.h"

void ButiEngine::ThanksSceneManager::OnUpdate()
{
    if (GameDevice::GetInput()->TriggerKey(Keys::Esc))
    {
        GameDevice::SetIsEnd(true);
    }

    if (!isClicked && (GameDevice::GetInput()->TriggerKey(Keys::Space) || GameDevice::GetInput()->GetAnyButtonTrigger())) {
        isClicked = true;

        gameObject.lock()->GetApplication().lock()->GetSoundManager()->PlaySE(SoundTag("Sound/thsnks.wav"), 0.1f);
        m_vlp_timer->Start();
    }

    float alpha = 1.0f - Easing::EaseInCirc(m_vlp_timer->GetPercent());
    m_vwp_thanksTexture.lock()->GetCBuffer<ButiRendering::ObjectInformation>()->Get().color.w = alpha;

    if (m_vlp_timer->Update())
    {
        auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
        std::string sceneName = "TitleScene";
        sceneManager->RemoveScene(sceneName);
        sceneManager->LoadScene(sceneName);
        sceneManager->ChangeScene(sceneName);
    }
}

void ButiEngine::ThanksSceneManager::OnSet()
{
}

void ButiEngine::ThanksSceneManager::Start()
{
    m_vlp_timer = ObjectFactory::Create<RelativeTimer>(90);

    auto thanksTexture = GetManager().lock()->GetGameObject("ThanksTexture");
    m_vwp_thanksTexture = thanksTexture.lock()->GetGameComponent<MeshDrawComponent>();

    auto anim = thanksTexture.lock()->AddGameComponent<TransformAnimation>();

    anim->SetSpeed(1 / 90.0f);
    anim->SetTargetTransform(thanksTexture.lock()->transform->Clone());
    anim->GetTargetTransform()->SetLocalScale(Vector3(1980, 1080, 1));
    anim->SetEaseType(Easing::EasingType::EaseInOutCirc);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::ThanksSceneManager::Clone()
{
	return ObjectFactory::Create<ThanksSceneManager>();
}
