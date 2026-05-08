#include "gtpch.h"
#include "TextureSheet.h"
namespace GT
{
	GT::TextureSheet::TextureSheet(const Ref<Texture2D>& texture)
		: m_Texture(texture)
	{
	}

	GT::TextureSheet::TextureSheet(const std::string& path)
	{
		m_Texture = Texture2D::Create(path);
	}

	void GT::TextureSheet::Bind(unsigned int slot) const
	{
		m_Texture->Bind(slot);
	}

	void GT::TextureSheet::Unbind() const
	{
		m_Texture->Unbind();
	}

	unsigned int GT::TextureSheet::GetWidth() const
	{
		return m_Texture->GetWidth();
	}

	unsigned int GT::TextureSheet::GetHeight() const
	{
		return m_Texture->GetHeight();
	}

	unsigned int GT::TextureSheet::GetRendererID() const
	{
		return m_Texture->GetRendererID();
	}

	void TextureSheet::AddSpriteCoords(const char* name, const TexCoords& coords)
	{
		m_TexCoords[name] = coords;
	}

	void TextureSheet::AddSpriteCoords(const char* name, int x, int y, int width, int height)
	{
		auto it = m_TexCoords.find(name);
		if (it == m_TexCoords.end())
		{
			float min[2], max[2];
			min[0] = (float)x / (float)GetWidth();
			min[1] = (float)y / (float)GetHeight();
			max[0] = (float)(x + width) / (float)GetWidth();
			max[1] = (float)(y + height) / (float)GetHeight();

			TexCoords coords;
			coords.texcoord[0] = { min[0], min[1] };
			coords.texcoord[1] = { max[0], min[1] };
			coords.texcoord[2] = { max[0], max[1] };
			coords.texcoord[3] = { min[0],  max[1] };
			m_TexCoords[name] = coords;
		}
		else
		{
			GT_CORE_WARN("Sprite Name '{0}' already exists in TextureSheet. Skipping AddSpriteCoords.", name);
		}
	}

	TexCoords TextureSheet::GetSpriteCoords(const std::string& name) const
	{
		auto it = m_TexCoords.find(name);
		if (it != m_TexCoords.end())
		{
			return it->second;
		}
		else
		{
			GT_CORE_WARN("Sprite '{0}' not found in TextureSheet.", name);
			return m_TexCoords.begin()->second; // Return first as fallback
		}
	}

	bool GT::TextureSheet::operator==(const Texture& other) const
	{
		return m_Texture->operator==(other);
	}

	bool GT::TextureSheet::operator==(const TextureSheet& other) const
	{
		return m_Texture == other.m_Texture;
	}

}

