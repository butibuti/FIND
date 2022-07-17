#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Map;

	class InvisibleBlock :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "InvisibleBlock";
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
BUTI_REGIST_GAMECOMPONENT(InvisibleBlock, true)