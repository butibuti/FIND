#include "stdafx_u.h"
#include "StageSelectManager.h"
#include "NumberManager.h"
#include "InputManager.h"

std::uint8_t ButiEngine::StageSelectManager::m_stageNum = 0;
std::uint8_t ButiEngine::StageSelectManager::m_maxStageNum = 1;

void ButiEngine::StageSelectManager::OnUpdate()
{
	if (!m_vlp_animTimer->IsOn() && !m_isEnd)
	{
		if (InputManager::IsPushRightKey())
		{
			OnPushRight();
		}
		else if (InputManager::IsPushLeftKey())
		{
			OnPushLeft();
		}
		else if (InputManager::IsTriggerDecisionKey())
		{
			//auto seTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/decide.wav");
			//gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->Play(seTag, 0.1f);

			m_isEnd = true;
		}
	}

	if (m_isEnd)
	{
		m_endTimer++;

		auto player = GetManager().lock()->GetGameObject("PlayerModel");
		float per = float(m_endTimer) / 30;
		float playerX = per;
		float playerY = Easing::Parabola(per) + (-1.6f);
		float playerZ = -per + 3.0f;
		player.lock()->transform->SetWorldPosition(Vector3(playerX, playerY, playerZ));
		player.lock()->transform->RollLocalRotationX_Degrees(-5.0f);

		auto m_vwp_stageNumber = GetManager().lock()->GetGameObject("StageNumber");
		if (m_endTimer < 55)
		{
			//float alpha = 1.0f - Easing::Liner(per);
			//if (alpha < 0.0f)
			//{
			//	alpha = 0.0f;
			//}

			//auto rArrow = GetManager().lock()->GetGameObject("RightArrow");
			//auto lArrow = GetManager().lock()->GetGameObject("LeftArrow");

			//auto rMeshDraw = rArrow.lock()->GetGameComponent<MeshDrawComponent>();
			//auto lMeshDraw = lArrow.lock()->GetGameComponent<MeshDrawComponent>();

			//auto rLightBuff = rMeshDraw->GetCBuffer<LightVariable>("LightBuffer");
			//auto lLightBuff = lMeshDraw->GetCBuffer<LightVariable>("LightBuffer");

			//rLightBuff->Get().lightDir.w = alpha;
			//lLightBuff->Get().lightDir.w = alpha;
		}
		if (m_endTimer >= 55)
		{
			player.lock()->SetIsRemove(true);
			m_vwp_stageNumber.lock()->GetGameComponent<NumberManager>()->Remove();
			m_vwp_stageNumber.lock()->SetIsRemove(true);
			auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
			std::string sceneName = "Stage_" + std::to_string(m_stageNum);
			sceneManager->RemoveScene(sceneName);
			sceneManager->LoadScene(sceneName);
			sceneManager->ChangeScene(sceneName);
		}
		else if (m_endTimer >= 7)
		{
			m_stageNumberObjectScale += 50;
			auto numManager = m_vwp_stageNumber.lock()->GetGameComponent<NumberManager>();
			numManager->SetScale(Vector3(300.0f, m_stageNumberObjectScale, 1.0f));
			numManager->TranslateY(50.0f);
		}
		else if (m_endTimer >= 0)
		{
			m_stageNumberObjectScale -= 60;
			auto numManager = m_vwp_stageNumber.lock()->GetGameComponent<NumberManager>();
			numManager->SetScale(Vector3(300.0f, m_stageNumberObjectScale, 1.0f));
			numManager->TranslateY(20.0f);
		}
	}

	if (m_vlp_animTimer->Update())
	{
		m_vlp_animTimer->Stop();
	}
}

void ButiEngine::StageSelectManager::OnSet()
{
}

void ButiEngine::StageSelectManager::Start()
{
	m_vwp_stageNumber = GetManager().lock()->GetGameObject("StageNumber");
	m_vwp_stageNumber.lock()->transform->SetLocalScale(Vector3(300, 300, 300));

	m_vlp_animTimer = ObjectFactory::Create<RelativeTimer>(15);

	m_isEnd = false;
	m_endTimer = 0;
	m_stageNumberObjectScale = 500.0f;
	m_pushCount = 0;

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
	m_stageNum++;
	if (m_stageNum > m_maxStageNum)
	{
		m_stageNum--;
		return "ThanksScene";
	}

	std::string nextSceneName = "Stage_" + std::to_string(m_stageNum);
	return nextSceneName;
}

void ButiEngine::StageSelectManager::SetMaxStageNum()
{
	for (int i = 0;; i++) {
		std::string path = "Resources/Scene/Stage_" + std::to_string(i) + "/mapInfo.map";
		if (!Util::ExistFile(path)) {
			m_maxStageNum = i - 1;
			break;
		}
	}
}

void ButiEngine::StageSelectManager::RestartAnimTimer()
{
	m_vlp_animTimer->Reset();
	m_vlp_animTimer->Start();
}

void ButiEngine::StageSelectManager::OnPushRight()
{
	//auto seTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/select.wav");
	//gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->Play(seTag, 0.1f);

	m_stageNum++;
	if (m_stageNum > m_maxStageNum)
	{
		m_stageNum = 0;
	}
	m_vwp_stageNumber.lock()->GetGameComponent<NumberManager>()->SetNumber(m_stageNum);

	GetManager().lock()->AddObjectFromCereal("ArrowEffect", ObjectFactory::Create<Transform>(Vector3(500, 100, 9 - m_pushCount), 0.0f, Vector3(300, 300, 1)));

	m_pushCount++;
	if (m_pushCount > 5)
	{
		m_pushCount = 0;
	}

	RestartAnimTimer();
}

void ButiEngine::StageSelectManager::OnPushLeft()
{
	//auto seTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/select.wav");
	//gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->Play(seTag, 0.1f);

	m_stageNum--;
	if (m_stageNum < 0)
	{
		m_stageNum = m_maxStageNum;
	}
	m_vwp_stageNumber.lock()->GetGameComponent<NumberManager>()->SetNumber(m_stageNum);

	GetManager().lock()->AddObjectFromCereal("ArrowEffect", ObjectFactory::Create<Transform>(Vector3(-500, 100, 9 - m_pushCount), Vector3(0, 0, 180), Vector3(300, 300, 1)));

	m_pushCount++;
	if (m_pushCount > 5)
	{
		m_pushCount = 0;
	}

	RestartAnimTimer();
}
