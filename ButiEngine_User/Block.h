#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

class Map;

class Block :public GameComponent
{
public:
	std::string GetGameComponentName()const override {
		return "Block";
	}
	void OnUpdate()override;
	void OnSet()override;
	void OnRemove()override;
	void OnShowUI()override;
	void Start()override;
	Value_ptr<GameComponent> Clone()override;
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(isActive);
		archive(m_pase);
		archive(m_current);
	}
private:
	Value_weak_ptr<Map> m_vwp_mapComponent;
	float m_pase;
	float m_current = 0.5f;
	Value_ptr<ButiRendering::CBuffer<ButiRendering::ObjectInformation>> m_vlp_objectInformation;
};
}
BUTI_REGIST_GAMECOMPONENT(Block, true)
