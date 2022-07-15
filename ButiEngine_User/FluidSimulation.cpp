#include "FluidSimulation.h"
#include"Header/Common/CollisionPrimitive.h"






using Particles = std::vector<ButiEngine::FluidSimulation::Particle>;

constexpr float PI = 3.141592653589793f;
namespace FluidSimulation {
float area;
float particleMass;
float intStiff;
float extStiff;
float extDamp;
float Poly6Kern;
float SpikyKern;
float LapKern;
float DeltaTime;
float radius;
float epsilon;
ButiEngine::Vector3     initMin;
ButiEngine::Vector3     initMax;

ButiEngine::Vector3 simMin = ButiEngine::Vector3(-20, 0, -5);
ButiEngine::Vector3 simMax = ButiEngine::Vector3(20, 50, 5);
ButiEngine::Vector3 gravityVec(0, -1.0f, 0);
float SPH_PDIST;
float simulationScale;
float density;
float visc;
float speedLimit;
std::int32_t particleCount=256;
}



void InitParticle(Particles& ps, std::vector< ButiEngine::Value_ptr<ButiEngine::GameObject>>* vec_gameObj)
{
    using namespace FluidSimulation;
    area         = 0.01; // m
    particleMass = 0.00020543; // kg
    intStiff = 1.00;
    extStiff = 10000.0;
    extDamp  = 256.0;
    Poly6Kern = 315.0 / ( 64.0 * PI * pow( area, 9 ) );
    SpikyKern = -45.0 / ( PI * pow( area, 6 ) );
    LapKern   = 45.0 / ( PI * pow( area, 6 ) );
    DeltaTime        = 0.004;
    radius    = 0.004; // m
    epsilon   = 0.00001;
    initMin = ButiEngine::Vector3(-2,0,-2);
    initMax= ButiEngine::Vector3(2,20,2);
    density = 600.0; // kg / m^3
    SPH_PDIST = pow( particleMass / density, 1/3.0 );
    simulationScale = 0.004; // unit size
    visc = 0.2; // pascal-second (Pa.s) = 1 kg m^-1 s^-1
    speedLimit = 200.0;

    float d;
    d = SPH_PDIST * 0.87 / simulationScale;

    std::int32_t i = 0;

    if (vec_gameObj) {
        ButiEngine::FluidSimulation::Particle p;
        for (float z = initMin.z; z <= initMax.z; z += 1.0) {
            for (float y = initMin.y; y <= initMax.y; y += 1.0) {
                for (float x = initMin.x; x <= initMax.x; x += 1.0, i++) {
                    if (i >= vec_gameObj->size()) {
                        return;
                    }
                    float xRand = ButiEngine::ButiRandom::GetRandom<float>(0.0f, 1.0f, 10);
                    float yRand = ButiEngine::ButiRandom::GetRandom<float>(0.0f, 1.0f, 10);
                    float zRand = ButiEngine::ButiRandom::GetRandom<float>(0.0f, 1.0f, 10);
                    p.vlp_objectTransform = (*vec_gameObj)[i]->transform;
                    p.vlp_objectTransform->SetLocalPosition( ButiEngine::Vector3(x + xRand, y + yRand, z + zRand));
                    p.v = ButiEngine::Vector3();
                    p.f = ButiEngine::Vector3();
                    ps.push_back(p);
                }
            }
        }
    }
    else {
        for (float z = initMin.z; z <= initMax.z; z += 1.0) {
            for (float y = initMin.y; y <= initMax.y; y += 1.0) {
                for (float x = initMin.x; x <= initMax.x; x += 1.0, i++) {
                    if (i >= ps.size()) {
                        return;
                    }
                    float xRand = ButiEngine::ButiRandom::GetRandom<float>(0.0f, 1.0f, 10);
                    float yRand = ButiEngine::ButiRandom::GetRandom<float>(0.0f, 1.0f, 10);
                    float zRand = ButiEngine::ButiRandom::GetRandom<float>(0.0f, 1.0f, 10);

                    (ps)[i].vlp_objectTransform->SetLocalPosition(ButiEngine::Vector3(x + xRand, y + yRand, z + zRand));
                    (ps)[i].v = ButiEngine::Vector3();
                    (ps)[i].f = ButiEngine::Vector3();
                }
            }
        }
    }

}

void CalculateDensityPressure(Particles* ps)
{
    using namespace FluidSimulation;
    float r2, c, sum, H2;
    ButiEngine::Vector3 dr;

    H2 = area * area;
    auto end = ps->end();
    for (auto p = ps->begin(); p != end; p++)
    {
        sum = 0.0;
        for (auto pj = p + 1; pj != end; pj++)
        {
            dr = (p->vlp_objectTransform->GetLocalPosition() - pj->vlp_objectTransform->GetLocalPosition()) * simulationScale;
            r2 = dr.GetLengthSqr();
            if (H2 > r2)
            {
                c = H2 - r2;
                float c3 = c * c * c;
                sum += c3;
                pj->sum += c3;
            }
        }
        p->sum += sum;
    } 
    for (auto p = ps->begin(); p != end; p++)
    {
        
        p->rho = p->sum * particleMass * Poly6Kern;
        p->p = (p->rho - density) * intStiff;
        p->rho = 1.0 / p->rho;
        p->sum = 0;
    }
}

