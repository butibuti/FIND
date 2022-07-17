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

		void SetNumber(int arg_number);

		void TranslateY(float y);
		void SetScale(Vector3 scale);

		void Remove();
	private:
		std::weak_ptr<GameObject> number1;
		std::weak_ptr<GameObject> number10;
		int number;
	};

}
BUTI_REGIST_GAMECOMPONENT(NumberManager, true)