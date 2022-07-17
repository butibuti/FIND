#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class StartPlayerDirecting :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "StartPlayerDirecting";
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

		bool IsStart() { return start; }
		void SetSpawnPos(Vector3 v) { spawnPos = v; }
		Vector3 GetSpawnPos() { return spawnPos; }
		void SetStartPos(Vector3 v) { startPos = v; }

		void Animation();
	private:
		std::shared_ptr<RelativeTimer> timer;
		bool start;
		bool fallStart;

		Vector3 spawnPos;
		Vector3 startPos;
	};

}
BUTI_REGIST_GAMECOMPONENT(StartPlayerDirecting, true)