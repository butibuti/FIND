#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Player;

	class CameraController :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "CameraController";
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
			archive(m_pase);
		}

		void ZoomInOut();
		void ZoomIn();
		void ZoomOut();
	private:
		Vector3 m_offset;
		Value_weak_ptr<GameObject> m_vwp_cameraAxis;
		Value_weak_ptr<Player> m_vwp_playerComponent;
		float m_pase;
	};

}
BUTI_REGIST_GAMECOMPONENT(CameraController, true)