#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class InvisibleBlockManager :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "InvisibleBlockManager";
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

		void CheckSeen();

		void Reset();
	private:
	};

}
BUTI_REGIST_GAMECOMPONENT(InvisibleBlockManager, true)