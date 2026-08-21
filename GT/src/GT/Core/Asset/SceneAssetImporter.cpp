#include "gtpch.h"
#include "SceneAssetImporter.h"
#include "GT/Scene/SceneSerializer.h"
namespace GT
{
	Ref<Asset> SceneAssetImporter::ImportScene(const AssetMetadata& metadata)
	{
		Ref<Scene> scene = CreateRef<Scene>();
		SceneSerializer serializer(scene);
		serializer.Deserialize(metadata.FilePath);
		return scene;
	}
	Ref<Asset> SceneAssetImporter::LoadScene(const std::filesystem::path& path)
	{
		return Ref<Asset>();
	}
}
