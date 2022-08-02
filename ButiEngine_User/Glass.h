#ifndef GLASS_H
#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
class Map;
class GlassBlock :public GameComponent
{
public:
	std::string GetGameComponentName()const override {
		return "GlassBlock";
	}
	void OnUpdate()override;
	Value_ptr<GameComponent> Clone()override;
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(isActive);
	}
	Vector3 GetMapPos() { return m_mapPos; }
	void SetMapPos(const Vector3& arg_mapPos) { m_mapPos = arg_mapPos.GetRound(); }
	void SetMap(Value_weak_ptr<Map> arg_vwp_map) { m_vwp_map = arg_vwp_map; }
	void HiddenSurface();
private:
	Vector3 m_mapPos;
	Value_weak_ptr<Map> m_vwp_map;
};

}
BUTI_REGIST_GAMECOMPONENT(GlassBlock, true)
#endif // !GLASS_H
