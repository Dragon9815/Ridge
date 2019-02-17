#pragma once

#include "Ridge/Debug/ImGuiRenderer.h"
#include "DXContext.h"

namespace Ridge {
	class DXImGuiRenderer : public ImGuiRenderer 
	{
	public:
		DXImGuiRenderer(DXContext* context);

		virtual bool Init() override;
		virtual void Shutdown() override;
		virtual void NewFrame() override;
		virtual void RenderDrawData(ImDrawData* drawData) override;

	private:
		void InvalidateDeviceObjects();
		bool CreateDeviceObjects();
		void CreateFontsTexture();

	private:
		DXContext* m_context;

		IDXGIFactory*            m_pFactory = NULL;
		ID3D11Buffer*            m_pVB = NULL;
		ID3D11Buffer*            m_pIB = NULL;
		ID3D10Blob*              m_pVertexShaderBlob = NULL;
		ID3D11VertexShader*      m_pVertexShader = NULL;
		ID3D11InputLayout*       m_pInputLayout = NULL;
		ID3D11Buffer*            m_pVertexConstantBuffer = NULL;
		ID3D10Blob*              m_pPixelShaderBlob = NULL;
		ID3D11PixelShader*       m_pPixelShader = NULL;
		ID3D11SamplerState*      m_pFontSampler = NULL;
		ID3D11ShaderResourceView*m_pFontTextureView = NULL;
		ID3D11RasterizerState*   m_pRasterizerState = NULL;
		ID3D11BlendState*        m_pBlendState = NULL;
		ID3D11DepthStencilState* m_pDepthStencilState = NULL;
		int                      m_VertexBufferSize = 5000, m_IndexBufferSize = 10000;
	};
}