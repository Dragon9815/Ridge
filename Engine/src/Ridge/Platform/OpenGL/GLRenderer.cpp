#include "RidgePCH.h"
#include "GLRenderer.h"

#include "GLUtil.h"

#include <glad/glad.h>

namespace Ridge { namespace Graphics {

	GLRenderer::GLRenderer(GLContext* context)
		: m_context(context)
	{

	}

	void GLRenderer::OnEvent(Event & e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(GLRenderer::OnWindowResizeEvent));
	}

	void GLRenderer::PrepareFrame()
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void GLRenderer::SetClearColor(float r, float g, float b, float a)
	{
		GLCall(glClearColor(r, g, b, a));
	}

	bool GLRenderer::OnWindowResizeEvent(WindowResizeEvent & e)
	{
		GLCall(glViewport(0, 0, e.GetWidth(), e.GetHeight()));
		return false;
	}

	/*Renderer* Renderer::Create() {
		return new GLRenderer();
	}*/
} }