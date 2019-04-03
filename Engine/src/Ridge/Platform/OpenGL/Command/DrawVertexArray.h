#pragma once

#include "Ridge/Common.h"
#include "Ridge/Types.h"

#include "Ridge/Graphics/Rendering/Command/DrawVertexArray.h"

namespace Ridge {
	namespace Graphics {

		class GLDrawVertexArrayCommand : public DrawVertexArrayCommand
		{
		public:
			GLDrawVertexArrayCommand( VertexArray * vao, IndexBuffer * ibo, uint indexCount );

			virtual void Execute( Renderer * renderer ) override;
		};

	}
}