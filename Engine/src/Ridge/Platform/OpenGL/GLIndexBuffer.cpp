#include "RidgePCH.h"
#include "GLIndexBuffer.h"

#include "GLUtil.h"

namespace Ridge { namespace Graphics {

	GLuint CreateAndFillBuffer(void* data, uint size) {
		GLuint id;
		GLCall(glGenBuffers(1, &id));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
		return id;
	}

	GLIndexBuffer::GLIndexBuffer(uint16* data, uint count)
	{
		m_id = CreateAndFillBuffer(data, count * sizeof(uint16));
	}

	GLIndexBuffer::GLIndexBuffer(uint* data, uint count)
	{
		m_id = CreateAndFillBuffer(data, count * sizeof(uint));
	}

	GLIndexBuffer::~GLIndexBuffer()
	{
		Unbind();
		GLCall(glDeleteBuffers(1, &m_id));
	}

	void GLIndexBuffer::Bind()
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
	}

	void GLIndexBuffer::Unbind()
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

} }