#pragma once

#include "Ridge/Graphics/Context.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN

#include <glad/glad.h>
#include <gl/wglext.h>

namespace Ridge { namespace Graphics {
	class GLContext : public Context {
	public:
		GLContext(const WindowProps& windowProps, void* deviceHandle);
		~GLContext();

		virtual bool Init() override;
		virtual void SwapBuffersImpl() override;

		virtual Renderer* CreateRendererImpl() override;
		virtual ImGuiRenderer* CreateImGuiRendererImpl() override;
		virtual ShaderProgram* CreateShaderProgramImpl() override;

	private:
		bool LoadGLExtensions();

		bool WGLExtensionSupported(const char* extensionName);

	private:
		HWND m_hWnd;
		HDC m_deviceContext;
		WindowProps m_windowProps;
		HGLRC m_renderContext;

		PFNWGLGETEXTENSIONSSTRINGARBPROC _wglGetExtentionsStringARB;
		PFNWGLSWAPINTERVALEXTPROC _wglSwapIntervalEXT;
		PFNWGLGETSWAPINTERVALEXTPROC _wglGetSwapIntervalEXT;
	};
} }