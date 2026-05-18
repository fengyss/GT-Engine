// ShadowMap.cpp
#include "gtpch.h"
#include <glad/glad.h>
#include "ShadowMap.h"
namespace GT
{


    ShadowMap::ShadowMap(unsigned int width, unsigned int height)
        : m_Width(width), m_Height(height)
    {
        GLCall(glGenFramebuffers(1, &m_FBO));

        // 1. 生成深度纹理
        GLCall(glGenTextures(1, &m_DepthTexture));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_DepthTexture));

        // 2. 设置纹理格式：只存储深度（GL_DEPTH_COMPONENT）
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
            width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr));

        // 3. 纹理过滤与环绕方式（非常重要！）
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        // 防止采样超出边界时出现阴影泄漏
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
        float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // 超出边界视为无阴影（白色）
        GLCall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor));

        // 4. 绑定 FBO 并附加深度纹理
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));
        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture, 0));

        // 5. 显式告诉 OpenGL：我们不读取颜色数据（提高效率）
        GLCall(glDrawBuffer(GL_NONE));
        GLCall(glReadBuffer(GL_NONE));

        // 6. 检查 FBO 是否完整
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::SHADOWMAP::FRAMEBUFFER_INCOMPLETE" << std::endl;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    void ShadowMap::Bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    }
    void ShadowMap::Unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}