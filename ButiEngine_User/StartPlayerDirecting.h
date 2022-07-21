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

		bool IsStart() { return m_isStart; }
		void SetSpawnPos(Vector3 v) { m_spawnPos = v; }
		Vector3 GetSpawnPos() { return m_spawnPos; }
		void SetStartPos(Vector3 v) { m_startPos = v; }

		void Animation();
	private:
		Value_ptr<RelativeTimer> m_vlp_timer;
		bool m_isStart;
		bool m_isFallStart;

		Vector3 m_spawnPos;
		Vector3 m_startPos;
	};

}
BUTI_REGIST_GAMECOMPONENT(StartPlayerDirecting, true)