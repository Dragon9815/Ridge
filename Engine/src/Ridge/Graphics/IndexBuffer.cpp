#include "RidgePCH.h"
#include "IndexBuffer.h"

#include "Context.h"

#include "Ridge/Platform/OpenGL/GLIndexBuffer.h"

namespace Ridge { namespace Graphics {
	IndexBuffer* IndexBuffer::Create(uint16* data, uint count) {
		RIDGE_API_OBJECT_FACTORY(IndexBuffer, data, count)
	}

	IndexBuffer* IndexBuffer::Create(uint* data, uint count) {
		RIDGE_API_OBJECT_FACTORY(IndexBuffer, data, count)
	}
} }