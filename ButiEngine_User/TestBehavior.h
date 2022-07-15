#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
class TestBehavior :public GameComponent
{
public:

	std::string GetGameComponentName()const override {
		return "TestBehavior";
	}
	void OnUpdate()override;
	void OnSet()override;
	void Start()override;
	void OnRemove()override;
	void OnShowUI()override;
	void ShowGUI()override;
	Vector3 GetWorldPosition();
	void AdditionMass(const float arg_mass);
	Value_ptr<GameComponent> Clone()override;
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(isActive);

	}
private:
	Value_ptr< SpriteParticleController> vlp_particleComponent;
	Value_ptr<GameObject> vlp_UIObject;
};

}

BUTI_REGIST_GAMECOMPONENT(TestBehavior,true);