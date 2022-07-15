#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class ImmediateParticleController;
	class SquareParticleEmitter :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "SquareParticleEmitter";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void OnShowUI()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
			archive(colorMin);
			archive(colorMax);
			archive(colorPaseMin);
			archive(colorPaseMax);
			archive(leftUP);
			archive(leftDown);
			archive(rightUP);
			archive(rightDown);
			archive(thickness);
			archive(sizeMin);
			archive(sizeMax);
			archive(speedMin);
			archive(speedMax);
			archive(accelMin);
			archive(accelMax);
			archive(lifeMin);
			archive(lifeMax);
			archive(increase);
		}
	private:
		void ParticleInformationSet(Particle3D& arg_refParticle);
		Vector4 colorMin, colorMax,colorPaseMin,colorPaseMax;
		Vector3 leftUP, leftDown, rightUP, rightDown,center;
		float	thickness, sizeMin, sizeMax, speedMin,speedMax,accelMin,accelMax, lifeMin, lifeMax;
		std::int32_t increase=0;
		Value_ptr<ImmediateParticleController> vlp_particleController;
	};

}

BUTI_REGIST_GAMECOMPONENT(SquareParticleEmitter,true);