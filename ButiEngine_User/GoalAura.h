#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class GoalAura :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "GoalAura";
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
BUTI_REGIST_GAMECOMPONENT(GoalAura, true)