void CalculateForce(Particles* ps)
{
    using namespace FluidSimulation;
    float pterm, vterm, r,r2, c;
    vterm = LapKern * visc;
    ButiEngine::Vector3 dr, force, fcurr;

    auto end = ps->end();
    float h2 = area * area;
    for (auto p = ps->begin(); p != end; p++)
    {
        for (auto pj = p+1; pj != end; pj++)
        {
            dr = (p->vlp_objectTransform->GetLocalPosition() - pj->vlp_objectTransform->GetLocalPosition()) * simulationScale;
            r2 = (dr.GetLengthSqr());

            if (h2 > r2)
            {
                r = std::sqrt(r2);
                c = area - r;
                float rhoMul = c * p->rho * pj->rho;
                pterm = -0.5 * c * SpikyKern * (p->p + pj->p) / r;
                fcurr = pterm * dr + vterm * (pj->v - p->v);
                fcurr *=rhoMul;
                p->f += fcurr;

                fcurr = -1*pterm * dr + vterm * (p->v - pj->v);
                fcurr *= rhoMul;
                pj->f += fcurr;
            }
        }

    }
}

void CalculatePosition(Particles* ps)
{
    using namespace FluidSimulation;
    ButiEngine::Vector3 accel, norm;
    float speed, diff, adj;
    float diameter =radius*2;
    auto end = ps->end();
    for (auto p = ps->begin(); p != end; p++)
    {

        accel = p->f * particleMass;  

        speed = (accel.GetLengthSqr());
        if (speed > speedLimit * speedLimit) {
            accel *= speedLimit / sqrt(speed);
        }

        // Z walls
        diff = diameter - (p->vlp_objectTransform->GetLocalPosition().z - simMin.z) * simulationScale;
        if (diff > epsilon)
        {
            norm.x = 0.0; norm.y = 0.0; norm.z = 1.0;
            adj = extStiff * diff - extDamp * norm.Dot(p->v);
            accel += adj * norm;
        }
        diff = diameter - (simMax.z - p->vlp_objectTransform->GetLocalPosition().z) * simulationScale;
        if (diff > epsilon)
        {
            norm.x = 0.0; norm.y = 0.0; norm.z = -1.0;
            adj = extStiff * diff - extDamp * norm.Dot(p->v);
            accel += adj * norm;
        }

        // X walls
        diff = diameter - (p->vlp_objectTransform->GetLocalPosition().x - simMin.x) * simulationScale;
        if (diff > epsilon)
        {
            norm.x = 1.0; norm.y = 0.0; norm.z = 0.0;
            adj = extStiff * diff - extDamp * norm.Dot(p->v);
            accel += adj * norm;
        }
        diff = diameter - (simMax.x - p->vlp_objectTransform->GetLocalPosition().x) * simulationScale;
        if (diff > epsilon)
        {
            norm.x = -1.0; norm.y = 0.0; norm.z = 0.0;
            adj = extStiff * diff - extDamp * norm.Dot(p->v);
            accel += adj * norm;
        }

        // Y walls
        diff = diameter - (p->vlp_objectTransform->GetLocalPosition().y - simMin.y) * simulationScale;
        if (diff > epsilon)
        {
            norm.x = 0.0; norm.y = 1.0; norm.z = 0.0;
            adj = extStiff * diff - extDamp * norm.Dot(p->v);
            accel += adj * norm;
        }
        diff = diameter - (simMax.y - p->vlp_objectTransform->GetLocalPosition().y) * simulationScale;
        if (diff > epsilon)
        {
            norm.x = 0.0; norm.y = -1.0; norm.z = 0.0;
            adj = extStiff * diff - extDamp * norm.Dot(p->v);
            accel += adj * norm;
        }

        p->f = ButiEngine::Vector3();
        accel += gravityVec * 9.8;
        p->v += accel * DeltaTime;
        p->vlp_objectTransform->Translate(p->v * DeltaTime / simulationScale);

    }
}

void process(Particles* ps)
{
    CalculateDensityPressure(ps);
    CalculateForce(ps);
    CalculatePosition(ps);
}


