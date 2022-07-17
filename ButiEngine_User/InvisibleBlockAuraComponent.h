#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class InvisibleBlockAuraComponent :public GameComponent
	{
	public:
		InvisibleBlockAuraComponent() {}

		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		std::string GetGameComponentName()override {
			return "InvisibleBlockAuraComponent";
		}
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
		void OnShowUI();

		void AnimInitialize();
	private:
		unsigned int life;
		unsigned int progressFrame;
	};

}
BUTI_REGIST_GAMECOMPONENT(ButiEngine::InvisibleBlockAuraComponent)