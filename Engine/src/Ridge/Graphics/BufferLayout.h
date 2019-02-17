#pragma once

#include "Ridge/Common.h"
#include "Ridge/Types.h"

#include "Ridge/Graphics/Context.h"

#include "Ridge/Math/Vector.h"
#include "Ridge/Math/Matrix.h"

#include <string>

namespace Ridge { namespace Graphics {
	struct RIDGE_API BufferElement
	{
		std::string name;
		uint type;
		uint size;
		uint count;
		uint offset;
		bool normalized;
	};

	class RIDGE_API BufferLayout {
	public:
		BufferLayout();
		~BufferLayout();

		template<typename T>
		void Push(const std::string& name, uint count = 1, bool normalized = false) {
			RIDGE_ASSERT(false, "Unknown type");
		}

		template<>
		void Push<float>(const std::string& name, uint count, bool normalized);
		template<>
		void Push<uint32>(const std::string& name, uint count, bool normalized);
		template<>
		void Push<uint8>(const std::string& name, uint count, bool normalized);
		template<>
		void Push<Vector2f>(const std::string& name, uint count, bool normalized);
		template<>
		void Push<Vector3f>(const std::string& name, uint count, bool normalized);

		inline const std::vector<BufferElement>& GetLayout() const { return m_layout; }
		inline uint32 GetStride() const { return m_stride; }

	private:
		void Push(const std::string& name, uint type, uint size, uint count, bool normalized);

	private:
		std::vector<BufferElement> m_layout;
		uint32 m_stride;
	};

} }