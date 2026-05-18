// ShadowMap.h
#pragma once
#include <glm/glm.hpp>

typedef unsigned int GLuint;

namespace GT
{
    class ShadowMap {
    public:
        ShadowMap() = default;
        ShadowMap(unsigned int width, unsigned int height);
        ~ShadowMap() = default;

        void Bind() const;   // 绑定 FBO，准备写入
        void Unbind() const; // 解绑，恢复默认帧缓冲

        inline GLuint GetDepthTextureID() const { return m_DepthTexture; }
        inline glm::mat4 GetLightSpaceMatrix() const { return m_LightSpaceMatrix; }

        unsigned int GetWidth() { return m_Width; }
        unsigned int GetHeight() { return m_Height; }

    private:
        GLuint m_FBO;          // 帧缓冲对象
        GLuint m_DepthTexture; // 深度纹理 ID
        unsigned int m_Width, m_Height;
        glm::mat4 m_LightSpaceMatrix;
    };
}