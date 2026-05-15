#pragma once
#include "GT/Renderer/RendererAPI.h"

namespace GT
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
		virtual void DrawArrays(const Ref<VertexArray>& vertexArray, uint32_t count) override;
		virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) override;

		virtual void SetLineWidth(float width) override;

		virtual void SetBlendMode(BlendMode mode) override;

		virtual void DispatchCompute(uint32_t groupsX, uint32_t groupsY, uint32_t groupsZ) override;
		virtual void SetMemoryBarrier(MemoryBarrierType barrier) override;
	private:
	};
}


