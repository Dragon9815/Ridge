#pragma once

#include "Ridge/Graphics/Renderer.h"
#include "Ridge/Events/ApplicationEvent.h"

#include "GLContext.h"

namespace Ridge { namespace Graphics {
	class GLRenderer : public Renderer {
	public:
		GLRenderer(GLContext* context);

		void OnEvent(Event& e) override;
		void PrepareFrame() override;

		void SetClearColor(float r, float g, float b, float a) override;

	private:
		bool OnWindowResizeEvent(WindowResizeEvent& e);

	private:
		GLContext* m_context;
	};
} }