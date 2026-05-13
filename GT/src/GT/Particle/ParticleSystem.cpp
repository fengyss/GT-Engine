#include "gtpch.h"
#include "ParticleSystem.h"
#include "GT/Scene/Entity.h"

namespace GT {
	std::unordered_map<uint32_t, Ref<ParticleEmitter>> ParticleSystem::m_Emitters;
	Ref<Scene> ParticleSystem::m_activeScene = nullptr;
	std::vector<Ref<Particle>> ParticleSystem::m_Particles;

	void ParticleSystem::OnUpdate(Timestep dt, const glm::vec3& cameraPos) {
		//ParallelUpdate(dt);
		if (m_activeScene == nullptr) 
		{
			GT_CORE_ERROR("Particle System didn't set current active scene!");
			return;
		}
		for (const auto& [entity, emitter] : m_Emitters) {

			Entity e = { entity, m_activeScene.get() };
			if (!e.HasComponent<ParticleComponent>())
			{
				DestroyEmitter(e);
				break;
			}
			auto particle = e.GetComponent<ParticleComponent>();
			if (particle.IsEmitting == false) continue;

			auto trans = e.GetComponent<TransformComponent>();

			emitter->SetPostion(trans.Translation);
			emitter->SetRotation(trans.Rotation);
			emitter->SetSaclar(trans.Scale);
			emitter->OnUpdate(dt);
			//m_TotalParticles += emitter->GetParticles().size();
			//GT_CORE_INFO("Total Particles: {0}", m_TotalParticles);
		}
		//SortParticlesByDistance(cameraPos);

	}
	void ParticleSystem::OnRender() {
		if (m_activeScene == nullptr)
		{
			GT_CORE_ERROR("Particle System didn't set current active scene!");
			return;
		}

		for (const auto& [entity, emitter] : m_Emitters) {
			Entity e = { entity, m_activeScene.get() };

			auto particle = e.GetComponent<ParticleComponent>();
			if (particle.IsEmitting == false) continue;
			if (e.HasComponent<SpriteRendererComponent>())
			{
				auto sprite = e.GetComponent<SpriteRendererComponent>();
				if (sprite.texture) ParticleRenderer::RenderParticles(emitter->GetParticles(), sprite.texture);
				else ParticleRenderer::RenderParticles(emitter->GetParticles());
			}
			else ParticleRenderer::RenderParticles(emitter->GetParticles());
		}

		//for (auto p : m_Particles) {
		//	ParticleRenderer::RenderParticle(p);
		//}
	}
	Ref<ParticleEmitter> ParticleSystem::CreateEmitter(Entity& entity) {
		
		auto& particle = entity.GetComponent<ParticleComponent>();
		auto emitter = CreateEmitter(particle.Config);
		m_Emitters[entity] = emitter;
		return emitter;
	}
	Ref<ParticleEmitter> ParticleSystem::CreateEmitter(ParticleEmitterConfig& config)
	{
		auto emitter = CreateRef<ParticleEmitter>();
		emitter->SetConfig(config);
		return emitter;
	}
	void ParticleSystem::DestroyEmitter(Entity& entity) {
		m_Emitters.erase(entity);
	}
	void ParticleSystem::SetMaxParticles(uint32_t maxParticles) {
	}
	void ParticleSystem::ParallelUpdate(float deltaTime) {
		// 多线程更新粒子逻辑
	}
	void ParticleSystem::SortParticlesByDistance(const glm::vec3& cameraPos) 
	{
		m_Particles.clear();
		for (const auto& [entity, emitter] : m_Emitters)
		{
			Entity e = { entity, m_activeScene.get() };

			auto particle = e.GetComponent<ParticleComponent>();
			if (particle.IsEmitting == false) continue;

			auto& particles = emitter->GetParticles();
			for (auto& p : particles)
			{
				p->distanceToCamera = glm::length(cameraPos - p->position);
			}
			m_Particles.insert(m_Particles.end(), particles.begin(), particles.end());
		}
		std::sort(m_Particles.begin(), m_Particles.end(),
			[](Ref<Particle> a, Ref<Particle> b) {
				return a->distanceToCamera > b->distanceToCamera;
			});
	}
} // namespace GT