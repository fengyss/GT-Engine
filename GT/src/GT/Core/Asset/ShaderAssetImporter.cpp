#include "gtpch.h"
#include "ShaderAssetImporter.h"
#include "GT/Renderer/Shader.h"
namespace GT
{
    Ref<Asset> ShaderAssetImporter::ImportShader(const AssetMetadata& metadata)
    {
        Ref<Shader> shader;
        switch (metadata.Type)
        {
        case AssetType::Shader:
            shader = Shader::Create(metadata.FilePath);


            int32_t samplers[32];
            for (uint32_t i = 0;i < 32;i++)
            {
                samplers[i] = i;
            }

           shader->Bind();
           shader->SetUniformiv("u_Textures", samplers, 32);

            break;
        case AssetType::ComputeShader:
            shader = Shader::CreateCompute(metadata.FilePath);
            break;
        case AssetType::GeometryShader:
            shader = Shader::CreateGeometry(metadata.FilePath);
            break;
        }
        shader->metadata = metadata;
        return shader;
    }

    Ref<Asset> LoadShader(const std::filesystem::path& path)
    {
        return Ref<Shader>();
    }
}