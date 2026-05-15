#include "gtpch.h"
#include "ParticleRenderer.h"
#include "GT/Renderer/Renderer.h"
#include "GT/Renderer/Shader.h"
#include "GT/Renderer/Texture.h"
#include "GT/Renderer/Buffer.h"
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"
#include <algorithm>

namespace GT {

    static glm::mat4 s_viewProjection;
    RefHandle<Shader> ParticleShader;
    Ref<VertexArray> vertexArray;
    Ref<VertexBuffer> vbo;
    std::vector<ParticleGPUVertex> Vertices;
    std::vector<ParticleGPUVertex> Alp_Vertices;
    std::vector<ParticleGPUVertex> Add_Vertices;
    std::vector<ParticleGPUVertex> Mul_Vertices;
    Ref<Texture> TextureSlots[32];
    int TextureSlotIndex = 1;
    void ParticleRenderer::Init()
    { 
        ParticleShader = CreateHandle<Shader>("Particles");

        int32_t samplers[32];
        for (uint32_t i = 0;i < 32;i++)
        {
            samplers[i] = i;
        }
        ParticleShader->Get()->SetUniformiv("u_Textures", samplers, 32);

        vertexArray = VertexArray::Create();
        vbo = VertexBuffer::Create(nullptr, sizeof(ParticleGPUVertex) * 10000);
        vbo->SetLayout({
                { ShaderDataType::Float3, "a_Position"  },
                { ShaderDataType::Float4, "a_Color"  },
                { ShaderDataType::Float, "a_Size"  },
                { ShaderDataType::Float, "a_TexIndex"  },
            });
        vertexArray->AddVertexBuffer(vbo);
        TextureSlots[0] = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        TextureSlots[0]->SetData(&whiteTextureData, sizeof(uint32_t));
    }
    void ParticleRenderer::BeginScene(Camera& camera, glm::mat4& transform)
	{
		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

		SetViewProjection(viewProj);

	}
	void ParticleRenderer::BeginScene(EditorCamera& camera)
	{

		SetViewProjection(camera.GetViewProjection());

	}

	void ParticleRenderer::BeginScene(OrthographicCamera& camera)
	{
        SetViewProjection(camera.GetViewProjectionMatrix());
	}
	void  ParticleRenderer::EndScene()
    {
        Ref<Shader> shader = ParticleShader->Get();
        shader->Bind();
        shader->SetUniformMat4("u_ViewProjection", s_viewProjection);
        
        if (!Vertices.empty())
        {
            for (int i = 0;i < TextureSlotIndex;i++)
            {
                TextureSlots[i]->Bind(i);
            }
            Flush(BlendMode::None);
            uint32_t count = Vertices.size();
            vbo->SetData(Vertices.data(), count * sizeof(ParticleGPUVertex));
            RenderCommand::DrawArrays(vertexArray, count);
            Vertices.clear();
        }
        if (!Alp_Vertices.empty())
        {
            for (int i = 0;i < TextureSlotIndex;i++)
            {
                TextureSlots[i]->Bind(i);
            }
            Flush(BlendMode::Alpha);
            uint32_t count = Alp_Vertices.size();
            vbo->SetData(Alp_Vertices.data(), count * sizeof(ParticleGPUVertex));
            RenderCommand::DrawArrays(vertexArray, count);
            Alp_Vertices.clear();
        }
        if (!Mul_Vertices.empty())
        {
            for (int i = 0;i < TextureSlotIndex;i++)
            {
                TextureSlots[i]->Bind(i);
            }
            Flush(BlendMode::Multiply);
            uint32_t count = Mul_Vertices.size();
            vbo->SetData(Mul_Vertices.data(), count * sizeof(ParticleGPUVertex));
            RenderCommand::DrawArrays(vertexArray, count);
            Mul_Vertices.clear();
        }
        if (!Add_Vertices.empty())
        {
            for (int i = 0;i < TextureSlotIndex;i++)
            {
                TextureSlots[i]->Bind(i);
            }
            Flush(BlendMode::Additive);
            uint32_t count = Add_Vertices.size();
            vbo->SetData(Add_Vertices.data(), count * sizeof(ParticleGPUVertex));
            RenderCommand::DrawArrays(vertexArray, count);
            Add_Vertices.clear();
        }
        TextureSlotIndex = 1;
	}
    void ParticleRenderer::SetViewProjection(const glm::mat4& viewProjection)
    {
        s_viewProjection = viewProjection;
    }

