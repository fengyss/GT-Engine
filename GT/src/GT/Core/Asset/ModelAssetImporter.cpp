#include "gtpch.h"

#include "ModelAssetImporter.h"
#include "ModelAsset.h"
namespace GT
{
    Ref<Asset> ModelAssetImporter::ImportModel(const AssetMetadata& metadata)
    {
        Ref<ModelAsset> model = CreateRef<ModelAsset>(metadata.FilePath);
        model->ID = metadata.ID;
        model->Name = metadata.Name;
        return model;
    }
    Ref<Asset> LoadModel(const std::filesystem::path& path)
    {
		return CreateRef<ModelAsset>(path);
    }
}