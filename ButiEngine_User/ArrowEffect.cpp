#include "stdafx_u.h"
#include "ArrowEffect.h"

void ButiEngine::Arroweffect::OnUpdate()
{
	//progressFrame++;
	//if (progressFrame > life)
	//{
	//	gameObject.lock()->SetIsRemove(true);
	//	return;
	//}

	//auto t = gameObject.lock()->transform;
	//float per = float(progressFrame) / life;
	//float scale = 500.0f + Easing::Liner(per) * 1500.0f;
	//t->SetLocalScale(Vector3(scale, scale, 1));

	//auto meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent>();
	//auto lightBuff = meshDraw->GetCBuffer<LightVariable>("LightBuffer");
	//float alpha = 0.3f - Easing::Liner(per) * 0.3f;
	//if (alpha < 0.0f)
	//{
	//	alpha = 0.0f;
	//}
	//lightBuff->Get().lightDir.w = alpha;
}

void ButiEngine::Arroweffect::OnSet()
{
}

void ButiEngine::Arroweffect::OnRemove()
{
}

void ButiEngine::Arroweffect::OnShowUI()
{
}

void ButiEngine::Arroweffect::Start()
{
	life = 30;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Arroweffect::Clone()
{
	return ObjectFactory::Create<Arroweffect>();
}
