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

		void SetID(const std::uint8_t arg_id) { m_id = arg_id; }
		int GetID() { return m_id; }
		void Active();
		void UnActive();
		void SetSeen(const bool arg_flag) { m_isSeen = arg_flag; }
		bool IsSeen() { return m_isSeen; }
		void SetMapPos(const Vector3& arg_mapPos) { m_mapPos = arg_mapPos; }
		void Seen();
	private:
		std::uint8_t m_id;
		bool m_isActive;
		bool m_isSeen;
		Vector3 m_mapPos;
	};

}
BUTI_REGIST_GAMECOMPONENT(InvisibleBlock, true)