#include "gtpch.h"
#include "Renderer3D.h"
#include "GT/Assets/AssetsHandle.h"
#include"GT/Renderer/Frustum.h"

namespace GT
{
	Renderer3DState Renderer3D::state = Renderer3DState::None;
	int Renderer3D::s_CurrentEntityID = -1;
	Ref<AssetsHandle<Shader>> Renderer3D::s_ModelShader;
	glm::mat4 Renderer3D::s_ViewProjectionMatrix = glm::mat4(1.0f);


	static Renderer3D::Statistics s_stats;
	void Renderer3D::Init()
	{
		s_ModelShader = CreateHandle<Shader>("Model");
	}

	void Renderer3D::Shutdown()
	{
		GT_PROFILE_FUNCTION();
		// Clean up opengl resources before opengl context is destroyed
		s_ModelShader.reset();
	}

	void Renderer3D::BeginScene(Camera& camera, glm::mat4& transform)
	{
		GT_PROFILE_FUNCTION();

		GT_CORE_ASSERT(state != Renderer3DState::BeginScene, "Renderer3D::BeginScene is already Called!");
		state = Renderer3DState::BeginScene;

		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

		SetViewProjection(viewProj);

	}

	void Renderer3D::BeginScene(EditorCamera& camera)
	{
		GT_PROFILE_FUNCTION();

		GT_CORE_ASSERT(state != Renderer3DState::BeginScene, "Renderer3D::BeginScene is already Called!");
		state = Renderer3DState::BeginScene;

		s_ModelShader->Get()->Bind();
		s_ModelShader->Get()->SetUniform3f("u_ViewPos", camera.GetPosition());
		SetViewProjection(camera.GetViewProjection());
	}

	void Renderer3D::BeginScene(OrthographicCamera& camera)
	{
		GT_PROFILE_FUNCTION();

		GT_CORE_ASSERT(state != Renderer3DState::BeginScene, "Renderer3D::BeginScene is already Called!");
		state = Renderer3DState::BeginScene;

		SetViewProjection(camera.GetViewProjectionMatrix());
	}
	void Renderer3D::SetViewProjection(const glm::mat4& viewProjection)
	{
		s_ModelShader->Get()->Bind();
		s_ModelShader->Get()->SetUniformMat4("u_ViewProjection", viewProjection);
		s_ViewProjectionMatrix = viewProjection;
	}
	void Renderer3D::SetLight(const glm::vec3& lightpos, const glm::vec3& lightcolor)
	{
		s_ModelShader->Get()->Bind();
		s_ModelShader->Get()->SetUniform3f("u_LightPos", lightpos);
		s_ModelShader->Get()->SetUniform3f("u_LightColor", lightcolor);
	}
	void  Renderer3D::EndScene()
	{
		GT_PROFILE_FUNCTION();

		GT_CORE_ASSERT(state != Renderer3DState::EndScene, "You Should Call BeginScene First!");
		state = Renderer3DState::EndScene;


	}
	void Renderer3D::DrawModel(const glm::mat4& transform, Ref<Model>& model)
	{
		if (!model->hasShader) model->SetShader(s_ModelShader);
		else 
		{
			model->shader->Get()->Bind();
			model->shader->Get()->SetUniform1i("u_EntityID", s_CurrentEntityID);
			model->shader->Get()->SetUniformMat4("u_ViewProjection", s_ViewProjectionMatrix);
		}
		model->Draw(transform,ExtractFrustum(s_ViewProjectionMatrix));
		s_stats.DrawCalls++;
		s_stats.Meshes += model->GetMeshCount();
		s_stats.VerticiesCount += model->GetVertexCount();
	}
	void Renderer3D::SetCurrentEntityID(int entityID)
	{
		s_CurrentEntityID = entityID;
	}

	Renderer3D::Statistics Renderer3D::GetStats()
	{
		return s_stats;
	}
	void Renderer3D::ResetStats()
	{
		s_stats = Statistics();
	}
}