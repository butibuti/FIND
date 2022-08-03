#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Player;
	class MapEditor;
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
		void ZoomIn(const bool arg_isForced = false);
		void ZoomOut();

		void AddChaseComponent();
		void RemoveChaseComponent();
		void SetCameraAxisPosition();
	private:
		Vector3 m_offset;
		Value_weak_ptr<GameObject> m_vwp_cameraAxis;
		Value_weak_ptr<GameObject> m_vwp_player;
		Value_weak_ptr<Player> m_vwp_playerComponent;

#ifdef DEBUG
		Value_weak_ptr<MapEditor> m_vwp_mapEditor;
#endif
		float m_pase;
	};

}
BUTI_REGIST_GAMECOMPONENT(CameraController, true)