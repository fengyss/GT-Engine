#include "gtpch.h"
#include "Renderer2D.h"
#include "RenderCommand.h"
#include <glm/gtc/matrix_transform.hpp>
#include "GT/Assets/AssetsHandle.h"
#include "GT/Assets/AssetsManager.h"

namespace GT
{
	int Renderer2D::s_CurrentEntityID = -1;

	RendererState Renderer2D::state = RendererState::None;

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;

		// Editor-only
		int EntityID = -1;
	};
	struct CircleVertex
	{
		glm::vec3 WorldPosition;
		glm::vec3 LocalPosition;
		glm::vec4 Color;
		float Thickness;
		float Fade;

		// Editor-only
		int EntityID;
	};
	struct LineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;

		// Editor-only
		int EntityID;
	};

	struct Renderer2DData
	{
		const uint32_t MaxQuads = 5000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps

		RefHandle<Shader> QuadShader;
		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;

		Ref<VertexArray> CircleVertexArray;
		Ref<VertexBuffer> CircleVertexBuffer;
		RefHandle<Shader> CircleShader;

		RefHandle<Shader> LineShader;
		Ref<VertexArray> LineVertexArray;
		Ref<VertexBuffer> LineVertexBuffer;



		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		uint32_t CircleIndexCount = 0;
		CircleVertex* CircleVertexBufferBase = nullptr;
		CircleVertex* CircleVertexBufferPtr = nullptr;

		uint32_t LineVertexCount = 0;
		LineVertex* LineVertexBufferBase = nullptr;
		LineVertex* LineVertexBufferPtr = nullptr;

		float LineWidth = 2.0f;

		Ref<Texture2D> WhiteTexture;


		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = white texture

		// local position
		glm::vec4 QuadVertexPositions[4];
		glm::vec4 CubeVertexPositions[6][4];
		glm::vec2 QuadTexCoords[4];

		Renderer2D::Statistics Stats;
	};

	static Renderer2DData s_Data;
	static QuadState quadState;
	static CircleState circleState;
	

	void Renderer2D::Init()
	{
		GT_PROFILE_FUNCTION();


		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		{
			BufferLayout layout = {
				{ ShaderDataType::Float3, "a_Position"  },
				{ ShaderDataType::Float4, "a_Color"  },
				{ ShaderDataType::Float2, "a_TexCoord"  },
				{ ShaderDataType::Float, "a_TexIndex"  },
				{ ShaderDataType::Float, "a_TilingFactor"  },
				{ ShaderDataType::Int, "a_EntityID"  },
			};
			s_Data.QuadVertexBuffer->SetLayout(layout);
		}
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];



		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for(uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;
			offset += 4;
		}

		Ref<IndexBuffer> squareIB;
		squareIB = IndexBuffer::Create((float*)quadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(squareIB);
		delete[] quadIndices;


		// Circles
		s_Data.CircleVertexArray = VertexArray::Create();

		s_Data.CircleVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(CircleVertex));
		s_Data.CircleVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_WorldPosition" },
			{ ShaderDataType::Float3, "a_LocalPosition" },
			{ ShaderDataType::Float4, "a_Color"         },
			{ ShaderDataType::Float,  "a_Thickness"     },
			{ ShaderDataType::Float,  "a_Fade"          },
			{ ShaderDataType::Int,    "a_EntityID"      }
			});
		s_Data.CircleVertexArray->AddVertexBuffer(s_Data.CircleVertexBuffer);
		s_Data.CircleVertexArray->SetIndexBuffer(squareIB); // Use quad IB
		s_Data.CircleVertexBufferBase = new CircleVertex[s_Data.MaxVertices];

		// Lines
		s_Data.LineVertexArray = VertexArray::Create();

		s_Data.LineVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(LineVertex));
		s_Data.LineVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color"    },
			{ ShaderDataType::Int,    "a_EntityID" }
			});
		s_Data.LineVertexArray->AddVertexBuffer(s_Data.LineVertexBuffer);
		s_Data.LineVertexBufferBase = new LineVertex[s_Data.MaxVertices];



		
		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));


		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0;i < s_Data.MaxTextureSlots;i++)
		{
			samplers[i] = i;
		}

		
		s_Data.QuadShader = CreateHandle<Shader>("Renderer2D_Quad");
		s_Data.CircleShader = CreateHandle<Shader>("Renderer2D_Circle");
		s_Data.LineShader = CreateHandle<Shader>("Renderer2D_Line");

		s_Data.QuadShader->Get()->Bind();
		s_Data.QuadShader->Get()->SetUniformiv("u_Textures", samplers, s_Data.MaxTextureSlots);





		// Set texture slot0 to white texture
		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		if (0) 
		{
			// top left
			s_Data.QuadVertexPositions[0] = { 0.0f, -1.0f, 0.0f, 1.0f };
			s_Data.QuadVertexPositions[1] = { 1.0f, -1.0f, 0.0f, 1.0f };
			s_Data.QuadVertexPositions[2] = { 1.0f, 0.0f, 0.0f, 1.0f };
			s_Data.QuadVertexPositions[3] = { 0.0f, 0.0f, 0.0f, 1.0f };
		}
		else 
		{
			// center
			s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
			s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
			s_Data.QuadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
			s_Data.QuadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };

			// front
			s_Data.CubeVertexPositions[0][0] = { -0.5f, -0.5f, 0.5f, 1.0f };
			s_Data.CubeVertexPositions[0][1] = { 0.5f, -0.5f, 0.5f, 1.0f };
			s_Data.CubeVertexPositions[0][2] = { 0.5f, 0.5f, 0.5f, 1.0f };
			s_Data.CubeVertexPositions[0][3] = { -0.5f, 0.5f, 0.5f, 1.0f };
			// back
			s_Data.CubeVertexPositions[1][0] = { -0.5f, -0.5f, -0.5f, 1.0f };
			s_Data.CubeVertexPositions[1][1] = { 0.5f, -0.5f, -0.5f, 1.0f };
			s_Data.CubeVertexPositions[1][2] = { 0.5f, 0.5f, -0.5f, 1.0f };
			s_Data.CubeVertexPositions[1][3] = { -0.5f, 0.5f, -0.5f, 1.0f };
			// left
			s_Data.CubeVertexPositions[2][0] = { -0.5f, -0.5f, 0.5f, 1.0f };
			s_Data.CubeVertexPositions[2][1] = { -0.5f, -0.5f, -0.5f, 1.0f };
			s_Data.CubeVertexPositions[2][2] = { -0.5f, 0.5f, -0.5f, 1.0f };
			s_Data.CubeVertexPositions[2][3] = { -0.5f, 0.5f, 0.5f, 1.0f };
			// right
			s_Data.CubeVertexPositions[3][0] = { 0.5f, -0.5f, 0.5f, 1.0f };
			s_Data.CubeVertexPositions[3][1] = { 0.5f, -0.5f, -0.5f, 1.0f };
			s_Data.CubeVertexPositions[3][2] = { 0.5f, 0.5f, -0.5f, 1.0f };
			s_Data.CubeVertexPositions[3][3] = { 0.5f, 0.5f, 0.5f, 1.0f };
			// bottom
			s_Data.CubeVertexPositions[4][0] = { -0.5f, -0.5f, 0.5f, 1.0f };
			s_Data.CubeVertexPositions[4][1] = { 0.5f, -0.5f, 0.5f, 1.0f };
			s_Data.CubeVertexPositions[4][2] = { 0.5f, -0.5f, -0.5f, 1.0f };
			s_Data.CubeVertexPositions[4][3] = { -0.5f, -0.5f, -0.5f, 1.0f };
			// top
			s_Data.CubeVertexPositions[5][0] = { -0.5f, 0.5f, 0.5f, 1.0f };
			s_Data.CubeVertexPositions[5][1] = { 0.5f, 0.5f, 0.5f, 1.0f };
			s_Data.CubeVertexPositions[5][2] = { 0.5f, 0.5f, -0.5f, 1.0f };
			s_Data.CubeVertexPositions[5][3] = { -0.5f, 0.5f, -0.5f, 1.0f };

		}
		s_Data.QuadTexCoords[0] = { 0.0f, 0.0f };
		s_Data.QuadTexCoords[1] = { 1.0f, 0.0f };
		s_Data.QuadTexCoords[2] = { 1.0f, 1.0f };
		s_Data.QuadTexCoords[3] = { 0.0f, 1.0f };
	}
    void Renderer2D::Shutdown()  
    {  
        GT_PROFILE_FUNCTION();  
		// Clean up opengl resources before opengl context is destroyed
		s_Data.QuadShader.reset();
		s_Data.QuadVertexArray.reset();
		s_Data.QuadVertexBuffer.reset();

		s_Data.CircleShader.reset();
		s_Data.CircleVertexArray.reset();
		s_Data.CircleVertexBuffer.reset();
    }

	void Renderer2D::BeginScene(Camera& camera, glm::mat4& transform)
	{
		GT_PROFILE_FUNCTION();

		GT_CORE_ASSERT(state != RendererState::BeginScene, "Renderer2D::BeginScene is already Called!");
		state = RendererState::BeginScene;

		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

		SetViewProjection(viewProj);

		s_Data.TextureSlotIndex = 1;
		StartNewBatch();
	}
	glm::vec3 camerapos;
	void Renderer2D::BeginScene(EditorCamera& camera)
	{
		GT_PROFILE_FUNCTION();

		GT_CORE_ASSERT(state != RendererState::BeginScene, "Renderer2D::BeginScene is already Called!");
		state = RendererState::BeginScene;

		SetViewProjection(camera.GetViewProjection());
		camerapos = camera.GetPosition();
		
		s_Data.TextureSlotIndex = 1;
		StartNewBatch();
	}

	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		GT_PROFILE_FUNCTION();

		GT_CORE_ASSERT(state != RendererState::BeginScene, "Renderer2D::BeginScene is already Called!");
		state = RendererState::BeginScene;

		SetViewProjection(camera.GetViewProjectionMatrix());
		
		s_Data.TextureSlotIndex = 1;
		StartNewBatch();
	}
	void  Renderer2D::EndScene()
	{
		GT_PROFILE_FUNCTION();

		GT_CORE_ASSERT(state != RendererState::EndScene, "You Should Call BeginScene First!");
		state = RendererState::EndScene;

		if(s_Data.Stats.QuadCount == 0)
			return;
		
		Flush();
	}

	void Renderer2D::Flush()
	{
		GT_PROFILE_FUNCTION();

		// For Quad
		if(s_Data.QuadIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
			s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

			for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			{
				s_Data.TextureSlots[i]->Bind(i);
			}
			s_Data.QuadShader->Get()->Bind();
			RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
			s_Data.Stats.DrawCalls++;
		}

		// For Circle
		if(s_Data.CircleIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CircleVertexBufferPtr - (uint8_t*)s_Data.CircleVertexBufferBase);
			s_Data.CircleVertexBuffer->SetData(s_Data.CircleVertexBufferBase, dataSize);
			s_Data.CircleShader->Get()->Bind();
			RenderCommand::DrawIndexed(s_Data.CircleVertexArray, s_Data.CircleIndexCount);
			s_Data.Stats.DrawCalls++;
		}

		// For Line
		if (s_Data.LineVertexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferBase);
			s_Data.LineVertexBuffer->SetData(s_Data.LineVertexBufferBase, dataSize);

			s_Data.LineShader->Get()->Bind();
			RenderCommand::SetLineWidth(s_Data.LineWidth);
			RenderCommand::DrawLines(s_Data.LineVertexArray, s_Data.LineVertexCount);
			s_Data.Stats.DrawCalls++;
		}
	}
	glm::vec3 cameraright, cameraup;
	void Renderer2D::SetViewProjection(const glm::mat4& viewProjection)
	{
		s_Data.QuadShader->Get()->Bind();
		s_Data.QuadShader->Get()->SetUniformMat4("u_ViewProjection", viewProjection);
		s_Data.CircleShader->Get()->Bind();
		s_Data.CircleShader->Get()->SetUniformMat4("u_ViewProjection", viewProjection);
		s_Data.LineShader->Get()->Bind();
		s_Data.LineShader->Get()->SetUniformMat4("u_ViewProjection", viewProjection);

		cameraright = viewProjection[0];
		cameraup = viewProjection[1];

	}

	void Renderer2D::DrawLine(const glm::vec3& p0, glm::vec3& p1, const glm::vec4& color)
	{
		s_Data.LineVertexBufferPtr->Position = p0;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr->EntityID = s_CurrentEntityID;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexBufferPtr->Position = p1;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr->EntityID = s_CurrentEntityID;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexCount += 2;
	}

	void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		glm::vec3 p0 = glm::vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p1 = glm::vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p2 = glm::vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
		glm::vec3 p3 = glm::vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);

		DrawLine(p0, p1, color);
		DrawLine(p1, p2, color);
		DrawLine(p2, p3, color);
		DrawLine(p3, p0, color);
	}

	void Renderer2D::DrawRect(const glm::vec3& position0, const glm::vec3& position1, const glm::vec3& position2, const glm::vec3& position3, const glm::vec4& color)
	{
		glm::vec3 p0 = position0;
		glm::vec3 p1 = position1;
		glm::vec3 p2 = position2;
		glm::vec3 p3 = position3;

		DrawLine(p0, p1, color);
		DrawLine(p1, p2, color);
		DrawLine(p2, p3, color);
		DrawLine(p3, p0, color);
	}


	void Renderer2D::DrawRect(const glm::mat4& transform, const glm::vec4& color)
	{
		glm::vec3 lineVertices[4];
		for (size_t i = 0; i < 4; i++)
			lineVertices[i] = transform * s_Data.QuadVertexPositions[i];

		DrawLine(lineVertices[0], lineVertices[1], color);
		DrawLine(lineVertices[1], lineVertices[2], color);
		DrawLine(lineVertices[2], lineVertices[3], color);
		DrawLine(lineVertices[3], lineVertices[0], color);
	}

	float Renderer2D::GetLineWidth()
	{
		return s_Data.LineWidth;
	}

	void Renderer2D::SetLineWidth(float width)
	{
		s_Data.LineWidth = width;
	}

	void Renderer2D::SetCurrentEntityID(int entityID)
	{
		s_CurrentEntityID = entityID;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		for (size_t i = 0; i < 4; i++)
			quadState.Position[i] = transform * s_Data.QuadVertexPositions[i];
		quadState.Color = color;
		quadState.TextureIndex = 0.0f;
		quadState.TilingFactor = 1.0f;
		quadState.EntityID = s_CurrentEntityID;
		SetTextureCoords();

		Draw(quadState);
	}

	void Renderer2D::DrawParticleQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		glm::vec3 center = position;
		//glm::vec3 right = cameraright * size.x;
		//glm::vec3 up = cameraup * size.y;

		glm::vec3 lookDir = glm::normalize(camerapos - position);

		glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), lookDir));
		glm::vec3 up = glm::normalize(glm::cross(lookDir, right));

		// Quad
		quadState.Position[0] = center - right - up; // TL
		quadState.Position[1] = center + right - up; // TR
		quadState.Position[2] = center + right + up; // BR
		quadState.Position[3] = center - right + up; // BL

		quadState.Color = color;
		quadState.TextureIndex = 0.0f;
		quadState.TilingFactor = 1.0f;
		quadState.EntityID = s_CurrentEntityID;
		SetTextureCoords();

		Draw(quadState);
	}

	void Renderer2D::DrawParticleQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const Ref<Texture2D>& texture)
	{
		glm::vec3 center = position;
		glm::vec3 right = cameraright * size.x;
		glm::vec3 up = cameraup * size.y;
		// Quad
		quadState.Position[0] = center - right - up; // TL
		quadState.Position[1] = center + right - up; // TR
		quadState.Position[2] = center + right + up; // BR
		quadState.Position[3] = center - right + up; // BL

		quadState.Color = color;
		quadState.TextureIndex = GetTextureSlotIndex(texture);
		quadState.TilingFactor = 1.0f;
		quadState.EntityID = s_CurrentEntityID;
		SetTextureCoords();

		Draw(quadState);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D>& texture, int tilingfactor)
	{
		for (size_t i = 0; i < 4; i++)
		{
			quadState.Position[i] = transform * s_Data.QuadVertexPositions[i];
		}

		SetTextureCoords();

		quadState.Color = color;
		quadState.TextureIndex = GetTextureSlotIndex(texture);
		quadState.TilingFactor = tilingfactor;


		quadState.EntityID = s_CurrentEntityID;
		Draw(quadState);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const SpriteRendererComponent& sprite)
	{
		for (size_t i = 0; i < 4; i++)
		{
			quadState.Position[i] = transform * s_Data.QuadVertexPositions[i];
		}

		

		quadState.Color = sprite.Color;


		if (sprite.texture)
		{
			SetTextureCoords(sprite.UVOffset, sprite.UVSize);
			quadState.TextureIndex = GetTextureSlotIndex(sprite.texture->Get());
			quadState.TilingFactor = sprite.TilingFactor;
		}
		else 
		{
			SetTextureCoords();
			quadState.TextureIndex = 0.0f;
			quadState.TilingFactor = 1.0f;
		}

		quadState.EntityID = s_CurrentEntityID;
		Draw(quadState);
	}

	void Renderer2D::DrawCube(const glm::mat4& transform, const glm::vec4& color)
	{
		quadState.TextureIndex = 0.0f;
		quadState.TilingFactor = 1.0f;
		quadState.EntityID = s_CurrentEntityID;
		quadState.Color = color;
		SetTextureCoords();
		for (size_t i = 0; i < 6; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				quadState.Position[j] = transform * s_Data.CubeVertexPositions[i][j];
			}

			if (i <= 1)
				DrawRect(quadState.Position[0], quadState.Position[1], quadState.Position[2], quadState.Position[3], glm::vec4(glm::vec3(0.0f), 1.0f));
			else if (i <= 3)
			{
				DrawLine(quadState.Position[0], quadState.Position[1], { 0.3f,0.7f,0.1f,1.0f });
				DrawLine(quadState.Position[2], quadState.Position[3], { 0.3f,0.7f,0.1f,1.0f });
			}
			Draw(quadState);
		}

	}
	void Renderer2D::DrawCube(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D>& texture)
	{

		quadState.TextureIndex = GetTextureSlotIndex(texture);
		quadState.TilingFactor = 1.0f;
		quadState.EntityID = s_CurrentEntityID;
		quadState.Color = color;
		SetTextureCoords();
		for (size_t i = 0; i < 6; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				quadState.Position[j] = transform * s_Data.CubeVertexPositions[i][j];
			}

			if (i <= 1)
				DrawRect(quadState.Position[0], quadState.Position[1], quadState.Position[2], quadState.Position[3], glm::vec4(glm::vec3(0.0f), 1.0f));
			else if (i <= 3)
			{
				DrawLine(quadState.Position[0], quadState.Position[1], { 0.3f,0.7f,0.1f,1.0f });
				DrawLine(quadState.Position[2], quadState.Position[3], { 0.3f,0.7f,0.1f,1.0f });
			}
			Draw(quadState);
		}

	}

	void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade)
	{
		GT_PROFILE_FUNCTION();


		for (size_t i = 0; i < 4; i++)
		{
			circleState.WorldPosition[i] = transform * s_Data.QuadVertexPositions[i];
			circleState.LocalPosition[i] = s_Data.QuadVertexPositions[i] * 2.0f;
			 // *2.0f because the circle shader uses the local position to calculate the distance to the center of the circle, and the quad vertex positions are from -0.5 to 0.5
			 // so we need to multiply by 2 to get the distance from -1 to 1
			 // which is the range of the local position in the circle shader
			 // if we don't multiply by 2, the distance will be from -0.5 to 0.5, and the circle will be smaller than it should be
			 // and the thickness and fade will not work correctly
		}
		circleState.color = color;
		circleState.Thickness = thickness;
		circleState.Fade = fade;
		circleState.EntityID = s_CurrentEntityID;

		Draw(circleState);
	}

	void Renderer2D::Draw(const QuadState& state)
	{
		for (int i = 0;i < 4;i++)
		{
			s_Data.QuadVertexBufferPtr->Position = state.Position[i];
			s_Data.QuadVertexBufferPtr->Color = state.Color;

			s_Data.QuadVertexBufferPtr->TexCoord = state.TexCoords[i];
				//state.TexCoords[i];

			s_Data.QuadVertexBufferPtr->TexIndex = state.TextureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = state.TilingFactor;
			s_Data.QuadVertexBufferPtr->EntityID = state.EntityID;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;

		if(s_Data.QuadIndexCount >= s_Data.MaxIndices)
		{
			Flush();
			StartNewBatch();
		}
	}

	void Renderer2D::Draw(const CircleState& state)
	{
		for (size_t i = 0; i < 4; i++)
		{
			s_Data.CircleVertexBufferPtr->WorldPosition = state.WorldPosition[i];
			s_Data.CircleVertexBufferPtr->LocalPosition = state.LocalPosition[i];
			s_Data.CircleVertexBufferPtr->Color = state.color;
			s_Data.CircleVertexBufferPtr->Thickness = state.Thickness;
			s_Data.CircleVertexBufferPtr->Fade = state.Fade;
			s_Data.CircleVertexBufferPtr->EntityID = state.EntityID;
			s_Data.CircleVertexBufferPtr++;
		}

		s_Data.CircleIndexCount += 6;

		s_Data.Stats.QuadCount++;

		if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
		{
			Flush();
			StartNewBatch();
		}
	}


	float Renderer2D::GetTextureSlotIndex(const Ref<Texture2D>& texture)
	{
		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}
		//std::cout << textureIndex << '\n';
		return textureIndex;
	}

	void Renderer2D::SetTextureCoords(glm::vec2 offset, glm::vec2 size)
	{
		auto TopLeft = offset;
		auto BottomRight = offset + size;
		quadState.TexCoords[0] = { TopLeft.x,TopLeft.y };
		quadState.TexCoords[1] = { BottomRight.x,TopLeft.y };
		quadState.TexCoords[2] = { BottomRight.x,BottomRight.y };
		quadState.TexCoords[3] = { TopLeft.x,BottomRight.y };
	}


	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

	void Renderer2D::StartNewBatch()
	{

		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.QuadIndexCount = 0;

		s_Data.CircleIndexCount = 0;
		s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;

		s_Data.LineVertexCount = 0;
		s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;

		s_Data.TextureSlotIndex = 1;

	}

}