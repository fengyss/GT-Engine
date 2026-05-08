#include "gtpch.h"
#include "Framebuffer.h"
#include "GT/Renderer/Renderer.h"
#include "GT/Platform/OpenGL/OpenGlFramebuffer.h"

namespace GT
{
	Ref<Framebuffer> GT::Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			GT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return  CreateRef<OpenGlFramebuffer>(spec);
		}
		GT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}

