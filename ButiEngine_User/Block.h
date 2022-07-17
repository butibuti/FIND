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
			archive(pase);
			archive(current);
		}
	private:
		std::weak_ptr<Map> wkp_mapComponent;
		float pase;
		float current = 0.5f;
		//std::shared_ptr<CBuffer<LightVariable>> shp_lightBuffer;
	};
}
BUTI_REGIST_GAMECOMPONENT(Block, true)
