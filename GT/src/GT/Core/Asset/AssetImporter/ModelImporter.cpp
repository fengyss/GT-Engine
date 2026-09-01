#include "gtpch.h"

#include "ModelImporter.h"
#include "../AssetManager.h"
#include "../ModelAsset.h"
namespace GT
{
    Ref<AssetMetadata> ModelImporter::GeneratorMetadataFromPath(const std::filesystem::path& path)
    {
        Ref<AssetMetadata> meta = CreateRef<AssetMetadata>();
        meta->FilePath = path;
        meta->Name = path.stem().string();
        meta->ID = UUID();
        meta->Type = AssetType::Model;
        return meta;

    }
    Ref<Asset> ModelImporter::ImportModel(const Ref<AssetMetadata> metadata)
    {
#if GT_DEBUG
        if (!AssetManager::ExistedUUID(metadata->ID))
            AssetManager::RegisterMetadata(*metadata);
#endif

        Ref<ModelAsset> asset = CreateRef<ModelAsset>(metadata->FilePath);

        asset->ID = metadata->ID;
        asset->Name = metadata->Name;
		asset->filepath = metadata->FilePath;
        return asset;
    }
    Ref<Asset> ModelImporter::ImportModel(const std::filesystem::path& path)
    {
        Ref<AssetMetadata> metadata = GeneratorMetadataFromPath(path);

        AssetManager::SaveMetadata(metadata);
        AssetManager::RegisterMetadata(*metadata);

        return ImportModel(metadata);
    }

    Ref<Asset> ModelImporter::ReloadModel(const std::filesystem::path& path)
    {
        return CreateRef<ModelAsset>(path);
    }
}