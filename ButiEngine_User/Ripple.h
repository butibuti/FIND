#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Ripple :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "Ripple";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
	private:
		Value_ptr<RelativeTimer> m_vlp_timer;
		float scale;
	};

}
BUTI_REGIST_GAMECOMPONENT(Ripple, true)