#include "gtpch.h"
#include "SceneAssetImporter.h"
#include "GT/Scene/SceneSerializer.h"
namespace GT
{
	Ref<Asset> SceneAssetImporter::ImportScene(const AssetMetadata& metadata)
	{
		SceneSerializer serializer;
		return serializer.Deserialize(metadata.FilePath);
	}
	Ref<Asset> SceneAssetImporter::LoadScene(const std::filesystem::path& path)
	{
		return Ref<Asset>();
	}
}