void ButiEngine::FluidSimulation::OnUpdate()
{

    using namespace FluidSimulation;

    static float xLeftWall = 0.0f;
    static std::int32_t wallDir = 1;

    xLeftWall += 0.005f*wallDir;
    if (xLeftWall >= 1.0f) {
        xLeftWall = 1.0f;
        wallDir = -1 * wallDir;
    }else if (xLeftWall <= 0.0f) {
        xLeftWall = 0.0f;
        wallDir = -1 * wallDir;
    }

    simMin.x = -20 + Easing::GetEase(xLeftWall, Easing::EasingType::Parabola) * 35;

    process(&vec_particle);
    
    if (GameDevice::GetInput()->CheckKey(Keys::Q)) {
        auto newParticleObj = GetManager().lock()->AddObjectFromCereal("Particle").lock();
        vec_vlp_gameObject.push_back(newParticleObj);
        Particle newParticle;
        newParticle.vlp_objectTransform = newParticleObj->transform;
        float xRand = ButiEngine::ButiRandom::GetRandom<float>(0.0f, 1.0f, 10);
        float yRand = ButiEngine::ButiRandom::GetRandom<float>(0.0f, 1.0f, 10);
        float zRand = ButiEngine::ButiRandom::GetRandom<float>(0.0f, 1.0f, 10);
        newParticleObj->transform->SetWorldPostionX(-20.0f+ xRand * 40.0f);
        newParticleObj->transform->SetWorldPostionY(30.0f + yRand * 5.0f);
        newParticleObj->transform->SetWorldPostionZ(-5.0f+yRand*10.0f);
        vec_particle.push_back(newParticle);
        particleCount++;
    }
}


void ButiEngine::FluidSimulation::OnSet()
{

}

void ButiEngine::FluidSimulation::Start()
{
    /*using namespace FluidSimulation;
    for (std::int32_t i = 0; i <particleCount; i++) {
        vec_vlp_gameObject.push_back(GetManager().lock()->AddObjectFromCereal("Particle").lock());
    }

    InitParticle(vec_particle, &vec_vlp_gameObject);
    RegistGUI();*/
}


void ButiEngine::FluidSimulation::OnRemove()
{

    UnRegistGUI();
}

void ButiEngine::FluidSimulation::OnShowUI()
{
    using namespace FluidSimulation;
    if (GUI::DragFloat("MASS", particleMass, 0.00001f, 0.0f, 1.0f, "%.8f")) {
        SPH_PDIST = pow(particleMass / density, 1 / 3.0);
    }
    GUI::DragFloat("intStiff", intStiff, 0.00001f, 0.0f, 1.0f, "%.8f");
    GUI::DragFloat("extStiff", extStiff, 0.00001f, 0.0f, 1.0f, "%.8f");
    GUI::DragFloat("extDamp", extDamp, 0.00001f, 0.0f, 1.0f, "%.8f");


    GUI::DragFloat("DeltaTime", DeltaTime, 0.00001f, 0.0f, 1.0f, "%.8f");
    GUI::DragFloat("radius", radius, 0.00001f, 0.0f, 1.0f, "%.8f");
    GUI::DragFloat("epsilon", epsilon, 0.00001f, 0.0f, 1.0f, "%.8f");
    GUI::DragFloat3("simMin", simMin, 0.1f, -10.0f, 10.0f, "%.8f");
    GUI::DragFloat3("simMax", simMax, 0.1f, -10.0f, 10.0f, "%.8f");
    if (GUI::DragFloat3("gravityVec", gravityVec, 0.1f, -1.0f, 1.0f, "%.3f")) {
        gravityVec.Normalize();
    }
    if (GUI::DragFloat("area", area, 0.1f, 0.0f, 1.0f, "%.8f")) {

        Poly6Kern = 315.0 / (64.0 * PI * pow(area, 9));
        SpikyKern = -45.0 / (PI * pow(area, 6));
        LapKern = 45.0 / (PI * pow(area, 6));
    }
    if (GUI::DragFloat("density", density, 0.1f, 0.0f, 6000.0f, "%.8f")) {
        SPH_PDIST = pow(particleMass / density, 1 / 3.0);
    }
    GUI::DragFloat("simulationScale", simulationScale, 0.00001f, 0.0f, 1.0f, "%.8f");
    GUI::DragFloat("visc", visc, 0.00001f, 0.0f, 1.0f, "%.8f");
    GUI::DragFloat("speedLimit", speedLimit, 0.00001f, 0.0f, 1.0f, "%.8f");
    GUI::DragInt("ParticleCount", particleCount, 1, 0, 1250);
    if (GUI::Button("ParamReset")) {
        InitParticle(vec_particle,nullptr);
    }

}

void ButiEngine::FluidSimulation::ShowGUI()
{
    using namespace FluidSimulation;
    GUI::BulletText(u8"マウス+WASDでFPS移動");
    GUI::BulletText(u8"LShift、SpaceでY軸移動");
    GUI::BulletText(u8"Qでパーティクル追加");
    GUI::BulletText(u8"パーティクル数："+std::to_string(particleCount));

}

ButiEngine::Vector3 ButiEngine::FluidSimulation::GetWorldPosition()
{
    return gameObject.lock()->transform->GetWorldPosition();
}

void ButiEngine::FluidSimulation::AdditionMass(const float arg_mass)
{
    mass += arg_mass;

    if (vlp_befPoint) {
        vlp_befPoint->AdditionMass(mass);
    }
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FluidSimulation::Clone()
{
    auto clone = ObjectFactory::Create<FluidSimulation>();
    clone->mass = mass;
    clone->acc = acc;
    clone->attenuation = attenuation;
    clone->bounce = bounce;
    clone->leaderSpeed = leaderSpeed;
    clone->speed = speed;
    return clone;
}