#include "stdafx_u.h"
#include "InvisibleBlock.h"
#include "Map.h"
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
	auto meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent>();
	meshDraw->UnRegist();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::InvisibleBlock::Clone()
{
	return ObjectFactory::Create<InvisibleBlock>();
}

void ButiEngine::InvisibleBlock::Active()
{
	if (m_isActive)
	{
		return;
	}

	auto t = gameObject.lock()->transform;


	auto anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
	anim->SetSpeed(1.0f / 10);
	anim->SetTargetTransform(t->Clone());
	anim->GetTargetTransform()->SetLocalScale(1.1f);
	anim->GetTargetTransform()->RollLocalRotationX_Degrees(0.1f);

	anim->SetEaseType(Easing::EasingType::Parabola);

	auto mapComp = GetManager().lock()->GetGameObject("Map").lock()->GetGameComponent<Map>();
	mapComp->ChangeBlock(m_mapPos, GameSettings::MAP_CHIP_BLOCK);
	m_isActive = true;
	auto meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent>();
	meshDraw->Regist();

	gameObject.lock()->GetApplication().lock()->GetSoundManager()->PlaySE(SoundTag("Sound/inv_Discover.wav"), 0.003f);
}

void ButiEngine::InvisibleBlock::UnActive()
{
	if (!m_isActive)
	{
		return;
	}

	auto mapComp = GetManager().lock()->GetGameObject("Map").lock()->GetGameComponent<Map>();
	mapComp->ChangeBlock(m_mapPos, GameSettings::MAP_CHIP_INVISIBLEBLOCK + m_id);
	m_isActive = false;
	auto meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent>();
	meshDraw->UnRegist();

	gameObject.lock()->GetApplication().lock()->GetSoundManager()->PlaySE(SoundTag("Sound/inv_Cover.wav"), 0.003f);
}

void ButiEngine::InvisibleBlock::Seen()
{
	m_isSeen = true;
}
