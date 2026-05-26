#pragma once
#include "Camera.h"
#include "EditorCamera.h"
#include "OrthographicCamera.h"
#include "GT/Scene/Components.h"
#include "Shader.h"
#include "Texture.h"
namespace GT
{

	struct QuadState
	{
		glm::vec3 Position[4];
		glm::vec4 Color = { 1.0f,1.0f,1.0f,1.0f };
		// Bottom_Left(0,0), Top_right(1,1)
		glm::vec2 TexCoords[4];
		int TextureIndex = 0;
		float TilingFactor = 1.0f;
		int EntityID = -1;
	};
	struct CircleState
	{
		glm::vec3 WorldPosition[4];
		glm::vec3 LocalPosition[4];
		glm::vec4 color = { 1.0f,1.0f,1.0f,1.0f };

		float Thickness = 1.0f;
		float Fade = 0.005f;
		int EntityID = -1;
		int TexIndex = 0;

	};
	struct UIState
	{
		glm::vec3 Position[4];
		glm::vec4 color = { 1.0f,1.0f,1.0f,1.0f };
		glm::vec2 TexCoords[4] =
		{   { 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};
		int EntityID = -1;
		int TexIndex = 0;

	};
	enum struct RendererState
	{
		None = 0,
		BeginScene,
		EndScene
	};
	struct Rect
	{
		glm::vec2 origin;
		glm::vec2 size;
	};
	class Renderer2D
	{
	public:
		static void Init();
		static void ShutDown();

		static void BeginScene(Camera& camera);

		static void SetViewProjection(const glm::mat4& viewProjection, const glm::mat4& Projection);

		static void EndScene();

		static void SetCurrentEntityID(int entityID);

		static void UI(const Rect& rect, const glm::vec4& color, const Ref<Texture2D>& texture = nullptr);
		static void Text(const std::string& text, glm::vec2 position, const glm::vec4& color);

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D>& texture, int tilingfactor = 1);
		static void DrawQuad(const glm::mat4& transform, const SpriteRendererComponent& sprite);

		static void DrawCube(const glm::mat4& transform, const glm::vec4& color);
		static void DrawCube(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D>& texture);

		static void DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness = 1.0f, float fade = 0.005f);
		static void DrawCircle(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D>& texture, float thickness = 1.0f, float fade = 0.005f);

		static void Draw(const QuadState& state);
		static void Draw(const CircleState& state);
		static void Draw(const UIState& state);


		static float GetTextureSlotIndex(const Ref<Texture2D>& texture);
		static void SetTextureCoords(glm::vec2 offset = {0.0f,0.0f}, glm::vec2 size = {1.0f,1.0f});

		static void DrawLine(const glm::vec3& p0, glm::vec3& p1, const glm::vec4& color);

		static void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawRect(const glm::vec3& position0, const glm::vec3& position1, const glm::vec3& position2, const glm::vec3& position3, const glm::vec4& color);
		static void DrawRect(const glm::mat4& transform, const glm::vec4& color);
		
		static float GetLineWidth();
		static void SetLineWidth(float width);

		struct Statistics
		{
			uint32_t DrawCalls = 0;

			uint32_t QuadCount = 0;
			uint32_t CircleCount = 0;
			uint32_t LineCount = 0;

			uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
		};

		

		static Statistics GetStats();
		static void ResetStats();

		static void OnWindowResize(unsigned int width, unsigned int height);
	private:
		static glm::mat4 m_viewProjection;
		static RendererState state;
		static int s_CurrentEntityID;
		static void StartNewBatch();
		static void Flush();
	};
}


