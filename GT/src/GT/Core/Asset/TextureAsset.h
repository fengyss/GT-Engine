#pragma once
#include "glm/glm.hpp"

#include "GT/Core/Log.h"
#include "GT/Core/Base.h"
#include "GT/Core/Buffer.h"
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
	enum class ImageFormat
	{
		None = 0,
		R8,
		RGB8,
		RGBA8,
		RGBA32F
	};
	struct TextureSpecification
	{
		uint32_t Width = 1;
		uint32_t Height = 1;
		ImageFormat Format = ImageFormat::RGBA8;
		bool GenerateMips = true;
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
	class TextureAsset : public Asset
	{
	public:
		virtual ~TextureAsset() = default;

		virtual void Bind(unsigned int slot = 0) const = 0;
		virtual void Unbind()  const = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual bool IsLoaded() const = 0;

		virtual unsigned int GetRendererID() const = 0;	
		virtual const std::filesystem::path& GetPath() const = 0;
		virtual Image GetData() const = 0;

		virtual uint32_t GetMemorySize() const = 0;


		virtual void SetData(void* data, unsigned int size) const = 0;

		virtual bool operator==(const TextureAsset& other) const = 0;


	};

	class Texture2DAsset : public TextureAsset
	{
	public:
		virtual ~Texture2DAsset() = default;

		static AssetType GetStaticType() { return AssetType::Texture2D; }
		
		virtual AssetType GetType() const override { return GetStaticType(); }

		static Ref<Texture2DAsset> Create(const std::filesystem::path& path);
		static Ref<Texture2DAsset> Create(const int width,const int height);
		static Ref<Texture2DAsset> Create(TextureSpecification& spec, Buffer& data);
		static Ref<Texture2DAsset> Create(TextureSpecification& spec, const std::filesystem::path& data);
	};


}


