#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	struct FloatBlockData {
		float time;
		int direction=1;
		int moveDir=1;
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
			archive(pase);
			archive(endColor);
			archive(startColor);
		}

		void SetIsClear(const bool arg_isClear);
		void Brast();
		void SetStartColor(const Vector4& arg_color);
		void SetEndColor(const Vector4 & arg_color);
	private:
		Vector3 center;
		float pase;
		Vector4 endColor;
		Vector4 startColor;
		float scale = 0.0f;
		bool isClear = false;
		bool isBrast = false;

		//std::vector<std::shared_ptr<Transform>> vec_shp_transform;
		//std::vector<std::shared_ptr<GameObject>> vec_gameObjects;
		std::vector<FloatBlockData> vec_blockData;
		//std::vector<std::shared_ptr<CBuffer<LightVariable>>>vec_shp_lightBuffer;
	};

}
BUTI_REGIST_GAMECOMPONENT(BackGround, true)