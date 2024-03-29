#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class FloatMotion :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "FloatMotion";
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
		float m_defaultY;
		float m_theta;
		float m_anglePase;
		float m_thetaPase;
	};

}
BUTI_REGIST_GAMECOMPONENT(FloatMotion, true)