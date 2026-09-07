#pragma once
#include "ParticleEmitter.h"
#include "ParticleRenderer.h"
#include "GT/Core/Timestep.h"
#include "GT/Scene/Scene.h"
namespace GT {

    class Entity;

    class ParticleSystem {
    public:
        ParticleSystem();
        ~ParticleSystem() = default;



        static void OnUpdate(Scene* scene,Timestep dt);
        static void OnRender(Scene* scene);

        // ȫ������
        static void SetMaxParticles(uint32_t maxParticles);

        // ����ͳ��
        static uint32_t GetTotalParticles();
        static uint32_t GetDrawCalls();

    private:
        static void ParallelUpdate(float deltaTime);
        static void SortParticlesByDistance(const glm::vec3& cameraPos);
    };
} // namespace GT