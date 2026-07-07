// ShadowMap.cpp
#include "gtpch.h"
#include "ShadowMap.h"
namespace GT
{


    ShadowMap::ShadowMap(unsigned int width, unsigned int height)
        : m_Width(width), m_Height(height)
    {

        FramebufferSpecification fbSpec;
        fbSpec.Attachments = { FramebufferTextureFormate::Depth };
        fbSpec.Width = width;
        fbSpec.Height = height;
        m_Framebuffer = Framebuffer::Create(fbSpec);


        //// 防止采样超出边界时出现阴影泄漏
        //GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
        //GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
        //float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // 超出边界视为无阴影（白色）
        //GLCall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor));

    }
    void ShadowMap::Bind() const
    {
        m_Framebuffer->Bind();
    }
    void ShadowMap::Unbind() const
    {
        m_Framebuffer->Unbind();
    }
    uint32_t ShadowMap::GetDepthTextureID()
    {
        return m_Framebuffer->GetDepthAttachmentRendererID();
    }
}