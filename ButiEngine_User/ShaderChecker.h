#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class ShaderChecker :public GameComponent
	{
	public:
		ShaderChecker() {}

		void OnUpdate()override;
		std::string GetGameComponentName()const override {
			return "ShaderChecker";
		}
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
			archive(changeSceneName);
		}

		void ShowGUI()override;
		void OnShowUI()override;
		void Start()override;
	private:
		std::string changeSceneName;
		bool isCameraControll;
		Value_ptr<GameObject> vlp_gameObject;
		Value_ptr<GameObject> vlp_camera;
	};

}
BUTI_REGIST_GAMECOMPONENT(ShaderChecker,true)
