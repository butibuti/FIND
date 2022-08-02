#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class CameraMesh;
	enum class LookDirection;
	class Player;
	class Map;

	class EyeBlock :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "EyeBlock";
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

		void SetMapPos(const Vector3& arg_mapPos) { m_mapPos = arg_mapPos; }
		Vector3 GetMapPos() { return m_mapPos; }

		void Dead();

		void CheckLookBlock();
		void FlashMeshSet(const Vector3& arg_playerPos);
		void Flash();
		void Expansion();
		void Shrink();
	private:
		void SetLookDirection();
		Value_weak_ptr<GameObject> GetRightBlock(Vector3& ref_output_pos);
		Value_weak_ptr<GameObject> GetLeftBlock(Vector3& ref_output_pos);
		Value_weak_ptr<GameObject> GetUpBlock(Vector3& ref_output_pos);
		Value_weak_ptr<GameObject> GetDownBlock(Vector3& ref_output_pos);
		Value_weak_ptr<GameObject> GetFrontBlock(Vector3& ref_output_pos);
		Value_weak_ptr<GameObject> GetBackBlock(Vector3& ref_output_pos);

		bool IsBlock(std::uint16_t arg_mapChipNum);

		Value_weak_ptr<GameObject> m_vwp_cameraMesh;
		Value_weak_ptr<CameraMesh> m_vwp_cameraMeshComponent;

		Value_weak_ptr<Map> m_vwp_mapComponent;

		Vector3 m_mapPos;
		Vector3 m_playerPos;
		LookDirection m_lookDirection;
		float m_scale;
	};

}
BUTI_REGIST_GAMECOMPONENT(EyeBlock, true)