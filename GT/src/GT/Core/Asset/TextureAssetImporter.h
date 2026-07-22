#pragma once
#include "Asset.h"
namespace GT
{
	class TextureAssetImporter
	{
	public:
		// AssetMetadata filepath is relative to project asset directory
		static Ref<Asset> ImportTexture2D(const AssetMetadata& metadata);

		// Reads file directly from filesystem
		// (i.e. path has to be relative / absolute to working directory)
		static Ref<Asset> LoadTexture2D(const std::filesystem::path& path);
	};
}

