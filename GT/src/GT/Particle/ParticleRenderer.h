#pragma once
#include "ParticleTypes.h"
namespace GT {

    class ParticleRenderer {
    public:
        ParticleRenderer();
        ~ParticleRenderer();

        static void RenderParticles(const std::vector<Particle>& particles, const glm::mat4& transform);
    private:
    };
} 