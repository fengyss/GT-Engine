#pragma once
#include "GT/Scene/SceneSerializer.h"

#include "GT/Core/Base.h"
#include "GT/Core/Asset/Asset.h"
#include "GT/Core/Asset/AssetMetadata.h"
namespace GT
{
	class SceneSerializer;

	class SceneImporter
	{
	public:
		// load scene from metadata, and return a Ref<Asset>.
		// metadata should already registered at AssetManager,
		// In Debug mode will check, if not will register first
		static Ref<Asset> ImportScene(const Ref<AssetMetadata> metadata);

		// This function should be called when meta file is not existed!
		// will generate a meta file based on scene file 
		static Ref<Asset> ImportScene(const std::filesystem::path& path);
	private:
		static SceneSerializer serializer;
	};
}

