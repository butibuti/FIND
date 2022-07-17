#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	namespace Collision
	{
		class CollisionPrimitive_Box_AABB;
	}

	class DefaultGoalComponent :public GameComponent
	{
	public:
		DefaultGoalComponent() {}

		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		std::string GetGameComponentName()override {
			return "DefaultGoalComponent";
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
		bool active;
		std::shared_ptr<Collision::CollisionPrimitive_Box_AABB> shp_AABB;
	};

}
BUTI_REGIST_GAMECOMPONENT(ButiEngine::DefaultGoalComponent)