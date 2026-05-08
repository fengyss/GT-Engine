#pragma once
#include "ParticleTypes.h"
#include "ParticlePool.h"
#include "GT/Core/TimeStep.h"

namespace GT {

    class ParticleEmitter {
    public:
        ParticleEmitter();
        ~ParticleEmitter() = default;

        // 核心功能
        void Emit(uint32_t count = 1);
        void Update(float deltaTime);

        // 配置接口
        void SetConfig(const ParticleEmitterConfig& config) { m_Config = config; }
        const ParticleEmitterConfig& GetConfig() const { return m_Config; }
		std::vector<Particle>& GetParticles() const { return m_ParticlePool->GetParticles(); }

        // ECS集成
        void OnUpdate(Timestep dt) ;
        void OnRender() ;

        // 编辑器支持
#ifdef GT_DEBUG
        void OnImGuiRender() ;
#endif

    private:
        ParticleEmitterConfig m_Config;
        float m_SpawnAccumulator = 0.0f;
        Ref<ParticlePool> m_ParticlePool;


        // 随机分布生成
        glm::vec3 GenerateRandomPosition();
        glm::vec3 GenerateRandomVelocity();
        glm::vec4 GenerateRandomColor();

        // 粒子初始化
        void InitializeParticle(Particle& particle);
    };
} // namespace GT