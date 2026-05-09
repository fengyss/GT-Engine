#pragma once
#include "ParticleTypes.h"
#include "ParticlePool.h"
#include "GT/Core/TimeStep.h"

namespace GT {

    class ParticleEmitter {
    public:
        ParticleEmitter();
        ~ParticleEmitter() = default;

        void Init();

        // 核心功能
        void Emit(uint32_t count = 1);
        void Update(float deltaTime);

        // 配置接口
        void SetConfig(const ParticleEmitterConfig& config) { m_Config = config; Init(); }
        const ParticleEmitterConfig& GetConfig() const { return m_Config; }
		std::vector<Particle>& GetParticles() const { return m_ParticlePool->GetParticles(); }

        // Normally should not be called
        // if you need special init function for particle
        // you shouled set it to  ParticleEmitterConfig.init_func
        void SetInitParticleFunc(std::function<void(Particle&)>& func) { m_InitParticleFunc = func; }

        // ECS集成
        void OnUpdate(Timestep dt) ;
        void OnRender() ;

        // 编辑器支持
#ifdef GT_DEBUG
        void OnImGuiRender();
#endif

    private:
        ParticleEmitterConfig m_Config;
        float m_SpawnAccumulator = 0.0f;
        Ref<ParticlePool> m_ParticlePool;
        float m_StartTime = 0.0f;

        // will called at the end of InitializeParticle
        // and override all it influnced value
        // default function only decide pos and vel based on shape
        std::function<void(Particle&)> m_InitParticleFunc;

        // 随机分布生成
        glm::vec3 GenerateRandomPosition();
        glm::vec3 GenerateRandomVelocity();
        glm::vec3 GenerateRandomRotationVelocity();
        glm::vec4 GenerateRandomColor();

        glm::vec3 GenerateBoxRandomVec3();
        glm::vec3 GenerateSphereRandomVec3(float radius);
        glm::vec2 GenerateCircleRandomVec2(float radius);

        // 粒子初始化
        void InitializeParticle(Particle& particle);
        void InitPointParticle(Particle& particle);
        void InitBoxParticle(Particle& particle);
        void InitSphereParticle(Particle& particle);
        void InitRingParticle(Particle& particle);
        void InitConeParticle(Particle& particle);
    };
} // namespace GT