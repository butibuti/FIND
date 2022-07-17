#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class MapData
	{
	public:
		MapData(){}
		Vector3 GetSize() {
			return Vector3(mapData[0][0].size(),mapData.size(),mapData[0].size());
		}
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(mapData);
		}
		std::vector<std::vector<std::vector<int>>> mapData;
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
			archive(vec_mapData);
		}

		void PutBlock(int stageNum);
		std::shared_ptr< MapData >GetCurrentMapData() { return currentMapData; }
		MapData& GetMapData(const unsigned int index) { return vec_mapData[index]; }

		std::vector<std::vector<std::vector<std::shared_ptr<GameObject>>>>& GetMapObjectData() { return mapObjectData; }
		Vector3 GetPlayerPos() { return playerPos; }
		void ChangeBlock(Vector3 mapPos, int mapChipNum);
		const Vector4& GetMapEndColor() { return mapEndColor; }
		const Vector4& GetMapStartColor() { return mapStartColor; }
		void SetMapEndColor(const Vector4& arg_color) { mapEndColor = arg_color; }
		void SetMapStartColor(const Vector4& arg_color) { mapStartColor = arg_color; }
		void SetMapInvEndColor(const Vector4& arg_color) { mapEndColor_inv = arg_color; }
		void SetMapInvStartColor(const Vector4& arg_color) { mapStartColor_inv = arg_color; }

		void ShakeStart(float arg_amplitude);
		void ShakeStop();
	private:
		std::vector<MapData> vec_mapData;
		std::shared_ptr< MapData > currentMapData;
		std::vector<std::vector<float>> randomBlockPos;
		std::vector<std::vector<std::vector<std::shared_ptr<GameObject>>>> mapObjectData;
		std::shared_ptr<RelativeTimer> stageEndTimer;
		
		Vector3 playerPos;
		Vector4 mapEndColor = Vector4(0.2f, 0.2f, 0.9f, 1.0f);
		Vector4 mapStartColor = Vector4(0.4f, 0.4f, 0.7f, 1.0f);
		Vector4 mapEndColor_inv = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
		Vector4 mapStartColor_inv = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
		int currentStageNum;

		void DestoroyMapChip();
		void CreateRandom();
		void AddTransformAnimation(std::weak_ptr<ButiEngine::GameObject> gameObject, float y);
	};

	void OutputCereal(const std::shared_ptr<MapData>& v, const std::string& path);

	void InputCereal(std::shared_ptr<MapData>& v, const std::string& path);
}
BUTI_REGIST_GAMECOMPONENT(Map, true)