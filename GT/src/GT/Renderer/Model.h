#pragma once
#include "GT/Core/Asset/ModelAsset.h"
namespace GT
{
	class Model
	{
    public:
        Model() = default;

        AssetType GetType();

        const std::string& GetName();




        uint32_t GetMemorySize();


        std::vector<MeshAsset>    meshes;
        std::filesystem::path filepath;
        bool gammaCorrection;
        bool isLoaded = false;
        bool hasShader = false;
        uint32_t VertexCount = 0;
        std::string Name;
        // constructor, expects a filepath to a 3D model.

        operator bool()
        {
            return false;
        }

        Model(const std::filesystem::path& path, bool gamma = false);
        ~Model();

        // draws the model, and thus all its meshes
        void Draw(const glm::mat4& transform);
        void Draw(const glm::mat4& transform, const Shader& shader);
        void DrawForShadowMap(const glm::mat4& transform, const Shader& shader);
        void Draw(const glm::mat4& transform, const Frustum& frustum);
        void SetShader(const Shader& shader);
        const std::vector<MeshAsset> GetMeshes();
        uint32_t GetMeshCount();
        void CalculateVertexCount();
        uint32_t GetVertexCount();
        GPUAABB GetAABB();
	};
}

