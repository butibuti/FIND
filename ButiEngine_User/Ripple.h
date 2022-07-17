#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Ripple :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "Ripple";
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

		static unsigned char GetCount()
		{
			return count; 
		}
		void AnimInitialize();
	private:
		static unsigned char count;
		int progressFrame;
		int life;
		float scale;
	};

}
BUTI_REGIST_GAMECOMPONENT(Ripple, true)