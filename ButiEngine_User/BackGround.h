#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	struct FloatBlockData {
		float time;
		std::int16_t direction=1;
		std::int16_t moveDir=1;
		float movePase = 1.0;
		float moveTime = 0.0;
		float moveMax;
		Vector3 initPos;
		Vector3 initScale;
	};

	class BackGround :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "BackGround";
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
			archive(m_pase);
			archive(m_endColor);
			archive(m_startColor);
		}

		void SetIsClear(const bool arg_isClear);
		void Brast();
		void SetStartColor(const Vector4& arg_color);
		void SetEndColor(const Vector4 & arg_color);
	private:
		Vector3 m_center;
		float m_pase;
		Vector4 m_endColor;
		Vector4 m_startColor;
		float m_scale = 0.0f;
		bool m_isClear = false;
		bool m_isBrast = false;

		std::vector<Value_ptr<Transform>> m_vec_vlp_transform;
		std::vector<Value_weak_ptr<GameObject>> m_vec_vwp_gameObjects;
		std::vector<FloatBlockData> m_vec_blockData;
		std::vector<Value_ptr<ButiRendering::CBuffer<ButiRendering::ObjectInformation>>>m_vec_vlp_objectInformation;
	};

}
BUTI_REGIST_GAMECOMPONENT(BackGround, true)