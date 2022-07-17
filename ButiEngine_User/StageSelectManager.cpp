#include "stdafx_u.h"
#include "StageSelectManager.h"
#include "NumberManager.h"
#include "InputManager.h"

int ButiEngine::StageSelectManager::stageNum = 0;
int ButiEngine::StageSelectManager::maxStageNum = 15;

void ButiEngine::StageSelectManager::OnUpdate()
{
	//if (!animTimer->IsOn() && !end)
	//{
	//	if (GameSettings::CheckRight())
	//	{
	//		OnPushRight();
	//	}
	//	else if (GameSettings::CheckLeft())
	//	{
	//		OnPushLeft();
	//	}
	//	else if (!end && GameSettings::TriggerDecision())
	//	{

	//		auto seTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/decide.wav");

	//		gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->Play(seTag, 0.1f);
	//		end = true;
	//	}
	//}

	//if (end)
	//{
	//	endTimer++;

	//	auto player = GetManager().lock()->GetGameObject("PlayerModel");
	//	float per = float(endTimer) / 30;
	//	float playerX = per;
	//	float playerY = Easing::Parabola(per) + (-1.6f);
	//	float playerZ = -per + 3.0f;
	//	player.lock()->transform->SetWorldPosition(Vector3(playerX, playerY, playerZ));
	//	player.lock()->transform->RollLocalRotationX_Degrees(-5.0f);

	//	auto obj_stageNumber = GetManager().lock()->GetGameObject("StageNumber");
	//	if (endTimer < 55)
	//	{
	//		float alpha = 1.0f - Easing::Liner(per);
	//		if (alpha < 0.0f)
	//		{
	//			alpha = 0.0f;
	//		}

	//		auto rArrow = GetManager().lock()->GetGameObject("RightArrow");
	//		auto lArrow = GetManager().lock()->GetGameObject("LeftArrow");

	//		auto rMeshDraw = rArrow.lock()->GetGameComponent<MeshDrawComponent>();
	//		auto lMeshDraw = lArrow.lock()->GetGameComponent<MeshDrawComponent>();

	//		auto rLightBuff = rMeshDraw->GetCBuffer<LightVariable>("LightBuffer");
	//		auto lLightBuff = lMeshDraw->GetCBuffer<LightVariable>("LightBuffer");

	//		rLightBuff->Get().lightDir.w = alpha;
	//		lLightBuff->Get().lightDir.w = alpha;
	//	}
	//	if (endTimer >= 55)
	//	{
	//		player.lock()->SetIsRemove(true);
	//		obj_stageNumber.lock()->GetGameComponent<NumberManagerComponent>()->Remove();
	//		obj_stageNumber.lock()->SetIsRemove(true);
	//		auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
	//		std::string sceneName = "Stage" + std::to_string(stageNum) + "Scene";
	//		sceneManager->RemoveScene(sceneName);
	//		sceneManager->LoadScene(sceneName);
	//		sceneManager->ChangeScene(sceneName);
	//	}
	//	else if (endTimer >= 7)
	//	{
	//		stageNumberObjectScale += 50;
	//		auto numManager = obj_stageNumber.lock()->GetGameComponent<NumberManagerComponent>();
	//		numManager->SetScale(Vector3(300.0f, stageNumberObjectScale, 1.0f));
	//		numManager->TranslateY(50.0f);
	//	}
	//	else if (endTimer >= 0)
	//	{
	//		stageNumberObjectScale -= 60;
	//		auto numManager = obj_stageNumber.lock()->GetGameComponent<NumberManagerComponent>();
	//		numManager->SetScale(Vector3(300.0f, stageNumberObjectScale, 1.0f));
	//		numManager->TranslateY(20.0f);
	//	}
	//}

	//if (animTimer->Update())
	//{
	//	animTimer->Stop();
	//}
}

void ButiEngine::StageSelectManager::OnSet()
{
}

void ButiEngine::StageSelectManager::Start()
{
	//auto player = GetManager().lock()->AddObjectFromCereal("PlayerModel");

	//player.lock()->transform->SetLocalScale(Vector3(0.5, 0.5, 0.5));
	//obj_stageNumber = GetManager().lock()->AddObjectFromCereal("StageNumber");
	//obj_stageNumber.lock()->transform->SetLocalScale(Vector3(300, 300, 300));
	//animTimer = ObjectFactory::Create<RelativeTimer>(15);

	//end = false;
	//endTimer = 0;
	//stageNumberObjectScale = 500.0f;
	//pushCount = 0;

	//auto finalScreen = GetManager().lock()->GetGameObject("FinalScreen");
	//auto anim = finalScreen.lock()->AddGameComponent<TransformAnimation>();

	//anim->SetSpeed(1 / 60.0f);
	//anim->SetTargetTransform(finalScreen.lock()->transform->Clone());
	//anim->GetTargetTransform()->SetLocalScale(Vector3(1980, 1080, 1));
	//anim->SetEaseType(Easing::EasingType::EaseOutCirc);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::StageSelectManager::Clone()
{
	return ObjectFactory::Create<StageSelectManager>();
}

std::string ButiEngine::StageSelectManager::GetNextSceneName()
{
	stageNum++;
	if (stageNum > maxStageNum)
	{
		stageNum--;
		return "ThanksScene";
	}

	std::string nextSceneName = "Stage_" + std::to_string(stageNum);
	return nextSceneName;
}

void ButiEngine::StageSelectManager::SetMaxStageNum()
{
	//for (int i = 0;; i++) {
	//	std::string path = "Resources/Scene/Stage_" + std::to_string(i) + "/mapInfo.map";
	//	if (!Util::CheckFileExistence(path)) {
	//		maxStageNum = i - 1;
	//		break;
	//	}
	//}
}

void ButiEngine::StageSelectManager::RestartAnimTimer()
{
	//animTimer->Reset();
	//animTimer->Start();
}

void ButiEngine::StageSelectManager::OnPushRight()
{
	//auto seTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/select.wav");

	//gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->Play(seTag, 0.1f);

	//stageNum++;
	//if (stageNum > maxStageNum)
	//{
	//	stageNum = 0;
	//}
	//obj_stageNumber.lock()->GetGameComponent<NumberManagerComponent>()->SetNumber(stageNum);

	//GetManager().lock()->AddObjectFromCereal("ArrowEffect", ObjectFactory::Create<Transform>(Vector3(500, 100, 9 - pushCount), 0.0f, Vector3(300, 300, 1)));

	//pushCount++;
	//if (pushCount > 5)
	//{
	//	pushCount = 0;
	//}

	//RestartAnimTimer();
}

void ButiEngine::StageSelectManager::OnPushLeft()
{
	//auto seTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/select.wav");

	//gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->Play(seTag, 0.1f);
	//stageNum--;
	//if (stageNum < 0)
	//{
	//	stageNum = maxStageNum;
	//}
	//obj_stageNumber.lock()->GetGameComponent<NumberManagerComponent>()->SetNumber(stageNum);

	//GetManager().lock()->AddObjectFromCereal("ArrowEffect", ObjectFactory::Create<Transform>(Vector3(-500, 100, 9 - pushCount), Vector3(0, 0, 180), Vector3(300, 300, 1)));

	//pushCount++;
	//if (pushCount > 5)
	//{
	//	pushCount = 0;
	//}

	//RestartAnimTimer();
}
