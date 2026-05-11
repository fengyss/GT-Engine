#pragma once
#include "ParticleTypes.h"
#include "GT/Assets/AssetsHandle.h"
namespace GT {

    class ParticleRenderer {
    public:
        ParticleRenderer();
        ~ParticleRenderer();

        static void RenderParticles(const std::vector<Ref<Particle>> particles);
        static void RenderParticles(const std::vector<Ref<Particle>> particles, const RefHandle<Texture2D> tex);
        static void RenderParticle(const Ref < Particle> particle);
        static void Flush(BlendMode mode);
    private:
    };
} 