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
		auto view = scene->Reg().view<SpriteRendererComponent, ParticleComponent>();
		for (auto e : view)
		{
			auto& [sprite,p] = view.get<SpriteRendererComponent, ParticleComponent>(e);

			if (p.IsEmitting == false) continue;
			RefHandle<Texture2D> tex = sprite.texture;
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