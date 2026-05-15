#include "gtpch.h"
#include "Renderer.h"
#include "GT/Platform/OpenGL/OpenGLShader.h"
#include "Renderer2D.h"
#include "Renderer3D.h"
#include "GT/Particle/ParticleRenderer.h"
namespace GT
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		GT_PROFILE_FUNCTION();
		RenderCommand::Init();
		Renderer2D::Init();
		Renderer3D::Init();
		ParticleRenderer::Init();
	}
	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}
	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
	void Renderer::Submit(const Ref<Shader>& shader, 
		const Ref<VertexArray>& vertexArray, 
		const glm::mat4& transform)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4("u_Transform", transform);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
	
}