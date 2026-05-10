#pragma once
#include "GT/Renderer/Texture.h"
namespace GT
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::filesystem::path& path);
		OpenGLTexture2D(const int width,const int height);
		~OpenGLTexture2D();

		virtual void Bind(unsigned int slot = 0) const override;
		virtual void Unbind() const override;

		virtual bool IsLoaded() const override { return m_IsLoaded; };

		virtual unsigned int GetRendererID() const override { return m_RendererID; }
		virtual const std::filesystem::path& GetPath() const override { return m_Path; }
		virtual const std::string& GetName() const override { return m_Name; }
		virtual TextureType GetType() const override { return m_TextureType; }

		virtual Image GetData() const override;

		virtual unsigned int GetWidth() const override { return m_Width; }
		virtual unsigned int GetHeight() const override { return m_Height; }

		virtual void SetData(void* data, unsigned int size) override;
		virtual void SetType(TextureType type) override { m_TextureType = type; }

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == other.GetRendererID();
		}
	private:
		bool m_IsLoaded = false;
		std::filesystem::path m_Path;
		std::string m_Name;

		uint32_t ID = 0;

		TextureType m_TextureType = TextureType::TextureTypeNone;

		unsigned int m_Width = 0, m_Height = 0;
		unsigned int m_RendererID = 0;
		unsigned int m_InternalFormat = 0, m_DataFormat = 0;
	};
}


