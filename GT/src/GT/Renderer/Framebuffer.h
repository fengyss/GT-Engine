#pragma once
#include "GT/Core/Base.h"

namespace GT
{
	enum class FramebufferTextureFormate
	{
		None = 0,
		// Color
		RGBA8,
		RED_INTEGER,

		// Depth/stencil
		Depth24Stencil8,
		// Defaults
		Depth = Depth24Stencil8,
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormate format)
			:TextureFormat(format) {}

		FramebufferTextureFormate TextureFormat = FramebufferTextureFormate::None;
		// TODO: filtering/wrap
	};
	struct FramebufferAttachmentSpeciation
	{
		FramebufferAttachmentSpeciation() = default;
		FramebufferAttachmentSpeciation(std::initializer_list<FramebufferTextureSpecification> attachments)
			: Attachments(attachments) {}

		std::vector<FramebufferTextureSpecification> Attachments;
	};

	struct FramebufferSpecification
	{
		uint32_t Width, Height;
		FramebufferAttachmentSpeciation Attachments;

		uint32_t Samples = 1;

		bool SwapChainTarget = false; //glframebufferswap(0)
	};

	class Framebuffer
	{
	public:

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t Width, uint32_t Height) = 0;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;
		virtual const FramebufferSpecification& GetSpecification() const = 0;

	private:

	};
}


