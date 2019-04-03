#pragma once

#include "Ridge/Graphics/VertexBuffer.h"

#include <glad/glad.h>

namespace Ridge {
	namespace Graphics {
		class GLVertexBuffer : public VertexBuffer
		{
		public:
			GLVertexBuffer( BufferUsage usage );
			~GLVertexBuffer( );

			virtual void Resize( uint size ) override;
			virtual void SetData( const Vertex * vertices, uint vertexCount, const BufferLayout & layout ) override;

			virtual void Bind( ) override;
			virtual void Unbind( ) override;

		private:
			GLuint m_id;
			uint32 m_size;
			BufferUsage m_usage;
		};
	}
}