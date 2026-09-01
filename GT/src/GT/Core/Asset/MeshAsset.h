#pragma once

#include "GT/Renderer/Shader.h"
#include "GT/Renderer/Texture.h"
#include "GT/Renderer/VertexArray.h"

#include <string>
#include <vector>
#include "GT/Renderer/Frustum.h"
#define MAX_BONE_INFLUENCE 4

#include "GT/Core/ID.h"

namespace GT
{
	class MeshAsset : public Asset
    {
    public:
        // mesh Data
        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;

        std::vector<Texture2D> textures;
        unsigned int VAO;


        virtual AssetType GetType() const override { return AssetType::Mesh; };
        virtual uint32_t GetMemorySize() const override { return 0; };

        virtual const std::string& GetName() const override { return Name; };

        // constructor
        MeshAsset() {};
        MeshAsset(const std::vector<Vertex>& _vertices, const std::vector<unsigned int>& _indices, const std::vector<Texture2D>& _textures);
        ~MeshAsset()
        {
            m_VertexArray.reset();
            VBuffer.reset();
        }
        // render the mesh
        void Draw(const Shader& shader);
        void Draw(const glm::mat4& transform, const Shader& shader);
        void DrawForShadowMap(const glm::mat4& transform);
        uint32_t GetVertexCount() { return vertices.size(); }

        const glm::vec3& GetMin() const { return m_Min; }
        const glm::vec3& GetMax() const { return m_Max; }
        GPUAABB GetAABB() { return aabb; }

    private:
        void ComputeAABB(const std::vector<Vertex>& vertices);
        GPUAABB aabb;
        glm::vec3 m_Min;
        glm::vec3 m_Max;
    private:
        // render data 
        Ref<VertexArray> m_VertexArray;
        Ref<VertexBuffer> VBuffer;
        // initializes all the buffer objects/arrays
        void setupMesh();
    };

}