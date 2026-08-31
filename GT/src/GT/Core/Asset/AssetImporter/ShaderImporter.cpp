#include "gtpch.h"
#include "ShaderImporter.h"
#include "../ShaderAsset.h"
#include "../AssetManager.h"

namespace GT
{

    Ref<AssetMetadata> ShaderImporter::GeneratorMetadataFromPath(const std::filesystem::path& path)
    {
        Ref<AssetMetadata> meta = CreateRef<AssetMetadata>();
        meta->FilePath = path;
        meta->Name = path.stem().string();
        meta->ID = UUID();

        
        auto ext = path.extension().string();

        if (ext == ".glsl") meta->Type = AssetType::Shader;
        else if (ext == ".geom") meta->Type = AssetType::GeometryShader;
        else if (ext == ".comp") meta->Type = AssetType::ComputeShader;

        return meta;
    }


    Ref<Asset> ShaderImporter::ImportShader(const Ref<AssetMetadata> metadata)
    {

#if GT_DEBUG
        if (!AssetManager::ExistedUUID(metadata->ID)) 
            AssetManager::RegisterMetadata(*metadata);
#endif

        Ref<ShaderAsset> shader;
        switch (metadata->Type)
        {
        case AssetType::Shader:
            shader = ShaderAsset::Create(metadata->FilePath);
            break;
        case AssetType::ComputeShader:
            shader = ShaderAsset::CreateCompute(metadata->FilePath);
            break;
        case AssetType::GeometryShader:
            shader = ShaderAsset::CreateGeometry(metadata->FilePath);
            break;
        default:
            GT_CORE_ERROR("Unknown shader type from {0}!",metadata->FilePath.string());
            break;
        }

        int32_t samplers[32];
        for (uint32_t i = 0;i < 32;i++)
        {
            samplers[i] = i;
        }

        shader->Bind();
        shader->SetUniformiv("u_Textures", samplers, 32);


        shader->ID = metadata->ID;
        shader->Name = metadata->Name;

        return shader;
    }

    Ref<Asset> ShaderImporter::ImportShader(const std::filesystem::path& path)
    {
		Ref<AssetMetadata> metadata = GeneratorMetadataFromPath(path);

        AssetManager::SaveMetadata(metadata);
        AssetManager::RegisterMetadata(*metadata);

		return ImportShader(metadata);
    }


    Ref<Asset> ShaderImporter::ReloadShader(const std::filesystem::path& path)
    {

        Ref<ShaderAsset> shader;
        //= CreateRef<ShaderAsset>();

        auto ext = path.extension();
        if (ext == ".glsl")
            shader = ShaderAsset::Create(path);
        else if (ext == ".geom")
            shader = ShaderAsset::CreateGeometry(path);
        else if (ext == ".comp")
            shader = ShaderAsset::CreateCompute(path);
        else
            GT_CORE_ERROR("Unknown shader type from {0}!", path.string());


        int32_t samplers[32];
        for (uint32_t i = 0;i < 32;i++)
        {
            samplers[i] = i;
        }

        shader->Bind();
        shader->SetUniformiv("u_Textures", samplers, 32);

        return shader;
    }
}