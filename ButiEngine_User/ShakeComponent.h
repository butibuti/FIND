#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class ShakeComponent :public GameComponent
	{
	public:
		ShakeComponent() {}

		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		std::string GetGameComponentName()override {
			return "ShakeComponent";
		}
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
		void OnShowUI();

		void Start(float arg_amplitude);
		void Stop();

		void SetDefaultPos(Vector3 arg_defaultPos)
		{
			defaultPos = arg_defaultPos;
		}
	private:
		Vector3 defaultPos;
		bool start;
		float amplitude;
	};

}
BUTI_REGIST_GAMECOMPONENT(ButiEngine::ShakeComponent)