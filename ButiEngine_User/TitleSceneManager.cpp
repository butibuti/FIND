#include "stdafx_u.h"
#include "TitleSceneManager.h"

void ButiEngine::TitleSceneManager::OnUpdate()
{
	//if (GameSettings::TriggerDecision())
	//{
	//	auto seTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/potion.wav");

	//	gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->Play(seTag, 0.1f);
	//	shp_timer->Start();
	//	auto mainCam = GetCamera("main");
	//	auto camera = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("Camera");
	//	auto anim = camera.lock()->AddGameComponent<TransformAnimation>();

	//	mainCam.lock()->shp_transform = camera.lock()->transform;

	//	anim->SetTargetTransform(camera.lock()->transform->Clone());
	//	anim->GetTargetTransform()->TranslateZ(20);
	//	anim->SetEaseType(Easing::EasingType::EaseInBack);
	//	anim->SetSpeed(1.0f / 60.0f);
	//}
	//if (shp_timer->Update()) {
	//	shp_timer->Stop();
	//	auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
	//	sceneManager->RemoveScene("StageSelectScene");
	//	sceneManager->LoadScene("StageSelectScene");
	//	sceneManager->ChangeScene("StageSelectScene");

	//}
}

void ButiEngine::TitleSceneManager::OnSet()
{
}

void ButiEngine::TitleSceneManager::Start()
{
	////gameObject.lock()->GetApplication().lock()->GetGraphicDevice()->SetClearColor(Vector4(0.025f, 0.025f, 0.025f, 1.0f));
	//shp_timer = ObjectFactory::Create<RelativeTimer>(60);

	//auto bgmTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/cube_BGM.wav");

	//gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->PlayBGM(bgmTag, 0.1f);


	//auto finalScreen = GetManager().lock()->GetGameObject("FinalScreen");
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
