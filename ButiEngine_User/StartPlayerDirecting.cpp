#include "stdafx_u.h"
#include "StartPlayerDirecting.h"
#include "Player.h"
#include "InvisibleBlock.h"
#include "CameraMesh.h"

void ButiEngine::StartPlayerDirecting::OnUpdate()
{
	//if (start)
	//{
	//	return;
	//}
	//if (!fallStart && timer->Update())
	//{
	//	timer->Stop();
	//	fallStart = true;

	//	Animation();
	//}
	//auto anim = gameObject.lock()->GetGameComponent<TransformAnimation>();
	//if (fallStart && !anim)
	//{
	//	//”g–ä
	//	auto pos = gameObject.lock()->transform->GetWorldPosition();
	//	pos.y -= 0.3f;
	//	GetManager().lock()->AddObjectFromCereal("Ripple", ObjectFactory::Create<Transform>(pos, Vector3(90, 0, 0), 0.0f));

	//	//ƒtƒ‰ƒbƒVƒ…
	//	GetManager().lock()->GetGameObject("CameraMesh").lock()->GetGameComponent<CameraMeshComponent>()->Flash();

	//	gameObject.lock()->GetBehavior<PlayerBehavior>()->CheckLookBlock();
	//	GetManager().lock()->GetGameObject("InvisibleBlockManager").lock()->GetGameComponent<InvisibleBlockManagerComponent>()->CheckSeen();

	//	auto seTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/BigMove_0.wav");
	//	gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->Play(seTag, 0.1f);

	//	start = true;
	//}
}

void ButiEngine::StartPlayerDirecting::OnSet()
{
}

void ButiEngine::StartPlayerDirecting::Start()
{
	//timer = ObjectFactory::Create<RelativeTimer>(60);
	//timer->Start();
	//start = false;
	//fallStart = false;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::StartPlayerDirecting::Clone()
{
	return ObjectFactory::Create<StartPlayerDirecting>();
}

void ButiEngine::StartPlayerDirecting::Animation()
{
	//start = false;

	//auto t = gameObject.lock()->transform;

	//auto anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
	//anim->SetSpeed(1.0f / 15);
	//anim->SetTargetTransform(t->Clone());
	//anim->GetTargetTransform()->TranslateY(startPos.y - spawnPos.y);
	//anim->GetTargetTransform()->RollLocalRotationX_Degrees(0.1f);

	//anim->SetEaseType(Easing::EasingType::Liner);
}
