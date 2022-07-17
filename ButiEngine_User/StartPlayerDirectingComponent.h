#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class StartPlayerDirectingComponent :public GameComponent
	{
	public:
		StartPlayerDirectingComponent() {}

		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		std::string GetGameComponentName()override {
			return "StartPlayerDirectingComponent";
		}
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
		void OnShowUI();

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
BUTI_REGIST_GAMECOMPONENT(ButiEngine::StartPlayerDirectingComponent)