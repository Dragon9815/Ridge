#pragma once

#include "Ridge/Common.h"
#include "Ridge/Types.h"

namespace Ridge { namespace Graphics {

	class RIDGE_API IndexBuffer {
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

	public:
		static IndexBuffer* Create(uint16* data, uint count);
		static IndexBuffer* Create(uint* data, uint count);
	};

} }