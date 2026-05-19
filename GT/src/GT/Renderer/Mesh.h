#pragma once

#include "Shader.h"

#include "Texture.h"
#include "VertexArray.h"

#include <string>
#include <vector>
#include "GT/Assets/AssetsHandle.h"
#include "GT/Renderer/Frustum.h"
#define MAX_BONE_INFLUENCE 4

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};



namespace GT
{
    class Mesh {
    public:
        // mesh Data
        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;

        std::vector<RefHandle<Texture2D>>    textures;
        unsigned int VAO;

        // constructor
        Mesh() {};
        Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<RefHandle<Texture2D>> _textures);

        // render the mesh
        void Draw(Ref<Shader> shader);
        void Draw(const glm::mat4& transform, Ref<Shader> shader);
        void DrawForShadowMap(const glm::mat4& transform, Ref<Shader> shader);
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