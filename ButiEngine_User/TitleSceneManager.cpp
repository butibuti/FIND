#include "stdafx_u.h"
#include "TitleSceneManager.h"
#include "InputManager.h"
#include "Map.h"

void ButiEngine::TitleSceneManager::OnUpdate()
{
	if (GameDevice::GetInput()->TriggerKey(Keys::Esc))
	{
		GameDevice::SetIsEnd(true);
	}
	auto anim = m_vwp_camera.lock()->GetGameComponent<TransformAnimation>();
	if (!anim && InputManager::IsTriggerDecisionKey())
	{
		m_vlp_timer->Start();

		gameObject.lock()->GetApplication().lock()->GetSoundManager()->PlaySE(SoundTag("Sound/potion.wav"), 0.1f);

		anim = m_vwp_camera.lock()->AddGameComponent<TransformAnimation>();
		anim->SetTargetTransform(m_vwp_camera.lock()->transform->Clone());
		anim->GetTargetTransform()->TranslateZ(20);
		anim->SetEaseType(Easing::EasingType::EaseInBack);
		anim->SetSpeed(1.0f / 60.0f);
	}

	if (m_vlp_timer->Update()) 
	{
		m_vlp_timer->Stop();

		Map::ResetStageSelectPlayerData();

		auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
		auto sceneName = "NewStageSelectScene";
		//sceneName = "StageSelectScene";
		sceneManager->RemoveScene(sceneName);
		sceneManager->LoadScene(sceneName);
		sceneManager->ChangeScene(sceneName);

	}
}

void ButiEngine::TitleSceneManager::OnSet()
{
}

void ButiEngine::TitleSceneManager::Start()
{
	m_vlp_timer = ObjectFactory::Create<RelativeTimer>(60);

	m_vwp_camera = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("Camera");
	auto mainCamera = GetCamera("main");
	mainCamera->vlp_transform->SetBaseTransform(m_vwp_camera.lock()->transform, true);
	GetCamera("BloomSource")->vlp_transform->SetBaseTransform(mainCamera->vlp_transform, true);

	gameObject.lock()->GetApplication().lock()->GetSoundManager()->PlayBGM(SoundTag("Sound/cube_BGM.wav"), 0.1f);

	m_vwp_camera.lock()->transform->TranslateZ(-150);

	auto anim = m_vwp_camera.lock()->AddGameComponent<TransformAnimation>();
	anim->SetTargetTransform(m_vwp_camera.lock()->transform->Clone());
	anim->GetTargetTransform()->TranslateZ(150);
	anim->SetEaseType(Easing::EasingType::EaseOutCirc);
	anim->SetSpeed(1.0f / 30.0f);


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
