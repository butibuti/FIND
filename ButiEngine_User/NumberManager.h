#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class NumberManager :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "NumberManager";
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

		void SetNumber(std::uint16_t arg_number);

		void TranslateY(float arg_y);
		void SetScale(const Vector3& arg_scale);

		void Remove();
	private:
		Value_weak_ptr<GameObject> m_vwp_number1;
		Value_weak_ptr<GameObject> m_vwp_number10;
		std::uint16_t m_number;
	};

}
BUTI_REGIST_GAMECOMPONENT(NumberManager, true)