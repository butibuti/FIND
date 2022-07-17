#pragma once
#include"Header/BehaviorHeader.h"
namespace ButiEngine {

	class MapComponent;
	class PlayerCameraComponent;
	class InvisibleBlockManagerComponent;
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

	class PlayerBehavior :public Behavior
	{
	public:
		std::string GetBehaviorName()override {
			return "PlayerBehavior";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		std::shared_ptr<Behavior> Clone()override;
		void OnCollisionEnter(std::weak_ptr<GameObject> arg_other)override;
		void OnCollision(std::weak_ptr<GameObject> arg_other)override;
		void OnCollisionEnd(std::weak_ptr<GameObject> arg_other)override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
		void OnShowUI()override;

		bool GetGoal() { return goal; }

		bool IsRollFinish();
		const Vector3 &GetMapPos() const{ return mapPos; }
		const Vector3 &GetNextMapPos() { return nextMapPos; }

		void SetStartPos(Vector3 pos) { startPos = pos; }
		void SetStartRotation(float rot) { startRotation = rot; }

		void CheckLookBlock();
		void RollCameraDirection(const int rotateDir);
	private:
		float length;
		std::shared_ptr<MapComponent> shp_map;
		std::shared_ptr<InvisibleBlockManagerComponent> shp_invisibleBlockManager;
		Vector3 mapPos;
		Vector3 nextMapPos;
		Vector3 offset;
		Vector3 startPos;
		float startRotation;
		bool goal;
		bool fall;
		bool fallStart;
		Vector3 afterFallPos;
		float scale;
		LookDirection lookDirection;
		CameraDirection cameraDirection=CameraDirection::Front;

		std::shared_ptr<RelativeTimer> timer;
		std::shared_ptr<RelativeTimer> fallTimer;

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
		std::weak_ptr<GameObject> GetRightBlock(Vector3 mapPos);
		std::weak_ptr<GameObject> GetLeftBlock(Vector3 mapPos);
		std::weak_ptr<GameObject> GetUpBlock(Vector3 mapPos);
		std::weak_ptr<GameObject> GetDownBlock(Vector3 mapPos);
		std::weak_ptr<GameObject> GetFrontBlock(Vector3 mapPos);
		std::weak_ptr<GameObject> GetBackBlock(Vector3 mapPos);


		SoundTag moveSounds[3];
		void Fall();
		MoveDirection CheckMoveDirection(Vector3 movePos);
		void CheckExistUnderBlock(Vector3 movePos);
	};

}

BUTI_REGIST_BEHAVIOR(ButiEngine::PlayerBehavior);