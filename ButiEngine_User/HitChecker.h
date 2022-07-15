#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class HitChecker :public GameComponent
	{
	public:
		HitChecker() {}

		void OnUpdate()override;
		std::string GetGameComponentName()const override {
			return "HitChecker";
		}
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		void ShowGUI()override;
	private:

		std::string hitObjName;
	};

}
BUTI_REGIST_GAMECOMPONENT(HitChecker,true)