#include "gtpch.h"
#include "ShaderAsset.h"
#include "GT/Renderer/Renderer.h"
#include "GT/Platform/OpenGL/OpenGLShader.h"

namespace GT
{
	Ref<ShaderAsset> ShaderAsset::Create(const std::filesystem::path& filepath)
	{

		GT_CORE_TRACE("Loading [Shader] from path: {0}", filepath);

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			GT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return  CreateRef<OpenGLShader>(filepath);
		}
		GT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	Ref<ShaderAsset> ShaderAsset::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			GT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}
		GT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<ShaderAsset> ShaderAsset::CreateCompute(const std::filesystem::path& filepath)
	{
		GT_CORE_TRACE("Loading [Compute Shader] from path: {0}", filepath.string());
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			GT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLShader>(filepath, ShaderType::Compute);
		}
		GT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
		
	}

	Ref<ShaderAsset> ShaderAsset::CreateGeometry(const std::filesystem::path& filepath)
	{
		GT_CORE_TRACE("Loading [Geometry Shader] from path: {0}", filepath.string());
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			GT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLShader>(filepath, ShaderType::Geometry);
		}
		GT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}