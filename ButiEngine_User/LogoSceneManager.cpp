
#include "LogoSceneManager.h"
#include"Header/ApplicationCreater.h"
void ButiEngine::LogoSceneManager::OnUpdate()
{
   
    auto app = GetManager().lock()->GetApplication().lock();

    if (vlp_transTimer->Update()) {
        isTrans = true;
        vlp_transTimer->Stop();
        
    }
    if (isTrans) {
        app->GetResourceContainer()->WaitLoading();
        //app->GetGraphicDevice()->SetClearColor(Vector4((255.0f / 255.0f), (254.0f / 255.0f), (250.0f / 255.0f), 1.0f));
        app->GetSceneManager()->LoadScene(gameObject.lock()->GetApplication().lock()->GetAppInitData()->initSceneName);
        app->GetSceneManager()->ChangeScene(gameObject.lock()->GetApplication().lock()->GetAppInitData()->initSceneName);
    }
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::LogoSceneManager::Clone()
{
    auto output = ObjectFactory::Create<LogoSceneManager>();
    if (vlp_timer) {
        output->vlp_timer = vlp_timer->Clone();
    }if (vlp_transTimer) {
        output->vlp_transTimer = vlp_transTimer->Clone();
    }
    return output;
}

void ButiEngine::LogoSceneManager::OnShowUI()
{
    if (GUI::TreeNode("Timer")) {
        vlp_timer->ShowGUI();
        GUI::TreePop();
    }
    if (GUI::TreeNode("TransitionTimer")) {
        vlp_transTimer->ShowGUI();
        GUI::TreePop();
    }
    GUI::BulletText("logoSpeed");
    GUI::DragFloat("##logoapp", logoApperanceSpeed, 0.001f);
}

void ButiEngine::LogoSceneManager::Start()
{
    vlp_logoBuffer = gameObject.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>();
    vlp_teamBuffer = GetManager().lock()->GetGameObject("TeamLogo").lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>();



    GetManager().lock()->AddObjectFromCereal("LoadAnim", ObjectFactory::Create<Transform>(Vector3(0, 0, 0.1), Vector3Const::Zero, Vector3(0, 0, 1)));

    vlp_transTimer->Start();
    auto app = GetManager().lock()->GetApplication().lock();
    app->InitLoadResources_async();
    
}

void ButiEngine::LogoSceneManager::OnSet()
{
    if (!vlp_timer) {
        vlp_timer = ObjectFactory::Create<RelativeTimer>(60.0f);
    }
    if (!vlp_transTimer) {
        vlp_transTimer = ObjectFactory::Create<RelativeTimer>(60.0f);
    }
}
