#pragma once
#include "Asset.h"
namespace GT
{
	class SceneAssetImporter
	{
	public:
		// AssetMetadata filepath is relative to project asset directory
		static Ref<Asset> ImportScene(const AssetMetadata& metadata);

		// Reads file directly from filesystem
		// (i.e. path has to be relative / absolute to working directory)
		static Ref<Asset> LoadScene(const std::filesystem::path& path);
	};
}

