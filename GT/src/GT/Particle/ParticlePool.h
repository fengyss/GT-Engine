#pragma once
#include <vector>
#include <queue>
#include <mutex>
#include "ParticleTypes.h"

namespace GT {

    class ParticlePool {
    public:
        ParticlePool(size_t maxParticles = 3000);

        // 获取可用粒子索引，-1表示池满
        int32_t GetAvailableParticleIndex();
        std::queue<int32_t>& GetAvailableParticleIndices() { return m_AvailableIndices; }

        // 回收粒子
        void ReturnParticle(int32_t index);

        Ref<Particle> GetNext();

        // 批量操作
        void UpdateAll(float deltaTime);
        void Clear();

        // 直接访问粒子数据（用于GPU上传）
        const std::vector<Ref<Particle>>& GetParticles() const { return m_Particles; }
        std::vector<Ref<Particle>>& GetParticles() { return m_Particles; }

        size_t GetActiveCount() const { return m_ActiveCount; }

    private:
        std::vector<Ref<Particle>> m_Particles;
        std::queue<int32_t> m_AvailableIndices;
        size_t m_MaxParticles;
        size_t m_ActiveCount = 0;
        mutable std::mutex m_Mutex;

        // SIMD优化的更新函数
        void UpdateParticleSIMD(Particle& particle, float deltaTime);
    };
} // namespace GT