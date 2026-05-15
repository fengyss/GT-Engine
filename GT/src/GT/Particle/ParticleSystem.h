#pragma once
#include "ParticleEmitter.h"
#include "ParticleRenderer.h"
#include "GT/Core/TimeStep.h"
#include "GT/Scene/Scene.h"
namespace GT {

    class Entity;

    class ParticleSystem {
    public:
        ParticleSystem();
        ~ParticleSystem() = default;



        static void OnUpdate(Scene* scene,Timestep dt);
        static void OnRender(Scene* scene);

        // 全局设置
        static void SetMaxParticles(uint32_t maxParticles);

        // 性能统计
        static uint32_t GetTotalParticles();
        static uint32_t GetDrawCalls();

    private:
        static void ParallelUpdate(float deltaTime);
        static void SortParticlesByDistance(const glm::vec3& cameraPos);
    };
} // namespace GT