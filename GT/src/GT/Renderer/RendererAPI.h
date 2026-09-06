#pragma once
#include "VertexArray.h"	

namespace glm {
	typedef vec<4, float, defaultp>		vec4;
}

namespace GT
{
	enum class BlendMode;

	enum class MemoryBarrierType
	{
		ShaderStorage = 0x00002000 // GL_SHADER_STORAGE_BARRIER_BIT
	};

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL = 1
		};
	public:

		virtual void Init() = 0;
		virtual void SetColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0 ;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
		virtual void DrawArrays(const Ref<VertexArray>& vertexArray, uint32_t count) = 0;
		virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) = 0;

		virtual void SetLineWidth(float width) = 0;

		virtual void SetBlendMode(BlendMode mode) = 0;
		
		virtual void DispatchCompute(uint32_t groupsX, uint32_t groupsY, uint32_t groupsZ) = 0;
		virtual void SetMemoryBarrier(MemoryBarrierType barrier) = 0;

		virtual void GetMemoryUsage(uint64_t& totalMemory, uint64_t& usedMemory) = 0;
		
		inline static API GetAPI() { return s_API; }

	private:
		static API s_API;
	};
}


