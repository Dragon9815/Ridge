#pragma once

#include "Ridge/Graphics/VertexBuffer.h"

namespace Ridge::Graphics {
	class DXVertexBuffer : VertexBuffer 
	{
	public:


		virtual void Resize(uint size) override
		{
			throw std::logic_error("The method or operation is not implemented.");
		}


		virtual void SetLayout(const BufferLayout& layout) override
		{
			throw std::logic_error("The method or operation is not implemented.");
		}


		virtual void SetData(uint size, const void* data) override
		{
			throw std::logic_error("The method or operation is not implemented.");
		}


		virtual void Bind() override
		{
			throw std::logic_error("The method or operation is not implemented.");
		}


		virtual void Unbind() override
		{
			throw std::logic_error("The method or operation is not implemented.");
		}

	};
}