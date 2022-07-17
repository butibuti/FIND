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
		unsigned int life;
		unsigned int progressFrame;
	};

}
BUTI_REGIST_GAMECOMPONENT(InvisibleBlockAura, true)