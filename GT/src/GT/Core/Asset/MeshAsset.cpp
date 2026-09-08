#include "gtpch.h"
#include "MeshAsset.h"

#include "GT/Renderer/RenderCommand.h"
#include "GT/Math/Random.h"

namespace GT
{

    // constructor
    MeshAsset::MeshAsset(const std::vector<Vertex>& _vertices, const std::vector<unsigned int>& _indices)
        :vertices(_vertices), indices(_indices)
    {
        setupMesh();
        ComputeAABB(_vertices);
    }
    // render the mesh
    void MeshAsset::Draw(const glm::mat4& ptransform, const Shader& shader)
    {

        shader->Bind();

        shader->SetUniformMat4("u_Transform", transform * ptransform);

        RenderCommand::DrawIndexed(m_VertexArray, indices.size());

		//glm::vec3 trs = Random::RangeVec3(-0.01f, 0.01f);
  //      transform = glm::translate(transform, trs);
    }

    void MeshAsset::DrawForShadowMap(const glm::mat4& transform)
    {
        RenderCommand::DrawIndexed(m_VertexArray, indices.size());
    }

    void MeshAsset::ComputeAABB(const std::vector<Vertex>& vertices)
    {
        m_Min = glm::vec3(std::numeric_limits<float>::max());
        m_Max = glm::vec3(std::numeric_limits<float>::lowest());

        for (const auto& v : vertices)
        {
            m_Min = glm::min(m_Min, v.Position);
            m_Max = glm::max(m_Max, v.Position);
        }
        aabb = { m_Min,m_Max };
    }


    // initializes all the buffer objects/arrays
    void MeshAsset::setupMesh()
    {
        m_VertexArray = VertexArray::Create();

        VBuffer = VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(Vertex));
        {
            BufferLayout layout = {
                { ShaderDataType::Float3, "a_Position"  },
                { ShaderDataType::Float3, "a_Normal"  },
                { ShaderDataType::Float2, "a_TexCoord"  },
                { ShaderDataType::Float3, "a_Tangent"  },
                { ShaderDataType::Float3, "a_Bitangent"  },
                { ShaderDataType::Int4, "a_m_BoneIDs"  },
                { ShaderDataType::Int4, "a_Weights"  },
            };
            VBuffer->SetLayout(layout);
        }
        VBuffer->Bind();
        VBuffer->SetData(vertices.data(), vertices.size() * sizeof(Vertex));
        m_VertexArray->AddVertexBuffer(VBuffer);
        m_VertexArray->SetIndexBuffer(IndexBuffer::Create(indices.data(), indices.size()));
        vertices.clear();
        indices.clear();
    }
}