#pragma once
#include "GT/Renderer/Framebuffer.h"

namespace GT
{
	class OpenGlFramebuffer : public Framebuffer
	{
	public:
		OpenGlFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGlFramebuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t Width, uint32_t Height) override;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override;
		virtual const FramebufferSpecification& GetSpecification() const override;

	private:
		uint32_t m_RendererID = 0;
		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecs;
		FramebufferTextureSpecification m_DepthAttachmentSpec;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;
	};
}


