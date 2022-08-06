#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Map;
	class PlayerCamera;
	class InvisibleBlockManager;
	class NextStageBlock;
	class EyeBlock;

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

		void SetEyeBlock(Value_weak_ptr<GameObject> arg_eyeBlock);
		Value_weak_ptr<GameObject> GetEyeBlock() { return m_vwp_eyeBlock; }

		Value_weak_ptr<GameObject> GetLookObject() { return m_vwp_lookObject; }

		bool IsGoal() { return m_isGoal; }

		bool IsRollFinish();
		const Vector3& GetMapPos() const{ return m_mapPos; }
		const Vector3& GetNextMapPos() { return m_nextMapPos; }
		const Vector3& GetStartPos() { return m_startPos; }

		void SetStartPos(const Vector3& arg_pos) { m_startPos = arg_pos; }
		void SetStartRotation(float arg_rot) { m_startRotation = arg_rot; }

		void CheckLookBlock();
		void RollCameraDirection(const std::uint16_t arg_rotateDir);
		void FlashMeshSet(){
			m_lookDirection = CheckLookDirection(gameObject.lock()->transform);
			FlashMeshSet(gameObject.lock()->transform, m_lookDirection, m_mapPos);
		};

		static void SetCanPutEyeBlock(const bool arg_canPutEyeBlock);
		static bool GetCanAlreadyPutEyeBlock();
		static void SetCanAlreadyPutEyeBlock(const bool arg_canAlreadyPutEyeBlock);
	private:
		static bool m_canPutEyeBlock;
		Value_weak_ptr<GameObject> m_vwp_eyeBlock;
		Value_weak_ptr<EyeBlock> m_vwp_eyeBlockComponent;
		Value_weak_ptr<Map> m_vwp_mapComponent;
		Value_weak_ptr<InvisibleBlockManager> m_vwp_invisibleBlockManagerComponent;
		Value_weak_ptr<NextStageBlock> m_vwp_nextStageBlockComponent;
		Value_weak_ptr<GameObject> m_vwp_lookObject;

		Value_weak_ptr<GameObject> m_vwp_rightCameraMesh, m_vwp_leftCameraMesh;
		Value_ptr<Transform> m_vlp_rightEyeTransform, m_vlp_leftEyeTransform;

		Vector3 m_mapPos, m_nextMapPos, m_offset, m_startPos, m_afterFallPos,m_activeGoalPos;
		bool m_isGoal,m_isFall, m_isFallStart,m_isTouchNextStageBlock, m_isFoundGoal;
		float m_scale, m_startRotation, m_length;
		LookDirection m_lookDirection;
		CameraDirection m_cameraDirection=CameraDirection::Front;

		Value_ptr<RelativeTimer> m_vlp_timer,m_vlp_flashMeshTimer, m_vlp_expantionTimer, m_vlp_fallTimer;

		void Goal();
		void CheckTouchNextStageBlock();
		void CheckGoal();
		LookDirection CheckLookDirection(Value_ptr<Transform> arg_vlp_transform);
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
		void FlashMeshSet(Value_ptr<Transform> arg_vlp_transform, const LookDirection arg_dir, const Vector3& arg_pos);
		
		Value_weak_ptr<GameObject> GetRightBlock(const Vector3& arg_mapPos);
		Value_weak_ptr<GameObject> GetLeftBlock(const Vector3& arg_mapPos);
		Value_weak_ptr<GameObject> GetUpBlock(const Vector3& arg_mapPos);
		Value_weak_ptr<GameObject> GetDownBlock(const Vector3& arg_mapPos, std::int8_t& ref_output_diff, const bool arg_isCantThrough = false);
		Value_weak_ptr<GameObject> GetFrontBlock(const Vector3& arg_mapPos);
		Value_weak_ptr<GameObject> GetBackBlock(const Vector3& arg_mapPos);


		SoundTag m_moveSounds[3];
		void Fall();
		MoveDirection CheckMoveDirection(const Vector3& arg_movePos);
		void CheckExistUnderBlock(const Vector3& arg_movePos);

		bool IsBlock(std::uint16_t arg_mapChipNum);
		bool IsCantThroughBlock(const Vector3& arg_mapPos);
		bool CanPutEyeBlock(Value_weak_ptr<GameObject> arg_object);
	};

}

BUTI_REGIST_GAMECOMPONENT(Player, true);