#include "stdafx_u.h"
#include "StagePreviewParent.h"
#include "Header/GameObjects/DefaultGameComponent/ScaleAnimationComponent.h"

void ButiEngine::StagePreviewParent::OnUpdate()
{
	gameObject.lock()->transform->SetLocalRotation(m_vwp_cameraAxis.lock()->transform->GetLocalRotation());
}

void ButiEngine::StagePreviewParent::OnSet()
{
}

void ButiEngine::StagePreviewParent::Start()
{
	m_vwp_preview.lock()->transform->SetBaseTransform(gameObject.lock()->transform);
	m_vwp_preview.lock()->transform->SetLocalPosition(Vector3(0.0f, 0.5f, 0.0f));
	m_vwp_preview.lock()->transform->SetLocalScale(1.0f);

	m_startPos = gameObject.lock()->transform->GetLocalPosition();
	m_startScale = gameObject.lock()->transform->GetLocalScale();
	m_targetScale = Vector3(8.0f, 4.5f, 1.0f);
	m_theta = 0.0f;

	m_vwp_cameraAxis = GetManager().lock()->GetGameObject("CameraAxis");
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::StagePreviewParent::Clone()
{
	return ObjectFactory::Create<StagePreviewParent>();
}

void ButiEngine::StagePreviewParent::Appear()
{
	gameObject.lock()->transform->SetLocalPosition(m_startPos);
	AddScaleAnimation(m_targetScale);
}

void ButiEngine::StagePreviewParent::Disappear()
{
	AddScaleAnimation(m_startScale);
}

void ButiEngine::StagePreviewParent::CreatePreview(const std::uint16_t arg_stageNum)
{
	m_vwp_preview = GetManager().lock()->AddObjectFromCereal("StagePreview");

	auto materialName = "Material/StagePreview_" + std::to_string(arg_stageNum) + ".mat";
	auto meshDrawComponent = m_vwp_preview.lock()->GetGameComponent<MeshDrawComponent>();
	meshDrawComponent->SetMaterialTag(MaterialTag(materialName), 0);
	meshDrawComponent->ReRegist();
}

void ButiEngine::StagePreviewParent::AddScaleAnimation(const Vector3& arg_targetScale)
{
	auto anim = gameObject.lock()->GetGameComponent<ScaleAnimation>();
	if (anim)
	{
		anim->SetIsRemove(true);
	}
	anim = gameObject.lock()->AddGameComponent<ScaleAnimation>();
	anim->SetInitScale(gameObject.lock()->transform->GetLocalScale());
	anim->SetTargetScale(arg_targetScale);
	anim->SetSpeed(1.0f / 15);
	anim->SetEaseType(Easing::EasingType::EaseOutCirc);
}

void ButiEngine::StagePreviewParent::FloatMotion()
{
}
