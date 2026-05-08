#include "gtpch.h"
#include "OpenGLTexture2D.h"
#include "stb_image.h"
#include <glad/glad.h>
#include "GT/Math/Math.h"

namespace GT
{

	OpenGLTexture2D::OpenGLTexture2D(const int width, const int height)
		:m_Width(width), m_Height(height)
	{
		GT_PROFILE_FUNCTION();
		m_DataFormat = GL_RGBA;
		m_InternalFormat = GL_RGBA8;
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);



	}
	OpenGLTexture2D::OpenGLTexture2D(const std::filesystem::path& path)
		:m_Path(path)
	{
		GT_PROFILE_FUNCTION();
		m_Name = path.stem().string();

		// Load image data using stb_image or similar library
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		unsigned char* data = nullptr;
		{
			GT_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string& path)");
			data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);
		}

		if (data)
		{
			m_IsLoaded = true;
			m_Width = width;
			m_Height = height;
			ID = Math::fnv1a(path.string().c_str());

			GLenum internalFormat = 0, dataFormat = 0;
			if (channels == 4)
			{
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else if (channels == 3)
			{
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}
			else if (channels == 2) {
				internalFormat = GL_RG8;   // 或 GL_RG8UI / GL_RG16 等
				dataFormat = GL_RG;
			}
			else if (channels == 1) {
				internalFormat = GL_R8;    // 或 GL_R8UI / GL_R16 等
				dataFormat = GL_RED;
			}
			else {
				GT_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");
			}
			
			m_InternalFormat = internalFormat;
			m_DataFormat = dataFormat;

			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);

			glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);


			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else GT_CORE_ERROR("Failed to load image '{0}'", path.string());

	}
	OpenGLTexture2D::~OpenGLTexture2D()
	{
		GT_PROFILE_FUNCTION();
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(unsigned int slot) const
	{
		GT_PROFILE_FUNCTION();
		glBindTextureUnit(slot, m_RendererID);
	}

	void OpenGLTexture2D::Unbind() const
	{
		GT_PROFILE_FUNCTION();
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGLTexture2D::SetData(void* data, unsigned int size)
	{
		GT_PROFILE_FUNCTION();
		unsigned int bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		GT_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

}