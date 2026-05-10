#pragma once
#include "GT/Renderer/Texture.h"

namespace GT
{

	class TextureSheet  : public Texture2D
	{
	public:
		TextureSheet(const Ref<Texture2D>& texture);
		TextureSheet(const std::string& path);


		virtual void Bind(unsigned int slot = 0) const override;
		virtual void Unbind() const override;

		virtual unsigned int GetWidth() const override;
		virtual unsigned int GetHeight() const override;

		virtual bool IsLoaded() const override { return m_Texture->IsLoaded(); };

		virtual void SetData(void* data, unsigned int size) override {};

		virtual unsigned int GetRendererID() const override;
		virtual const std::filesystem::path& GetPath() const override { return m_Texture->GetPath(); };
		virtual const std::string& GetName() const override { return m_Texture->GetName(); };

		void AddSpriteCoords(const char* name, const TexCoords& coords);
		void AddSpriteCoords(const char* name, int x, int y, int width, int height);

		Ref<Texture2D> GetTexture() const { return m_Texture; }
		TexCoords GetSpriteCoords(const std::string& name) const;


		virtual bool operator==(const Texture& other) const override;
		virtual bool operator==(const TextureSheet& other) const ;
	private:
		Ref<Texture2D> m_Texture;
		std::unordered_map<std::string, TexCoords> m_TexCoords;
	};
}


