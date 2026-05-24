#pragma once
#include "RendererAPI.h"

namespace GT
{
	class RenderCommand
	{
	public:

		virtual ~RenderCommand() { delete s_RendererAPI; };
		static void Init();
		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetColor(color);
		}
		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}
		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0)
		{
			s_RendererAPI->DrawIndexed(vertexArray, indexCount);
		}

		static void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
		{
			s_RendererAPI->DrawLines(vertexArray, vertexCount);
		}

		static void SetLineWidth(float width)
		{
			s_RendererAPI->SetLineWidth(width);
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		inline static void SetBlendMode(BlendMode mode)
		{
			s_RendererAPI->SetBlendMode(mode);
		}
		inline static void DispatchCompute(uint32_t groupsX, uint32_t groupsY, uint32_t groupsZ)
		{
			s_RendererAPI->DispatchCompute(groupsX, groupsY, groupsZ);
		}
		inline static void SetMemoryBarrier(MemoryBarrierType barrier)
		{
			s_RendererAPI->SetMemoryBarrier(barrier);
		}

		inline static void DrawArrays(const Ref<VertexArray>& vertexArray, uint32_t count)
		{
			s_RendererAPI->DrawArrays(vertexArray, count);
		}
	private:
		static RendererAPI* s_RendererAPI;
	};
}


