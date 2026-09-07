#include "gtpch.h"
#include "stb_image.h"
#include "GT/Core/Buffer.h"
#include "TextureAssetImporter.h"
	#include "TextureAsset.h"

namespace GT
{
	Ref<Asset> TextureAssetImporter::ImportTexture2D(const AssetMetadata& metadata)
	{
		GT_PROFILE_FUNCTION();

		Ref<Texture2DAsset> texture = Texture2DAsset::Create(metadata.FilePath);
		if (!texture)
			return nullptr;
		texture->ID = metadata.ID;
		texture->Name = metadata.Name;
		return texture;
	}
	
	Ref<Asset> TextureAssetImporter::LoadTexture2D(const std::filesystem::path& path)
	{
		return Texture2DAsset::Create(path);
	}
}