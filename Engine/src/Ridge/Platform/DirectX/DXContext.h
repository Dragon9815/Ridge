#pragma once

#include "Ridge/Graphics/Context.h"

#include <d3d11.h>

namespace Ridge {
	class DXContext : public Context {
	public:
		DXContext(const WindowProps& windowProps, void* deviceContext);
		~DXContext();

		virtual bool Init() override;
		virtual void SwapBuffersImpl() override;
		virtual void Resize(float width, float height);

		virtual Renderer* CreateRendererImpl() override;
		virtual ImGuiRenderer* CreateImGuiRendererImpl() override;
		virtual ShaderProgram* CreateShaderProgramImpl() override;

	public:
		ID3D11Device*			m_device;
		ID3D11DeviceContext*	m_dxContext;
		IDXGISwapChain*			m_swapChain;
		ID3D11RenderTargetView* m_renderTargetView;
		D3D_DRIVER_TYPE			m_driverType;
		D3D_FEATURE_LEVEL		m_featureLevel;
		D3D11_VIEWPORT			m_viewport;

	private:
		HWND m_hWnd;
		HDC m_deviceContext;
		WindowProps m_windowProps;
	};
}