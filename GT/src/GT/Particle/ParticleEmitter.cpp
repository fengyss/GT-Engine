#include "gtpch.h"
#include "ParticleEmitter.h"
#include "GT/Math/Random.h"
#include "GT/Utils/PlatformUtils.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

namespace GT
{
#define BIND_ParticleInit_FN(x) std::bind(&x, this, std::placeholders::_1)

	ParticleEmitter::ParticleEmitter()
	{
		m_ParticlePool = CreateRef<ParticlePool>(3000);
	}
	ParticleEmitter::ParticleEmitter(const ParticleEmitterConfig& config)
		: m_Config(config) 
	{
		m_ParticlePool = CreateRef<ParticlePool>(3000);
		Init();
	};
	void ParticleEmitter::Emit(uint32_t count) {
		for (uint32_t i = 0; i < count; ++i) {
			Ref<Particle> particle = m_ParticlePool->GetNext();
			if (particle) InitializeParticle(particle);
			else break;
		}
	}


	void ParticleEmitter::Init()
	{
		m_StartTime = Time::GetTime();
		if (!m_Config.init_func)
		{
			switch (m_Config.shape)
			{
			case EmitterShape::Point:
				m_InitParticleFunc = BIND_ParticleInit_FN(ParticleEmitter::InitPointParticle);
				break;
			case EmitterShape::Box:
				m_InitParticleFunc = BIND_ParticleInit_FN(ParticleEmitter::InitBoxParticle);
				break;
			case EmitterShape::Sphere:
				m_InitParticleFunc = BIND_ParticleInit_FN(ParticleEmitter::InitSphereParticle);
				break;
			case EmitterShape::Ring:
				m_InitParticleFunc = BIND_ParticleInit_FN(ParticleEmitter::InitRingParticle);
				break;
			case EmitterShape::Cone:
				m_InitParticleFunc = BIND_ParticleInit_FN(ParticleEmitter::InitConeParticle);
				break;
			}
		}
		else m_InitParticleFunc = m_Config.init_func;
		
	}

	void ParticleEmitter::Update(float deltaTime) {

		m_SpawnAccumulator += deltaTime;
		float spawnInterval = 1.0f / m_Config.spawnRate;

		while (m_SpawnAccumulator >= spawnInterval) {
			Emit();
			m_SpawnAccumulator -= spawnInterval;
		}

		float currentTime = Time::GetTime() - m_StartTime;

		for (auto it = m_Config.bursts.begin(); it != m_Config.bursts.end(); )
		{
			if (currentTime < it->time)
			{
				it++;
				continue;
			}
			else
			{
				
				Emit(it->count);
				it->time += it->interval;
				if (it->cycles != 0)
				{
					it->cycles--;
					// after erase, it will be next one automatically
					if (it->cycles == 0) it = m_Config.bursts.erase(it);
				}
			}
		}

		int index = -1;
		for (auto particle : m_ParticlePool->GetParticles()) {
			index++;
			if (particle->lifeRemaining <= 0.0f) continue;

			// 位置更新
			particle->position += particle->velocity * deltaTime;

			//particle->position.y -= 9.8f * deltaTime;

			// 旋转更新
			particle->rotation += particle->Rvelocity * deltaTime;

			// 颜色衰减
			//particle->color.a = std::max(0.0f, 1.0f - (particle->age / particle->life));

			// 大小衰减
			particle->size = std::max(0.0f, std::min(particle->lifeRemaining,1.0f)) * m_Config.sizeStart;

			// 速度衰减
			particle->velocity = particle->velocity * std::max(0.1f,std::min(particle->lifeRemaining, 1.0f));

			// 生命周期检查
			particle->lifeRemaining -= deltaTime;

			if (particle->lifeRemaining <= 0.0f)
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
			m_Config.color.r * Random::Range(-m_Config.colorVariance.r, m_Config.colorVariance.r),
			m_Config.color.g * Random::Range(-m_Config.colorVariance.g, m_Config.colorVariance.g),
			m_Config.color.b * Random::Range(-m_Config.colorVariance.b, m_Config.colorVariance.b),
			m_Config.color.a * Random::Range(-m_Config.colorVariance.a, m_Config.colorVariance.a)
		);
	}

