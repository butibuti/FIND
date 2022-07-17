#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class BurstManager :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "BurstManager";
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

		void Burst();
	private:
	};

}
BUTI_REGIST_GAMECOMPONENT(BurstManager, true)