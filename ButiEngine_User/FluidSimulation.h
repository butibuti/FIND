#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

enum class MovePattern {
	Rest, ConstantVelocity, ConstantAccel, Equation, FreeFall
};

class FluidSimulation :public GameComponent
{
public:

	struct Particle
	{
		ButiEngine::Vector3 v;
		float rho=0;
		float sum=0;
		float p=0;
		ButiEngine::Vector3 f;
		ButiEngine::Value_ptr<ButiEngine::Transform> vlp_objectTransform;
	};
	std::string GetGameComponentName()const override {
		return "FluidSimulation";
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
		archive(isLeader);
		archive(leaderSpeed);
		archive(speed);
		archive(bounce);
		archive(attenuation);
		archive(acc);
		archive(mass);
		archive(inertia);
		archive(vlp_befPoint);

	}
private:

	float bounce, speed, mass = 1.0f, attenuation, leaderSpeed, inertia;
	bool isLeader;
	Vector3 velocity, fixVelocity, acc;

	Value_ptr<RelativeTimer>vlp_timer;
	Value_ptr<ButiRendering::IResource_Mesh> vlp_stageMesh;

	std::vector<Value_ptr<GameObject>> vec_vlp_gameObject;
	std::vector<ButiEngine::FluidSimulation::Particle> vec_particle;
	Value_ptr<FluidSimulation> vlp_befPoint;



	//Value_ptr<Transform> vlp_floorTransform;
};

}

BUTI_REGIST_GAMECOMPONENT(FluidSimulation,true);