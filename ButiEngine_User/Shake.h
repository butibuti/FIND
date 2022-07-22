#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Shake :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "Shake";
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

		void ShakeStart(const float arg_amplitude);
		void ShakeStop();

		void SetDefaultPos(const Vector3& arg_defaultPos)
		{
			m_defaultPos = arg_defaultPos;
		}
	private:
		Vector3 m_defaultPos;
		bool m_isStart;
		float m_amplitude;
	};

}
BUTI_REGIST_GAMECOMPONENT(Shake, true)