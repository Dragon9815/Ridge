#include "RidgePCH.h"
#include "DXImGuiRenderer.h"

#include "imgui.h"

#include <stdio.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#ifdef _MSC_VER
#pragma comment(lib, "d3dcompiler") // Automatically link with d3dcompiler.lib as we are using D3DCompile() below.
#endif

namespace Ridge {

	struct VERTEX_CONSTANT_BUFFER
	{
		float   mvp[4][4];
	};

	DXImGuiRenderer::DXImGuiRenderer(DXContext* context)
		: m_context(context)
	{

	}

	bool DXImGuiRenderer::Init()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.BackendRendererName = "imgui_impl_dx11";

		// Get factory from device
		IDXGIDevice* pDXGIDevice = NULL;
		IDXGIAdapter* pDXGIAdapter = NULL;
		IDXGIFactory* pFactory = NULL;

		if (m_context->m_device->QueryInterface(IID_PPV_ARGS(&pDXGIDevice)) == S_OK)
			if (pDXGIDevice->GetParent(IID_PPV_ARGS(&pDXGIAdapter)) == S_OK)
				if (pDXGIAdapter->GetParent(IID_PPV_ARGS(&pFactory)) == S_OK)
				{
					m_pFactory = pFactory;
				}
		if (pDXGIDevice) pDXGIDevice->Release();
		if (pDXGIAdapter) pDXGIAdapter->Release();

