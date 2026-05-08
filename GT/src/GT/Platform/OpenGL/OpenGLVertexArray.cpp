#include "gtpch.h"
#include "OpenGLVertexArray.h"
#include <glad/glad.h>
namespace GT
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case GT::ShaderDataType::Float:    return GL_FLOAT;
			case GT::ShaderDataType::Float2:   return GL_FLOAT;
			case GT::ShaderDataType::Float3:   return GL_FLOAT;
			case GT::ShaderDataType::Float4:   return GL_FLOAT;
			case GT::ShaderDataType::Mat3:     return GL_FLOAT;
			case GT::ShaderDataType::Mat4:     return GL_FLOAT;
			case GT::ShaderDataType::Int:      return GL_INT;
			case GT::ShaderDataType::Int2:     return GL_INT;
			case GT::ShaderDataType::Int3:     return GL_INT;
			case GT::ShaderDataType::Int4:     return GL_INT;
			case GT::ShaderDataType::Bool:     return GL_BOOL;
		}
		GT_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		GT_PROFILE_FUNCTION();
		glCreateVertexArrays(1, &m_RendererID);
		glBindVertexArray(m_RendererID);
	}
	OpenGLVertexArray::~OpenGLVertexArray()
	{
		GT_PROFILE_FUNCTION();
		GLCall(glDeleteVertexArrays(1, &m_RendererID));
	}
	void OpenGLVertexArray::Bind() const
	{
		GT_PROFILE_FUNCTION();
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		GT_PROFILE_FUNCTION();
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{

		GT_PROFILE_FUNCTION();
		GT_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		for (const auto& element : vertexBuffer->GetLayout())
		{
			switch (element.Type)
			{
			case ShaderDataType::Float:     
			case ShaderDataType::Float2:    
			case ShaderDataType::Float3:    
			case ShaderDataType::Float4:
			{
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index, element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					vertexBuffer->GetLayout().GetStride(),
					(const void*)element.Offset);
				break;
			}     
			case ShaderDataType::Int:       
			case ShaderDataType::Int2:      
			case ShaderDataType::Int3:      
			case ShaderDataType::Int4:      
			case ShaderDataType::Bool:
			{
				glEnableVertexAttribArray(index);
				glVertexAttribIPointer(index, element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					vertexBuffer->GetLayout().GetStride(),
					(const void*)element.Offset);
				break;
			}
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
			{
				uint8_t count = element.GetComponentCount();
				for (uint8_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(index);
					glVertexAttribPointer(index, count,
						ShaderDataTypeToOpenGLBaseType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						vertexBuffer->GetLayout().GetStride(),
						(const void*)(element.Offset + sizeof(float) * count * i));
					glVertexAttribDivisor(index, 1);
				}
				break;
			}

			default:
				GT_CORE_ASSERT(false, "Unknown ShaderDataType!");
				break;
			}
			
			index++;
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		GT_PROFILE_FUNCTION();
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}


