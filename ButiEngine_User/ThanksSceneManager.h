#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class ThanksSceneManager :public GameComponent
	{
	public:
		ThanksSceneManager() {}

		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		std::string GetGameComponentName()override {
			return "ThanksSceneManager";
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
		std::shared_ptr<CBuffer<LightVariable>> buffer;
		float t = 1.0f;
		float power = 0.0f;
		float pase = 0.016f;
		bool isClicked = false;
	};

}
BUTI_REGIST_GAMECOMPONENT(ButiEngine::ThanksSceneManager)