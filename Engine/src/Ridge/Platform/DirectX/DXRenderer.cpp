#include "RidgePCH.h"
#include "DXRenderer.h"

#include <DirectXColors.h>

namespace Ridge {

	DXRenderer::DXRenderer(DXContext* context)
		: m_context(context)
	{
		CopyMemory(m_clearColor, DirectX::Colors::CornflowerBlue, 4 * sizeof(float));
	}

	void DXRenderer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(DXRenderer::OnWindowResizeEvent));
	}

	void DXRenderer::PrepareFrame()
	{
		m_context->m_dxContext->ClearRenderTargetView(m_context->m_renderTargetView, m_clearColor);
	}

	void DXRenderer::SetClearColor(float r, float g, float b, float a)
	{
		m_clearColor[0] = r;
		m_clearColor[1] = g;
		m_clearColor[2] = b;
		m_clearColor[3] = a;
	}

	bool DXRenderer::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		return false;
	}

}