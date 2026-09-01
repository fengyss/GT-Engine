#pragma once
#include "glm/glm.hpp"
namespace GT
{
	struct Point
	{
		int x, y;
	};
	//struct Rect
	//{
	//	Point src, dest;
	//};

	//----------- renderer2D-------------------
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
		{ { 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};
		int EntityID = -1;
		int TexIndex = 0;

	};
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
		int TexIndex = 0;
	};
	struct LineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;

		// Editor-only
		int EntityID;
	};
	struct UIVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		int TexIndex = 0;
		// Editor-only
		int EntityID;
	};


	struct Vertex {
		// position
		glm::vec3 Position;
		// normal
		glm::vec3 Normal;
		// texCoords
		glm::vec2 TexCoords;
		// tangent
		glm::vec3 Tangent;
		// bitangent
		glm::vec3 Bitangent;
		//bone indexes which will influence this vertex
		int m_BoneIDs[MAX_BONE_INFLUENCE];
		//weights from each bone
		float m_Weights[MAX_BONE_INFLUENCE];
	};
}