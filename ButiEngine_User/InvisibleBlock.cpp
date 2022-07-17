#include "stdafx_u.h"
#include "InvisibleBlock.h"
#include "MapComponent.h"
#include "Player.h"
#include "GameSettings.h"

void ButiEngine::InvisibleBlock::OnUpdate()
{
}

void ButiEngine::InvisibleBlock::OnSet()
{
}

void ButiEngine::InvisibleBlock::Start()
{
	//auto meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent_Static>();
	//meshDraw->UnRegist();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::InvisibleBlock::Clone()
{
	return ObjectFactory::Create<InvisibleBlock>();
}

void ButiEngine::InvisibleBlock::Active()
{
	//if (active)
	//{
	//	return;
	//}

	//auto t = gameObject.lock()->transform;


	//auto anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
	//anim->SetSpeed(1.0f / 10);
	//anim->SetTargetTransform(t->Clone());
	//anim->GetTargetTransform()->SetLocalScale(1.1f);
	//anim->GetTargetTransform()->RollLocalRotationX_Degrees(0.1f);

	//anim->SetEaseType(Easing::EasingType::Parabola);

	//auto mapComp = GetManager().lock()->GetGameObject("Map").lock()->GetGameComponent<MapComponent>();
	//mapComp->ChangeBlock(mapPos, GameSettings::MAP_CHIP_BLOCK);
	//active = true;
	//auto meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent_Static>();
	//meshDraw->Regist();

	//auto seTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/inv_Discover.wav");

	//gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->Play(seTag, 0.003f);
}

void ButiEngine::InvisibleBlock::UnActive()
{
	//if (!active)
	//{
	//	return;
	//}

	//auto mapComp = GetManager().lock()->GetGameObject("Map").lock()->GetGameComponent<MapComponent>();
	//mapComp->ChangeBlock(mapPos, GameSettings::MAP_CHIP_INVISIBLEBLOCK + id);
	//active = false;
	//auto meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent_Static>();
	//meshDraw->UnRegist();
	//auto seTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/inv_Cover.wav");

	//gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->Play(seTag, 0.003f);
}

void ButiEngine::InvisibleBlock::Seen()
{
}
