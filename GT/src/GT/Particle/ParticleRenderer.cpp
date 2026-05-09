#include "gtpch.h"
#include "ParticleRenderer.h"
#include "GT/Renderer/Renderer.h"
#include "GT/Renderer/Shader.h"
#include "GT/Renderer/Texture2D.h"
#include "GT/Renderer/Buffer.h"
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"
#include <algorithm>

namespace GT {

    void ParticleRenderer::RenderParticles(const std::vector<Particle>& particles, const glm::mat4& transform)
    {
        if (particles.empty()) return;

        for (const Particle& particle : particles) {
            if (particle.lifeRemaining == 0.0f) continue;


            glm::mat4 rotation = glm::toMat4(glm::quat(particle.rotation));

            glm::mat4 model = glm::translate(transform, particle.position) *
                rotation *
                glm::scale(glm::mat4(1.0f), glm::vec3(particle.size));


			Renderer2D::DrawQuad(model, particle.color);
            //Renderer2D::DrawCircle(model, particle.color);
        }
    }


}