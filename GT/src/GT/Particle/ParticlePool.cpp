#include "gtpch.h"

#include "ParticlePool.h"
#include <algorithm>
#include <cmath>
#include <queue>

namespace GT {

    ParticlePool::ParticlePool(size_t maxParticles)
        : m_MaxParticles(maxParticles), m_ActiveCount(0)
    {
        m_Particles.resize(m_MaxParticles);
        //m_AvailableIndices.reserve(m_MaxParticles);

        // 初始化可用索引队列（反向填充以提高缓存友好性）
        for (int32_t i = static_cast<int32_t>(m_MaxParticles) - 1; i >= 0; --i) {
            m_AvailableIndices.push(i);
        }
    }

    int32_t ParticlePool::GetAvailableParticleIndex()
    {
        std::lock_guard<std::mutex> lock(m_Mutex);

        if (m_AvailableIndices.empty()) {
            return -1; // 池满
        }

        uint32_t index = m_AvailableIndices.front();
        m_AvailableIndices.pop();
        ++m_ActiveCount;

        return index;
    }

    void ParticlePool::ReturnParticle(int32_t index)
    {
        if (index < 0 || index >= static_cast<int32_t>(m_MaxParticles)) {
            return;
        }

        std::lock_guard<std::mutex> lock(m_Mutex);

        Particle& particle = m_Particles[index];
        if (particle.lifeRemaining == 0.0f) {
            return; // 已经回收
        }
		particle.lifeRemaining = 0.0f; // 标记为无效
        m_AvailableIndices.push(index);
        --m_ActiveCount;
    }

    Particle& ParticlePool::GetNext()
    {
        int32_t index = GetAvailableParticleIndex();
        if (index == -1) {
            // 池满时返回第一个粒子（循环使用）
            // 不在生成新粒子
            static Particle dummy;
            return dummy;
        }
        return m_Particles[index];
    }

    void ParticlePool::UpdateAll(float deltaTime)
    {

        for (auto& particle : GetParticles()) {
            if (particle.lifeRemaining <= 0.0f)
            {
                particle.lifeRemaining = 0.0f;
            }
        }
    }

    void ParticlePool::Clear()
    {
        std::lock_guard<std::mutex> lock(m_Mutex);

        m_ActiveCount = 0;
        m_AvailableIndices = std::queue<int32_t>();

        for (int32_t i = static_cast<int32_t>(m_MaxParticles) - 1; i >= 0; --i) {
            m_Particles[i].lifeRemaining = 0.0f;
            m_AvailableIndices.push(i);
        }
    }

    void ParticlePool::UpdateParticleSIMD(Particle& particle, float deltaTime)
    {
        // 基础更新（非SIMD版本）
        // 实际项目中应使用SIMD指令集（如SSE/AVX/NEON）

        // 位置更新
        //particle.position += particle.velocity * deltaTime;

        // 旋转更新
        //particle.rotation += particle.angularVelocity * deltaTime;

        // 颜色衰减
        //particle.color.a = std::max(0.0f, 1.0f - (particle.age / particle.life));

        // 大小衰减
        //particle.size = std::max(0.0f, particle.size - particle.sizeDelta * deltaTime);

        // 速度衰减
        //particle.velocity *= (1.0f - particle.damping * deltaTime);
    }

} // namespace GT