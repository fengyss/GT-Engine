#include "gtpch.h"
#include "ParticleSystem.h"
#include "GT/Scene/Entity.h"

namespace GT {

	void ParticleSystem::OnUpdate(Scene* scene, Timestep dt)
	{
		auto view = scene->Reg().view<TransformComponent, ParticleComponent>();
		for (auto e : view)
		{
			auto& [trans, p] = view.get<TransformComponent, ParticleComponent>(e);

			if (p.IsEmitting == false) continue;

			p.Emitter->SetPostion(trans.Translation);
			p.Emitter->SetRotation(trans.Rotation);
			p.Emitter->SetSaclar(trans.Scale);
			p.Emitter->OnUpdate(dt);
		}
	}
	void ParticleSystem::OnRender(Scene* scene)
	{
		auto view = scene->Reg().view<ParticleComponent>();
		for (auto e : view)
		{
			auto& p = view.get<ParticleComponent>(e);
			if (p.IsEmitting == false) continue;
			Texture2D tex;
			Entity entity = { e,scene };
			if (entity.HasComponent<SpriteRendererComponent>())
			{
				auto& sprite = entity.GetComponent<SpriteRendererComponent>();
				tex = sprite.texture;
			}
			ParticleRenderer::RenderParticles(p.Config.blendMode, p.Emitter->GetParticles(), tex);
		}
	}
	void ParticleSystem::SetMaxParticles(uint32_t maxParticles) {
	}
	void ParticleSystem::ParallelUpdate(float deltaTime) {
		// 多线程更新粒子逻辑
	}
	void ParticleSystem::SortParticlesByDistance(const glm::vec3& cameraPos) 
	{
	}
} // namespace GT