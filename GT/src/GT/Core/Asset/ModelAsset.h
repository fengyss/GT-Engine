#pragma once



#include "GT/Renderer/Frustum.h"

#include "GT/Core/Asset/Asset.h"

#include "GT/Renderer/Texture.h"
#include "GT/Renderer/Mesh.h"
#include "GT/Renderer/Shader.h"
#include "animdata.h"

struct aiMesh;
struct aiNode;
struct aiMaterial;
struct aiScene;
enum aiTextureType;

namespace GT
{




    class ModelAsset : public Asset
    {
    public:
        ModelAsset() = default;
        static AssetType GetStaticType() { return AssetType::Model; }

        virtual AssetType GetType() const { return GetStaticType(); }

        virtual const std::string& GetName() const override { return Name; }




        virtual uint32_t GetMemorySize() const override { return 0; }

        auto& GetBoneInfoMap() { return m_BoneInfoMap; }
        int& GetBoneCount() { return m_BoneCounter; }

        // model data 
        std::vector<Texture2D> textures;
        std::vector<Mesh> meshes;
		Shader shader;

        std::map<std::string, BoneInfo> m_BoneInfoMap;
        int m_BoneCounter = 0;

        std::filesystem::path filepath;
        bool gammaCorrection;
		bool isLoaded = false;
		bool hasShader = false;
		uint32_t VertexCount = 0;


        operator bool()
        {
            return false;
        }
       
        ModelAsset(const std::filesystem::path& path, bool gamma = false) : gammaCorrection(gamma)
        {
            loadModel(path);
            if (isLoaded) 
            {
                CalculateVertexCount();
				Name = filepath.stem().string();
            }
        }
        ~ModelAsset();

        // draws the model, and thus all its meshes
        void Draw(const glm::mat4& transform);
        void Draw(const glm::mat4& transform, const Shader& shader);
        void DrawForShadowMap(const glm::mat4& transform);
        void Draw(const glm::mat4& transform,const Frustum& frustum);
        void SetShader(const Shader& shader) {
            this->shader = shader; 
            hasShader = true;
        }
        //void SetShader(AssetHandle shader) {
            //this->shader = shader;
            //hasShader = true;
        //}
        const std::vector<Mesh> GetMeshes() { return meshes; }
        inline uint32_t GetMeshCount()
        {
            return 0;
        }
		uint32_t GetMeshCount() const { return meshes.size(); }
        void CalculateVertexCount() 
        {
            VertexCount = 0;
            for (auto& mesh : meshes)
                VertexCount += mesh->GetVertexCount();
		}
        uint32_t GetVertexCount() 
        {
            return VertexCount;
		}
        GPUAABB GetAABB() { return aabb; }
    private:
        GPUAABB aabb = { glm::vec3(std::numeric_limits<float>::max()), glm::vec3(std::numeric_limits<float>::lowest()) };
        // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
        void loadModel(const std::filesystem::path& path);

        // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
        void processNode(aiNode* node, const aiScene* scene);

        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        
        void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);

        // checks all material textures of a given type and loads the textures if they're not loaded yet.
        // the required info is returned as a Texture struct.
        std::vector<Texture2D> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    };

}
