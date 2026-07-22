#pragma once
#include "Asset.h"
namespace GT
{
	class ModelAssetImporter
	{
	public:
		// AssetMetadata filepath is relative to project asset directory
		static Ref<Asset> ImportModel(const AssetMetadata& metadata);

		// Reads file directly from filesystem
		// (i.e. path has to be relative / absolute to working directory)
		static Ref<Asset> LoadModel(const std::filesystem::path& path);
	};
}

