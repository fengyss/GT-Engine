#include "gtpch.h"
#include "ParticleRenderer.h"
#include "GT/Renderer/Renderer.h"
#include "GT/Renderer/Shader.h"
#include "GT/Renderer/Texture2D.h"
#include "GT/Renderer/Buffer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

namespace GT {

    void ParticleRenderer::RenderParticles(const std::vector<Particle>& particles, const glm::mat4& transform)
    {
        if (particles.empty()) return;

        for (const Particle& particle : particles) {
            if (particle.lifeRemaining == 0.0f) continue;

            glm::mat4 model = glm::scale(transform, glm::vec3(particle.size));
            //model = glm::rotate(model, particle.rotation, glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::translate(model, particle.position);


			Renderer2D::DrawQuad(model, particle.color);
            //Renderer2D::DrawCircle(model, particle.color);
        }
    }


}