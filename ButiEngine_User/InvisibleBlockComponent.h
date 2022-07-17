#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class MapComponent;
	namespace Collision
	{
		class CollisionPrimitive_Box_AABB;
	}

	class InvisibleBlockComponent :public GameComponent
	{
	public:
		InvisibleBlockComponent() {}

		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		bool IsContaineVisibility();
		std::string GetGameComponentName()override {
			return "InvisibleBlockComponent";
		}
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
		void OnShowUI();

		void SetID(int arg_id) { id = arg_id; }
		int GetID() { return id; }
		void Active();
		void UnActive();
		void SetSeen(bool arg_flag) { seen = arg_flag; }
		bool GetSeen() { return seen; }
		void SetMapPos(Vector3 arg_mapPos) { mapPos = arg_mapPos; }
		void Seen();
	private:
		int id;
		bool active;
		bool seen;
		Vector3 mapPos;
	};

}
BUTI_REGIST_GAMECOMPONENT(ButiEngine::InvisibleBlockComponent)