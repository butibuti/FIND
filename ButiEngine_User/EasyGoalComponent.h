#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	namespace Collision
	{
		class CollisionPrimitive_Box_AABB;
	}

	class EasyGoalComponent :public GameComponent
	{
	public:
		EasyGoalComponent() {}

		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		std::string GetGameComponentName()override {
			return "EasyGoalComponent";
		}
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
		void OnShowUI();

		bool IsActive() { return active; }

		void Seen();
	private:
		std::shared_ptr<Collision::CollisionPrimitive_Box_AABB> shp_AABB;
		bool active;
	};

}
BUTI_REGIST_GAMECOMPONENT(ButiEngine::EasyGoalComponent)