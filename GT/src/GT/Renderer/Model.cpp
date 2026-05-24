#include "gtpch.h"
#include "Model.h"
#include "GT/Project/Project.h"
#include "GT/Assets/AssetsHandle.h"

#include "GT/Renderer/RenderCommand.h"
#include "GT/Renderer/Renderer2D.h"
#include "GT/Renderer/Renderer3D.h"

namespace GT
{
    TextureType GetTypeFromAssimpType(aiTextureType type)
    {
        switch (type)
        {
        case aiTextureType_DIFFUSE:  return TextureType::Diffuse;
        case aiTextureType_SPECULAR: return TextureType::Specular;
        case aiTextureType_NORMALS:  return TextureType::Normal;
        case aiTextureType_HEIGHT:   return TextureType::Height;
        case aiTextureType_EMISSIVE: return TextureType::Emission;
        default:
            GT_CORE_WARN("Texture assimp type {0} is not supported!", int(type));
            return TextureType::TextureTypeNone;
        }
	}

    bool IsAABBInsidePlane(glm::vec4 plane, GPUAABB aabb)
    {
        glm::vec3 positive = aabb.Min;
        if (plane.x >= 0) positive.x = aabb.Max.x;
        if (plane.y >= 0) positive.y = aabb.Max.y;
        if (plane.z >= 0) positive.z = aabb.Max.z;

        return glm::dot(glm::vec3(plane), positive) + plane.w >= 0.0;
    }
    
    void Model::Draw(const glm::mat4& transform)
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
        {
            meshes[i].Draw(transform, shader->Get());
        }
    }
    void Model::Draw(const glm::mat4& transform, const Ref<Shader> shader)
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
        {
            meshes[i].Draw(transform, shader);
        }
    }

    void Model::DrawForShadowMap(const glm::mat4& transform, const Ref<Shader> shader)
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
        {
            meshes[i].DrawForShadowMap(transform, shader);
        }
    }
    void Model::Draw(const glm::mat4& transform, const Frustum& frustum)
    {
        
        
        for (unsigned int i = 0; i < meshes.size(); i++)
        {
            bool visible = true;
            glm::vec3 min = meshes[i].GetMin();
            glm::vec3 max = meshes[i].GetMax();

            min = transform * glm::vec4(min, 1.0f);
            max = transform * glm::vec4(max, 1.0f);
            GPUAABB aabb = { min ,max };
            for (int p = 0;p < 6;p++)
            {
                glm::vec4 plane = {frustum.Planes[p].Normal, frustum.Planes[p].Distance};
                if (!IsAABBInsidePlane(plane, aabb))
                {
                    visible = false;
                    break;
                }

            }

           

            if(visible)
            {
                RenderCommand::SetLineWidth(0.3f);
                meshes[i].Draw(transform, shader->Get());
                Renderer3D::GetStats().DrawCalls++;
                Renderer3D::GetStats().Meshes++;
                Renderer3D::GetStats().VerticiesCount += meshes[i].GetVertexCount();
            }
        }
    }

    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void Model::loadModel(const std::filesystem::path& path)
    {
        // read file via ASSIMP
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path.string(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // check for errors
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
			GT_CORE_ERROR("ERROR::ASSIMP:: {0}", importer.GetErrorString());
            return;
        }
        isLoaded = true;
        // retrieve the directory path of the filepath
        filepath = path;

        // process ASSIMP's root node recursively
        processNode(scene->mRootNode, scene);

        for (auto& mesh : meshes)
        {
            aabb.Min = min(aabb.Min, mesh.GetMin());
            aabb.Max = max(aabb.Max, mesh.GetMax());
        }

    }

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void Model::processNode(aiNode* node, const aiScene* scene)
    {
        // process each mesh located at the current node
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene. 
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }

    }

    Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
    {
        // data to fill
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        // walk through each of the mesh's vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // normals
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }
            // texture coordinates
            if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
                // tangent
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
                // bitangent
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        
        LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        LoadMaterialTextures(material, aiTextureType_EMISSIVE, "texture_emission");
		return Mesh(vertices, indices, textures);
    }

    void Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
    {
        if (mat->GetTextureCount(type))
            GT_CORE_TRACE("Loading [{0}] form material::{1}", typeName, mat->GetName().C_Str());

        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString path;
            mat->GetTexture(type, i, &path);
            bool skip = false;
            for (unsigned int j = 0; j < textures.size(); j++)
            {
                if (std::strcmp(textures[j]->GetPath().filename().string().c_str(), path.C_Str()) == 0)
                {
                    skip = true; 
                    break;
                }
            }
            if (!skip)
            {   
                std::filesystem::path texturePath = filepath.parent_path() / std::filesystem::path(std::string(path.C_Str()));

                RefHandle<Texture2D> handle = CreateHandle<Texture2D>(texturePath);
                textures.push_back(handle);

                auto tex = handle->Get();
                tex->SetType(GetTypeFromAssimpType(type));
            }
        }
    }

    void ModelLibrary::Clear()
    {
        for(auto& [id, model] : m_Models)
        {
            model.reset();
		}
	}


    void ModelLibrary::Add(uint32_t ID, const Ref<Model>& model)
    {
        m_Models[ID] = model;
    }

    Ref<Model> ModelLibrary::Load(uint32_t ID, const std::filesystem::path& filepath)
    {
        if (Exists(ID))
        {
            GT_CORE_INFO("Model {1} with ID {0} already loaded!", ID, filepath.filename().string());
            return m_Models[ID];
        }
        else {
            auto model = CreateRef<Model>(filepath);
            Add(ID, model);
            return model;
        }
    }
    Ref<Model> ModelLibrary::Reload(uint32_t ID, const std::filesystem::path& filepath)
    {
        GT_CORE_WARN("Model {1} with ID {0} reloaded!", ID, filepath.filename().string());
        m_Models[ID] = CreateRef<Model>(filepath);
        return m_Models[ID];
    }
    Ref<Model> ModelLibrary::Get(uint32_t ID)
    {
        if (Exists(ID)) return m_Models[ID];
        else
        {
            GT_CORE_ERROR("Model ID:{0} not found in library!", ID);
            return nullptr;
        }
    }

    
}