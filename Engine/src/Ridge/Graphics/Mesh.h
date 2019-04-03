#pragma once

#include "Ridge/Common.h"

#include "Ridge/Graphics/Vertex.h"
#include "Ridge/Graphics/BufferLayout.h"
#include "Ridge/Graphics/VertexArray.h"
#include "Ridge/Graphics/IndexBuffer.h"

namespace Ridge {
	namespace Graphics {
		class RIDGE_API Mesh
		{
		public:
			Mesh( );
			virtual ~Mesh( );

			void AddVertices( Vertex vertices[ ], uint VertexCount, BufferLayout vertexLayout );
			void AddIndices( uint16 indices[ ], uint IndexCount );

			void Draw( );

		private:
			std::unique_ptr<VertexArray> vao;
			std::unique_ptr<IndexBuffer> ibo;
		};
	}
}