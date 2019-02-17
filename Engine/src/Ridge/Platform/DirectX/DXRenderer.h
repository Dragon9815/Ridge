#pragma once

#include "Ridge/Graphics/Renderer.h"
#include "Ridge/Events/ApplicationEvent.h"

#include "DXContext.h"

namespace Ridge {
	class DXRenderer : public Renderer {
	public:
		DXRenderer(DXContext* context);

		virtual void OnEvent(Event& e) override;
		virtual void PrepareFrame() override;

		virtual void SetClearColor(float r, float g, float b, float a) override;

	private:
		bool OnWindowResizeEvent(WindowResizeEvent& e);

	private:
		DXContext* m_context;
		float m_clearColor[4];
	};
}