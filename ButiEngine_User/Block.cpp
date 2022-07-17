#include "stdafx_u.h"
#include "Block.h"
#include "MapComponent.h"

void ButiEngine::Block::OnUpdate()
{
    //current += pase;

    //if (current >= 1.0f) {
    //    current = 1.0f;
    //    pase *= -1;
    //}
    //else
    //    if (current <= 0.0f) {
    //        current = 0.0f;
    //        pase *= -1;
    //    }

    //shp_lightBuffer->Get().lightDir = wkp_mapComponent.lock()->GetMapStartColor() * (1 - current) + wkp_mapComponent.lock()->GetMapEndColor() * current;
}

void ButiEngine::Block::OnSet()
{
}

void ButiEngine::Block::OnRemove()
{
}

void ButiEngine::Block::OnShowUI()
{
    GUI::BulletText("CurrentGlowPower");
    GUI::DragFloat("##currentGlowPower", current, 0.01f, 0, 1);
    GUI::BulletText("GlowPowerPase");
    GUI::DragFloat("##tGlowPowerPase", pase, 0.01f, 0, 1);
}

void ButiEngine::Block::Start()
{
    //shp_lightBuffer = gameObject.lock()->GetGameComponent<MeshDrawComponent_Static>()->GetCBuffer<LightVariable>("LightBuffer");
    //auto pos = gameObject.lock()->transform->GetWorldPosition();
    //current = (sin(MathHelper::Dot(Vector2(pos.x, pos.z), Vector2(1.9898 + pos.y, 8.233))) * 437.645);
    //current -= (int)current;

    //auto map = GetManager().lock()->GetGameObject("Map");
    //wkp_mapComponent = map.lock()->GetGameComponent<Map>();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Block::Clone()
{
    auto output = ObjectFactory::Create<Block>();
    //output->wkp_mapComponent = wkp_mapComponent;
    output->pase = pase;
    output->current = current;
    return output;
}
