#pragma once
#include "GT/Core/Base.h"
#include "GT/Core/Asset/Asset.h"
#include "GT/Core/Asset/AssetMetadata.h"
namespace GT
{
	class TextureImporter
	{
	public:
		static Ref<AssetMetadata> GeneratorMetadataFromPath(const std::filesystem::path& path);

		// load Texture2D from metadata, and return a Ref<Asset>.
		// metadata should already registered at AssetManager,
		// In Debug mode will check, if not will register first
		static Ref<Asset> ImportTexture2D(const Ref<AssetMetadata> metadata);

		// This function should be called when meta file is not existed!
		// load Texture2D from file path, and generate a metadata for the shader,
		// and register the metadata to AssetManager.
		// return a Ref<Asset> to the shader.
		static Ref<Asset> ImportTexture2D(const std::filesystem::path& path);


		static Ref<Asset> ReloadTexture2D(const std::filesystem::path& path);


	};
}

