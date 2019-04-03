#pragma once

#include "Ridge/Common.h"
#include "Ridge/Types.h"

#include "Ridge/Graphics/Rendering/RenderCommand.h"
#include "Ridge/Graphics/VertexArray.h"
#include "Ridge/Graphics/IndexBuffer.h"

namespace Ridge {
	namespace Graphics {

		class RIDGE_API DrawVertexArrayCommand : public RenderCommand
		{
		public:
			DrawVertexArrayCommand( VertexArray * vao, IndexBuffer * ibo, uint indexCount )
				: m_vao( vao ), m_ibo( ibo ), m_indexCount( indexCount )
			{
			}

			virtual void Execute( Renderer * renderer ) override = 0;

		protected:
			VertexArray * m_vao;
			IndexBuffer * m_ibo;
			uint m_indexCount;
		};

	}
}
