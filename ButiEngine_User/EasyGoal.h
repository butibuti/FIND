#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class EasyGoal :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "EasyGoal";
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

		bool IsActive() { return active; }

		void Seen();
	private:
		bool active;
	};

}
BUTI_REGIST_GAMECOMPONENT(EasyGoal, true)