#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	enum class NextStageBlockStatus
	{
		InActive,
		Active,
		Cleared,
	};

	class NextStageBlock :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "NextStageBlock";
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
		bool IsActive() { return m_isActive; }
		void SetStageNum(const std::uint8_t arg_stageNum) { m_stageNum = arg_stageNum; }
		void AppearPreview();
		void DisappearPreview();

		void Seen();

		static void InitializeStatus();
		static void SetStatus(const std::uint16_t arg_index, const NextStageBlockStatus arg_status) { m_vec_statuss[arg_index] = arg_status; }
		static bool IsAllCleard();
	private:
		static std::vector<NextStageBlockStatus> m_vec_statuss;

		Value_weak_ptr<GameObject> m_vwp_stagePreviewParent;

		std::uint8_t m_stageNum;
		bool m_isActive;
	};

}
BUTI_REGIST_GAMECOMPONENT(NextStageBlock, true)