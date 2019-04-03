#include "RidgePCH.h"
#include "Mesh.h"

namespace Ridge {
	namespace Graphics {

		Mesh::Mesh( )
			: vao( VertexArray::Create( ) )
		{
			
		}

		Mesh::~Mesh( )
		{
		}

		void Mesh::AddVertices( Vertex* vertices, uint VertexCount, BufferLayout vertexLayout )
		{
			VertexBuffer * buffer = VertexBuffer::Create( );
			buffer->SetData( vertices, VertexCount );
		}

		void Mesh::AddIndices( uint16 indices[ ], uint IndexCount )
		{
		}

		void Mesh::Draw( )
		{
			if( ibo.get() != nullptr )
			{

			}
		}

	}
}