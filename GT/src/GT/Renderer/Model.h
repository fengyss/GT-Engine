#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"

#include "GT/Renderer/Frustum.h"

#include "GT/Core/Asset/AssetHandle.h"
#include "GT/Core/Asset/Asset.h"

namespace GT
{
    class Model : public Asset
    {
    public:

        static AssetType GetStaticType() { return AssetType::Model; }

        virtual AssetType GetType() const { return GetStaticType(); }


        virtual uint32_t GetMemorySize() const override { return 0; }

        // model data 
        std::vector<RefHandle<Texture2D>> textures;
		RefHandle<Shader> shader;

        //std::vector<AssetHandle> textures;
        //AssetHandle shader;
        
        std::vector<Mesh>    meshes;
        std::filesystem::path filepath;
        bool gammaCorrection;
		bool isLoaded = false;
		bool hasShader = false;
		uint32_t VertexCount = 0;
        std::string name = "NONE";
        // constructor, expects a filepath to a 3D model.
       
        Model(const std::filesystem::path& path, bool gamma = false) : gammaCorrection(gamma)
        {
            loadModel(path);
            if (isLoaded) 
            {
                CalculateVertexCount();
				name = filepath.stem().string();
            }
        }
        ~Model();

        // draws the model, and thus all its meshes
        void Draw(const glm::mat4& transform);
        void Draw(const glm::mat4& transform, const Ref<Shader> shader);
        void DrawForShadowMap(const glm::mat4& transform, const Ref<Shader> shader);
        void Draw(const glm::mat4& transform,const Frustum& frustum);
        void SetShader(RefHandle<Shader> shader) {
            this->shader = shader; 
            hasShader = true;
        }
        //void SetShader(AssetHandle shader) {
            //this->shader = shader;
            //hasShader = true;
        //}
        const std::vector<Mesh> GetMeshes() { return meshes; }
		uint32_t GetMeshCount() const { return meshes.size(); }
        void CalculateVertexCount() 
        {
            VertexCount = 0;
            for (auto& mesh : meshes)
                VertexCount += mesh.GetVertexCount();
		}
        uint32_t GetVertexCount() 
        {
            return VertexCount;
		}
        const std::string& GetName() { return name; }
        GPUAABB GetAABB() { return aabb; }
    private:
        GPUAABB aabb = { glm::vec3(std::numeric_limits<float>::max()), glm::vec3(std::numeric_limits<float>::lowest()) };
        // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
        void loadModel(const std::filesystem::path& path);

        // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
        void processNode(aiNode* node, const aiScene* scene);

        Mesh processMesh(aiMesh* mesh, const aiScene* scene);

        // checks all material textures of a given type and loads the textures if they're not loaded yet.
        // the required info is returned as a Texture struct.
        std::vector<Ref<Texture>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
        void Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    };

    class ModelLibrary
    {
    public:

        Ref<Model> Load(uint32_t ID, const std::filesystem::path& filepath);
        Ref<Model> Reload(uint32_t ID, const std::filesystem::path& filepath);
        void Clear();
        Ref<Model> Get(uint32_t ID);

        bool Exists(uint32_t ID) const
        {
            return m_Models.find(ID) != m_Models.end();
        }
    private:
        void Add(uint32_t ID, const Ref<Model>& model);
        std::unordered_map<uint32_t, Ref<Model>> m_Models;
    };

}
