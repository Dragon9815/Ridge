#pragma once

#include "Ridge/Graphics/VertexArray.h"

#include <vector>

#include <glad/glad.h>

namespace Ridge { namespace Graphics {

	class GLVertexArray : public VertexArray {

	public:
		GLVertexArray();
		virtual ~GLVertexArray();

		virtual VertexBuffer* GetBuffer(int index = 0) override;
		virtual void PushBuffer(VertexBuffer* buffer, const BufferLayout& bufferLayout) override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void Draw(uint32 count) const override;

	private:
		GLuint m_id;
		std::vector<VertexBuffer*> m_buffers;
	};
} }