    void ParticleRenderer::RenderParticles(const std::vector<Ref<Particle>> particles)
    {
        //if (particles.empty()) return;

        for (const auto particle : particles) {
            if (particle->lifeRemaining == 0.0f) continue;

            glm::vec3 pos = particle->position;
            glm::vec2 size = glm::vec2(particle->size);

			Renderer2D::DrawParticleQuad(pos,size, particle->color);
            //Renderer2D::DrawCircle(model, particle.color);
        }
    }

    void ParticleRenderer::RenderParticles(const std::vector<Ref<Particle>> particles, BlendMode mode)
    {
        ParticleGPUVertex vertex;
        switch (mode)
        {
        case BlendMode::None:
            for (auto& p : particles)
            {
                if (p->lifeRemaining == 0.0f) continue;
                vertex = { p->position,p->color,p->size };
                Vertices.emplace_back(vertex);
            }
            break;
        case BlendMode::Alpha:
            for (auto& p : particles)
            {
                if (p->lifeRemaining == 0.0f) continue;
                vertex = { p->position,p->color,p->size };
                Alp_Vertices.emplace_back(vertex);
            }
            break;
        case BlendMode::Additive:
            for (auto& p : particles)
            {
                if (p->lifeRemaining == 0.0f) continue;
                vertex = { p->position,p->color,p->size };
                Add_Vertices.emplace_back(vertex);
            }
            break;
        case BlendMode::Multiply:
            for (auto& p : particles)
            {
                if (p->lifeRemaining == 0.0f) continue;
                vertex = { p->position,p->color,p->size };
                Mul_Vertices.emplace_back(vertex);
            }
            break;
        }
    }
    float ParticleRenderer::GetTextureSlotIndex(const Ref<Texture2D>& texture)
    {
        float textureIndex = 0.0f;
        for (uint32_t i = 1; i < TextureSlotIndex; i++)
        {
            if (*TextureSlots[i].get() == *texture.get())
            {
                textureIndex = (float)i;
                break;
            }
        }
        if (textureIndex == 0.0f)
        {
            textureIndex = (float)TextureSlotIndex;
            TextureSlots[TextureSlotIndex] = texture;
            TextureSlotIndex++;
        }
        //std::cout << textureIndex << '\n';
        return textureIndex;
    }
    void ParticleRenderer::RenderParticles(BlendMode mode, const std::vector<Ref<Particle>> particles, const RefHandle<Texture2D> tex)
    {
        ParticleGPUVertex vertex;
        float Texindex = 0;
        if (tex)
        {
            Texindex = GetTextureSlotIndex(tex->Get());
        }
        switch (mode)
        {
        case BlendMode::None:
            for (auto& p : particles)
            {
                if (p->lifeRemaining == 0.0f) continue;
                vertex = { p->position,p->color,p->size, Texindex };
                Vertices.emplace_back(vertex);
            }
            break;
        case BlendMode::Alpha:
            for (auto& p : particles)
            {
                if (p->lifeRemaining == 0.0f) continue;
                vertex = { p->position,p->color,p->size, Texindex };
                Alp_Vertices.emplace_back(vertex);
            }
            break;
        case BlendMode::Additive:
            for (auto& p : particles)
            {
                if (p->lifeRemaining == 0.0f) continue;
                vertex = { p->position,p->color,p->size, Texindex };
                Add_Vertices.emplace_back(vertex);
            }
            break;
        case BlendMode::Multiply:
            for (auto& p : particles)
            {
                if (p->lifeRemaining == 0.0f) continue;
                vertex = { p->position,p->color,p->size, Texindex };
                Mul_Vertices.emplace_back(vertex);
            }
            break;
        }
    }


    void ParticleRenderer::RenderParticles(const std::vector<Ref<Particle>> particles, const RefHandle<Texture2D> tex)
    {
        for (const auto particle : particles) {
            if (particle->lifeRemaining == 0.0f) continue;

            glm::vec3 pos = particle->position;
            glm::vec2 size = glm::vec2(particle->size);

            Renderer2D::DrawParticleQuad(pos, size, particle->color,tex->Get());
            //Renderer2D::DrawCircle(model, particle.color);
        }
    }

    void ParticleRenderer::RenderParticle(const Ref<Particle> particle)
    {
        if (particle->lifeRemaining == 0.0f) return;

        glm::vec3 pos = particle->position;
        glm::vec2 size = glm::vec2(particle->size);

        Renderer2D::DrawParticleQuad(pos, size, particle->color);
    }

    void ParticleRenderer::Flush(BlendMode mode)
    {
        RenderCommand::SetBlendMode(mode);

    }


}