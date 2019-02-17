#include "RidgePCH.h"
#include "VertexBuffer.h"

#include "Context.h"

#include "Ridge/Platform/OpenGL/GLVertexBuffer.h"
#include "Ridge/Platform/DirectX/DXVertexBuffer.h"

namespace Ridge { namespace Graphics {

	VertexBuffer* VertexBuffer::Create(BufferUsage usage /* = BufferUsage::Static */) 
	{
		RIDGE_API_OBJECT_FACTORY(VertexBuffer, usage);
	}

} }