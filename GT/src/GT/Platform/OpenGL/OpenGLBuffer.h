#pragma once
#include "GT/Renderer/Buffer.h"
namespace GT
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(const void* vertices, uint32_t size);
		~OpenGLVertexBuffer();
		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
		const BufferLayout GetLayout() const override { return m_Layout; }


		virtual void SetData(const void* data, uint32_t size) override;

	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(float* vertices, uint32_t count);
		OpenGLIndexBuffer(uint32_t* vertices, uint32_t count);
		~OpenGLIndexBuffer();

		void Bind() const;
		void Unbind() const;
		uint32_t GetCount() const { return m_count; }
	private:
		uint32_t m_RendererID;
		uint32_t m_count;
	};

	class OpenGLStorageBuffer : public StorageBuffer
	{
	public:
		OpenGLStorageBuffer(uint32_t size, const void* data);
		~OpenGLStorageBuffer() = default;

		void SetData(const void* data, uint32_t size) override;
		void Bind(uint32_t slot = 0) override;

	private:
		uint32_t m_RendererID;
		uint32_t m_Size;
	};
}


