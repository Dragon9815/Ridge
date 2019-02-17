#include "RidgePCH.h"
#include "DXVertexArray.h"

namespace Ridge::Graphics {

	VertexBuffer* DXVertexArray::GetBuffer(int index /*= 0*/)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void DXVertexArray::PushBuffer(VertexBuffer* buffer, const BufferLayout& bufferLayout)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void DXVertexArray::Bind() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void DXVertexArray::Unbind() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void DXVertexArray::Draw(uint32 count) const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

}