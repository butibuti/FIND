#include "SquareParticleEmitter.h"

void ButiEngine::SquareParticleEmitter::OnUpdate()
{
    if (vlp_particleController&&GameDevice::GetInput()->TriggerKey(Keys::A)) {
        Particle3D part;
        for (std::int32_t i = 0; i < increase; i++) {
            float t = ButiRandom::GetRandom(0.0f, 1.0f, 20);
            part.position = leftUP + (rightUP - leftUP) * t;
            ParticleInformationSet(part);
            part.position.z += thickness;
            vlp_particleController->AddParticle(part);
            t = ButiRandom::GetRandom(0.0f, 1.0f, 20);
            part.position = leftUP + (rightUP - leftUP) * t;
            ParticleInformationSet(part);
            part.position.z += thickness;
            vlp_particleController->AddParticle(part);

        }

    }

}

void ButiEngine::SquareParticleEmitter::OnSet()
{
    vlp_particleController = gameObject.lock()->GetGameComponent<ImmediateParticleController>();
}

void ButiEngine::SquareParticleEmitter::Start()
{
    if (!vlp_particleController)
        vlp_particleController = gameObject.lock()->GetGameComponent<ImmediateParticleController>();
}

void ButiEngine::SquareParticleEmitter::OnShowUI()
{
    GUI::ColorEdit4("particleColorBaseMin", colorMin);
    GUI::ColorEdit4("particleColorBaseMax", colorMax);
    GUI::ColorEdit4("particleColorPaseMin", colorPaseMin);
    GUI::ColorEdit4("particleColorPaseMax",colorPaseMax);
    if (GUI::DragFloat3("leftUp", leftUP, 0.01f, -1000, 1000) ||
        GUI::DragFloat3("rightUp", rightUP, 0.01f, -1000, 1000) ||
        GUI::DragFloat3("leftDown", leftDown, 0.01f, -1000, 1000) ||
        GUI::DragFloat3("rightDown", rightDown, 0.01f, -1000, 1000)) {
        center = (leftUP + rightUP + leftDown + rightDown) * 0.25f;
    }
    GUI::DragFloat("speedMin", speedMin, 0.01f, 0.0f, 10.0f);
    GUI::DragFloat("speedMax", speedMax, 0.01f, 0.0f, 10.0f);
    GUI::DragFloat("lifeMin", lifeMin, 0.01f, 0.0f, 10.0f);
    GUI::DragFloat("lifeMax", lifeMax, 0.01f, 0.0f, 10.0f);
    GUI::DragFloat("sizeMin", sizeMin, 0.01f, 0.0f, 10.0f);
    GUI::DragFloat("sizeMax", sizeMax, 0.01f, 0.0f, 10.0f);
    GUI::DragFloat("accelMin", accelMin, 0.01f, 0.0f, 10.0f);
    GUI::DragFloat("accelMax", accelMax, 0.01f, 0.0f, 10.0f);
    GUI::DragInt("increase", increase, 1, 0, 10);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::SquareParticleEmitter::Clone()
{
    return ObjectFactory::Create<SquareParticleEmitter>();
}

void ButiEngine::SquareParticleEmitter::ParticleInformationSet(Particle3D& arg_refParticle)
{
    float t = 0;
    arg_refParticle.velocity = (arg_refParticle.position - center).GetNormalize()* ButiRandom::GetRandom(speedMin, speedMax, 10);
    arg_refParticle.size = ButiRandom::GetRandom(sizeMin, sizeMax, 10);
    t = ButiRandom::GetRandom(0, 1, 5);
    arg_refParticle.color = colorMin + (colorMax - colorMin) * t;
    t = ButiRandom::GetRandom(0, 1, 5);
    arg_refParticle.colorPase = colorPaseMin + (colorPaseMax - colorPaseMin) * t;
    arg_refParticle.life = ButiRandom::GetRandom(lifeMin, lifeMax, 10);
    arg_refParticle.accelation = ButiRandom::GetRandom(accelMin, accelMax, 20);
}
