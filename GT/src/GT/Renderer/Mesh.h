#pragma once

#include "Shader.h"

#include "Texture.h"
#include "VertexArray.h"

#include <string>
#include <vector>
#include "GT/Assets/AssetsManager.h"
#include "GT/Assets/AssetsHandle.h"

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

        std::vector<Ref<AssetsHandle<Texture2D>>>    textures;
        unsigned int VAO;

        // constructor
        Mesh() {};
        Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<Ref<AssetsHandle<Texture2D>>> _textures);

        // render the mesh
        void Draw(Ref<Shader> shader);
        void Draw(const glm::mat4& transform, Ref<Shader> shader);
		uint32_t GetVertexCount() { return vertices.size(); }
    private:
        // render data 
		Ref<VertexArray> m_VertexArray;
        Ref<VertexBuffer> VBuffer;
        // initializes all the buffer objects/arrays
        void setupMesh();
    };

}