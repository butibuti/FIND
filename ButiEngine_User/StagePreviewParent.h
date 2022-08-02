#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class ScaleAnimation;

	class StagePreviewParent :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "StagePreviewParent";
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

		void Appear();
		void Disappear();
		void CreatePreview(const std::uint16_t arg_stageNum);
	private:
		void AddScaleAnimation(const Vector3& arg_targetScale);
		void FloatMotion();

		Value_weak_ptr<GameObject> m_vwp_preview;
		Value_weak_ptr<GameObject> m_vwp_cameraAxis;

		Vector3 m_startPos;
		Vector3 m_startScale;
		Vector3 m_targetScale;


		float m_theta;
	};

}
BUTI_REGIST_GAMECOMPONENT(StagePreviewParent, true)