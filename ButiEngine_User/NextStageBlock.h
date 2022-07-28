#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

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

		static void InitializeIsActives();
	private:
		static std::vector<bool> m_vec_isActives;

		Value_weak_ptr<GameObject> m_vwp_stagePreviewParent;

		std::uint8_t m_stageNum;
		bool m_isActive;
	};

}
BUTI_REGIST_GAMECOMPONENT(NextStageBlock, true)