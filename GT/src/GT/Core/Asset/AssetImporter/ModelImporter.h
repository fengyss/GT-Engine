#pragma once
#include "GT/Core/Base.h"
#include "GT/Core/Asset/Asset.h"
#include "GT/Core/Asset/AssetMetadata.h"
namespace GT
{
	class ModelImporter
	{
	public:
		// This function should be called when meta file is not existed!
		// This function should not be called, 
		// because when model is created there should have a meta file,
		// call this function will return nullptr and log a warning meesage
		static Ref<AssetMetadata> GeneratorMetadataFromPath(const std::filesystem::path& path);

		// load model from metadata, and return a Ref<Asset>.
		// metadata should already registered at AssetManager,
		// In Debug mode will check, if not will register first
		static Ref<Asset> ImportModel(const Ref<AssetMetadata> metadata);

		// This function should be called when meta file is not existed!
		// generator a meta file based on model file
		static Ref<Asset> ImportModel(const std::filesystem::path& path);


		static Ref<Asset> ReloadModel(const std::filesystem::path& path);
	};
}

