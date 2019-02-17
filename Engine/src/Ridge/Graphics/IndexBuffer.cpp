#include "RidgePCH.h"
#include "IndexBuffer.h"

#include "Context.h"

#include "Ridge/Platform/OpenGL/GLIndexBuffer.h"
// #include "Ridge/Platform/DirectX/DXIndexBuffer.h"





namespace Ridge { namespace Graphics {
	
	/*IndexBuffer* IndexBuffer::Create() {
		switch (Context::GetRenderAPI()) {
		case RenderAPI::OPENGL:		return new GLIndexBuffer();
// 		case RenderAPI::DIRECT3D:	return new DXIndexBuffer();
 		}
		return nullptr;
	}*/
	IndexBuffer* IndexBuffer::Create(uint16* data, uint count) {
		RIDGE_API_OBJECT_FACTORY(IndexBuffer, data, count)
	}

	IndexBuffer* IndexBuffer::Create(uint* data, uint count) {
		RIDGE_API_OBJECT_FACTORY(IndexBuffer, data, count)
	}

} }