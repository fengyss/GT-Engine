#pragma once
#include "GT/Core/Base.h"
#include "GT/Core/Asset/Asset.h"
#include "GT/Core/Asset/AssetMetadata.h"
namespace GT
{
	class ShaderImporter
	{
	public:
		static Ref<AssetMetadata> GeneratorMetadataFromPath(const std::filesystem::path& path);

		// load shader from metadata, and return a Ref<Asset> to the shader.
		// metadata should already registered at AssetManager,
		// In Debug mode will check, if not will register first
		static Ref<Asset> ImportShader(const Ref<AssetMetadata> metadata);

		// This function should be called when meta file is not existed!
		// load shader from file path, and generate a metadata for the shader,
		// and register the metadata to AssetManager.
		// return a Ref<Asset> to the shader.
		static Ref<Asset> ImportShader(const std::filesystem::path& path);


		static Ref<Asset> ReloadShader(const std::filesystem::path& path);

	};
}
