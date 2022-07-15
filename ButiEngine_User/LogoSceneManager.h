#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
class LogoSceneManager :public GameComponent
{
public:
	LogoSceneManager() {}

	void OnUpdate()override;
	std::string GetGameComponentName()const override {
		return "LogoSceneManager";
	}
	Value_ptr<GameComponent> Clone()override;
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(isActive);
		archive(vlp_timer);
		archive(vlp_transTimer);
		archive(logoApperanceSpeed);
	}
	void OnShowUI()override;
	void Start()override;
	void OnSet()override;
private:
	Value_ptr<ButiRendering::CBuffer<ButiRendering::ObjectInformation>> vlp_logoBuffer, vlp_teamBuffer;
	Value_ptr<RelativeTimer> vlp_timer, vlp_logoTimer, vlp_transTimer;
	float logoApperanceSpeed, logoTime, teamLogoTime = 0.0f;
	bool isTrans = false;
};

}
BUTI_REGIST_GAMECOMPONENT(LogoSceneManager,true)