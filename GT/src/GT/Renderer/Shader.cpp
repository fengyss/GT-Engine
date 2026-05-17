#include "gtpch.h"
#include "Shader.h"
#include "Renderer.h"
#include "GT/Platform/OpenGL/OpenGLShader.h"

namespace GT
{
	Ref<Shader> Shader::Create(const std::filesystem::path& filepath)
	{

		GT_CORE_TRACE("Loading [Shader] from path: {0}", filepath.string());

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
	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
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

	Ref<Shader> Shader::CreateCompute(const std::filesystem::path& filepath)
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

	Ref<Shader> Shader::CreateGeometry(const std::filesystem::path& filepath)
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



	void ShaderLibrary::Add(uint32_t ID, const Ref<Shader>& shader)
	{
		m_Shaders[ID] = shader;
	}
	Ref<Shader> ShaderLibrary::Load(uint32_t ID,const std::filesystem::path& filepath)
	{
		if (Exists(ID)) 
		{
			GT_CORE_INFO("Shader {1} with ID {0} already loaded!", ID, filepath.filename().string());
			return m_Shaders[ID];
		}
		else {
			auto shader = Shader::Create(filepath);
			Add(ID, shader);
			return shader;
		}
		
	}
	Ref<Shader> ShaderLibrary::Reload(uint32_t ID, const std::filesystem::path& filepath)
	{
		GT_CORE_WARN("Shader {1} with ID {0} reloaded!", ID, filepath.filename().string());
		m_Shaders[ID] = Shader::Create(filepath);
		int samplers[32];
		for (int i = 0;i < 32;i++) samplers[i] = i;
		m_Shaders[ID]->Bind();
		m_Shaders[ID]->SetUniformiv("u_Textures", samplers, 32);
		return m_Shaders[ID];
	}
	Ref<Shader> ShaderLibrary::Get(uint32_t ID)
	{
		if (Exists(ID)) return m_Shaders[ID];
		else
		{
			GT_CORE_ERROR("Shader ID:{0} not found in library!", ID);
			return nullptr;
		}
	}
}