#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class TitleBlock :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "TitleBlock";
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
		float defaultY;
		float angle;
		float theta;
	};

}
BUTI_REGIST_GAMECOMPONENT(TitleBlock, true)