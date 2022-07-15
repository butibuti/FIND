#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

class ConfigManager :public GameComponent
{
public:

	std::string GetGameComponentName()const override {
		return "ConfigManager";
	}
	void OnUpdate()override;
	void OnSet()override;
	void Start()override;
	void End()override;
	Value_ptr<GameComponent> Clone()override;
	static bool IsConfig();
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(isActive);
	}
private:

};

}

BUTI_REGIST_GAMECOMPONENT(ConfigManager, true);