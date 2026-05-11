#include "gtpch.h"
#include "ParticleRenderer.h"
#include "GT/Renderer/Renderer.h"
#include "GT/Renderer/Shader.h"
#include "GT/Renderer/Texture.h"
#include "GT/Renderer/Buffer.h"
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"
#include <algorithm>

namespace GT {

    void ParticleRenderer::RenderParticles(const std::vector<Ref<Particle>> particles)
    {
        //if (particles.empty()) return;

        for (const auto particle : particles) {
            if (particle->lifeRemaining == 0.0f) continue;

            glm::vec3 pos = particle->position;
            glm::vec2 size = glm::vec2(particle->size);

			Renderer2D::DrawParticleQuad(pos,size, particle->color);
            //Renderer2D::DrawCircle(model, particle.color);
        }
    }

    void ParticleRenderer::RenderParticles(const std::vector<Ref<Particle>> particles, const RefHandle<Texture2D> tex)
    {
        for (const auto particle : particles) {
            if (particle->lifeRemaining == 0.0f) continue;

            glm::vec3 pos = particle->position;
            glm::vec2 size = glm::vec2(particle->size);

            Renderer2D::DrawParticleQuad(pos, size, particle->color,tex->Get());
            //Renderer2D::DrawCircle(model, particle.color);
        }
    }

    void ParticleRenderer::RenderParticle(const Ref<Particle> particle)
    {
        if (particle->lifeRemaining == 0.0f) return;

        glm::vec3 pos = particle->position;
        glm::vec2 size = glm::vec2(particle->size);

        Renderer2D::DrawParticleQuad(pos, size, particle->color);
    }

    void ParticleRenderer::Flush(BlendMode mode)
    {
        RenderCommand::SetBlendMode(mode);

    }


}