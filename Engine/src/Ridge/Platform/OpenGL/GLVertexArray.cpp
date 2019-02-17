#include "RidgePCH.h"
#include "GLVertexArray.h"

#include "GLUtil.h"


namespace Ridge { namespace Graphics {
	GLVertexArray::GLVertexArray()
	{
		GLCall(glCreateVertexArrays(1, &m_id));
	}

	GLVertexArray::~GLVertexArray()
	{
		GLCall(glDeleteVertexArrays(1, &m_id));
	}

	Ridge::Graphics::VertexBuffer* GLVertexArray::GetBuffer(int index /*= 0*/)
	{
		return m_buffers[index];
	}

	void GLVertexArray::PushBuffer(VertexBuffer* buffer, const BufferLayout& bufferLayout)
	{
		m_buffers.push_back(buffer);

		Bind();
		buffer->SetLayout(bufferLayout);
	}

	void GLVertexArray::Bind() const
	{
		GLCall(glBindVertexArray(m_id));
	}

	void GLVertexArray::Unbind() const
	{
		GLCall(glBindVertexArray(0));
	}

	void GLVertexArray::Draw(uint32 count) const
	{
		GLCall(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, NULL));
	}

} }