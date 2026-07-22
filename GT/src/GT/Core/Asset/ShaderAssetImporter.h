#pragma once
#include "Asset.h"
namespace GT
{
	class ShaderAssetImporter
	{
	public:
		// AssetMetadata filepath is relative to project asset directory
		static Ref<Asset> ImportShader(const AssetMetadata& metadata);

		// Reads file directly from filesystem
		// (i.e. path has to be relative / absolute to working directory)
		static Ref<Asset> LoadShader(const std::filesystem::path& path);
	};
}
