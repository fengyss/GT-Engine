#include "gtpch.h"
#include "OpenGLBuffer.h"
#include <glad/glad.h>

namespace GT
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
	{
		GT_PROFILE_FUNCTION();

		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);

	}
	//// Vertex Buffer ////
	OpenGLVertexBuffer::OpenGLVertexBuffer(const void* vertices, uint32_t size)
	{
		GT_PROFILE_FUNCTION();
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}
	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		GT_PROFILE_FUNCTION();
		GLCall(glDeleteBuffers(1, &m_RendererID));
	}
	void OpenGLVertexBuffer::Bind() const
	{
		GT_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}
	void OpenGLVertexBuffer::Unbind() const
	{
		GT_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
	{
		GT_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	//// Index Buffer ////
	OpenGLIndexBuffer::OpenGLIndexBuffer(float* vertices, uint32_t count)
		:m_count(count)
	{
		GT_PROFILE_FUNCTION();
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(uint32_t), vertices, GL_STATIC_DRAW);
	}
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* vertices, uint32_t count)
		:m_count(count)
	{
		GT_PROFILE_FUNCTION();
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), vertices, GL_STATIC_DRAW);
	}
	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		GT_PROFILE_FUNCTION();
		glDeleteBuffers(1, &m_RendererID);
	}
	void OpenGLIndexBuffer::Bind() const
	{
		GT_PROFILE_FUNCTION();
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	}
	void OpenGLIndexBuffer::Unbind() const
	{
		GT_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}