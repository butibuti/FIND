#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class ThanksSceneManager :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "ThanksSceneManager";
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
		Value_weak_ptr<MeshDrawComponent> m_vwp_thanksTexture;
		float t = 1.0f;
		float power = 0.0f;
		float pase = 0.016f;
		bool isClicked = false;
	};

}
BUTI_REGIST_GAMECOMPONENT(ThanksSceneManager, true)