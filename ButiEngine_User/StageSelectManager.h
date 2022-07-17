#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class StageSelectManager :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "StageSelectManager";
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

		static int GetStageNum() { return stageNum; }
		static std::string GetNextSceneName();
		static void SetMaxStageNum();

		static int stageNum;
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
BUTI_REGIST_GAMECOMPONENT(StageSelectManager, true)