#include "gtpch.h"
#include "Renderer2D.h"
#include "RenderCommand.h"
#include <glm/gtc/matrix_transform.hpp>

#include "glad/glad.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "GT/Utils/PlatformUtils.h"

namespace GT
{
	int Renderer2D::s_CurrentEntityID = -1;
	RendererState Renderer2D::state = RendererState::None;
	glm::mat4 Renderer2D::m_viewProjection;

	struct Renderer2DData
	{
		const uint32_t MaxQuads = 2000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps

		Shader QuadShader;
		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;

		Ref<VertexArray> CircleVertexArray;
		Ref<VertexBuffer> CircleVertexBuffer;
		Shader CircleShader;

		Shader LineShader;
		Ref<VertexArray> LineVertexArray;
		Ref<VertexBuffer> LineVertexBuffer;


		Shader UIShader;
		Ref<VertexArray> UIVertexArray;
		Ref<VertexBuffer> UIVertexBuffer;


		Shader TextShader;
		Ref<VertexArray> TextVertexArray;
		Ref<VertexBuffer> TextVertexBuffer;


		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		uint32_t CircleIndexCount = 0;
		CircleVertex* CircleVertexBufferBase = nullptr;
		CircleVertex* CircleVertexBufferPtr = nullptr;

		uint32_t LineVertexCount = 0;
		LineVertex* LineVertexBufferBase = nullptr;
		LineVertex* LineVertexBufferPtr = nullptr;


		uint32_t UIVertexCount = 0;
		UIVertex* UIVertexBufferBase = nullptr;
		UIVertex* UIVertexBufferPtr = nullptr;

		float LineWidth = 2.0f;

		Texture2D WhiteTexture;


		std::array<Texture2D, MaxTextureSlots> TextureSlots;
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
	static UIState uiState;
		struct Character {
		unsigned int TextureID; // ID handle of the glyph texture
		glm::ivec2   Size;      // Size of glyph
		glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
		unsigned int Advance;   // Horizontal offset to advance to next glyph
	};

