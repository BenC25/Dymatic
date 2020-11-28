#include "dypch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Dymatic {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case Dymatic::ShaderDataType::Float:		return GL_FLOAT;
			case Dymatic::ShaderDataType::Float2:		return GL_FLOAT;
			case Dymatic::ShaderDataType::Float3:		return GL_FLOAT;
			case Dymatic::ShaderDataType::Float4:		return GL_FLOAT;
			case Dymatic::ShaderDataType::Mat3:			return GL_FLOAT;
			case Dymatic::ShaderDataType::Mat4:			return GL_FLOAT;
			case Dymatic::ShaderDataType::Int:			return GL_INT;
			case Dymatic::ShaderDataType::Int2:			return GL_INT;
			case Dymatic::ShaderDataType::Int3:			return GL_INT;
			case Dymatic::ShaderDataType::Int4:			return GL_INT;
			case Dymatic::ShaderDataType::Bool:			return GL_BOOL;
		}
		DY_CORE_ASSERT(false, "Unknown Dymatic ShaderDataType!")
			return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		DY_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		DY_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		DY_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		DY_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		DY_PROFILE_FUNCTION();

		DY_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();


		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			index++;
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		DY_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

}