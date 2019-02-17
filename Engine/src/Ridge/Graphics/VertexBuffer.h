#pragma once

#include "Ridge/Common.h"
#include "Ridge/Types.h"

#include "BufferLayout.h"

namespace Ridge { namespace Graphics {
	enum class BufferUsage {
		Static, Dynamic
	};

	class RIDGE_API VertexBuffer {
	public:
		virtual ~VertexBuffer() {}

		virtual void Resize(uint size) = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual void SetData(uint size, const void* data) = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

	public:
		static VertexBuffer* Create(BufferUsage usage = BufferUsage::Static);
	};
} }