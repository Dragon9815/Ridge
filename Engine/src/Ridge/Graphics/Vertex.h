#pragma once

#include "Ridge/Common.h"
#include "Ridge/Types.h"

#include <vector>

namespace Ridge {
	
	struct RIDGE_API VertexFormatElement {
		RidgeType Type;
		int Count;
	};

	class RIDGE_API VertexFormat {
	public:
		void Push(RidgeType type, int count) 
		{
			m_elements.push_back({ type, count });
			m_length += count;
		}

		int Length() {
			return m_length;
		}

	private:
		std::vector<VertexFormatElement> m_elements;
		int m_length;
	};

	class RIDGE_API Vertex {
	public:
		void AddPosition(float x, float y, float z) { 
			m_pos[0] = x;
			m_pos[1] = y;
			m_pos[2] = z;
		}
		void AddColor(float r, float g, float b);
		void AddUVs(float u, float v);

		VertexFormat& GetFormat() { return m_format; }
	private:
		VertexFormat m_format;

		float m_pos[3];
		float m_color[3];
		float m_uv[2];
	};
}