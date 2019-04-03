#pragma once

#include "Ridge/Common.h"
#include "Ridge/Types.h"

#include "RenderCommand.h"
#include "Ridge/Graphics/VertexArray.h"

namespace Ridge {
	namespace Graphics {

		class RIDGE_API RenderCommandFactory
		{
		public:
			RenderCommand * CreateDrawVertexArrayCommand( VertexArray * vao, IndexBuffer * ibo, uint indexCount );

		};

	}
}