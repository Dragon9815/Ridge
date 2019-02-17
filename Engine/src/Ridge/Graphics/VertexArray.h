#pragma once

#include "Ridge/Common.h"
#include "Ridge/Types.h"
#include "VertexBuffer.h"

#include <vector>

namespace Ridge { namespace Graphics {
	class RIDGE_API VertexArray 
	{
	public:
		virtual ~VertexArray() {}

		virtual VertexBuffer* GetBuffer(int index = 0) = 0;
		virtual void PushBuffer(VertexBuffer* buffer, const BufferLayout& bufferLayout) = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void Draw(uint32 count) const = 0;

	public:
		static VertexArray* Create();
	};
} }