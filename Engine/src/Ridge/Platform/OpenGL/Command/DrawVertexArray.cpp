#include "RidgePCH.h"
#include "DrawVertexArray.h"
namespace Ridge {
	namespace Graphics {
		GLDrawVertexArrayCommand::GLDrawVertexArrayCommand( VertexArray * vao, IndexBuffer * ibo, uint indexCount )
			: DrawVertexArrayCommand( vao, ibo, indexCount )
		{
		}

		void GLDrawVertexArrayCommand::Execute( Renderer * renderer )
		{
			
		}
	}
}