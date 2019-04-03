#include "RidgePCH.h"
#include "RenderCommandFactory.h"

#include "Ridge/Graphics/Context.h"

#include "Ridge/Platform/OpenGL/Command/DrawVertexArray.h"

#ifdef RIDGE_ENABLE_OPENGL
#define RC_FACTORY_OPENGL( name, ... ) case RenderAPI::OPENGL: return new GL##name##Command( __VA_ARGS__ );
#else
#define RC_FACTORY_OPENGL( name, ... )
#endif // RIDGE_ENABLE_OPENGL

#ifdef RIDGE_ENABLE_DIRECT3D
#define RC_FACTORY_DIRECTX( name, ... ) case RenderAPI::OPENGL: return new DX##name( __VA_ARGS__ );
#else
#define RC_FACTORY_DIRECTX( name, ... )
#endif // RIDGE_ENABLE_OPENGL

#define RC_FACTORY_BODY( name, ... )\
switch(Context::GetRenderAPI()) {\
RC_FACTORY_OPENGL( name, __VA_ARGS__ )\
RC_FACTORY_DIRECTX( name, __VA_ARGS__ )\
}\
return nullptr;

#define RC_FACTORY_FUNCTION( name, ... ) RenderCommand * RenderCommandFactory::Create##name##Command( __VA_ARGS__ )

namespace Ridge {
	namespace Graphics {

		RC_FACTORY_FUNCTION( DrawVertexArray, VertexArray * vao, IndexBuffer * ibo, uint indexCount )
		{
			RC_FACTORY_BODY( DrawVertexArray, vao, ibo, index );
		}
		
	}
}
