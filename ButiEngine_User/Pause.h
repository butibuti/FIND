#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

class PauseManager :public GameComponent
{
public:

	std::string GetGameComponentName()const override {
		return "PauseManager";
	}
	void OnUpdate()override;
	void OnSet()override;
	void Start()override;
	void End()override;
	Value_ptr<GameComponent> Clone()override;
	static bool IsPause();
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(isActive);
	}
private:

};

}

BUTI_REGIST_GAMECOMPONENT(PauseManager, true);