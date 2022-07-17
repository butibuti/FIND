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
			archive(pase);
		}

		void ZoomInOut();
		void ZoomIn();
		void ZoomOut();
	private:
		Vector3 offset;
		//std::shared_ptr<GameObject> shp_cameraAxis;
		//std::shared_ptr<Player> shp_player;
		float pase;
	};

}
BUTI_REGIST_GAMECOMPONENT(CameraController, true)