	std::map<GLchar, Character> Characters;
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
			{ ShaderDataType::Int,    "a_EntityID"      },
			{ ShaderDataType::Int,    "a_TexIndex"      },
			});
		s_Data.CircleVertexArray->AddVertexBuffer(s_Data.CircleVertexBuffer);
		s_Data.CircleVertexArray->SetIndexBuffer(squareIB); // Use quad IB
		s_Data.CircleVertexBufferBase = new CircleVertex[s_Data.MaxVertices];



		// Lines
		s_Data.LineVertexArray = VertexArray::Create();

		s_Data.LineVertexBuffer = VertexBuffer::Create(1000 * sizeof(LineVertex));
		s_Data.LineVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color"    },
			{ ShaderDataType::Int,    "a_EntityID" }
			});
		s_Data.LineVertexArray->AddVertexBuffer(s_Data.LineVertexBuffer);
		s_Data.LineVertexBufferBase = new LineVertex[1000];

		// UIs
		s_Data.UIVertexArray = VertexArray::Create();

		s_Data.UIVertexBuffer = VertexBuffer::Create(1000 * sizeof(UIVertex));
		s_Data.UIVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color"    },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Int,    "a_TexIndex" },
			{ ShaderDataType::Int,    "a_EntityID" }
			});
		s_Data.UIVertexArray->AddVertexBuffer(s_Data.UIVertexBuffer);
		s_Data.UIVertexArray->SetIndexBuffer(squareIB); // Use quad IB
		s_Data.UIVertexBufferBase = new UIVertex[1000];


	
		
		s_Data.WhiteTexture = Texture2D("Checkerboard");

		
		s_Data.QuadShader = Shader("Renderer2D_Quad");
		s_Data.CircleShader = Shader("Renderer2D_Circle");
		s_Data.LineShader = Shader("Renderer2D_Line");
		s_Data.UIShader = Shader("Renderer2D_UI");





		// Set texture slot0 to white texture
		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		{
			// center
			s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
			s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
			s_Data.QuadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
			s_Data.QuadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };


			s_Data.CubeVertexPositions[0][0] = { -0.5f, -0.5f,  0.5f, 1.0f }; 
			s_Data.CubeVertexPositions[0][1] = { 0.5f, -0.5f,  0.5f, 1.0f }; 
			s_Data.CubeVertexPositions[0][2] = { 0.5f,  0.5f,  0.5f, 1.0f }; 
			s_Data.CubeVertexPositions[0][3] = { -0.5f,  0.5f,  0.5f, 1.0f }; 

			s_Data.CubeVertexPositions[1][0] = { 0.5f, -0.5f, -0.5f, 1.0f }; 
			s_Data.CubeVertexPositions[1][1] = { -0.5f, -0.5f, -0.5f, 1.0f }; 
			s_Data.CubeVertexPositions[1][2] = { -0.5f,  0.5f, -0.5f, 1.0f }; 
			s_Data.CubeVertexPositions[1][3] = { 0.5f,  0.5f, -0.5f, 1.0f }; 

			s_Data.CubeVertexPositions[2][0] = { -0.5f, -0.5f, -0.5f, 1.0f }; 
			s_Data.CubeVertexPositions[2][1] = { -0.5f, -0.5f,  0.5f, 1.0f }; 
			s_Data.CubeVertexPositions[2][2] = { -0.5f,  0.5f,  0.5f, 1.0f }; 
			s_Data.CubeVertexPositions[2][3] = { -0.5f,  0.5f, -0.5f, 1.0f }; 

			s_Data.CubeVertexPositions[3][0] = { 0.5f, -0.5f,  0.5f, 1.0f }; 
			s_Data.CubeVertexPositions[3][1] = { 0.5f, -0.5f, -0.5f, 1.0f }; 
			s_Data.CubeVertexPositions[3][2] = { 0.5f,  0.5f, -0.5f, 1.0f }; 
			s_Data.CubeVertexPositions[3][3] = { 0.5f,  0.5f,  0.5f, 1.0f }; 

			s_Data.CubeVertexPositions[4][0] = { -0.5f, -0.5f, -0.5f, 1.0f };
			s_Data.CubeVertexPositions[4][1] = { 0.5f, -0.5f, -0.5f, 1.0f };
			s_Data.CubeVertexPositions[4][2] = { 0.5f, -0.5f,  0.5f, 1.0f };
			s_Data.CubeVertexPositions[4][3] = { -0.5f, -0.5f,  0.5f, 1.0f }; 

			s_Data.CubeVertexPositions[5][0] = { -0.5f,  0.5f,  0.5f, 1.0f };
			s_Data.CubeVertexPositions[5][1] = { 0.5f,  0.5f,  0.5f, 1.0f };
			s_Data.CubeVertexPositions[5][2] = { 0.5f,  0.5f, -0.5f, 1.0f };
			s_Data.CubeVertexPositions[5][3] = { -0.5f,  0.5f, -0.5f, 1.0f }; 

		}
		s_Data.QuadTexCoords[0] = { 0.0f, 0.0f };
		s_Data.QuadTexCoords[1] = { 1.0f, 0.0f };
		s_Data.QuadTexCoords[2] = { 1.0f, 1.0f };
		s_Data.QuadTexCoords[3] = { 0.0f, 1.0f };


		s_Data.TextShader = Shader("Renderer2D_Text");
		
		s_Data.TextShader->Bind();
		s_Data.TextShader->SetUniform1i("u_Text", 0);
		s_Data.TextShader->SetUniformMat4("projection", glm::ortho(
			0.0f, 600.0f,
			0.0f, 600.0f
		));
		// FreeType
		// --------
		FT_Library ft;
		// All functions return a value different than 0 whenever an error occurred
		if (FT_Init_FreeType(&ft))
		{
			GT_CORE_ASSERT(false, "Failed to initialize FreeType Library");
		}

		std::string font_name = "Resources/fonts/opensans/OpenSans-Bold.ttf";
		if (font_name.empty())
		{
			std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
		}

		// load font as face
		FT_Face face;
		if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
			GT_CORE_ASSERT(false, "Failed to load font");
		}
		else {
			// set size to load glyphs as
			FT_Set_Pixel_Sizes(face, 0, 48);

			// disable byte-alignment restriction
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			// load first 128 characters of ASCII set
			for (unsigned char c = 0; c < 128; c++)
			{
				// Load character glyph 
				if (FT_Load_Char(face, c, FT_LOAD_RENDER))
				{
					std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
					continue;
				}
				// generate texture
				unsigned int texture;
				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);
				glTexImage2D(
					GL_TEXTURE_2D,
					0,
					GL_RED,
					face->glyph->bitmap.width,
					face->glyph->bitmap.rows,
					0,
					GL_RED,
					GL_UNSIGNED_BYTE,
					face->glyph->bitmap.buffer
				);
				// set texture options
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				// now store character for later use
				Character character = {
					texture,
					glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
					glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
					static_cast<unsigned int>(face->glyph->advance.x)
				};
				Characters.insert(std::pair<char, Character>(c, character));
			}
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		// destroy FreeType once we're finished
		FT_Done_Face(face);
		FT_Done_FreeType(ft);


		// configure VAO/VBO for texture quads
		// -----------------------------------
		s_Data.TextVertexArray = VertexArray::Create();
		s_Data.TextVertexBuffer = VertexBuffer::Create(sizeof(float) * 6 * 4);
		BufferLayout textLayout = {
			{ ShaderDataType::Float4, "a_vertex" }, // <vec2 pos, vec2 tex>
		};
		s_Data.TextVertexBuffer->SetLayout(textLayout);
		s_Data.TextVertexArray->AddVertexBuffer(s_Data.TextVertexBuffer);

	}
    void Renderer2D::ShutDown()  
    {  
        GT_PROFILE_FUNCTION();  
		// Clean up opengl resources before opengl context is destroyed


		s_Data.QuadShader.~Shader();
		s_Data.CircleShader.~Shader();
		s_Data.LineShader.~Shader();
		s_Data.UIShader.~Shader();
		s_Data.TextShader.~Shader();
		s_Data.WhiteTexture.~Texture2D();

		for (int i = 0;i < 32;i++) s_Data.TextureSlots[i].~Texture2D();

		s_Data.QuadVertexArray.reset();
		s_Data.QuadVertexBuffer.reset();
		delete s_Data.QuadVertexBufferBase;
		s_Data.QuadVertexBufferPtr = nullptr;

		

		s_Data.CircleVertexArray.reset();
		s_Data.CircleVertexBuffer.reset();
		delete s_Data.CircleVertexBufferBase;
		s_Data.CircleVertexBufferPtr = nullptr;




		s_Data.LineVertexArray.reset();
		s_Data.LineVertexBuffer.reset();
		delete s_Data.LineVertexBufferBase;
		s_Data.LineVertexBufferPtr = nullptr;


		s_Data.UIVertexArray.reset();
		s_Data.UIVertexBuffer.reset();
		delete s_Data.UIVertexBufferBase;
		s_Data.UIVertexBufferPtr = nullptr;


		s_Data.TextVertexArray.reset();
		s_Data.TextVertexBuffer.reset();


    }

	void Renderer2D::BeginScene(Camera& camera)
	{
		GT_PROFILE_FUNCTION();

		GT_CORE_ASSERT(state != RendererState::BeginScene, "Renderer2D::BeginScene is already Called!");
		state = RendererState::BeginScene;

		
		SetViewProjection(camera.GetViewProjection(), camera.GetProjection());

		s_Data.TextureSlotIndex = 1;
		StartNewBatch();
	}
	void  Renderer2D::EndScene()
	{
		GT_PROFILE_FUNCTION();

		GT_CORE_ASSERT(state != RendererState::EndScene, "You Should Call BeginScene First!");
		state = RendererState::EndScene;

		
		Flush();

	}

	void Renderer2D::Flush()
	{
		GT_PROFILE_FUNCTION();

		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		{
			s_Data.TextureSlots[i]->Bind(i);
		}

		// For Quad
		if(s_Data.QuadIndexCount)
		{
			s_Data.QuadShader->Bind();
			s_Data.QuadShader->SetUniformMat4("u_ViewProjection", m_viewProjection);
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
			s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);
			RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
			s_Data.Stats.DrawCalls++;
		}

		// For Circle
		if(s_Data.CircleIndexCount)
		{
			s_Data.CircleShader->Bind();
			s_Data.CircleShader->SetUniformMat4("u_ViewProjection", m_viewProjection);
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CircleVertexBufferPtr - (uint8_t*)s_Data.CircleVertexBufferBase);
			s_Data.CircleVertexBuffer->SetData(s_Data.CircleVertexBufferBase, dataSize);
			RenderCommand::DrawIndexed(s_Data.CircleVertexArray, s_Data.CircleIndexCount);
			s_Data.Stats.DrawCalls++;
		}

		// For Line
		if (s_Data.LineVertexCount)
		{
			s_Data.LineShader->Bind();
			s_Data.LineShader->SetUniformMat4("u_ViewProjection", m_viewProjection);
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferBase);
			s_Data.LineVertexBuffer->SetData(s_Data.LineVertexBufferBase, dataSize);

			RenderCommand::SetLineWidth(s_Data.LineWidth);
			RenderCommand::DrawLines(s_Data.LineVertexArray, s_Data.LineVertexCount);
			s_Data.Stats.DrawCalls++;
		}

		if (s_Data.UIVertexCount)
		{
			s_Data.UIShader->Bind();
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.UIVertexBufferPtr - (uint8_t*)s_Data.UIVertexBufferBase);
			s_Data.UIVertexBuffer->SetData(s_Data.UIVertexBufferBase, dataSize);
			RenderCommand::DrawIndexed(s_Data.UIVertexArray, s_Data.UIVertexCount);
		}
	}
	glm::vec3 cameraright, cameraup;
	void Renderer2D::SetViewProjection(const glm::mat4& viewProjection, const glm::mat4& Projection)
	{

		m_viewProjection = viewProjection;
		cameraright = viewProjection[0];
		cameraup = viewProjection[1];

		//s_Data.TextShader->Bind();
		//s_Data.TextShader->SetUniformMat4("projection", viewProjection);

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

		s_Data.Stats.LineCount++;
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

	void Renderer2D::UI(const Rect& rect, const glm::vec4& color, const Texture2D& texture)
	{
		uiState.color = color;
		if(texture)
		{
			uiState.TexIndex = GetTextureSlotIndex(texture);
		}
		else
		{
			uiState.TexIndex = 0.0f;
		}
		uiState.EntityID = s_CurrentEntityID;
		uiState.Position[0] = { rect.origin.x, rect.origin.y, 0.0f };
		uiState.Position[1] = { rect.origin.x + rect.size.x, rect.origin.y, 0.0f };
		uiState.Position[2] = { rect.origin.x + rect.size.x, rect.origin.y - rect.size.y, 0.0f };
		uiState.Position[3] = { rect.origin.x, rect.origin.y - rect.size.y, 0.0f };

		Draw(uiState);
	}

	float scale = 1.0f;
	void Renderer2D::Text(const std::string& text, glm::vec2 position, const glm::vec4& color)
	{

		s_Data.TextShader->Bind();
		s_Data.TextShader->SetUniform4f("u_TextColor", color);

		s_Data.TextVertexArray->Bind();
		scale = 1.0f;
		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			Character ch = Characters[*c];

			float xpos = position.x + ch.Bearing.x * scale;
			float ypos = position.y - (ch.Size.y - ch.Bearing.y) * scale;

			float w = ch.Size.x * scale;
			float h = ch.Size.y * scale;

			float vertices[6][4] = {
				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos,     ypos,       0.0f, 1.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },

				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },
				{ xpos + w, ypos + h,   1.0f, 0.0f }
			};

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, ch.TextureID);

			s_Data.TextVertexBuffer->SetData(vertices, sizeof(vertices)); 
			s_Data.TextVertexBuffer->Bind();

			//RenderCommand::DrawArrays(s_Data.TextVertexArray, 6);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			position.x += (ch.Advance >> 6) * scale;
		}
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

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, const Texture2D& texture, int tilingfactor)
	{
		
		float scale = float(texture->GetHeight()) / float(texture->GetWidth());
		for (size_t i = 0; i < 4; i++)
		{
			quadState.Position[i] = glm::scale(transform, glm::vec3(1.0f, scale, 1.0f)) * s_Data.QuadVertexPositions[i];
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
		float scale = 1.0f;
		quadState.Color = sprite.Color;


		if (sprite.texture)
		{
			SetTextureCoords(sprite.UVOffset, sprite.UVSize);
			quadState.TextureIndex = GetTextureSlotIndex(sprite.texture);
			quadState.TilingFactor = sprite.TilingFactor;
			auto& texture = sprite.texture;
			scale = float(texture->GetHeight()) / float(texture->GetWidth());
		}
		else 
		{
			SetTextureCoords();
			quadState.TextureIndex = 0.0f;
			quadState.TilingFactor = 1.0f;
		}

		for (size_t i = 0; i < 4; i++)
		{
			quadState.Position[i] = glm::scale(transform, glm::vec3(1.0f, scale, 1.0f)) * s_Data.QuadVertexPositions[i];
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
	void Renderer2D::DrawCube(const glm::mat4& transform, const glm::vec4& color, const Texture2D& texture)
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
		}
		circleState.color = color;
		circleState.Thickness = thickness;
		circleState.Fade = fade;
		circleState.EntityID = s_CurrentEntityID;

		Draw(circleState);
	}

	void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, const Texture2D& texture, float thickness, float fade)
	{
		GT_PROFILE_FUNCTION();

		for (size_t i = 0; i < 4; i++)
		{
			circleState.WorldPosition[i] = transform * s_Data.QuadVertexPositions[i];
			circleState.LocalPosition[i] = s_Data.QuadVertexPositions[i] * 2.0f;
		}
		circleState.color = color;
		circleState.Thickness = thickness;
		circleState.Fade = fade;
		circleState.EntityID = s_CurrentEntityID;
		circleState.TexIndex = GetTextureSlotIndex(texture);

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
			s_Data.CircleVertexBufferPtr->TexIndex = state.TexIndex;
			s_Data.CircleVertexBufferPtr++;
		}

		s_Data.CircleIndexCount += 6;

		s_Data.Stats.CircleCount++;

		if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
		{
			Flush();
			StartNewBatch();
		}
	}

	void Renderer2D::Draw(const UIState& state)
	{
		for(int i=0;i<4;i++)
		{
			s_Data.UIVertexBufferPtr->Position = state.Position[i];
			s_Data.UIVertexBufferPtr->Color = state.color;
			s_Data.UIVertexBufferPtr->TexCoord = state.TexCoords[i];
			s_Data.UIVertexBufferPtr->EntityID = state.EntityID;
			s_Data.UIVertexBufferPtr->TexIndex = state.TexIndex;
			s_Data.UIVertexBufferPtr++;
		}
		s_Data.UIVertexCount += 6;
		 if (s_Data.UIVertexCount >= s_Data.MaxVertices)
		{
			Flush();
			StartNewBatch();
		 }
	}


	float Renderer2D::GetTextureSlotIndex(const Texture2D& texture)
	{
		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (s_Data.TextureSlots[i] == texture)
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

		s_Data.UIVertexCount = 0;
		s_Data.UIVertexBufferPtr = s_Data.UIVertexBufferBase;

		s_Data.TextureSlotIndex = 1;

	}

}