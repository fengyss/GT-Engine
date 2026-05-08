#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Texture2D.h"
#include "Mesh.h"
#include "Shader.h"

#include "GT/Assets/AssetsHandle.h"

//#include <string>
//#include <fstream>
//#include <sstream>
//#include <iostream>
//#include <map>
//#include <vector>


namespace GT
{
    class Model
    {
    public:
        // model data 
        std::vector<Ref<AssetsHandle<Texture2D>>> textures;
		Ref<AssetsHandle<Shader>> shader;

        
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
		~Model() = default;

        // draws the model, and thus all its meshes
        void Draw(const glm::mat4& transform);
        void SetShader(Ref<AssetsHandle<Shader>> shader) {
            this->shader = shader; 
            hasShader = true;
        }
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

    private:
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

}
