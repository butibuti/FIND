#include "ShaderChecker.h"

void ButiEngine::ShaderChecker::OnUpdate()
{

    if (!isCameraControll&&vlp_gameObject)  {

        if (GameDevice::GetInput()->CheckKey(Keys::A)) {
            vlp_gameObject->transform->RollWorldRotationY_Degrees(1.0f);
        }
        if (GameDevice::GetInput()->CheckKey(Keys::D)) {
            vlp_gameObject->transform->RollWorldRotationY_Degrees(-1.0f);
        }
        if (GameDevice::GetInput()->CheckKey(Keys::W)) {
            vlp_gameObject->transform->RollLocalRotationX_Degrees(1.0f);
        }
        if (GameDevice::GetInput()->CheckKey(Keys::S)) {
            vlp_gameObject->transform->RollLocalRotationX_Degrees(-1.0f);
        }
    }

    if (GameDevice::GetInput()->TriggerKey(Keys::Space)&&changeSceneName.size()) {

        auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
        std::string sceneName = changeSceneName;
        sceneManager->RemoveScene(sceneName);
        sceneManager->LoadScene(sceneName);
        sceneManager->ChangeScene(sceneName);
    }
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::ShaderChecker::Clone()
{
    return ObjectFactory::Create<ShaderChecker>();
}

void ButiEngine::ShaderChecker::ShowGUI()
{
    GUI::Begin(u8"�������");
    GUI::Bullet();
    if (isCameraControll||!vlp_gameObject) {
        GUI::BulletText(u8"�J����FPS�ړ�,F�Ń}�E�X�Œ�/�Œ����");
    }
    else {
        GUI::BulletText(u8"WASD�Ń��f����]");
    }

    if(vlp_gameObject)
    if (!isCameraControll) {
        /*if (GUI::Button(u8"�J�����ړ�")) {
            isCameraControll = true;
            vlp_camera->GetGameComponent<FPSViewBehavior>()->SetIsActive(true);
        }*/
        GUI::Bullet();
        GUI::SameLine();
        if (GUI::Button(u8"��]���Z�b�g")) {
            vlp_gameObject->transform->RollIdentity();
        }
    }
    else {

        if (GUI::Button(u8"���f����]")) {
            isCameraControll = false;
            vlp_camera->GetGameComponent<FPSViewBehavior>()->SetIsActive(false);
        }
    }

    if (changeSceneName.size()) {

        GUI::BulletText(u8"�X�y�[�X�L�[�ŃV�F�[�_��؂�ւ����V�[����");
    }
    GUI::End();

}

void ButiEngine::ShaderChecker::OnShowUI()
{
    static char input[128];
    GUI::InputTextWithHint("##",changeSceneName, input, 128);
    if (GUI::Button("changeScene")) {
        changeSceneName = input;
    }
}

void ButiEngine::ShaderChecker::Start()
{
    vlp_gameObject = GetManager().lock()->GetGameObject("Bunny").lock();
    vlp_camera = GetManager().lock()->GetGameObject("Camera").lock();
    RegistGUI(); 
}
