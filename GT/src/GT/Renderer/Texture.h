#pragma once
#include "glm/glm.hpp"
#include "GT/Core/Log.h"
#include <string>
#include "GT/Core/Asset/Asset.h"
namespace GT
{
	struct TexCoords
	{
		glm::vec2 texcoord[4];
	};

	enum TextureType
	{
		TextureTypeNone = 0,
		Diffuse = 1u << 31,
		Specular = 1u << 30,
		Normal = 1u << 29,
		Height = 1u << 28,
		Emission = 1u << 27,

	};
	struct Image
	{
		unsigned char* data;
		int height, width, channels;
	};
	static std::string GetStrOfType(TextureType type)
	{
		switch (type)
		{
		case GT::Diffuse:
			return "texture_diffuse";
			break;
		case GT::Specular:
			return "texture_specular";
			break;
		case GT::Normal:
			return "texture_normal";
			break;
		case GT::Height:
			return "texture_height";
			break;
		case GT::Emission:
			return "texture_emission";
			break;
		default:
			GT_CORE_ASSERT(false,"Unknown TextureType!");
			break;
		}
	}
	class Texture
	{
	public:
		virtual ~Texture() = default;
		virtual void Bind(unsigned int slot = 0) const = 0;
		virtual void Unbind() const = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual bool IsLoaded() const = 0;

		virtual unsigned int GetRendererID() const = 0;	
		virtual const std::filesystem::path& GetPath() const = 0;
		virtual const std::string& GetName() const = 0;
		virtual Image GetData() const = 0;


		virtual void SetType(TextureType type) = 0;

		virtual TextureType GetType() const = 0;

		virtual void SetData(void* data, unsigned int size) = 0;

		virtual bool operator==(const Texture& other) const = 0;

	};
	class Texture2D : public Texture
	{
	public:
		virtual ~Texture2D() = default;


		static Ref<Texture2D> Create(const std::filesystem::path& path);
		static Ref<Texture2D> Create(const int width,const int height);
	};


	class TextureLibrary
	{
	public:

		Ref<Texture2D> Load(uint32_t ID, const std::filesystem::path& filepath);
		Ref<Texture2D> Reload(uint32_t ID, const std::filesystem::path& filepath);
		void Clear() { m_Textures.clear(); }
		Ref<Texture2D> Get(uint32_t ID);

		bool Exists(uint32_t ID) const
		{
			return m_Textures.find(ID) != m_Textures.end();
		}
	private:
		void Add(uint32_t ID, const Ref<Texture2D>& texture);
		std::unordered_map<uint32_t, Ref<Texture2D>> m_Textures;
	};
}


