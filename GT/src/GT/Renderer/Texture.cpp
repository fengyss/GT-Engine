#include "gtpch.h"
#include "Texture.h"

#include "GT/Renderer/Renderer.h"
#include "GT/Platform/OpenGL/OpenGLTexture.h"
#include "GT/Math/Math.h"

namespace GT
{

	Ref<Texture2D> Texture2D::Create(const std::filesystem::path& path)
	{
		GT_CORE_TRACE("Loading [Texture2D] from path: {0}", path.string());
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

	Ref<Texture2D> Texture2D::Create(const int width, const int height)
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

	Ref<Texture2D> Texture2D::Create(TextureSpecification& spec, Buffer& data)
	{
		return Ref<Texture2D>();
	}

	void TextureLibrary::Clear()
	{
		for(auto& [id, texture] : m_Textures)
		{
			texture.reset();
		}
	}

	void TextureLibrary::Add(uint32_t ID, const Ref<Texture2D>& texture)
	{
		m_Textures.emplace(ID, texture);
	}
	Ref<Texture2D> TextureLibrary::Load(uint32_t ID, const std::filesystem::path& filepath)
	{
		if (Exists(ID))
		{
			GT_CORE_INFO("Texture {1} with ID {0} already loaded!", ID, filepath.filename().string());
			return m_Textures[ID];
		}
		else 
		{
			auto tex = Texture2D::Create(filepath);
			Add(ID, tex);
			return tex;
		}
	}

	Ref<Texture2D> TextureLibrary::Reload(uint32_t ID, const std::filesystem::path& filepath)
	{
		GT_CORE_WARN("Texture {1} with ID {0} reloaded!", ID,filepath.filename().string());
		auto type = m_Textures[ID]->GetTextureType();
		m_Textures[ID] = Texture2D::Create(filepath);
		m_Textures[ID]->SetTextureType(type);
		return m_Textures[ID];
	}


	Ref<Texture2D> TextureLibrary::Get(uint32_t ID)
	{
		if(Exists(ID)) return m_Textures[ID];
		else
		{
			GT_CORE_ERROR("Texture not found in library: {0}", ID);
			return nullptr;
		}
	}
}