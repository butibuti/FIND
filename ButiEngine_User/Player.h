#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Map;
	class PlayerCamera;
	class InvisibleBlockManager;

	enum class MoveDirection
	{
		Up,
		Normal,
		Down,
		Fall,
		No,
	};

	enum class LookDirection
	{
		Right,
		Left,
		Up,
		Down,
		Front,
		Back,
	};

	enum class CameraDirection
	{
		Front,
		Left,
		Back,
		Right,
	};

	class Player :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "Player";
		}
		void OnUpdate()override;
		void OnSet()override;
		void OnRemove()override;
		void OnShowUI()override;
		void Start()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		bool IsGoal() { return m_isGoal; }

		bool IsRollFinish();
		const Vector3 &GetMapPos() const{ return m_mapPos; }
		const Vector3 &GetNextMapPos() { return m_nextMapPos; }

		void SetStartPos(const Vector3& arg_pos) { m_startPos = arg_pos; }
		void SetStartRotation(float arg_rot) { m_startRotation = arg_rot; }

		void CheckLookBlock();
		void RollCameraDirection(const std::uint16_t arg_rotateDir);
	private:
		float m_length;
		Value_weak_ptr<Map> m_vwp_mapComponent;
		Value_weak_ptr<InvisibleBlockManager> m_vwp_invisibleBlockManagerComponent;
		Vector3 m_mapPos;
		Vector3 m_nextMapPos;
		Vector3 m_offset;
		Vector3 m_startPos;
		float m_startRotation;
		bool m_isGoal;
		bool m_isFall;
		bool m_isFallStart;
		Vector3 m_afterFallPos;
		float m_scale;
		LookDirection m_lookDirection;
		CameraDirection m_cameraDirection=CameraDirection::Front;

		Value_ptr<RelativeTimer> m_vlp_timer;
		Value_ptr<RelativeTimer> m_vlp_fallTimer;

		void CheckGoal();
		void CheckLookDirection();
		void Contoroll();
		void OnPushRight();
		void OnPushLeft();
		void OnPushFront();
		void OnPushBack();
		void Expansion();
		void Shrink();
		void MoveRightUp();
		void MoveRight();
		void MoveRightDown();
		void MoveLeftUp();
		void MoveLeft();
		void MoveLeftDown();
		void MoveUpFront();
		void MoveFront();
		void MoveDownFront();
		void MoveUpBack();
		void MoveBack();
		void MoveDownBack();
		Value_weak_ptr<GameObject> GetRightBlock(const Vector3& arg_mapPos);
		Value_weak_ptr<GameObject> GetLeftBlock(const Vector3& arg_mapPos);
		Value_weak_ptr<GameObject> GetUpBlock(const Vector3& arg_mapPos);
		Value_weak_ptr<GameObject> GetDownBlock(const Vector3& arg_mapPos);
		Value_weak_ptr<GameObject> GetFrontBlock(const Vector3& arg_mapPos);
		Value_weak_ptr<GameObject> GetBackBlock(const Vector3& arg_mapPos);


		SoundTag m_moveSounds[3];
		void Fall();
		MoveDirection CheckMoveDirection(const Vector3& arg_movePos);
		void CheckExistUnderBlock(const Vector3& arg_movePos);

		bool IsBlock(std::uint16_t arg_mapChipNum);
	};

}

BUTI_REGIST_GAMECOMPONENT(Player, true);