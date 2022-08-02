#include"stdafx.h"
#include"MapEditor.h"
#include"GameSettings.h"
#include"InvisibleBlock.h"
#include "InputManager.h"
#include "NextStageBlock.h"
#include "Player.h"
#include"StartPlayerDirecting.h"
std::int32_t g_mapIndex = 0, g_cursorPos[3] = {0,0,0}, g_cursorPos_another[3] = { 0,0,0 }, g_currentBlockIndex = 0, g_currentPlayerDirection = 0
, g_invisibleID=0, g_goalMode = 0, g_stageBlockIndex = 0;

enum class InputDir :std::int8_t {
    None = 0, Front= 1, Back= 2, Right= 3, Left= 4, Up= 5, Down= 6
};
ButiEngine::MapEditor::EditorMode g_editMode = ButiEngine::MapEditor::EditorMode::Controll;
ButiEngine::MapEditor::BlockMode g_putMode = ButiEngine::MapEditor::BlockMode::Block;
std::int32_t ButiEngine::MapEditor::GetEditMapIndex()
{
    return g_mapIndex;
}
void PushPutModeNotification(){
    std::string message;
    switch (g_putMode)
    {
    case ButiEngine::MapEditor::BlockMode::Block:message = u8"ブロック配置"; break;
    case ButiEngine::MapEditor::BlockMode::Invisible:message = u8"インビジブルブロック配置"; break;
    case ButiEngine::MapEditor::BlockMode::Glass:message = u8"ガラスブロック配置"; break;
    case ButiEngine::MapEditor::BlockMode::Goal:message = u8"ゴール配置"; break;
    case ButiEngine::MapEditor::BlockMode::Player:message = u8"プレイヤー位置変更"; break;
    case ButiEngine::MapEditor::BlockMode::Stage:message = u8"ステージ選択ブロック配置"; break;
    case ButiEngine::MapEditor::BlockMode::AlreadySeenStage:message = u8"既に見えてるステージ選択ブロック配置"; break;
    }
    ButiEngine::GUI::PushNotification(message);
}
void ButiEngine::MapEditor::OnUpdate()
{
    auto player = GetManager().lock()->GetGameObject("Player");
    if (!player.lock() || player.lock()->GetGameComponent<StartPlayerDirecting>()->IsAnimation()) {
        return;
    }
    GetManager().lock()->GetGameObject("Player").lock()->GetGameComponent<Player>()->SetIsActive(g_editMode == EditorMode::Controll);
    m_vwp_cursor_one.lock()->GetTransform()->SetLocalPosition(Vector3(g_cursorPos[0], g_cursorPos[1], g_cursorPos[2])-m_offset);
    m_vwp_cursor_two.lock()->GetTransform()->SetLocalPosition(Vector3(g_cursorPos_another[0], g_cursorPos_another[1], g_cursorPos_another[2]) - m_offset);

    if (GameDevice::GetInput()->GetPadButtonTrigger(PadButtons::XBOX_START)) {
        g_editMode= static_cast<EditorMode>(static_cast<std::int8_t>(g_editMode) + 1);
        if (g_editMode == EditorMode::None) {
            g_editMode = EditorMode::Controll;
        }
        if (g_editMode == EditorMode::AnotherCursor) {
            m_vwp_cursor_two.lock()->Regist();
        }
        else {
            m_vwp_cursor_two.lock()->UnRegist();
        }

        if (g_editMode == EditorMode::StageSize || g_editMode == EditorMode::Controll) {
            m_vwp_cursor_one.lock()->UnRegist();
        }
        else {
            m_vwp_cursor_one.lock()->Regist();
        }
        std::string mode;
        switch (g_editMode)
        {
        case  EditorMode::Controll:mode = u8"プレイヤー操作"; break;
        case  EditorMode::Cursor:mode = u8"カーソル操作"; break;
        case  EditorMode::AnotherCursor:mode = u8"カーソルその2操作"; break;
        case  EditorMode::StageSize:mode = u8"ステージ操作"; break;
        }
        GUI::PushNotification(mode);

    }

    if ((g_editMode == EditorMode::Controll)) { return; }

    InputDir inputDir = InputDir::None;
    auto trigger = GameDevice::GetInput()->GetRightTrigger();
    bool isDirInputed = false;

    bool isChanged = false;
    if (g_putMode == BlockMode::Player) {
        if (GameDevice::GetInput()->GetPadButtonTrigger(PadButtons::XBOX_LEFT)) {
            g_currentPlayerDirection--;
            if (g_currentPlayerDirection < 0) {
                g_currentPlayerDirection = 7;
            }isChanged = true;
        }
        if (GameDevice::GetInput()->GetPadButtonTrigger(PadButtons::XBOX_RIGHT)) {
            g_currentPlayerDirection++;
            if (g_currentPlayerDirection > 7) {
                g_currentPlayerDirection = 0;
            }isChanged = true;
        }
        if (isChanged) {
            const char* ary_playerDir[8] = {"Front","Right","Back","Left","Up","Up_rotate","Down","Down_rotate"};
            
            GUI::PushNotification(Util::ToUTF8( std::string("Player:")+ary_playerDir[g_currentPlayerDirection]+"   "));
            return;
        }
    }
    else if (g_putMode == BlockMode::Goal) {
        if (GameDevice::GetInput()->GetPadButtonTrigger(PadButtons::XBOX_LEFT)) {
            isChanged = true;
            if (g_goalMode < 0) {
                g_goalMode = 2;
            }
        }
        if (GameDevice::GetInput()->GetPadButtonTrigger(PadButtons::XBOX_RIGHT)) {
            g_goalMode++;
            if (g_goalMode > 2) {
                g_goalMode = 0;
            }isChanged = true;
        }
        if (isChanged) {
            switch (g_goalMode)
            {
            case 0:
                GUI::PushNotification("Touch Only");
                break;
            case 1:
                GUI::PushNotification("Already Seen");
                break;
            case 2:
                GUI::PushNotification("Hidden");
                break;
            default:
                break;
            }
            return;
        }
    }
    else if (g_putMode == BlockMode::Stage|| g_putMode == BlockMode::AlreadySeenStage) {
        if (GameDevice::GetInput()->GetPadButtonTrigger(PadButtons::XBOX_LEFT)) {
            g_stageBlockIndex--;
            isChanged = true;
        }
        if (GameDevice::GetInput()->GetPadButtonTrigger(PadButtons::XBOX_RIGHT)) {
            g_stageBlockIndex++;
            isChanged = true;
        }
        if (isChanged) {
            g_stageBlockIndex = max(g_stageBlockIndex, 0);
            GUI::PushNotification(Util::ToUTF8("ステージ移動ブロックのID" + std::to_string(g_stageBlockIndex)));
            return;
        }
    }
    else if (g_putMode == BlockMode::Invisible) {
        if (GameDevice::GetInput()->GetPadButtonTrigger(PadButtons::XBOX_LEFT)) {
            g_invisibleID--;
            isChanged = true;
        }
        if (GameDevice::GetInput()->GetPadButtonTrigger(PadButtons::XBOX_RIGHT)) {
            g_invisibleID++;
            isChanged = true;
        }
        if (isChanged) {
            g_invisibleID = max(min(g_invisibleID, 31), 0);
            GUI::PushNotification(Util::ToUTF8("インビジブルブロックのID" + std::to_string(g_invisibleID)));
            return;
        }

    }
    if (!isChanged) {
        {
            if (GameDevice::GetInput()->GetLeftStick().x>0.25f) {
                isDirInputed = true;
                if (!m_vlp_inputTimer->IsOn()) {
                    switch (m_cameraDirection)
                    {
                    case CameraDirection::Front:
                    {
                        inputDir = InputDir::Right;
                    }
                    break;
                    case CameraDirection::Right:
                    {
                        inputDir = InputDir::Front;
                    }
                    break;
                    case CameraDirection::Back:
                    {
                        inputDir = InputDir::Left;
                    }
                    break;
                    case CameraDirection::Left:
                    {
                        inputDir = InputDir::Back;
                    }
                    break;
                    }
                    m_vlp_inputTimer->Start();
                }
            }
            else if (GameDevice::GetInput()->GetLeftStick().x < -0.25f) {
                isDirInputed = true;
                if (!m_vlp_inputTimer->IsOn()) {
                    switch (m_cameraDirection)
                    {
                    case CameraDirection::Front:
                    {
                        inputDir = InputDir::Left;
                    }
                    break;
                    case CameraDirection::Right:
                    {
                        inputDir = InputDir::Back;
                    }
                    break;
                    case CameraDirection::Back:
                    {
                        inputDir = InputDir::Right;
                    }
                    break;
                    case CameraDirection::Left:
                    {
                        inputDir = InputDir::Front;
                    }
                    break;
                    }
                    m_vlp_inputTimer->Start();
                }
            }
            else if (GameDevice::GetInput()->GetLeftStick().y > 0.25f && trigger > 0.1f) {
                isDirInputed = true;
                if (!m_vlp_inputTimer->IsOn()) {
                    inputDir = InputDir::Up;
                    m_vlp_inputTimer->Start();
                }
            }
            else if (GameDevice::GetInput()->GetLeftStick().y <- 0.25f && trigger > 0.1f) {
                isDirInputed = true;
                if (!m_vlp_inputTimer->IsOn()) {
                    inputDir = InputDir::Down;
                    m_vlp_inputTimer->Start();
                }
            }
            else if (GameDevice::GetInput()->GetLeftStick().y > 0.25f) {
                isDirInputed = true;
                if (!m_vlp_inputTimer->IsOn()) {
                    switch (m_cameraDirection)
                    {
                    case CameraDirection::Front:
                    {
                        inputDir = InputDir::Front;
                    }
                    break;
                    case CameraDirection::Right:
                    {
                        inputDir = InputDir::Left;
                    }
                    break;
                    case CameraDirection::Back:
                    {
                        inputDir = InputDir::Back;
                    }
                    break;
                    case CameraDirection::Left:
                    {
                        inputDir = InputDir::Right;
                    }
                    break;
                    default:
                        break;
                    }
                    m_vlp_inputTimer->Start();
                }
            }
            else if (GameDevice::GetInput()->GetLeftStick().y <-0.25f) {
                isDirInputed = true;
                if (!m_vlp_inputTimer->IsOn()) {
                    switch (m_cameraDirection)
                    {
                    case CameraDirection::Front:
                    {
                        inputDir = InputDir::Back;
                    }
                    break;
                    case CameraDirection::Right:
                    {
                        inputDir = InputDir::Right;
                    }
                    break;
                    case CameraDirection::Back:
                    {
                        inputDir = InputDir::Front;
                    }
                    break;
                    case CameraDirection::Left:
                    {
                        inputDir = InputDir::Left;
                    }
                    break;
                    default:
                        break;
                    }
                    m_vlp_inputTimer->Start();
                }
            }
        }
        if (!isDirInputed) {
            m_vlp_inputTimer->Stop();
            m_vlp_inputTimer->Reset();
        }
        else {
            if (m_vlp_inputTimer->Update()) {
                m_vlp_inputTimer->Stop();
            }
        }
    }


    if (g_editMode == EditorMode::StageSize) {
        switch (inputDir)
        {
        case InputDir::Right:
            for (std::int32_t i = 0; i < m_vlp_currentEdit->m_vec_mapDatas.size(); i++) {
                for (std::int32_t j = 0; j < m_vlp_currentEdit->m_vec_mapDatas[0].size(); j++) {
                    m_vlp_currentEdit->m_vec_mapDatas[i][j].push_back(0);
                }
            }
            Reload();
            break;
        case InputDir::Left:
            if (m_vwp_map.lock()->GetStartPlayerPos().Round().x == m_vlp_currentEdit->m_vec_mapDatas[0][0].size() - 1)
            {
                GUI::PushNotification(u8"プレイヤーがいる列です");
                break;
            }
            for (std::int32_t i = 0; i < m_vlp_currentEdit->m_vec_mapDatas.size(); i++) {
                for (std::int32_t j = 0; j < m_vlp_currentEdit->m_vec_mapDatas[0].size(); j++) {
                    m_vlp_currentEdit->m_vec_mapDatas[i][j].pop_back();
                }
            }
            Reload();
            break;
        case InputDir::Front:
        {
            for (std::int32_t i = 0; i < m_vlp_currentEdit->m_vec_mapDatas.size(); i++) {
                auto xsize = m_vlp_currentEdit->m_vec_mapDatas[0][0].size();
                m_vlp_currentEdit->m_vec_mapDatas[i].push_back(std::vector<std::int32_t>());
                (m_vlp_currentEdit->m_vec_mapDatas[i].end() - 1)->resize(xsize);
                ((*m_p_vec_gameObjects)[i].end() - 1)->resize(xsize);
            }
            Reload();
        }
        break;
        case InputDir::Back:
            if (m_vwp_map.lock()->GetStartPlayerPos().Round().z == m_vlp_currentEdit->m_vec_mapDatas[0].size() - 1)
            {
                GUI::PushNotification(u8"プレイヤーがいる行です");
                break;
            }
            for (std::int32_t i = 0; i < m_vlp_currentEdit->m_vec_mapDatas.size(); i++) {
                m_vlp_currentEdit->m_vec_mapDatas[i].pop_back();
            }
            Reload();
            break;
        case InputDir::Up:
        {

            auto zsize = m_vlp_currentEdit->m_vec_mapDatas[0].size();
            auto xsize = m_vlp_currentEdit->m_vec_mapDatas[0][0].size();
            m_vlp_currentEdit->m_vec_mapDatas.push_back(std::vector<std::vector<std::int32_t>>());
            (m_vlp_currentEdit->m_vec_mapDatas.end() - 1)->resize(zsize);
            for (std::int32_t i = 0; i < m_vlp_currentEdit->m_vec_mapDatas[0].size(); i++) {
                (m_vlp_currentEdit->m_vec_mapDatas[m_vlp_currentEdit->m_vec_mapDatas.size() - 1][i]).resize(xsize);
            }
            Reload();

        }   break;
        case InputDir::Down:
            if (m_vwp_map.lock()->GetStartPlayerPos().Round().y == m_vlp_currentEdit->m_vec_mapDatas.size() - 1)
            {
                GUI::PushNotification(u8"プレイヤーがいる面です");
                break;
            }
            m_vlp_currentEdit->m_vec_mapDatas.pop_back();
            Reload();
            break;
        }
        return;
    }

    int32_t(& p_selectCursor)[3]= g_editMode == EditorMode::AnotherCursor? g_cursorPos_another:g_cursorPos;
    switch (inputDir)
    {
    case InputDir::Right:
        (p_selectCursor)[0] += 1;
        break;
    case InputDir::Left:
        (p_selectCursor)[0] -= 1;
        break;
    case InputDir::Front:
        (p_selectCursor)[2] += 1;
        break;
    case InputDir::Back:
        (p_selectCursor)[2] -= 1;
        break;
    case InputDir::Up:
        (p_selectCursor)[1] += 1;
        break;
    case InputDir::Down:
        (p_selectCursor)[1] -= 1;
        break;
    }
    if (inputDir != InputDir::None) {ClampCursorPos();}

    if (GameDevice::GetInput()->GetPadButtonTrigger(PadButtons::XBOX_A)) {
        bool isReload = false;
        if (g_editMode==EditorMode::AnotherCursor) {
            Vector3 min = Vector3(g_cursorPos[0], g_cursorPos[1], g_cursorPos[2]).Min(Vector3(g_cursorPos_another[0], g_cursorPos_another[1], g_cursorPos_another[2]));
            Vector3 max = Vector3(g_cursorPos[0], g_cursorPos[1], g_cursorPos[2]).Max(Vector3(g_cursorPos_another[0], g_cursorPos_another[1], g_cursorPos_another[2]));
            for (std::int32_t y = min.y; y <= max.y; y++) {
                for (std::int32_t z = min.z; z <= max.z; z++) {
                    for (std::int32_t x = min.x; x <= max.x; x++) {
                        auto pos = Vector3(y, z, x);
                        switch (g_putMode)
                        {
                        case BlockMode::Block: Replace(pos, "Block", GameSettings::MAP_CHIP_BLOCK); break;
                        case BlockMode::Invisible: {
                            auto gameObject = Replace(pos, "InvisibleBlock", GameSettings::MAP_CHIP_INVISIBLEBLOCK + g_invisibleID);
                            gameObject->GetGameComponent<InvisibleBlock>()->SetID(g_invisibleID);
                            gameObject->GetGameComponent<InvisibleBlock>()->SetMapPos(Vector3(g_cursorPos[0], g_cursorPos[1], g_cursorPos[2]));

                        }break;
                        case BlockMode::Glass: Replace(pos, "Glass", GameSettings::MAP_CHIP_GLASS); break;
                        }
                       
                    }
                }
            }
        }
        else {
            switch (g_putMode)
            {
            case BlockMode::Block:Replace(Vector3(g_cursorPos[1], g_cursorPos[2], g_cursorPos[0]), "Block", GameSettings::MAP_CHIP_BLOCK); break;
            case BlockMode::Invisible: {
                auto gameObject = Replace(Vector3(g_cursorPos[1], g_cursorPos[2], g_cursorPos[0]), "InvisibleBlock", GameSettings::MAP_CHIP_INVISIBLEBLOCK + g_invisibleID);
                gameObject->GetGameComponent<InvisibleBlock>()->SetID(g_invisibleID);
                gameObject->GetGameComponent<InvisibleBlock>()->SetMapPos(Vector3(g_cursorPos[0], g_cursorPos[1], g_cursorPos[2]));

            }break;
            case BlockMode::Glass: Replace(Vector3(g_cursorPos[1], g_cursorPos[2], g_cursorPos[0]), "Glass", GameSettings::MAP_CHIP_GLASS); break;
            case BlockMode::Goal: {
                std::string goalObjName;
                switch (g_goalMode)
                {
                case 0: goalObjName = "TutorialGoal"; break;
                case 1: goalObjName = "EasyGoal";  break;
                case 2: goalObjName = "DefaultGoal"; break;
                }
                auto mapNum = m_vlp_currentEdit->m_vec_mapDatas[g_cursorPos[1]][g_cursorPos[2]][g_cursorPos[0]];
                if (mapNum == GameSettings::MAP_CHIP_PLAYER) {
                    Replace(Vector3(g_cursorPos[1], g_cursorPos[2], g_cursorPos[0]), goalObjName, GameSettings::MAP_CHIP_PLAYER_AND_GOAL + (GameSettings::MAP_CHIP_TUTORIALGOAL + g_goalMode) * 10 + 0);
                }
                else if (mapNum >= GameSettings::MAP_CHIP_PLAYER_ROTATE_90 && mapNum <= GameSettings::MAP_CHIP_PLAYER_ROTATE_MIN_90) {
                    Replace(Vector3(g_cursorPos[1], g_cursorPos[2], g_cursorPos[0]), goalObjName, GameSettings::MAP_CHIP_PLAYER_AND_GOAL + (GameSettings::MAP_CHIP_TUTORIALGOAL + g_goalMode) * 10 + (mapNum + 1 - GameSettings::MAP_CHIP_PLAYER_ROTATE_90));
                }
                else if (mapNum >= GameSettings::MAP_CHIP_PLAYER_AND_GOAL) {
                    int dir = (mapNum - GameSettings::MAP_CHIP_PLAYER_AND_GOAL) % 10;
                    Replace(Vector3(g_cursorPos[1], g_cursorPos[2], g_cursorPos[0]), goalObjName, GameSettings::MAP_CHIP_PLAYER_AND_GOAL + (GameSettings::MAP_CHIP_TUTORIALGOAL + g_goalMode) * 10 + dir);
                }
                else {
                    Replace(Vector3(g_cursorPos[1], g_cursorPos[2], g_cursorPos[0]), goalObjName, GameSettings::MAP_CHIP_TUTORIALGOAL + g_goalMode);
                }
            }break;
            case BlockMode::Stage: {
                auto gameObject = Replace(Vector3(g_cursorPos[1], g_cursorPos[2], g_cursorPos[0]), "NextStageBlock", GameSettings::MAP_CHIP_NEXT_STAGE_BLOCK + g_stageBlockIndex);
                gameObject->GetGameComponent<NextStageBlock>()->SetStageNum(g_stageBlockIndex);
            }break;
            case BlockMode::AlreadySeenStage: {
                auto gameObject = Replace(Vector3(g_cursorPos[1], g_cursorPos[2], g_cursorPos[0]), "NextStageBlock", GameSettings::MAP_CHIP_ALREADY_SEEN_NEXT_STAGE_BLOCK + g_stageBlockIndex);
                gameObject->GetGameComponent<NextStageBlock>()->SetStatus(g_stageBlockIndex,NextStageBlockStatus::Active);
                gameObject->GetGameComponent<NextStageBlock>()->SetStageNum(g_stageBlockIndex);
            }break;
            case BlockMode::Player: {
                auto playerInitPos = m_vwp_map.lock()->GetStartPlayerPos().Round();
                if (g_cursorPos[1] - 1<0||
                    (m_vlp_currentEdit->m_vec_mapDatas[g_cursorPos[1] - 1][g_cursorPos[2]][g_cursorPos[0]]!=GameSettings::MAP_CHIP_BLOCK&&
                        m_vlp_currentEdit->m_vec_mapDatas[g_cursorPos[1] - 1][g_cursorPos[2]][g_cursorPos[0]] != GameSettings::MAP_CHIP_GLASS)) {
                    break;
                }
                
                std::int32_t num = m_vlp_currentEdit->m_vec_mapDatas[playerInitPos.y][playerInitPos.z][playerInitPos.x];
                if (num == GameSettings::MAP_CHIP_PLAYER || (num >= GameSettings::MAP_CHIP_PLAYER_ROTATE_90 && num <= GameSettings::MAP_CHIP_PLAYER_DOWN_ROTATE_90)) {
                    m_vlp_currentEdit->m_vec_mapDatas[playerInitPos.y][playerInitPos.z][playerInitPos.x] = 0;
                }
                else if (num > GameSettings::MAP_CHIP_PLAYER_AND_GOAL && num < GameSettings::MAP_CHIP_NEXT_STAGE_BLOCK) {
                    std::int32_t playerGoalID = num - GameSettings::MAP_CHIP_PLAYER_AND_GOAL;
                    std::int32_t goal = playerGoalID / 10;
                    std::string goalObjName;

                    switch (goal)
                    {
                    case  GameSettings::MAP_CHIP_TUTORIALGOAL:
                        goalObjName = "TutorialGoal";
                        break;
                    case  GameSettings::MAP_CHIP_EASYGOAL:
                        goalObjName = "EasyGoal";
                        break;
                    case  GameSettings::MAP_CHIP_DEFAULTGOAL:
                        goalObjName = "DefaultGoal";
                        break;
                    default:
                        break;
                    }
                    Replace(Vector3(playerInitPos.y, playerInitPos.z, playerInitPos.x), goalObjName, goal);
                }


                if (g_currentPlayerDirection == 0) {
                    auto mapNum = m_vlp_currentEdit->m_vec_mapDatas[g_cursorPos[1]][g_cursorPos[2]][g_cursorPos[0]];
                    if (mapNum >= GameSettings::MAP_CHIP_TUTORIALGOAL && mapNum <= GameSettings::MAP_CHIP_DEFAULTGOAL) {
                        std::string goalObjName;
                        switch (mapNum)
                        {
                        case  GameSettings::MAP_CHIP_TUTORIALGOAL:
                            goalObjName = "TutorialGoal";
                            break;
                        case  GameSettings::MAP_CHIP_EASYGOAL:
                            goalObjName = "EasyGoal";
                            break;
                        case  GameSettings::MAP_CHIP_DEFAULTGOAL:
                            goalObjName = "DefaultGoal";
                            break;
                        default:
                            break;
                        }

                        Replace(Vector3(g_cursorPos[1], g_cursorPos[2], g_cursorPos[0]), goalObjName, GameSettings::MAP_CHIP_PLAYER_AND_GOAL + mapNum * 10);
                    }
                    
                    else {
                        m_vlp_currentEdit->m_vec_mapDatas[g_cursorPos[1]][g_cursorPos[2]][g_cursorPos[0]] = GameSettings::MAP_CHIP_PLAYER;
                    }

                }
                else {
                    auto mapNum = m_vlp_currentEdit->m_vec_mapDatas[g_cursorPos[1]][g_cursorPos[2]][g_cursorPos[0]];
                    if ((mapNum >= GameSettings::MAP_CHIP_TUTORIALGOAL && mapNum <= GameSettings::MAP_CHIP_DEFAULTGOAL)) {
                        
                        m_vlp_currentEdit->m_vec_mapDatas[g_cursorPos[1]][g_cursorPos[2]][g_cursorPos[0]] = GameSettings::MAP_CHIP_PLAYER_AND_GOAL + mapNum * 10+ g_currentPlayerDirection;
                    }
                    
                    else {
                        std::int32_t playerNum = GameSettings::MAP_CHIP_PLAYER_ROTATE_90 + g_currentPlayerDirection - 1;

                        m_vlp_currentEdit->m_vec_mapDatas[g_cursorPos[1]][g_cursorPos[2]][g_cursorPos[0]] = playerNum;
                    }

                }
                isReload = true;
            }break;
            }
            
        }
        if (isReload) {
            Reload();
        }
        else {
           Save();
        }
    }
    if (GameDevice::GetInput()->GetPadButtonTrigger(PadButtons::XBOX_B)) {
        if (g_editMode==EditorMode::AnotherCursor) {
            Vector3 min = Vector3(g_cursorPos[0], g_cursorPos[1], g_cursorPos[2]).Min(Vector3(g_cursorPos_another[0], g_cursorPos_another[1], g_cursorPos_another[2]));
            Vector3 max = Vector3(g_cursorPos[0], g_cursorPos[1], g_cursorPos[2]).Max(Vector3(g_cursorPos_another[0], g_cursorPos_another[1], g_cursorPos_another[2]));
            for (int y = min.y; y <= max.y; y++) {
                for (int z = min.z; z <= max.z; z++) {
                    for (int x = min.x; x <= max.x; x++) {
                        if (!(*m_p_vec_gameObjects)[y][z][x].lock()) { continue;}
                        m_vlp_currentEdit->m_vec_mapDatas[y][z][x] = 0;
                        (*m_p_vec_gameObjects)[y][z][x].lock()->SetIsRemove(true);
                        (*m_p_vec_gameObjects)[y][z][x] = nullptr;
                    }
                }
            }
        }
        else {
            if ((*m_p_vec_gameObjects)[g_cursorPos[1]][g_cursorPos[2]][g_cursorPos[0]].lock()) {
                m_vlp_currentEdit->m_vec_mapDatas[g_cursorPos[1]][g_cursorPos[2]][g_cursorPos[0]] = 0;
                (*m_p_vec_gameObjects)[g_cursorPos[1]][g_cursorPos[2]][g_cursorPos[0]].lock()->SetIsRemove(true);
                (*m_p_vec_gameObjects)[g_cursorPos[1]][g_cursorPos[2]][g_cursorPos[0]] = nullptr;
            }
        }
        Save();
    }
    if (GameDevice::GetInput()->GetPadButtonTrigger(PadButtons::XBOX_Y)) {
        g_putMode= static_cast<BlockMode>(static_cast<std::int8_t>(g_putMode) + 1);
        if (g_putMode == BlockMode::None) {
            g_putMode = BlockMode::Block;
        }
        PushPutModeNotification();
    }
    if (GameDevice::GetInput()->GetPadButtonTrigger(PadButtons::XBOX_X)) {
        g_putMode = static_cast<BlockMode>(static_cast<std::int8_t>(g_putMode) - 1);
        if (g_putMode == BlockMode::Min) {
            g_putMode = BlockMode::Stage;
        }
        PushPutModeNotification();
    }
    

}

