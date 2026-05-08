#include "gtpch.h"
#include "RenderCommand.h"

#include "GT/Platform/OpenGL/OpenGLRendererAPI.h"

namespace GT
{
	RendererAPI* RenderCommand::s_RendererAPI = nullptr;

	void RenderCommand::Init()
	{
		GT_PROFILE_FUNCTION();
		if (s_RendererAPI) return;

		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			GT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			break;

		case RendererAPI::API::OpenGL:
			s_RendererAPI = new OpenGLRendererAPI();
			break;

		default:
			GT_CORE_ASSERT(false, "Unknown RendererAPI!");
			break;
		}
		s_RendererAPI->Init();
	}
}