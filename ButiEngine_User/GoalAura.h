#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class GoalAura :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "GoalAura";
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

		void AnimInitialize();
		void SetColor(const Vector4& arg_color)
		{
			gameObject.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>()->Get().color = arg_color;
		}
		void SetTargetScale(const Vector3& arg_targetScale) { m_targetScale = arg_targetScale; }
		void SetAnimFrame(const std::uint16_t arg_animFrame) { m_animFrame = arg_animFrame; }

	private:
		Value_ptr<RelativeTimer> m_vlp_timer;

		Vector3 m_targetScale = 30.0f;
		std::uint16_t m_animFrame = 60;
	};

}
BUTI_REGIST_GAMECOMPONENT(GoalAura, true)