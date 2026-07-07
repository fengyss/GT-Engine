#pragma once
#include <glm/glm.hpp>
#include "GT/Renderer/Framebuffer.h"

namespace GT
{
    class ShadowMap {
    public:
        ShadowMap() = default;
        ShadowMap(unsigned int width, unsigned int height);
        ~ShadowMap() = default;

        void Bind() const;   // 绑定 FBO，准备写入
        void Unbind() const; // 解绑，恢复默认帧缓冲

        uint32_t GetDepthTextureID();

        inline glm::mat4 GetLightSpaceMatrix() const { return m_LightSpaceMatrix; }

        unsigned int GetWidth() { return m_Width; }
        unsigned int GetHeight() { return m_Height; }

    private:
        unsigned int m_Width, m_Height;

        glm::mat4 m_LightSpaceMatrix;

		Ref<Framebuffer> m_Framebuffer;
    };
}