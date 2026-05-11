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

        // 系统接口
        static void OnUpdate(Timestep dt, const glm::vec3& cameraPos);
        static void OnRender();

		static void SetActiveScene(const Ref<Scene>& scene) { m_activeScene = scene; }

        // 发射器管理
        static Ref<ParticleEmitter> CreateEmitter(Entity& entity);
        static Ref<ParticleEmitter> CreateEmitter(ParticleEmitterConfig& config);
        static void DestroyEmitter(Entity& entity);

        // 全局设置
        static void SetMaxParticles(uint32_t maxParticles);

        // 性能统计
        static uint32_t GetTotalParticles();
        static uint32_t GetDrawCalls();

    private:
        static std::unordered_map<uint32_t, Ref<ParticleEmitter>> m_Emitters;
        static std::vector<Ref<Particle>> m_Particles;
		static Ref<Scene> m_activeScene;
        static void ParallelUpdate(float deltaTime);
        static void SortParticlesByDistance(const glm::vec3& cameraPos);
    };
} // namespace GT