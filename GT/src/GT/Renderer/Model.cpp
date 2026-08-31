#include "gtpch.h"
#include "Model.h"

namespace GT
{


    AssetType Model::GetType()
    {
        return AssetType();
    }

    uint32_t Model::GetMemorySize()
    {
        return 0;
    }

    Model::Model(const std::filesystem::path& path, bool gamma)
    {
    }

    Model::~Model()
    {
    }

    void Model::Draw(const glm::mat4& transform, const Shader& shader)
    {
    }
    void Model::DrawForShadowMap(const glm::mat4& transform, const Shader& shader)
    {
    }
    void Model::Draw(const glm::mat4& transform, const Frustum& frustum)
    {
    }
    GPUAABB Model::GetAABB()
    {
        return GPUAABB();
    }
    void Model::SetShader(const Shader& shader)
    {

    }
}