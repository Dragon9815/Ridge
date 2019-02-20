#include "RidgePCH.h"
#include "Context.h"

#include "Ridge/Platform/OpenGL/GLContext.h"
#include "Ridge/Platform/DirectX/DXContext.h"

namespace Ridge {
	Context* Context::s_context = nullptr;
	RenderAPI Context::s_renderAPI = RenderAPI::OPENGL;

	bool Context::Create(WindowProps windowProps, void* deviceHandle)
	{
		switch (GetRenderAPI())
		{
#ifdef RIDGE_ENABLE_OPENGL
		case RenderAPI::OPENGL:		s_context = new Graphics::GLContext(windowProps, deviceHandle); break;
#endif
#ifdef RIDGE_ENABLE_DIRECT3D
		case RenderAPI::DIRECT3D:	s_context = new DXContext(windowProps, deviceHandle); break;
#endif
		default:					return false;
		}

		return s_context->Init();
	}

}