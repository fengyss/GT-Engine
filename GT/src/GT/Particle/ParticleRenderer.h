#pragma once
#include "GT/Renderer/Camera.h"
#include "GT/Renderer/EditorCamera.h"
#include "GT/Renderer/OrthographicCamera.h"
#include "ParticleTypes.h"
#include "GT/Assets/AssetsHandle.h"
namespace GT {

    struct ParticleGPUVertex {
        glm::vec3 position; // ÊÀ½ç×ø±ê
        glm::vec4 color;
        float size;
        float texIndex = 0.0f;
    };

    class ParticleRenderer {
    public:
        ~ParticleRenderer() = default;
        static void Init();
        static void BeginScene(Camera& camera, glm::mat4& transform);
        static void BeginScene(EditorCamera& camera);
        static void BeginScene(OrthographicCamera& camera);
        static void EndScene();
        static void SetViewProjection(const glm::mat4& viewProjection);

        static void RenderParticles(const std::vector<Ref<Particle>> particles);
        static void RenderParticles(const std::vector<Ref<Particle>> particles,BlendMode mode);
        static void RenderParticles(BlendMode mode, const std::vector<Ref<Particle>> particles, const RefHandle<Texture2D> tex = nullptr);
        static void RenderParticles(const std::vector<Ref<Particle>> particles, const RefHandle<Texture2D> tex);
        static void RenderParticle(const Ref < Particle> particle);


        static float GetTextureSlotIndex(const Ref<Texture2D>& texture);

        static void Flush(BlendMode mode);
    private:
    };
} 