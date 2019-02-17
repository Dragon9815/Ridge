#include "RidgePCH.h"
#include "GLVertexBuffer.h"

#include "GLUtil.h"

namespace Ridge { namespace Graphics {
	
	static GLenum ConvertUsage(BufferUsage usage) 
	{
		switch (usage) {
		case BufferUsage::Static:	return GL_STATIC_DRAW;
		case BufferUsage::Dynamic:	return GL_DYNAMIC_DRAW;
		}

		return 0;
	}

	GLVertexBuffer::GLVertexBuffer(BufferUsage usage)
		: m_usage(usage)
	{
		GLCall(glCreateBuffers(1, &m_id));
	}

	GLVertexBuffer::~GLVertexBuffer()
	{
		Unbind();
		GLCall(glDeleteBuffers(1, &m_id));
	}

	void GLVertexBuffer::Resize(uint size)
	{
		Bind();
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, ConvertUsage(m_usage)));
	}

	void GLVertexBuffer::SetLayout(const BufferLayout& layout)
	{
		Bind();
		const std::vector<BufferElement>& elements = layout.GetLayout();
		for (uint32 i = 0; i < elements.size(); i++) {
			auto& element = elements[i];
			GLCall(glEnableVertexAttribArray(i));
			GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), reinterpret_cast<void*>(element.offset)));
		}
	}

	void GLVertexBuffer::SetData(uint size, const void* data)
	{
		Bind();
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, ConvertUsage(m_usage)));
	}

	void GLVertexBuffer::Bind()
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_id));
	}

	void GLVertexBuffer::Unbind()
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

} }