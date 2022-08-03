#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class InputManager :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "InputManager";
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

		//ÉQÅ[ÉÄëSëÃ
		static bool IsTriggerDecisionKey();
		static bool IsTriggerPutEyeBlockKey();
		static bool IsPushRightKey();
		static bool IsPushLeftKey();
		static bool IsPushFrontKey();
		static bool IsPushBackKey();
		static bool IsTriggerCameraRotateRightKey();
		static bool IsTriggerCameraRotateLeftKey();
		static bool IsTriggerResetKey();
	private:
		static const float m_DEADZONE;
		static Vector2 m_currentLeftStick;
	};

}

BUTI_REGIST_GAMECOMPONENT(InputManager, true);