		return true;
	}

	void DXImGuiRenderer::Shutdown()
	{
		InvalidateDeviceObjects();
		if (m_pFactory) { m_pFactory->Release(); m_pFactory = NULL; }
	}

	void DXImGuiRenderer::NewFrame()
	{
		if (!m_pFontSampler)
			CreateDeviceObjects();
	}

	void DXImGuiRenderer::RenderDrawData(ImDrawData* drawData)
	{
		ID3D11DeviceContext* ctx = m_context->m_dxContext;

		// Create and grow vertex/index buffers if needed
		if (!m_pVB || m_VertexBufferSize < drawData->TotalVtxCount)
		{
			if (m_pVB) { m_pVB->Release(); m_pVB = NULL; }
			m_VertexBufferSize = drawData->TotalVtxCount + 5000;
			D3D11_BUFFER_DESC desc;
			memset(&desc, 0, sizeof(D3D11_BUFFER_DESC));
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.ByteWidth = m_VertexBufferSize * sizeof(ImDrawVert);
			desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.MiscFlags = 0;
			if (m_context->m_device->CreateBuffer(&desc, NULL, &m_pVB) < 0)
				return;
		}
		if (!m_pIB || m_IndexBufferSize < drawData->TotalIdxCount)
		{
			if (m_pIB) { m_pIB->Release(); m_pIB = NULL; }
			m_IndexBufferSize = drawData->TotalIdxCount + 10000;
			D3D11_BUFFER_DESC desc;
			memset(&desc, 0, sizeof(D3D11_BUFFER_DESC));
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.ByteWidth = m_IndexBufferSize * sizeof(ImDrawIdx);
			desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			if (m_context->m_device->CreateBuffer(&desc, NULL, &m_pIB) < 0)
				return;
		}

		// Copy and convert all vertices into a single contiguous buffer
		D3D11_MAPPED_SUBRESOURCE vtx_resource, idx_resource;
		if (ctx->Map(m_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &vtx_resource) != S_OK)
			return;
		if (ctx->Map(m_pIB, 0, D3D11_MAP_WRITE_DISCARD, 0, &idx_resource) != S_OK)
			return;
		ImDrawVert* vtx_dst = (ImDrawVert*)vtx_resource.pData;
		ImDrawIdx* idx_dst = (ImDrawIdx*)idx_resource.pData;
		for (int n = 0; n < drawData->CmdListsCount; n++)
		{
			const ImDrawList* cmd_list = drawData->CmdLists[n];
			memcpy(vtx_dst, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
			memcpy(idx_dst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
			vtx_dst += cmd_list->VtxBuffer.Size;
			idx_dst += cmd_list->IdxBuffer.Size;
		}
		ctx->Unmap(m_pVB, 0);
		ctx->Unmap(m_pIB, 0);

		// Setup orthographic projection matrix into our constant buffer
		// Our visible imgui space lies from drawData->DisplayPos (top left) to drawData->DisplayPos+data_data->DisplaySize (bottom right). 
		{
			D3D11_MAPPED_SUBRESOURCE mapped_resource;
			if (ctx->Map(m_pVertexConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource) != S_OK)
				return;
			VERTEX_CONSTANT_BUFFER* constant_buffer = (VERTEX_CONSTANT_BUFFER*)mapped_resource.pData;
			float L = drawData->DisplayPos.x;
			float R = drawData->DisplayPos.x + drawData->DisplaySize.x;
			float T = drawData->DisplayPos.y;
			float B = drawData->DisplayPos.y + drawData->DisplaySize.y;
			float mvp[4][4] =
			{
				{ 2.0f / (R - L),   0.0f,           0.0f,       0.0f },
				{ 0.0f,         2.0f / (T - B),     0.0f,       0.0f },
				{ 0.0f,         0.0f,           0.5f,       0.0f },
				{ (R + L) / (L - R),  (T + B) / (B - T),    0.5f,       1.0f },
			};
			memcpy(&constant_buffer->mvp, mvp, sizeof(mvp));
			ctx->Unmap(m_pVertexConstantBuffer, 0);
		}

		// Backup DX state that will be modified to restore it afterwards (unfortunately this is very ugly looking and verbose. Close your eyes!)
		struct BACKUP_DX11_STATE
		{
			UINT                        ScissorRectsCount, ViewportsCount;
			D3D11_RECT                  ScissorRects[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
			D3D11_VIEWPORT              Viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
			ID3D11RasterizerState*      RS;
			ID3D11BlendState*           BlendState;
			FLOAT                       BlendFactor[4];
			UINT                        SampleMask;
			UINT                        StencilRef;
			ID3D11DepthStencilState*    DepthStencilState;
			ID3D11ShaderResourceView*   PSShaderResource;
			ID3D11SamplerState*         PSSampler;
			ID3D11PixelShader*          PS;
			ID3D11VertexShader*         VS;
			UINT                        PSInstancesCount, VSInstancesCount;
			ID3D11ClassInstance*        PSInstances[256], *VSInstances[256];   // 256 is max according to PSSetShader documentation
			D3D11_PRIMITIVE_TOPOLOGY    PrimitiveTopology;
			ID3D11Buffer*               IndexBuffer, *VertexBuffer, *VSConstantBuffer;
			UINT                        IndexBufferOffset, VertexBufferStride, VertexBufferOffset;
			DXGI_FORMAT                 IndexBufferFormat;
			ID3D11InputLayout*          InputLayout;
		};
		BACKUP_DX11_STATE old;
		old.ScissorRectsCount = old.ViewportsCount = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
		ctx->RSGetScissorRects(&old.ScissorRectsCount, old.ScissorRects);
		ctx->RSGetViewports(&old.ViewportsCount, old.Viewports);
		ctx->RSGetState(&old.RS);
		ctx->OMGetBlendState(&old.BlendState, old.BlendFactor, &old.SampleMask);
		ctx->OMGetDepthStencilState(&old.DepthStencilState, &old.StencilRef);
		ctx->PSGetShaderResources(0, 1, &old.PSShaderResource);
		ctx->PSGetSamplers(0, 1, &old.PSSampler);
		old.PSInstancesCount = old.VSInstancesCount = 256;
		ctx->PSGetShader(&old.PS, old.PSInstances, &old.PSInstancesCount);
		ctx->VSGetShader(&old.VS, old.VSInstances, &old.VSInstancesCount);
		ctx->VSGetConstantBuffers(0, 1, &old.VSConstantBuffer);
		ctx->IAGetPrimitiveTopology(&old.PrimitiveTopology);
		ctx->IAGetIndexBuffer(&old.IndexBuffer, &old.IndexBufferFormat, &old.IndexBufferOffset);
		ctx->IAGetVertexBuffers(0, 1, &old.VertexBuffer, &old.VertexBufferStride, &old.VertexBufferOffset);
		ctx->IAGetInputLayout(&old.InputLayout);

		// Setup viewport
		D3D11_VIEWPORT vp;
		memset(&vp, 0, sizeof(D3D11_VIEWPORT));
		vp.Width = drawData->DisplaySize.x;
		vp.Height = drawData->DisplaySize.y;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = vp.TopLeftY = 0;
		ctx->RSSetViewports(1, &vp);

		// Bind shader and vertex buffers
		unsigned int stride = sizeof(ImDrawVert);
		unsigned int offset = 0;
		ctx->IASetInputLayout(m_pInputLayout);
		ctx->IASetVertexBuffers(0, 1, &m_pVB, &stride, &offset);
		ctx->IASetIndexBuffer(m_pIB, sizeof(ImDrawIdx) == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT, 0);
		ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		ctx->VSSetShader(m_pVertexShader, NULL, 0);
		ctx->VSSetConstantBuffers(0, 1, &m_pVertexConstantBuffer);
		ctx->PSSetShader(m_pPixelShader, NULL, 0);
		ctx->PSSetSamplers(0, 1, &m_pFontSampler);

		// Setup render state
		const float blend_factor[4] = { 0.f, 0.f, 0.f, 0.f };
		ctx->OMSetBlendState(m_pBlendState, blend_factor, 0xffffffff);
		ctx->OMSetDepthStencilState(m_pDepthStencilState, 0);
		ctx->RSSetState(m_pRasterizerState);

		// Render command lists
		int vtx_offset = 0;
		int idx_offset = 0;
		ImVec2 pos = drawData->DisplayPos;
		for (int n = 0; n < drawData->CmdListsCount; n++)
		{
			const ImDrawList* cmd_list = drawData->CmdLists[n];
			for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
			{
				const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
				if (pcmd->UserCallback)
				{
					// User callback (registered via ImDrawList::AddCallback)
					pcmd->UserCallback(cmd_list, pcmd);
				}
				else
				{
					// Apply scissor/clipping rectangle
					const D3D11_RECT r = { (LONG)(pcmd->ClipRect.x - pos.x), (LONG)(pcmd->ClipRect.y - pos.y), (LONG)(pcmd->ClipRect.z - pos.x), (LONG)(pcmd->ClipRect.w - pos.y) };
					ctx->RSSetScissorRects(1, &r);

					// Bind texture, Draw
					ID3D11ShaderResourceView* texture_srv = (ID3D11ShaderResourceView*)pcmd->TextureId;
					ctx->PSSetShaderResources(0, 1, &texture_srv);
					ctx->DrawIndexed(pcmd->ElemCount, idx_offset, vtx_offset);
				}
				idx_offset += pcmd->ElemCount;
			}
			vtx_offset += cmd_list->VtxBuffer.Size;
		}

		// Restore modified DX state
		ctx->RSSetScissorRects(old.ScissorRectsCount, old.ScissorRects);
		ctx->RSSetViewports(old.ViewportsCount, old.Viewports);
		ctx->RSSetState(old.RS); if (old.RS) old.RS->Release();
		ctx->OMSetBlendState(old.BlendState, old.BlendFactor, old.SampleMask); if (old.BlendState) old.BlendState->Release();
		ctx->OMSetDepthStencilState(old.DepthStencilState, old.StencilRef); if (old.DepthStencilState) old.DepthStencilState->Release();
		ctx->PSSetShaderResources(0, 1, &old.PSShaderResource); if (old.PSShaderResource) old.PSShaderResource->Release();
		ctx->PSSetSamplers(0, 1, &old.PSSampler); if (old.PSSampler) old.PSSampler->Release();
		ctx->PSSetShader(old.PS, old.PSInstances, old.PSInstancesCount); if (old.PS) old.PS->Release();
		for (UINT i = 0; i < old.PSInstancesCount; i++) if (old.PSInstances[i]) old.PSInstances[i]->Release();
		ctx->VSSetShader(old.VS, old.VSInstances, old.VSInstancesCount); if (old.VS) old.VS->Release();
		ctx->VSSetConstantBuffers(0, 1, &old.VSConstantBuffer); if (old.VSConstantBuffer) old.VSConstantBuffer->Release();
		for (UINT i = 0; i < old.VSInstancesCount; i++) if (old.VSInstances[i]) old.VSInstances[i]->Release();
		ctx->IASetPrimitiveTopology(old.PrimitiveTopology);
		ctx->IASetIndexBuffer(old.IndexBuffer, old.IndexBufferFormat, old.IndexBufferOffset); if (old.IndexBuffer) old.IndexBuffer->Release();
		ctx->IASetVertexBuffers(0, 1, &old.VertexBuffer, &old.VertexBufferStride, &old.VertexBufferOffset); if (old.VertexBuffer) old.VertexBuffer->Release();
		ctx->IASetInputLayout(old.InputLayout); if (old.InputLayout) old.InputLayout->Release();
	}

	void DXImGuiRenderer::InvalidateDeviceObjects()
	{
		if (!m_context->m_device)
			return;

		if (m_pFontSampler) { m_pFontSampler->Release(); m_pFontSampler = NULL; }
		if (m_pFontTextureView) { m_pFontTextureView->Release(); m_pFontTextureView = NULL; ImGui::GetIO().Fonts->TexID = NULL; } // We copied m_pFontTextureView to io.Fonts->TexID so let's clear that as well.
		if (m_pIB) { m_pIB->Release(); m_pIB = NULL; }
		if (m_pVB) { m_pVB->Release(); m_pVB = NULL; }

		if (m_pBlendState) { m_pBlendState->Release(); m_pBlendState = NULL; }
		if (m_pDepthStencilState) { m_pDepthStencilState->Release(); m_pDepthStencilState = NULL; }
		if (m_pRasterizerState) { m_pRasterizerState->Release(); m_pRasterizerState = NULL; }
		if (m_pPixelShader) { m_pPixelShader->Release(); m_pPixelShader = NULL; }
		if (m_pPixelShaderBlob) { m_pPixelShaderBlob->Release(); m_pPixelShaderBlob = NULL; }
		if (m_pVertexConstantBuffer) { m_pVertexConstantBuffer->Release(); m_pVertexConstantBuffer = NULL; }
		if (m_pInputLayout) { m_pInputLayout->Release(); m_pInputLayout = NULL; }
		if (m_pVertexShader) { m_pVertexShader->Release(); m_pVertexShader = NULL; }
		if (m_pVertexShaderBlob) { m_pVertexShaderBlob->Release(); m_pVertexShaderBlob = NULL; }
	}

	bool DXImGuiRenderer::CreateDeviceObjects()
	{
		if (!m_context->m_device)
			return false;
		if (m_pFontSampler)
			InvalidateDeviceObjects();

		// By using D3DCompile() from <d3dcompiler.h> / d3dcompiler.lib, we introduce a dependency to a given version of d3dcompiler_XX.dll (see D3DCOMPILER_DLL_A)
		// If you would like to use this DX11 sample code but remove this dependency you can: 
		//  1) compile once, save the compiled shader blobs into a file or source code and pass them to CreateVertexShader()/CreatePixelShader() [preferred solution]
		//  2) use code to detect any version of the DLL and grab a pointer to D3DCompile from the DLL. 
		// See https://github.com/ocornut/imgui/pull/638 for sources and details.

		// Create the vertex shader
		{
			static const char* vertexShader =
				"cbuffer vertexBuffer : register(b0) \
				{\
				float4x4 ProjectionMatrix; \
				};\
				struct VS_INPUT\
				{\
				float2 pos : POSITION;\
				float4 col : COLOR0;\
				float2 uv  : TEXCOORD0;\
				};\
				\
				struct PS_INPUT\
				{\
				float4 pos : SV_POSITION;\
				float4 col : COLOR0;\
				float2 uv  : TEXCOORD0;\
				};\
				\
				PS_INPUT main(VS_INPUT input)\
				{\
				PS_INPUT output;\
				output.pos = mul( ProjectionMatrix, float4(input.pos.xy, 0.f, 1.f));\
				output.col = input.col;\
				output.uv  = input.uv;\
				return output;\
				}";

			D3DCompile(vertexShader, strlen(vertexShader), NULL, NULL, NULL, "main", "vs_4_0", 0, 0, &m_pVertexShaderBlob, NULL);
			if (m_pVertexShaderBlob == NULL) // NB: Pass ID3D10Blob* pErrorBlob to D3DCompile() to get error showing in (const char*)pErrorBlob->GetBufferPointer(). Make sure to Release() the blob!
				return false;
			if (m_context->m_device->CreateVertexShader((DWORD*)m_pVertexShaderBlob->GetBufferPointer(), m_pVertexShaderBlob->GetBufferSize(), NULL, &m_pVertexShader) != S_OK)
				return false;

			// Create the input layout
			D3D11_INPUT_ELEMENT_DESC local_layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,   0, (size_t)(&((ImDrawVert*)0)->pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,   0, (size_t)(&((ImDrawVert*)0)->uv),  D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, (size_t)(&((ImDrawVert*)0)->col), D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			if (m_context->m_device->CreateInputLayout(local_layout, 3, m_pVertexShaderBlob->GetBufferPointer(), m_pVertexShaderBlob->GetBufferSize(), &m_pInputLayout) != S_OK)
				return false;

			// Create the constant buffer
			{
				D3D11_BUFFER_DESC desc;
				desc.ByteWidth = sizeof(VERTEX_CONSTANT_BUFFER);
				desc.Usage = D3D11_USAGE_DYNAMIC;
				desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
				desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
				desc.MiscFlags = 0;
				m_context->m_device->CreateBuffer(&desc, NULL, &m_pVertexConstantBuffer);
			}
		}

		// Create the pixel shader
		{
			static const char* pixelShader =
				"struct PS_INPUT\
            {\
            float4 pos : SV_POSITION;\
            float4 col : COLOR0;\
            float2 uv  : TEXCOORD0;\
            };\
            sampler sampler0;\
            Texture2D texture0;\
            \
            float4 main(PS_INPUT input) : SV_Target\
            {\
            float4 out_col = input.col * texture0.Sample(sampler0, input.uv); \
            return out_col; \
            }";

			D3DCompile(pixelShader, strlen(pixelShader), NULL, NULL, NULL, "main", "ps_4_0", 0, 0, &m_pPixelShaderBlob, NULL);
			if (m_pPixelShaderBlob == NULL)  // NB: Pass ID3D10Blob* pErrorBlob to D3DCompile() to get error showing in (const char*)pErrorBlob->GetBufferPointer(). Make sure to Release() the blob!
				return false;
			if (m_context->m_device->CreatePixelShader((DWORD*)m_pPixelShaderBlob->GetBufferPointer(), m_pPixelShaderBlob->GetBufferSize(), NULL, &m_pPixelShader) != S_OK)
				return false;
		}

		// Create the blending setup
		{
			D3D11_BLEND_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.AlphaToCoverageEnable = false;
			desc.RenderTarget[0].BlendEnable = true;
			desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
			desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			m_context->m_device->CreateBlendState(&desc, &m_pBlendState);
		}

		// Create the rasterizer state
		{
			D3D11_RASTERIZER_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.FillMode = D3D11_FILL_SOLID;
			desc.CullMode = D3D11_CULL_NONE;
			desc.ScissorEnable = true;
			desc.DepthClipEnable = true;
			m_context->m_device->CreateRasterizerState(&desc, &m_pRasterizerState);
		}

		// Create depth-stencil State
		{
			D3D11_DEPTH_STENCIL_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.DepthEnable = false;
			desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
			desc.StencilEnable = false;
			desc.FrontFace.StencilFailOp = desc.FrontFace.StencilDepthFailOp = desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			desc.BackFace = desc.FrontFace;
			m_context->m_device->CreateDepthStencilState(&desc, &m_pDepthStencilState);
		}

		CreateFontsTexture();

		return true;
	}

	void DXImGuiRenderer::CreateFontsTexture()
	{
		// Build texture atlas
		ImGuiIO& io = ImGui::GetIO();
		unsigned char* pixels;
		int width, height;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

		// Upload texture to graphics system
		{
			D3D11_TEXTURE2D_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.Width = width;
			desc.Height = height;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.SampleDesc.Count = 1;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = 0;

			ID3D11Texture2D *pTexture = NULL;
			D3D11_SUBRESOURCE_DATA subResource;
			subResource.pSysMem = pixels;
			subResource.SysMemPitch = desc.Width * 4;
			subResource.SysMemSlicePitch = 0;
			m_context->m_device->CreateTexture2D(&desc, &subResource, &pTexture);

			// Create texture view
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			ZeroMemory(&srvDesc, sizeof(srvDesc));
			srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = desc.MipLevels;
			srvDesc.Texture2D.MostDetailedMip = 0;
			m_context->m_device->CreateShaderResourceView(pTexture, &srvDesc, &m_pFontTextureView);
			pTexture->Release();
		}

		// Store our identifier
		io.Fonts->TexID = (ImTextureID)m_pFontTextureView;

		// Create texture sampler
		{
			D3D11_SAMPLER_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			desc.MipLODBias = 0.f;
			desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
			desc.MinLOD = 0.f;
			desc.MaxLOD = 0.f;
			m_context->m_device->CreateSamplerState(&desc, &m_pFontSampler);
		}
	}

}