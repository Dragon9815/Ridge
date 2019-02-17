#pragma once

#include "Ridge/Graphics/VertexArray.h"

namespace Ridge::Graphics {
	class DXVertexArray : public VertexArray 
	{
	public:
		virtual VertexBuffer* GetBuffer(int index = 0);
		virtual void PushBuffer(VertexBuffer* buffer, const BufferLayout& bufferLayout);

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual void Draw(uint32 count) const;
	};
}