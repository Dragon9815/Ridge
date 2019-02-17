#pragma once

#include "Ridge/Graphics/IndexBuffer.h"

#include <glad/glad.h>

namespace Ridge { namespace Graphics {

	class GLIndexBuffer : public IndexBuffer 
	{
	public:
		GLIndexBuffer(uint16* data, uint count);
		GLIndexBuffer(uint* data, uint count);
		virtual ~GLIndexBuffer();

		virtual void Bind() override;
		virtual void Unbind() override;

	private:
		GLuint m_id;
	};

} }