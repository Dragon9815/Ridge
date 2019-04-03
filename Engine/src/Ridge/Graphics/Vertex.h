#pragma once

#include "Ridge/Common.h"
#include "Ridge/Types.h"
#include "BufferLayout.h"

#include <vector>

namespace Ridge {
	namespace Graphics {

		class RIDGE_API Vertex
		{
		public:
			void AddPosition( float x, float y, float z )
			{
				m_pos[ 0 ] = x;
				m_pos[ 1 ] = y;
				m_pos[ 2 ] = z;
			}

			void AddColor( float r, float g, float b );
			void AddUVs( float u, float v );

			void ToArray( float * dest, const BufferLayout & bufferLayout );
		private:

			float m_pos[ 3 ];
			float m_color[ 3 ];
			float m_uv[ 2 ];
		};

	}
}