#include "stdafx_u.h"
#include "TitleSceneManager.h"
#include "InputManager.h"

void ButiEngine::TitleSceneManager::OnUpdate()
{
	if (InputManager::IsTriggerDecisionKey())
	{
		m_vlp_timer->Start();

		//auto seTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/potion.wav");
		//gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->Play(seTag, 0.1f);

		auto mainCam = GetCamera("BloomSource");
		auto camera = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("Camera");
		mainCam->vlp_transform = camera.lock()->transform;

		auto anim = camera.lock()->AddGameComponent<TransformAnimation>();
		anim->SetTargetTransform(camera.lock()->transform->Clone());
		anim->GetTargetTransform()->TranslateZ(20);
		anim->SetEaseType(Easing::EasingType::EaseInBack);
		anim->SetSpeed(1.0f / 60.0f);
	}

	if (m_vlp_timer->Update()) 
	{
		m_vlp_timer->Stop();
		auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
		sceneManager->RemoveScene("StageSelectScene");
		sceneManager->LoadScene("StageSelectScene");
		sceneManager->ChangeScene("StageSelectScene");

	}
}

void ButiEngine::TitleSceneManager::OnSet()
{
}

void ButiEngine::TitleSceneManager::Start()
{
	m_vlp_timer = ObjectFactory::Create<RelativeTimer>(60);

	//auto bgmTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/cube_BGM.wav");
	//gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->PlayBGM(bgmTag, 0.1f);


	//auto finalScreen = GetManager().lock()->GetGameObject("Screen");
	//finalScreen.lock()->transform->SetLocalScale(Vector3());
	//auto anim = finalScreen.lock()->AddGameComponent<TransformAnimation>();

	//anim->SetSpeed(1 / 60.0f);
	//anim->SetTargetTransform(finalScreen.lock()->transform->Clone());
	//anim->GetTargetTransform()->SetLocalScale(Vector3(1980, 1080, 1));
	//anim->SetEaseType(Easing::EasingType::EaseOutCirc);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::TitleSceneManager::Clone()
{
	return ObjectFactory::Create<TitleSceneManager>();
}
