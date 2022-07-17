#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class StageSelectManagerComponent :public GameComponent
	{
	public:
		static int stageNum;
		StageSelectManagerComponent() {}

		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		std::string GetGameComponentName()override {
			return "StageSelectManagerComponent";
		}
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
		void OnShowUI();

		static int GetStageNum() { return stageNum; }
		static std::string GetNextSceneName();
		static void SetMaxStageNum();
	private:
		std::shared_ptr<RelativeTimer> animTimer;
		std::weak_ptr<GameObject> obj_stageNumber;

		static int maxStageNum;

		bool end;
		unsigned int endTimer;

		float stageNumberObjectScale;

		int pushCount;

		void RestartAnimTimer();
		void OnPushRight();
		void OnPushLeft();
	};

}
BUTI_REGIST_GAMECOMPONENT(ButiEngine::StageSelectManagerComponent)