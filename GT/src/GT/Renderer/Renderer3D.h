#pragma once
#include "Camera.h"
#include "EditorCamera.h"
#include "OrthographicCamera.h"

#include "Shader.h"
#include "Model.h"
#include "GT/Assets/AssetsHandle.h"

namespace GT
{

	enum struct Renderer3DState
	{
		None = 0,
		BeginScene,
		EndScene
	};
	

	
	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(Camera& camera, glm::mat4& transform);
		static void BeginScene(EditorCamera& camera);
		static void BeginScene(OrthographicCamera& camera);

		static void SetViewProjection(const glm::mat4& viewProjection, const glm::vec3& viewPos);
		static void SetLight(const glm::vec3& lightpos, const glm::vec3& lightcolor);
		static void AddLight(Light& light);
		static void EndScene();

		static void DrawModel(const glm::mat4& transform, Ref<Model>& model);

		static void SetCurrentEntityID(int entityID);

		static void ShowAABB(bool show);
		static void Flush();
		static void DrawAABB(const glm::mat4& transform, GPUAABB aabb, glm::vec4 color = { 0.0f,1.0f,0.5f,1.0f });

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t Meshes = 0;

			uint32_t VerticiesCount = 0;

			uint32_t GetTotalVertexCount() const { return VerticiesCount; }
		};



		static Statistics& GetStats();
		static void ResetStats();

		static void OnWindowResize(unsigned int width, unsigned int height);
	private:
		static bool IsShowAABB;
		static Renderer3DState state;
		static int s_CurrentEntityID;
		static std::vector<Light> s_Lights;
		static glm::mat4 s_ViewProjectionMatrix;
		static glm::vec3& s_viewPos;
		static RefHandle<Shader> s_ModelShader;
	};
}


