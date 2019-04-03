#include "RidgePCH.h"
#include "DXContext.h"

#include <DirectXColors.h>

#include "DXRenderer.h"
#include "DXImGuiRenderer.h"

namespace Ridge {

	DXContext::DXContext(const WindowProps& windowProps, void* deviceHandle)
		:	m_hWnd(reinterpret_cast<HWND>(deviceHandle)),
			m_windowProps(windowProps)
	{
		m_deviceContext = GetDC(m_hWnd);
	}

	DXContext::~DXContext()
	{

	}

	bool DXContext::Init()
	{
		UINT createDeviceFlags = 0;

#ifdef RIDGE_DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_DRIVER_TYPE driverTypes[] ={
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE
		};
		UINT numDriverTypes = ARRAYSIZE(driverTypes);

		D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
		};
		UINT numFeatureLevels = ARRAYSIZE(featureLevels);

		DXGI_SWAP_CHAIN_DESC swapDesc;
		ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
		swapDesc.BufferCount = 1; // double buffered
		swapDesc.BufferDesc.Width = m_windowProps.ClientWidth;
		swapDesc.BufferDesc.Height = m_windowProps.ClientHeight;
		swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.OutputWindow = m_hWnd;
		swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapDesc.Windowed = TRUE;
		swapDesc.SampleDesc.Count = 1;
		swapDesc.SampleDesc.Quality = 0;
		swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // alt-enter fullscreen

		HRESULT result;
		for (UINT i = 0; i < numDriverTypes; i++)
		{
			result = D3D11CreateDeviceAndSwapChain(NULL, driverTypes[i], NULL, createDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &swapDesc, &m_swapChain, &m_device, &m_featureLevel, &m_dxContext);
			if (SUCCEEDED(result)) {
				m_driverType = driverTypes[i];
				break;
			}
		}

		if (FAILED(result)) {
			RIDGE_CORE_ERROR("Failed to create device and swap chain!");
			return false;
		}

		// Create render target view
		ID3D11Texture2D* backBufferTexture;
		m_swapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBufferTexture));
		m_device->CreateRenderTargetView(backBufferTexture, nullptr, &m_renderTargetView);

		// Bind render target view
		m_dxContext->OMSetRenderTargets(1, &m_renderTargetView, nullptr);

		// Viewport creation
		m_viewport.Width = static_cast<float>(m_windowProps.ClientWidth);
		m_viewport.Height = static_cast<float>(m_windowProps.ClientHeight);
		m_viewport.TopLeftX = 0;
		m_viewport.TopLeftY = 0;
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;

		m_dxContext->RSSetViewports(1, &m_viewport);

		return true;
	}

	void DXContext::SwapBuffersImpl()
	{
		m_swapChain->Present(0, 0);
	}

	void DXContext::Resize(float width, float height)
	{

	}

	Ridge::Renderer* DXContext::CreateRendererImpl()
{
		return new DXRenderer(this);
	}

	ImGuiRenderer* DXContext::CreateImGuiRendererImpl()
	{
		return new DXImGuiRenderer(this);
	}

	Ridge::ShaderProgram* DXContext::CreateShaderProgramImpl()
{
		throw std::logic_error("The method or operation is not implemented.");
	}

}