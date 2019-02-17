#include "RidgePCH.h"
#include "BufferLayout.h"

#include <glad/glad.h>
#include <d3d10.h>

namespace Ridge { namespace Graphics {


	BufferLayout::BufferLayout()
		: m_stride(0)
	{

	}

	BufferLayout::~BufferLayout()
	{

	}

	template<>
	void BufferLayout::Push<float>(const std::string& name, uint count, bool normalized) 
	{
		switch (Context::GetRenderAPI()) {
		case RenderAPI::OPENGL:
			Push(name, GL_FLOAT, sizeof(float), count, normalized);
			break;
		case RenderAPI::DIRECT3D:
			Push(name, DXGI_FORMAT_R32_FLOAT, sizeof(float), count, normalized);
			break;
		}
	}

	template<>
	void BufferLayout::Push<uint32>(const std::string& name, uint count, bool normalized) 
	{
		switch (Context::GetRenderAPI()) {
		case RenderAPI::OPENGL:
			Push(name, GL_UNSIGNED_INT, sizeof(uint32), count, normalized);
			break;
		case RenderAPI::DIRECT3D:
			Push(name, DXGI_FORMAT_R32_UINT, sizeof(uint32), count, normalized);
			break;
		}
	}

	template<>
	void BufferLayout::Push<uint8>(const std::string& name, uint count, bool normalized) 
	{
		switch (Context::GetRenderAPI()) {
		case RenderAPI::OPENGL:
			Push(name, GL_UNSIGNED_BYTE, sizeof(uint8), count, normalized);
			break;
		case RenderAPI::DIRECT3D:
			Push(name, DXGI_FORMAT_R8_UINT, sizeof(uint8), count, normalized);
			break;
		}
	}

	template<>
	void BufferLayout::Push<Vector2f>(const std::string& name, uint count, bool normalized) 
	{
		switch (Context::GetRenderAPI()) {
		case RenderAPI::OPENGL:
			Push(name, GL_FLOAT, sizeof(float), 2, normalized);
			break;
		case RenderAPI::DIRECT3D:
			Push(name, DXGI_FORMAT_R32G32_FLOAT, 2 * sizeof(float), count, normalized);
			break;
		}
	}

	template<>
	void BufferLayout::Push<Vector3f>(const std::string& name, uint count, bool normalized) 
	{
		switch (Context::GetRenderAPI()) {
		case RenderAPI::OPENGL:
			Push(name, GL_FLOAT, sizeof(float), 3, normalized);
			break;
		case RenderAPI::DIRECT3D:
			Push(name, DXGI_FORMAT_R32G32B32_FLOAT, 3 * sizeof(float), count, normalized);
			break;
		}
	}

	void BufferLayout::Push(const std::string& name, uint type, uint size, uint count, bool normalized)
	{
		m_layout.push_back({ name, type, size, count, m_stride, normalized });
		m_stride += size * count;
	}


} }