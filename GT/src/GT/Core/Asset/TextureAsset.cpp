#include "gtpch.h"
#include "TextureAsset.h"

#include "GT/Renderer/Renderer.h"
#include "GT/Platform/OpenGL/OpenGLTexture.h"
#include "GT/Math/Math.h"

namespace GT
{

	Ref<Texture2DAsset> Texture2DAsset::Create(const std::filesystem::path& path)
	{
		GT_CORE_TRACE("Loading [Texture2D] from path: {0}", path);
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			GT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(path);
		}

		GT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2DAsset> Texture2DAsset::Create(const int width, const int height)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				GT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLTexture2D>(width, height);
		}

		GT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;		
	}

	Ref<Texture2DAsset> Texture2DAsset::Create(TextureSpecification& spec, Buffer& data)
	{
		return Ref<Texture2DAsset>();
	}

	Ref<Texture2DAsset> Texture2DAsset::Create(TextureSpecification& spec, const std::filesystem::path& data)
	{
		return Ref<Texture2DAsset>();
	}

}