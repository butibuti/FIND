#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class GoalAuraComponent :public GameComponent
	{
	public:
		GoalAuraComponent() {}

		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		std::string GetGameComponentName()override {
			return "GoalAuraComponent";
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
BUTI_REGIST_GAMECOMPONENT(ButiEngine::GoalAuraComponent)