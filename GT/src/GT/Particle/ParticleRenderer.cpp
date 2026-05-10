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

    void ParticleRenderer::RenderParticles(const std::vector<Particle>& particles, const glm::vec3& position)
    {
        if (particles.empty()) return;

        for (const Particle& particle : particles) {
            if (particle.lifeRemaining == 0.0f) continue;

            glm::vec3 pos = particle.position + position ;
            glm::vec2 size = glm::vec2(particle.size);



			Renderer2D::DrawParticleQuad(pos,size, particle.color);
            //Renderer2D::DrawCircle(model, particle.color);
        }
    }


}