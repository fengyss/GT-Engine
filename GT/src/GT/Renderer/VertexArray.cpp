#include "gtpch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "GT/Platform/OpenGL/OpenGLVertexArray.h"

namespace GT
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			GT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexArray>();
		}
		GT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}