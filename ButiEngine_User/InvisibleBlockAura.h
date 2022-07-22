#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class InvisibleBlockAura :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "InvisibleBlockAura";
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

		void AnimInitialize();
	private:
		Value_ptr<RelativeTimer> m_vlp_timer;
	};

}
BUTI_REGIST_GAMECOMPONENT(InvisibleBlockAura, true)