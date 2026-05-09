#include "gtpch.h"
#include "ParticleEmitter.h"
#include "GT/Math/Random.h"

namespace GT
{
	ParticleEmitter::ParticleEmitter()
	{
		m_ParticlePool = CreateRef<ParticlePool>(10000); // 初始池大小
	}
	void ParticleEmitter::Emit(uint32_t count) {
		for (uint32_t i = 0; i < count; ++i) {
			Particle& particle = m_ParticlePool->GetNext();
			InitializeParticle(particle);
		}
	}
	void ParticleEmitter::Update(float deltaTime) {

		m_SpawnAccumulator += deltaTime;
		float spawnInterval = 1.0f / m_Config.spawnRate;

		while (m_SpawnAccumulator >= spawnInterval) {
			Emit();
			m_SpawnAccumulator -= spawnInterval;
		}


		int index = -1;
		for (auto& particle : m_ParticlePool->GetParticles()) {
			index++;
			if (particle.lifeRemaining <= 0.0f) continue;

			// 位置更新
			particle.position += particle.velocity * deltaTime;

			particle.position.y -= 9.8f * deltaTime;

			// 旋转更新
			particle.rotation += particle.Rvelocity * deltaTime;

			// 颜色衰减
			//particle.color.a = std::max(0.0f, 1.0f - (particle.age / particle.life));

			// 大小衰减
			particle.size = std::max(0.0f, std::min(particle.lifeRemaining,1.0f)) * m_Config.sizeStart;

			// 速度衰减
			particle.velocity *= std::min(particle.lifeRemaining, 1.0f);

			// 生命周期检查
			particle.lifeRemaining -= deltaTime;

			if (particle.lifeRemaining <= 0.0f)
			{
				m_ParticlePool->ReturnParticle(index);
			}
		}

		m_ParticlePool->UpdateAll(deltaTime);
	}
	void ParticleEmitter::OnUpdate(Timestep dt) {
		Update(dt.GetSeconds());
	}
	void ParticleEmitter::OnRender() {
		// 渲染逻辑由ParticleRenderer处理
	}

	glm::vec3 ParticleEmitter::GenerateRandomPosition()
	{
		if (m_Config.InUnitSphere) return Random::InUnitSphere() * m_Config.positionVariance;
		else
			return glm::vec3(
				Random::Range(-m_Config.positionVariance.x, m_Config.positionVariance.x),
				Random::Range(-m_Config.positionVariance.y, m_Config.positionVariance.y),
				Random::Range(-m_Config.positionVariance.z, m_Config.positionVariance.z)
			);
	}
	glm::vec3 ParticleEmitter::GenerateRandomVelocity()
	{
		if (m_Config.InUnitSphere) return Random::InUnitSphere() * m_Config.velocityVariance;
		else
			return glm::vec3(
				Random::Range(-m_Config.velocityVariance.x, m_Config.velocityVariance.x),
				Random::Range(-m_Config.velocityVariance.y, m_Config.velocityVariance.y),
				Random::Range(-m_Config.velocityVariance.z, m_Config.velocityVariance.z)
			);
	}
	glm::vec3 ParticleEmitter::GenerateRandomRotationVelocity()
	{
		if (m_Config.InUnitSphere) return Random::InUnitSphere() * m_Config.rotationVariance;
		else
			return glm::vec3(
				Random::Range(-m_Config.rotationVariance.x, m_Config.rotationVariance.x),
				Random::Range(-m_Config.rotationVariance.y, m_Config.rotationVariance.y),
				Random::Range(-m_Config.rotationVariance.z, m_Config.rotationVariance.z)
			);
	}
	glm::vec4 ParticleEmitter::GenerateRandomColor()
	{
		return glm::vec4(
			m_Config.color.r + Random::Range(-m_Config.colorVariance.r, m_Config.colorVariance.r),
			m_Config.color.g + Random::Range(-m_Config.colorVariance.g, m_Config.colorVariance.g),
			m_Config.color.b + Random::Range(-m_Config.colorVariance.b, m_Config.colorVariance.b),
			m_Config.color.a + Random::Range(-m_Config.colorVariance.a, m_Config.colorVariance.a)
		);
	}
	void ParticleEmitter::InitializeParticle(Particle& particle)
	{
		particle.position = GenerateRandomPosition();
		particle.velocity = GenerateRandomVelocity();
		particle.color = GenerateRandomColor();
		particle.Rvelocity = GenerateRandomRotationVelocity();

		particle.lifeRemaining = m_Config.lifetime;
		particle.size = m_Config.sizeStart;
	}
}