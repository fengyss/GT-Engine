#include "gtpch.h"
#include "Renderer3D.h"

#include "GT/Core/Asset/AssetManager.h"

#include"GT/Renderer/Frustum.h"
#include "ShadowMap.h"
#include "Renderer2D.h"
#include "glad/glad.h"
namespace GT
{
	Renderer3DState Renderer3D::state = Renderer3DState::None;
	int Renderer3D::s_CurrentEntityID = -1;
	Shader Renderer3D::s_ModelShader;
	Shader Renderer3D::m_ShadowShader;
	glm::mat4 Renderer3D::s_ViewProjectionMatrix = glm::mat4(1.0f);
	bool Renderer3D::IsShowAABB=false;
	glm::vec3& Renderer3D::s_viewPos = glm::vec3(0.0f);
	std::vector<Light_Matrix> Renderer3D::s_Lights;
	static Renderer3D::Statistics s_stats;
	ShadowMap Renderer3D::shadowmap;

	struct Trans_model
	{
		glm::mat4 transform;
		Model model;
		int entityID;
	};
	std::vector<Trans_model> models;

	void Renderer3D::Init()
	{
		s_ModelShader.Reset(AssetManager::GetAssetHandle("Model"));
		m_ShadowShader.Reset(AssetManager::GetAssetHandle("ShadowMap"));
		shadowmap = ShadowMap(4096, 4096);
	}

	void Renderer3D::ShutDown()
	{
		GT_PROFILE_FUNCTION();

		// Clean up opengl resources before opengl context is destroyed
		models.clear();

		m_ShadowShader.~Shader();
		s_ModelShader.~Shader();

	}

	void Renderer3D::BeginScene(Camera& camera)
	{
		GT_PROFILE_FUNCTION();

		GT_CORE_ASSERT(state != Renderer3DState::BeginScene, "Renderer3D::BeginScene is already Called!");
		state = Renderer3DState::BeginScene;


		SetViewProjection(camera.GetViewProjection(), camera.GetPosition());

	}

	void Renderer3D::SetViewProjection(const glm::mat4& viewProjection, const glm::vec3& viewPos)
	{
		s_ViewProjectionMatrix = viewProjection;
		s_viewPos = viewPos;
		models.clear();
	}
	void Renderer3D::SetLight(const glm::vec3& lightpos, const glm::vec3& lightcolor)
	{
		s_ModelShader->Bind();
		s_ModelShader->SetUniform3f("u_LightPos", lightpos);
		s_ModelShader->SetUniform3f("u_LightColor", lightcolor);
	}
	void Renderer3D::AddLight(const Light& light, const glm::mat4& lightSpaceMatrix)
	{
		s_Lights.push_back({ light, lightSpaceMatrix });
	}
	void  Renderer3D::EndScene()
	{
		GT_PROFILE_FUNCTION();

		GT_CORE_ASSERT(state != Renderer3DState::EndScene, "You Should Call BeginScene First!");
		state = Renderer3DState::EndScene;

		Flush();

		s_Lights.clear();

		//m_Framebuffer->Unbind();
	}

	void Renderer3D::Flush()
	{

		Shader& shader = s_ModelShader;
		shader->Bind();

		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, shadowmap.GetDepthTextureID());

		shader->SetUniform1i("u_ShadowMap", 7);

		shader->SetUniformMat4("u_ViewProjection", s_ViewProjectionMatrix);
		shader->SetUniform3f("u_ViewPos", s_viewPos);

		unsigned int lightslots = 0;
		for (auto& [light, spacematrix] : s_Lights)
		{
			switch (light.type)
			{
			case LightType::Ambient:
				break;
			case LightType::Point:
				lightslots |= 1u;
				PointLight plight = light.GetPointLight();
				shader->SetUniformPointLight("u_pointLight", plight);
				shader->SetUniformMat4("u_LightSpaceMatrix", spacematrix);
				break;
			case LightType::Directional:
				lightslots |= 2u;
				DirectionalLight dlight = light.GetDirectionalLight();
				shader->SetUniformDirectionalLight("u_dirLight", dlight);
				shader->SetUniformMat4("u_LightSpaceMatrix", spacematrix);
				break;
			case LightType::Spot:
				lightslots |= 4u;
				SpotLight slight = light.GetSpotLight();
				shader->SetUniformSpotLight("u_spotLight", slight);
				shader->SetUniformMat4("u_LightSpaceMatrix", spacematrix);
				break;
			}
		}

