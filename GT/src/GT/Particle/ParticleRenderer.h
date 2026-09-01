#pragma once
#include "GT/Renderer/Camera.h"
#include "GT/Renderer/Texture.h"
#include "GT/Renderer/EditorCamera.h"
#include "GT/Renderer/OrthographicCamera.h"
#include "ParticleTypes.h"

#include "GT/Core/Asset/Asset.h"

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
        static void ShutDown();
        static void BeginScene(Camera& camera);
        static void EndScene();
        static void SetViewProjection(const glm::mat4& viewProjection);

        static void RenderParticles(BlendMode mode, const std::vector<Ref<Particle>> particles, const Texture2D& tex);

        static float GetTextureSlotIndex(const Texture2D& texture);

        static void Flush(BlendMode mode);
    private:
    };
} 