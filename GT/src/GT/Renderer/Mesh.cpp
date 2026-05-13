#include "gtpch.h"
#include "Mesh.h"

#include "RenderCommand.h"


namespace GT
{

    // constructor
    Mesh::Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<RefHandle<Texture2D>> _textures)
		:vertices(_vertices), indices(_indices), textures(_textures)
    {
        setupMesh();
        ComputeAABB(_vertices);
    }
    // render the mesh
    void Mesh::Draw(const glm::mat4& transform, Ref<Shader> shader)
    {

        unsigned int texslot = 0;
        for (unsigned int i = 0; i < textures.size(); i++)
        {
			auto tex = textures[i]->Get();
			auto type = tex->GetType();

            texslot |= type;
            shader->SetUniform1i(GetStrOfType(type), i);

            tex->Bind(i);
        }

        shader->SetUniform1ui("u_TexSlot", texslot);
        shader->SetUniformMat4("u_Transform", transform);
        RenderCommand::DrawIndexed(m_VertexArray, indices.size());
    }

    void Mesh::ComputeAABB(const std::vector<Vertex>& vertices)
    {
        m_Min = glm::vec3(std::numeric_limits<float>::max());
        m_Max = glm::vec3(std::numeric_limits<float>::lowest());

        for (const auto& v : vertices)
        {
            m_Min = glm::min(m_Min, v.Position);
            m_Max = glm::max(m_Max, v.Position);
        }
    }


    // initializes all the buffer objects/arrays
    void Mesh::setupMesh()
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