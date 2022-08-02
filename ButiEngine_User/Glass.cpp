#include"stdafx.h"
#include"Glass.h"
#include"Map.h"
#include"GameSettings.h"
void ButiEngine::GlassBlock::OnUpdate()
{
    HiddenSurface();
    SetIsRemove(true);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::GlassBlock::Clone()
{
    return ObjectFactory::Create<GlassBlock>();
}

void ButiEngine::GlassBlock::HiddenSurface()
{
    auto mapData = m_vwp_map.lock()->GetCurrentMapData().lock();
    if (mapData->GetSize().x > m_mapPos.x + 1 &&
        mapData->m_vec_mapDatas[m_mapPos.y][m_mapPos.z][m_mapPos.x + 1] == GameSettings::MAP_CHIP_GLASS) {
        gameObject.lock()->GetGameComponent<MeshDrawComponent>(3)->UnRegist();

    }
    if (m_mapPos.x > 0 &&
        mapData->m_vec_mapDatas[m_mapPos.y][m_mapPos.z][m_mapPos.x - 1] == GameSettings::MAP_CHIP_GLASS) {
        gameObject.lock()->GetGameComponent<MeshDrawComponent>(2)->UnRegist();
    }

    if (mapData->GetSize().y > m_mapPos.y + 1 &&
        mapData->m_vec_mapDatas[m_mapPos.y + 1][m_mapPos.z][m_mapPos.x] == GameSettings::MAP_CHIP_GLASS) {
        gameObject.lock()->GetGameComponent<MeshDrawComponent>(5)->UnRegist();

    }
    if (m_mapPos.y > 0&&
        mapData->m_vec_mapDatas[m_mapPos.y - 1][m_mapPos.z][m_mapPos.x] == GameSettings::MAP_CHIP_GLASS) {
        gameObject.lock()->GetGameComponent<MeshDrawComponent>(4)->UnRegist();
    }

    if (mapData->GetSize().z > m_mapPos.z + 1 &&
        mapData->m_vec_mapDatas[m_mapPos.y][m_mapPos.z+1][m_mapPos.x] == GameSettings::MAP_CHIP_GLASS) {
        gameObject.lock()->GetGameComponent<MeshDrawComponent>(1)->UnRegist();

    }
    if (m_mapPos.z > 0 &&
        mapData->m_vec_mapDatas[m_mapPos.y][m_mapPos.z- 1][m_mapPos.x] == GameSettings::MAP_CHIP_GLASS) {
        gameObject.lock()->GetGameComponent<MeshDrawComponent>(0)->UnRegist();
    }
}
