#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class MapData
	{
	public:
		MapData(){}
		MapData(std::uint8_t arg_stageNum);
		Vector3 GetSize() {
			return Vector3(m_vec_mapDatas[0][0].size(),m_vec_mapDatas.size(),m_vec_mapDatas[0].size());
		}
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(m_vec_mapDatas);
		}
		std::vector<std::vector<std::vector<std::uint8_t>>> m_vec_mapDatas;
	};

	class Map :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "Map";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
			archive(m_vec_vlp_mapDatas);
		}

		void PutBlock(std::uint8_t arg_stageNum);
		Value_weak_ptr<MapData>GetCurrentMapData() { return m_vlp_currentMapData; }
		MapData& GetMapData(const std::uint8_t arg_index) { return *m_vec_vlp_mapDatas[arg_index]; }

		std::vector<std::vector<std::vector<Value_weak_ptr<GameObject>>>>& GetMapObjectData() { return m_vec_vwp_mapObjectDatas; }
		Vector3 GetPlayerPos() { return m_playerPos; }
		void ChangeBlock(const Vector3& arg_mapPos, std::uint8_t arg_mapChipNum);
		const Vector4& GetMapEndColor() { return m_mapEndColor; }
		const Vector4& GetMapStartColor() { return m_mapStartColor; }
		void SetMapEndColor(const Vector4& arg_color) { m_mapEndColor = arg_color; }
		void SetMapStartColor(const Vector4& arg_color) { m_mapStartColor = arg_color; }
		void SetMapInvEndColor(const Vector4& arg_color) { m_mapEndColor_inv = arg_color; }
		void SetMapInvStartColor(const Vector4& arg_color) { m_mapStartColor_inv = arg_color; }

		void ShakeStart(float arg_amplitude);
		void ShakeStop();
	private:
		std::vector<Value_ptr<MapData>> m_vec_vlp_mapDatas;
		Value_ptr<MapData> m_vlp_currentMapData;
		std::vector<std::vector<float>> m_vec_randomBlockPoss;
		std::vector<std::vector<std::vector<Value_weak_ptr<GameObject>>>> m_vec_vwp_mapObjectDatas;
		Value_ptr<RelativeTimer> m_vlp_stageEndTimer;
		
		Vector3 m_playerPos;
		Vector4 m_mapEndColor = Vector4(0.2f, 0.2f, 0.9f, 1.0f);
		Vector4 m_mapStartColor = Vector4(0.4f, 0.4f, 0.7f, 1.0f);
		Vector4 m_mapEndColor_inv = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
		Vector4 m_mapStartColor_inv = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
		std::uint8_t m_currentStageNum;

		void DestoroyMapChip();
		void CreateRandom();
		void AddTransformAnimation(Value_weak_ptr<GameObject> arg_gameObject, float arg_y);
	};
}
BUTI_REGIST_GAMECOMPONENT(Map, true)