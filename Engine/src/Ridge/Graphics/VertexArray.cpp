#include "RidgePCH.h"
#include "VertexArray.h"

#include "Context.h"

#include "Ridge/Platform/OpenGL/GLVertexArray.h"
#include "Ridge/Platform/DirectX/DXVertexArray.h"

namespace Ridge { namespace Graphics {
	VertexArray* VertexArray::Create() {
		RIDGE_API_OBJECT_FACTORY(VertexArray);
	}
} }