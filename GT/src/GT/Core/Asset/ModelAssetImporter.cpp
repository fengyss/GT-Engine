#include "gtpch.h"

#include "ModelAssetImporter.h"
#include "GT/Renderer/Model.h"
namespace GT
{
    Ref<Asset> ModelAssetImporter::ImportModel(const AssetMetadata& metadata)
    {
        Ref<Model> model = CreateRef<Model>(metadata.FilePath);
        model->metadata = metadata;
        return model;
    }
    Ref<Asset> LoadModel(const std::filesystem::path& path)
    {
        return Ref<Model>();
    }
}