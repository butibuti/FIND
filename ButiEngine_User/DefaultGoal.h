#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class DefaultGoal :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "DefaultGoal";
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

		bool IsActive() { return m_isActive; }

		void Seen();
	private:
		bool m_isActive;
	};

}
BUTI_REGIST_GAMECOMPONENT(DefaultGoal, true)