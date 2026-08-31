#include "gtpch.h"
#include "GT/Scene/SceneSerializer.h"
#include "SceneImporter.h"
#include "../AssetManager.h"
namespace GT
{
	SceneSerializer SceneImporter::serializer;

	Ref<Asset> SceneImporter::ImportScene(const Ref<AssetMetadata> metadata)
	{

#if GT_DEBUG
		if (!AssetManager::ExistedUUID(metadata->ID)) 
			AssetManager::RegisterMetadata(*metadata);
#endif

		AssetManager::LoadDependenciesOfUUID(metadata->ID);

		Ref<Scene> scene = serializer.Deserialize(metadata->FilePath);


		scene->ID = metadata->ID;
		scene->Name = metadata->Name;

		return scene;
	}
	Ref<Asset> SceneImporter::ImportScene(const std::filesystem::path& path)
	{
		Ref<AssetMetadata> metadata = serializer.GenerateMetadataFromFile(path);
		AssetManager::RegisterMetadata(*metadata);

		return ImportScene(metadata);
	}
}
