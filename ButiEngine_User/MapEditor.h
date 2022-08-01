#ifndef MAPEDITOR
#include"Map.h"
#include"Player.h"
namespace ButiEngine {
class MapEditor :public GameComponent{
public:
	MapEditor() {}
	static std::int32_t GetEditMapIndex();
	void OnUpdate()override;
	void ShowGUI()override;
	void OnSet()override;
	void Start()override;
	void OnShowUI()override;
	std::string GetGameComponentName()const override {
		return "MapEditor";
	}
	Value_ptr<GameComponent> Clone()override;
	void RollCameraDirection(const std::uint16_t arg_rotateDir);
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(isActive);
	}
	enum class EditorMode :std::int8_t {
		Controll = 0, Cursor = 1, AnotherCursor = 2, StageSize = 3, None = 4
	};
	enum class BlockMode :std::int8_t {
		Block = 0, Invisible = 1, Glass = 2, Goal = 3, Player = 4, Stage = 5, None = 6, Min = -1
	};
	EditorMode GetEditorMode()const;
	BlockMode GetBlockMode()const;
private:
	void ClampCursorPos();
	void Reload();
	void Save();
	Value_ptr<GameObject> Replace(const Vector3& arg_pos, const std::string& arg_blockName, const int blockNum);
	Vector3 m_offset;
	Value_ptr<MapData> m_vlp_currentEdit;
	CameraDirection m_cameraDirection = CameraDirection::Front;
	Value_weak_ptr<Map> m_vwp_map;
	Value_weak_ptr<MeshDrawComponent> m_vwp_cursor_one, m_vwp_cursor_two;
	std::vector<std::vector<std::vector<Value_weak_ptr<GameObject>>>>* m_p_vec_gameObjects;
	Value_ptr<RelativeTimer> m_vlp_inputTimer;
};
}

BUTI_REGIST_GAMECOMPONENT(MapEditor, true)
#endif // !MAPEDITOR
