#include "gtpch.h"
#include "OpenGlFramebuffer.h"
#include "glad/glad.h"

namespace GT
{
	namespace Utils
	{
		static GLenum TextureTarget(bool multisample)
		{
			return multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}
		static void CreateTextures(bool multisample, uint32_t* outID, uint32_t count)
		{
			glCreateTextures(TextureTarget(multisample), count, outID);
		}
		static void BindTexture(bool multisample, uint32_t id)
		{
			glBindTexture(TextureTarget(multisample), id);
		}
		static void AttachColorTexture(uint32_t id, int samples, GLenum internalformat, GLenum format, uint32_t width, uint32_t height, int index)
		{
			bool multisample = samples > 1;
			if (multisample)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalformat, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				
			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisample), id, 0);
		}
		static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
		{
			bool multisample = samples > 1;
			if (multisample)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

				//// 防止采样超出边界时出现阴影泄漏
		//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
		//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
		//float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // 超出边界视为无阴影（白色）
		//GLCall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor));


			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisample), id, 0);
		}
		static bool IsDepthFormat(FramebufferTextureFormate format)
		{
			switch (format)
			{
			case FramebufferTextureFormate::Depth24Stencil8:
				return true;
			}
			return false;
		}
		static GLenum GTFBTextureFormatToGL(FramebufferTextureFormate format)
		{
			switch (format)
			{
			case FramebufferTextureFormate::RGBA8: return GL_RGBA8;
			case FramebufferTextureFormate::RED_INTEGER: return GL_RED_INTEGER;
			}
			GT_CORE_ASSERT(false, "Unknown framebuffer texture format!");
			return 0;
		}
	}

	OpenGlFramebuffer::OpenGlFramebuffer(const FramebufferSpecification& spec)
		:m_Specification(spec)
	{
		for (auto format : m_Specification.Attachments.Attachments)
		{
			if (Utils::IsDepthFormat(format.TextureFormat))
				m_DepthAttachmentSpec = format;
			else 
				m_ColorAttachmentSpecs.emplace_back(format);
		}
		Invalidate();
	}
	OpenGlFramebuffer::~OpenGlFramebuffer()
	{
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
		glDeleteFramebuffers(1, &m_RendererID);
	}
	void OpenGlFramebuffer::Invalidate()
	{
		if (m_RendererID)
		{
			glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);
			glDeleteFramebuffers(1, &m_RendererID);

			m_ColorAttachments.clear();
			 m_DepthAttachment = 0;
			 m_RendererID = 0;
		}

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);


		bool multisample = m_Specification.Samples > 1;
		// Attachments
		if (m_ColorAttachmentSpecs.size())
		{
			m_ColorAttachments.resize(m_ColorAttachmentSpecs.size());
			Utils::CreateTextures(multisample, m_ColorAttachments.data(), m_ColorAttachments.size());

			for (size_t i = 0;i < m_ColorAttachments.size();i++)
			{
				Utils::BindTexture(multisample, m_ColorAttachments[i]);
				switch (m_ColorAttachmentSpecs[i].TextureFormat)
				{
				case FramebufferTextureFormate::RGBA8:
					Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGBA8,GL_RGBA, m_Specification.Width, m_Specification.Height, i);
					break;
				case FramebufferTextureFormate::RED_INTEGER:
					Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_R32I, GL_RED_INTEGER, m_Specification.Width, m_Specification.Height, i);
					break;
				}
			}
		}
		if(m_DepthAttachmentSpec.TextureFormat != FramebufferTextureFormate::None)
		{
			Utils::CreateTextures(multisample, &m_DepthAttachment, 1);
			Utils::BindTexture(multisample, m_DepthAttachment);
			switch (m_DepthAttachmentSpec.TextureFormat)
			{
			case FramebufferTextureFormate::Depth24Stencil8:
				Utils::AttachDepthTexture(m_DepthAttachment, m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
				break;
			}
		}

		if (m_ColorAttachments.size() > 1)
		{
			GT_CORE_ASSERT(m_ColorAttachments.size() <= 4, "GT only supports 4 color attachments!");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_ColorAttachments.size(), buffers);
		}
		else if(m_ColorAttachments.size() == 0)
		{
			// Only depth-pass
			glDrawBuffer(GL_NONE);
		}


		GT_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER)==GL_FRAMEBUFFER_COMPLETE,"Framebuffer is incomplete!");

		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void OpenGlFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);

		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTex, 0);
	}
	void OpenGlFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void OpenGlFramebuffer::Resize(uint32_t Width, uint32_t Height)
	{
		if (Width == 0 || Height == 0 || Width > 2560 || Height > 1440)
		{
			GT_CORE_WARN("Attemted to resize framebuffer to {0}, {1}", Width, Height);
			return;
		}
		if (m_Specification.Width == Width, m_Specification.Height == Height) return;

		m_Specification.Width = Width;
		m_Specification.Height = Height;
		Invalidate();
	}
	int OpenGlFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{
		GT_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "Color attachment index out of range!");
		Bind();
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}
	void OpenGlFramebuffer::ClearAttachment(uint32_t attachmentIndex, int value)
	{
		GT_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "Color attachment index out of range!");
		
		auto& spec = m_ColorAttachmentSpecs[attachmentIndex];

		glClearTexImage(m_ColorAttachments[attachmentIndex], 0,
			Utils::GTFBTextureFormatToGL(spec.TextureFormat)
			//GL_RED_INTEGER
			, GL_INT, &value);
		//GT_CORE_WARN("{0}",glGetError());
	}
	uint32_t OpenGlFramebuffer::GetColorAttachmentRendererID(uint32_t index) const
	{
		GT_CORE_ASSERT(index < m_ColorAttachments.size(), "Color attachment index out of range!");
		return m_ColorAttachments[index];
	}
	uint32_t OpenGlFramebuffer::GetDepthAttachmentRendererID() const
	{
		return m_DepthAttachment;
	}
	const FramebufferSpecification& OpenGlFramebuffer::GetSpecification() const
	{
		return m_Specification;
	}
}