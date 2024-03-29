#include "stdafx_u.h"
#include "StartPlayerDirecting.h"
#include "Player.h"
#include "CameraMesh.h"
#include "InvisibleBlockManager.h"
#include "EyeBlock.h"
#include "CameraController.h"

void ButiEngine::StartPlayerDirecting::OnUpdate()
{
	if (m_isStart)
	{
		return;
	}
	if (!m_isFallStart && m_vlp_timer->Update())
	{
		m_vlp_timer->Stop();
		m_isFallStart = true;

		Animation();
	}
	auto anim = gameObject.lock()->GetGameComponent<TransformAnimation>();
	if (m_isFallStart && !anim)
	{
		GetManager().lock()->GetGameObject("MainCamera").lock()->GetGameComponent<CameraController>()->AddChaseComponent();

		//�g��
		auto pos = gameObject.lock()->transform->GetWorldPosition();
		pos.y -= 0.3f;
		GetManager().lock()->AddObjectFromCereal("Ripple", ObjectFactory::Create<Transform>(pos, Vector3(90, 0, 0), 0.0f));

		//�t���b�V��
		gameObject.lock()->GetGameComponent<Player>()->FlashMeshSet();
		GetManager().lock()->GetGameObject("LeftCameraMesh").lock()->GetGameComponent<CameraMesh>()->Flash();

		gameObject.lock()->GetGameComponent<Player>()->CheckLookBlock();

		auto eyeBlock = gameObject.lock()->GetGameComponent<Player>()->GetEyeBlock();
		if (eyeBlock.lock())
		{
			auto eyeBlockComponent = eyeBlock.lock()->GetGameComponent<EyeBlock>();
			eyeBlockComponent->CheckLookBlock();
			eyeBlockComponent->FlashMeshSet(gameObject.lock()->GetGameComponent<Player>()->GetMapPos());
			eyeBlockComponent->Flash();
		}
		else
		{
			GetManager().lock()->GetGameObject("RightCameraMesh").lock()->GetGameComponent<CameraMesh>()->Flash();
		}

		GetManager().lock()->GetGameObject("InvisibleBlockManager").lock()->GetGameComponent<InvisibleBlockManager>()->CheckSeen();

		gameObject.lock()->GetApplication().lock()->GetSoundManager()->PlaySE(SoundTag("Sound/BigMove_0.wav"), 0.1f);

		m_isStart = true;
	}
}

void ButiEngine::StartPlayerDirecting::OnSet()
{
}

void ButiEngine::StartPlayerDirecting::Start()
{
	std::uint16_t countNum = 60;
	auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
	auto sceneName = sceneManager->GetCurrentScene()->GetSceneInformation()->GetSceneName();
	if (sceneName == "NewStageSelectScene")
	{
		countNum = 90;
	}
	m_vlp_timer = ObjectFactory::Create<RelativeTimer>(countNum);
	m_vlp_timer->Start();
	m_isStart = false;
	m_isFallStart = false;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::StartPlayerDirecting::Clone()
{
	return ObjectFactory::Create<StartPlayerDirecting>();
}

void ButiEngine::StartPlayerDirecting::Animation()
{
	m_isStart = false;

	auto t = gameObject.lock()->transform;

	auto anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
	anim->SetSpeed(1.0f / 15);
	anim->SetTargetTransform(t->Clone());
	anim->GetTargetTransform()->TranslateY(m_startPos.y - m_spawnPos.y);

	anim->SetEaseType(Easing::EasingType::Liner);

	GetManager().lock()->GetGameObject("MainCamera").lock()->GetGameComponent<CameraController>()->SetCameraAxisPosition();
}

bool ButiEngine::StartPlayerDirecting::IsAnimation() const
{
	return (!m_isStart);
}