		shader->SetUniform1ui("u_LightSlots", lightslots);
		for (auto& [transform, model,ID] : models)
		{
			shader->SetUniform1i("u_EntityID", ID);
			shader->SetUniformMat4("u_ViewProjection", s_ViewProjectionMatrix);

			model->Draw(transform, ExtractFrustum(s_ViewProjectionMatrix));

			if (IsShowAABB)
			{
				GPUAABB aabb = model->GetAABB();
				DrawAABB(transform, aabb);
			}
		}
	}
	void Renderer3D::DrawAABB(const glm::mat4& transform, GPUAABB aabb, glm::vec4 color)
	{
		glm::vec3 min = aabb.Min;
		glm::vec3 max = aabb.Max;

		glm::vec3 corners[8] = {
		  // bl br tr tl
		  transform * glm::vec4(min.x, min.y, min.z, 1.0f),
		  transform * glm::vec4(max.x, min.y, min.z, 1.0f),
		  transform * glm::vec4(max.x, max.y, min.z, 1.0f),
		  transform * glm::vec4(min.x, max.y, min.z, 1.0f),
		  // bl br tr tl
		  transform* glm::vec4(min.x, min.y, max.z, 1.0f),
		  transform* glm::vec4(max.x, min.y, max.z, 1.0f),
		  transform* glm::vec4(max.x, max.y, max.z, 1.0f),
		  transform* glm::vec4(min.x, max.y, max.z, 1.0f),
		};

		// 后方面
		for (int j = 0; j < 4; ++j)
			Renderer2D::DrawLine(corners[j], corners[(j + 1) % 4], color);
		// 前方面
		for (int j = 4; j < 8; ++j)
			Renderer2D::DrawLine(corners[j], corners[4 + ((j - 4 + 1) % 4)], color);
		// 连接前后
		for (int j = 0; j < 4; ++j)
			Renderer2D::DrawLine(corners[j], corners[j + 4], color);
	}
	void Renderer3D::ShowAABB(bool show) { IsShowAABB = show; }
	void Renderer3D::DrawModel(const glm::mat4& transform, Model& model)
	{
		if (!model->hasShader) model->SetShader(s_ModelShader);
		models.push_back({ transform, model,s_CurrentEntityID });
	}
	void Renderer3D::SetCurrentEntityID(int entityID)
	{
		s_CurrentEntityID = entityID;
	}

	void Renderer3D::RenderShadowMap(ShadowMap& shadowMap)
	{
		// 1. 绑定阴影 FBO
		shadowMap.Bind();

		// 2. 设置视口（必须是阴影贴图的大小）

		// 3. 清空深度缓冲
		glClear(GL_DEPTH_BUFFER_BIT);

		Shader& shader = m_ShadowShader;
		// 4. 使用深度着色器
		shader->Bind();

		for (auto& [light, spacematrix] : s_Lights)
		{
			switch (light.type)
			{
			case LightType::Ambient:
				break;
			case LightType::Directional:
				shader->SetUniformMat4("u_LightSpaceMatrix", spacematrix);
				break;
			case LightType::Point:
			case LightType::Spot:
				shader->SetUniformMat4("u_LightSpaceMatrix", spacematrix);
				break;
			}
		}

		// 5. 渲染场景中的所有物体
		for (auto& [transform, model, ID] : models)
		{
			shader->SetUniformMat4("u_Model", transform);
			model->DrawForShadowMap(transform);
		}

		// 6. 恢复默认 FBO
		shadowMap.Unbind();
	}

	Renderer3D::Statistics& Renderer3D::GetStats()
	{
		return s_stats;
	}
	void Renderer3D::ResetStats()
	{
		s_stats = Statistics();
	}
}