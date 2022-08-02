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

		void SetColor(const Vector4& arg_color)
		{
			gameObject.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>()->Get().color = arg_color;
			gameObject.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>()->Get().color.w = m_alpha;
		}

		void Flash();
	private:
		float m_alpha;
	};

}
BUTI_REGIST_GAMECOMPONENT(CameraMesh, true)