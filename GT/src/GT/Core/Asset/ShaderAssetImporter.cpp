#include "gtpch.h"
#include "ShaderAssetImporter.h"
#include "ShaderAsset.h"
namespace GT
{
    Ref<Asset> ShaderAssetImporter::ImportShader(const AssetMetadata& metadata)
    {
		Ref<ShaderAsset> shader;
        switch (metadata.Type)
        {
        case AssetType::Shader:
            shader = ShaderAsset::Create(metadata.FilePath);


            int32_t samplers[32];
            for (uint32_t i = 0;i < 32;i++)
            {
                samplers[i] = i;
            }

           shader->Bind();
           shader->SetUniformiv("u_Textures", samplers, 32);

            break;
        case AssetType::ComputeShader:
            shader = ShaderAsset::CreateCompute(metadata.FilePath);
            break;
        case AssetType::GeometryShader:
            shader = ShaderAsset::CreateGeometry(metadata.FilePath);
            break;
        }
        if (shader)
        {
            shader->ID = metadata.ID;
            shader->Name = metadata.Name;
        }
        return shader;
    }

    Ref<Asset> LoadShader(const std::filesystem::path& path)
    {
		return Ref<ShaderAsset>();
    }
}