#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class TitleBlockComponent :public GameComponent
	{
	public:
		TitleBlockComponent() {}

		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		std::string GetGameComponentName()override {
			return "TitleBlockComponent";
		}
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
		void OnShowUI();
	private:
		float defaultY;
		float angle;
		float theta;
	};

}
BUTI_REGIST_GAMECOMPONENT(ButiEngine::TitleBlockComponent)