	glm::vec3 ParticleEmitter::GenerateBoxRandomVec3()
	{
		return Random::RangeVec3(-1.0,1.0f);
	}
	glm::vec3 ParticleEmitter::GenerateSphereRandomVec3(float radius)
	{
		if (m_Config.InUnitSphere) return radius * Random::InUnitSphere();
		else return radius * Random::OnUnitSphere();
	}

	glm::vec2 ParticleEmitter::GenerateCircleRandomVec2(float radius)
	{
		return radius * Random::OnUnitCircle();
	}

	void ParticleEmitter::InitializeParticle(Ref<Particle> particle)
	{

		particle->position = GenerateRandomPosition();
		particle->velocity = GenerateRandomVelocity();

		//particle->color = GenerateRandomColor();
		particle->color = m_Config.colorVariance;
		particle->Rvelocity = GenerateRandomRotationVelocity();

		particle->lifeRemaining = m_Config.lifetime;
		particle->size = m_Config.sizeStart + (Random::NormalFloat() * m_Config.sizeVariance);


		m_InitParticleFunc(particle);

		particle->position += m_Postion;
		//particle->startsize *= m_Scalar.x;

	}
	void ParticleEmitter::SetRotation(const glm::vec3& roattion) {
		m_Rotation = roattion;
		glm::mat4 rotation = glm::toMat4(glm::quat(m_Rotation));
		m_Direction = rotation * glm::vec4(m_Config.direction, 1.0f);
		//m_Direction = m_Rotation;
	}
	void ParticleEmitter::InitPointParticle(Ref<Particle> particle)
	{
		particle->position = glm::vec3(0.0f);
		if (glm::length(m_Direction) == 0.0f) GT_CORE_ASSERT(false, "Dirction should not be 0!");
		particle->velocity = m_Config.velocity * glm::normalize(m_Direction);
	}
	void ParticleEmitter::InitBoxParticle(Ref<Particle> particle)
	{
		//particle->position = GenerateRandomPosition();
		particle->position = glm::vec3(0.0f);
		particle->velocity = m_Config.velocity * GenerateBoxRandomVec3();
	}
	void ParticleEmitter::InitSphereParticle(Ref<Particle> particle)
	{
		//particle->position = GenerateSphereRandomVec3();
		particle->position = glm::vec3(0.0f);
		particle->velocity = m_Config.velocity * GenerateSphereRandomVec3(m_Config.radius);
	}
	void ParticleEmitter::InitRingParticle(Ref<Particle> particle)
	{
		glm::vec2 dirction = GenerateCircleRandomVec2(m_Config.innerRadius);
		glm::vec2 pos = Random::Range(m_Config.innerRadius, m_Config.outerRadius) * dirction;
		glm::vec2 vel = m_Config.velocity * dirction;
		particle->position = glm::vec3(pos.x, 0.0f, pos.y);
		particle->velocity = glm::vec3(vel.x, 0.0f, vel.y);
	}
	void ParticleEmitter::InitConeParticle(Ref<Particle> particle)
	{
		particle->position = glm::vec3(0.0f);

		float theta = Random::Range(0.0f, glm::two_pi<float>());
		float coneAngle = glm::radians(m_Config.coneAngle);
		float cosAngle = cos(coneAngle);
		float u = Random::Range(cosAngle, 1.0f);

		float sinAngle = sqrt(1.0f - u * u);

		float x = sinAngle * cos(theta);
		float y = sinAngle * sin(theta);
		float z = u;

		// 将方向转到发射器朝向
		glm::mat3 rotation = glm::mat3(
			glm::normalize(glm::cross(m_Direction, glm::vec3(0, -1, 0))),
			glm::normalize(m_Direction),
			glm::normalize(glm::cross(m_Direction, glm::cross(m_Direction, glm::vec3(0, -1, 0))))
		);

		particle->velocity = glm::normalize(rotation * glm::vec3(x, y, z))*m_Config.velocity;
	}
}