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

		static std::int8_t GetStageNum() { return m_stageNum; }
		static std::string GetNextSceneName();
		static void SetMaxStageNum();
		static std::uint16_t GetMaxStageNum() { return m_maxStageNum; }

		static std::int8_t m_stageNum;
	private:
		Value_ptr<RelativeTimer> m_vlp_animTimer;
		Value_weak_ptr<GameObject> m_vwp_stageNumber;

		static std::uint16_t m_maxStageNum;

		bool m_isEnd;
		std::uint16_t m_endTimer;

		float m_stageNumberObjectScale;

		std::uint16_t m_pushCount;

		void RestartAnimTimer();
		void OnPushRight();
		void OnPushLeft();
	};

}
BUTI_REGIST_GAMECOMPONENT(StageSelectManager, true)