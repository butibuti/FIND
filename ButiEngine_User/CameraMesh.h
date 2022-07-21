#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class CameraMesh :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "CameraMesh";
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

		void Flash();
	private:
		float m_alpha;
	};

}
BUTI_REGIST_GAMECOMPONENT(CameraMesh, true)