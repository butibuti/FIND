#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Arroweffect :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "ArrowEffect";
		}
		void OnUpdate()override;
		void OnSet()override;
		void OnRemove()override;
		void OnShowUI()override;
		void Start()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
	private:
		Value_ptr<RelativeTimer> m_vlp_timer;
	};
}
BUTI_REGIST_GAMECOMPONENT(Arroweffect, true)