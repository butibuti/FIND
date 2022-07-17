#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class TitleManagerComponent :public GameComponent
	{
	public:
		TitleManagerComponent() {}

		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		std::string GetGameComponentName()override {
			return "TitleManagerComponent";
		}
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
		void OnShowUI();
	private:
		std::shared_ptr<RelativeTimer>shp_timer;
	};

}
BUTI_REGIST_GAMECOMPONENT(ButiEngine::TitleManagerComponent)