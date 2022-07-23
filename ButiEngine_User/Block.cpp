#include "stdafx_u.h"
#include "Block.h"
#include "Map.h"

void ButiEngine::Block::OnUpdate()
{
    m_current += m_pase;

    if (m_current >= 1.0f) {
        m_current = 1.0f;
        m_pase *= -1;
    }
    else
        if (m_current <= 0.0f) {
            m_current = 0.0f;
            m_pase *= -1;
        }

    m_vlp_objectInformation->Get().color = m_vwp_mapComponent.lock()->GetMapStartColor() * (1 - m_current) + m_vwp_mapComponent.lock()->GetMapEndColor() * m_current;
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
    GUI::DragFloat("##currentGlowPower", m_current, 0.01f, 0, 1);
    GUI::BulletText("GlowPowerPase");
    GUI::DragFloat("##tGlowPowerPase", m_pase, 0.01f, 0, 1);
}

void ButiEngine::Block::Start()
{
    m_vlp_objectInformation = gameObject.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>();
    auto pos = gameObject.lock()->transform->GetWorldPosition();
    m_current = (sin(Vector2(pos.x, pos.z).Dot(Vector2(1.9898 + pos.y, 8.233))) * 437.645);
    m_current -= static_cast<int>(m_current);

    auto map = GetManager().lock()->GetGameObject("Map");
    m_vwp_mapComponent = map.lock()->GetGameComponent<Map>();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Block::Clone()
{
    auto output = ObjectFactory::Create<Block>();
    output->m_vwp_mapComponent = m_vwp_mapComponent;
    output->m_pase = m_pase;
    output->m_current = m_current;
    return output;
}