void ButiEngine::MapEditor::ShowGUI()
{
}

void ButiEngine::MapEditor::OnSet()
{
}

void ButiEngine::MapEditor::Start()
{
    m_vwp_cursor_one = gameObject.lock()->GetGameComponent<MeshDrawComponent>(0);
    m_vwp_cursor_two = gameObject.lock()->GetGameComponent<MeshDrawComponent>(1);
    if (g_editMode != EditorMode::AnotherCursor) {
        m_vwp_cursor_two.lock()->UnRegist();
    }
    if (g_editMode == EditorMode::StageSize|| g_editMode == EditorMode::Controll) {
        m_vwp_cursor_one.lock()->UnRegist();
    }
    m_vwp_map = GetManager().lock()->GetGameObject("Map").lock()->GetGameComponent<Map>();
    
    m_vlp_currentEdit = m_vwp_map.lock()->GetCurrentMapData();
    m_offset = (m_vlp_currentEdit->GetSize() * 0.5f).Floor();
    m_p_vec_gameObjects =& m_vwp_map.lock()->GetMapObjectData();
    m_vlp_inputTimer = ObjectFactory::Create<RelativeTimer>(8);
}

void ButiEngine::MapEditor::OnShowUI()
{
    static std::int32_t selectMapIndex = 0;
    if (!m_vwp_map.lock()) {
        return;
    }
    GUI::Text("MapIndex");
    if (GUI::ArrowButton("##MapIndexmin", GUI::GuiDir_::GuiDir_Left)) {
        if (selectMapIndex > 0) {
            selectMapIndex--;
        }
    }
    GUI::SameLine();
    GUI::Text(std::to_string(selectMapIndex));
    GUI::SameLine();
    if (GUI::ArrowButton("##MapIndexPlus", GUI::GuiDir_::GuiDir_Right)) {
        selectMapIndex++;
    }
    if (GUI::Button("Change")) {
        Save();
        g_mapIndex= selectMapIndex;
        GetManager().lock()->GetScene().lock()->GetSceneManager().lock()->ReloadScene();
    }   

    GUI::BulletText("Stage Size");
    auto size = m_vlp_currentEdit->GetSize();
    GUI::Text(size);
    GUI::BulletText("Cursor Position");
    GUI::Text("X:"+std::to_string(g_cursorPos[0])+"   y:" + std::to_string(g_cursorPos[1]) +"   z:" + std::to_string(g_cursorPos[2]));
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::MapEditor::Clone()
{
	return ObjectFactory::Create<MapEditor>();
}

void ButiEngine::MapEditor::RollCameraDirection(const std::uint16_t arg_rotateDir)
{
    m_cameraDirection = (CameraDirection)((std::uint16_t)m_cameraDirection + arg_rotateDir);

    if (m_cameraDirection > CameraDirection::Right) {
        m_cameraDirection = (CameraDirection)((std::uint16_t)m_cameraDirection % 4);
    }
    else if (m_cameraDirection < CameraDirection::Front) {
        m_cameraDirection = (CameraDirection)((std::uint16_t)m_cameraDirection + 4);
    }
}

ButiEngine::MapEditor::EditorMode ButiEngine::MapEditor::GetEditorMode() const
{
    return g_editMode;
}

ButiEngine::MapEditor::BlockMode ButiEngine::MapEditor::GetBlockMode() const
{
    return g_putMode;
}

void ButiEngine::MapEditor::ClampCursorPos()
{
    g_cursorPos[0] = max(min(g_cursorPos[0], m_vlp_currentEdit->GetSize().x - 1), 0);
    g_cursorPos[1] = max(min(g_cursorPos[1], m_vlp_currentEdit->GetSize().y - 1), 0);
    g_cursorPos[2] = max(min(g_cursorPos[2], m_vlp_currentEdit->GetSize().z - 1), 0);

    g_cursorPos_another[0] = max(min(g_cursorPos_another[0], m_vlp_currentEdit->GetSize().x - 1), 0);
    g_cursorPos_another[1] = max(min(g_cursorPos_another[1], m_vlp_currentEdit->GetSize().y - 1), 0);
    g_cursorPos_another[2] = max(min(g_cursorPos_another[2], m_vlp_currentEdit->GetSize().z - 1), 0);
}

void ButiEngine::MapEditor::Reload()
{
    Save();
    GetManager().lock()->GetScene().lock()->GetSceneManager().lock()->ReloadScene();
}

void ButiEngine::MapEditor::Save()
{
    auto output = ObjectFactory::Create<MapData>();
    output->m_vec_mapDatas = m_vlp_currentEdit->m_vec_mapDatas;
    std::string path = "Scene/Stage_" + std::to_string(g_mapIndex) + "/mapData.map";
    OutputCereal(output, path);
}

ButiEngine::Value_ptr< ButiEngine::GameObject> ButiEngine::MapEditor::Replace(const Vector3& arg_pos, const std::string& arg_blockName, const int blockNum)
{
    Vector3 position = Vector3(arg_pos.z, arg_pos.x, arg_pos.y);
    Vector3 scale(GameSettings::BLOCK_SIZE, GameSettings::BLOCK_SIZE, GameSettings::BLOCK_SIZE);
    position -= m_offset;
    position *= GameSettings::BLOCK_SIZE;
    m_vlp_currentEdit->m_vec_mapDatas[arg_pos.x][arg_pos.y][arg_pos.z] = blockNum;
    auto gameObject = GetManager().lock()->AddObjectFromCereal(arg_blockName);
    gameObject.lock()->transform->SetWorldPosition(position);

    if (m_vwp_map.lock()->GetMapObjectData()[arg_pos.x][arg_pos.y][arg_pos.z].lock()) {

        m_vwp_map.lock()->GetMapObjectData()[arg_pos.x][arg_pos.y][arg_pos.z].lock()->SetIsRemove(true);
    }

    m_vwp_map.lock()->GetMapObjectData()[arg_pos.x][arg_pos.y][arg_pos.z] = gameObject;

    return gameObject.lock();
}
