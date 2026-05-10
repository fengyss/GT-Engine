#include "gtpch.h"
#include "ParticleSystem.h"
#include "GT/Scene/Entity.h"

namespace GT {
	std::unordered_map<uint32_t, Ref<ParticleEmitter>> ParticleSystem::m_Emitters;
	Ref<Scene> ParticleSystem::m_activeScene = nullptr;

	void ParticleSystem::OnUpdate(Timestep dt) {
		//ParallelUpdate(dt);
		if (m_activeScene == nullptr) 
		{
			GT_CORE_ERROR("Particle System didn't set current active scene!");
			return;
		}
		for (const auto& [entity, emitter] : m_Emitters) {
			emitter->OnUpdate(dt);
			//m_TotalParticles += emitter->GetParticles().size();
			//GT_CORE_INFO("Total Particles: {0}", m_TotalParticles);
		}

	}
	void ParticleSystem::OnRender() {
		if (m_activeScene == nullptr)
		{
			GT_CORE_ERROR("Particle System didn't set current active scene!");
			return;
		}
		//m_Renderer->BeginScene();
		for (const auto& [entity, emitter] : m_Emitters) {
			Entity e = { entity, m_activeScene.get() };

			auto particle = e.GetComponent<ParticleComponent>();
			if (particle.IsEmitting == false) continue;

			auto transform = e.GetComponent<TransformComponent>();
			ParticleRenderer::RenderParticles(emitter->GetParticles(), transform.Translation);
		}
		//m_Renderer->EndScene();
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
	void ParticleSystem::SortParticlesByDistance(const glm::vec3& cameraPos) {
		// 根据摄像机位置排序粒子以正确渲染透明度
	}
} // namespace GT