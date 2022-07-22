#include "stdafx_u.h"
#include "BackGround.h"

void ButiEngine::BackGround::OnUpdate()
{
    //auto endItr = vec_blockData.end();

    //int i = 0;
    //if (!isClear && !isBrast) {

    //    for (auto itr = vec_blockData.begin(); itr != endItr; itr++, i++) {
    //        itr->time += pase * itr->direction;
    //        itr->moveTime += itr->movePase * itr->moveDir;

    //        if (itr->time > 1.0f) {
    //            itr->time = 1.0f;
    //            itr->direction = -1;
    //        }
    //        else if (itr->time < 0.0f) {
    //            itr->time = 0.0f;
    //            itr->direction = 1;
    //        }
    //        if (itr->moveTime > 1.0f) {
    //            itr->moveTime = 1.0f;
    //            itr->moveDir = -1;
    //        }
    //        else if (itr->moveTime < 0.0f) {
    //            itr->moveTime = 0.0f;
    //            itr->moveDir = 1;
    //        }

    //        vec_shp_lightBuffer[i]->Get().lightDir = startColor * itr->time + endColor * (1 - itr->time);
    //        vec_shp_transform[i]->SetWorldPosition(itr->initPos + Vector3::YAxis * (Easing::EaseInOut(itr->moveTime) - 0.5) * itr->moveMax);
    //        vec_shp_transform[i]->SetLocalScale(itr->initScale * (Easing::EaseInOut(scale)));
    //        vec_shp_transform[i]->RollLocalRotationY_Degrees(itr->movePase * 100);

    //    }
    //    scale += 0.016f;
    //    if (scale > 1.0f) {
    //        scale = 1.0f;
    //    }
    //}
    //else if (!isBrast) {
    //    for (auto itr = vec_blockData.begin(); itr != endItr; itr++, i++) {

    //        itr->time += pase * itr->direction;
    //        if (itr->time > 1.0f) {
    //            itr->time = 1.0f;
    //            itr->direction = -1;
    //        }
    //        else if (itr->time < 0.0f) {
    //            itr->time = 0.0f;
    //            itr->direction = 1;
    //        }

    //        vec_shp_lightBuffer[i]->Get().lightDir = startColor * itr->time + endColor * (1 - itr->time);

    //        float x = ButiRandom::GetRandom(-1.0, 1.0, 10);
    //        float y = ButiRandom::GetRandom(-1.0, 1.0, 10);
    //        float z = ButiRandom::GetRandom(-1.0, 1.0, 10);

    //        vec_shp_transform[i]->SetWorldPosition(itr->initPos + Vector3(x, y, z) * 0.1);
    //    }
    //}

    //if (isBrast) {
    //    for (auto itr = vec_blockData.begin(); itr != endItr; itr++, i++) {


    //        auto anim = vec_gameObjects[i]->AddGameComponent<TransformAnimation>();

    //        anim->SetTargetTransform(vec_shp_transform[i]->Clone());
    //        auto targetPos = itr->initPos.Normalize() * 1000;
    //        anim->GetTargetTransform()->SetLocalPosition(targetPos);
    //        anim->GetTargetTransform()->SetLocalScale(Vector3());
    //        anim->SetSpeed(1.0f / 20.0f);
    //    }
    //}
}

void ButiEngine::BackGround::OnSet()
{
}

void ButiEngine::BackGround::OnRemove()
{
}

void ButiEngine::BackGround::SetIsClear(const bool arg_isClear)
{
}

void ButiEngine::BackGround::Brast()
{
    m_isBrast = true;
}

void ButiEngine::BackGround::SetStartColor(const Vector4& arg_color)
{
    m_startColor = arg_color;
}

void ButiEngine::BackGround::SetEndColor(const Vector4& arg_color)
{
    m_endColor = arg_color;
}

void ButiEngine::BackGround::OnShowUI()
{
    GUI::BulletText("StartColor");

    GUI::DragFloat4("##startcolor", m_startColor, 0.01f, 0, 1);
    GUI::BulletText("EndColor");

    GUI::DragFloat4("##Endcolor", m_endColor, 0.01f, 0, 1);
    GUI::BulletText("Pase");

    GUI::DragFloat("##pase", m_pase, 0.01f, 0, 1);
}

void ButiEngine::BackGround::Start()
{
    //auto floatBlockTag = gameObject.lock()->GetApplication().lock()->GetGameObjectTagManager()->GetObjectTag("FloatBlock");
    //auto floatBlocks = GetManager().lock()->GetGameObjects(floatBlockTag);

    //auto endItr = floatBlocks.end();

    //for (auto itr = floatBlocks.begin(); itr != endItr; itr++) {
    //    vec_gameObjects.push_back(*itr);
    //    vec_shp_transform.push_back((*itr)->transform);
    //    vec_shp_lightBuffer.push_back((*itr)->GetGameComponent<MeshDrawComponent_Static>()->GetCBuffer<LightVariable>("LightBuffer"));

    //    Vector3 pos = (*itr)->transform->GetWorldPosition();
    //    float current = (sin(MathHelper::Dot(Vector2(pos.x, pos.z), Vector2(1.9898 + pos.y, 8.233))) * 437.645);
    //    current -= (int)current;

    //    FloatBlockData data;
    //    data.time = current;
    //    data.initPos = pos;
    //    data.initScale = (*itr)->transform->GetLocalScale();

    //    (*itr)->transform->SetLocalScale(Vector3());

    //    data.moveTime = (sin(MathHelper::Dot(Vector2(pos.x, pos.z), Vector2(3.1 + pos.y, 1.234))) * 851.645);
    //    data.moveTime -= (int)data.moveTime;
    //    data.direction = ButiRandom::GetRandom(0, 1, 1);
    //    data.movePase = ButiRandom::GetRandom(1.0f, 5.0f, 5) * 0.001f;
    //    data.moveMax = ButiRandom::GetRandom(1.0f, 2.0f, 10) * 0.2;
    //    data.moveDir = ButiRandom::GetRandom(0, 1, 1);

    //    if (data.moveDir == 0) {
    //        data.moveDir = -1;
    //    }
    //    if (data.direction == 0) {
    //        data.direction = -1;
    //    }



    //    vec_blockData.push_back(data);

    //}
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::BackGround::Clone()
{
	return ObjectFactory::Create<BackGround>();
}
