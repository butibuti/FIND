#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class InvisibleBlockManagerComponent :public GameComponent
	{
	public:
		InvisibleBlockManagerComponent() {}

		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		std::string GetGameComponentName()override {
			return "InvisibleBlockManagerComponent";
		}
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
		void OnShowUI();

		void CheckSeen();

		void Reset();
	private:
	};

}
BUTI_REGIST_GAMECOMPONENT(ButiEngine::InvisibleBlockManagerComponent)