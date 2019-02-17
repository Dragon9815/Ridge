#include "RidgePCH.h"
#include "GLContext.h"

#include <glad/glad.h>

#include "GLRenderer.h"
#include "GLImGuiRenderer.h"
#include "GLShader.h"

namespace Ridge { namespace Graphics {

	GLContext::GLContext(const WindowProps& windowProps, void* deviceHandle)
		:	m_hWnd(reinterpret_cast<HWND>(deviceHandle)),
			m_windowProps(windowProps)
	{
		m_deviceContext = GetDC(m_hWnd);
	}

	GLContext::~GLContext()
	{

	}

	bool GLContext::Init() 
	{
		PIXELFORMATDESCRIPTOR pfd;
		ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 24;
		pfd.cStencilBits = 8;

		int format = ChoosePixelFormat(m_deviceContext, &pfd);
		if (!SetPixelFormat(m_deviceContext, format, &pfd)) {
			RIDGE_CORE_ERROR("Failed to set pixel format!");
			return false;
		}

		m_renderContext = wglCreateContext(m_deviceContext);
		if (!wglMakeCurrent(m_deviceContext, m_renderContext)) {
			RIDGE_CORE_ERROR("Failed to create and activate OpenGL context!");
			return false;
		}

		if (!gladLoadGL()) {
			RIDGE_CORE_ERROR("Failed to initalize GLAD!");
			return false;
		}

		RIDGE_CORE_INFO("GL Vendor: {}", glGetString(GL_VENDOR));
		RIDGE_CORE_INFO("GL Version: {}", glGetString(GL_VERSION));

		return LoadGLExtensions();
	}

	void GLContext::SwapBuffersImpl()
	{
		::SwapBuffers(m_deviceContext);
	}

	Ridge::Renderer* GLContext::CreateRendererImpl()
{
		return new GLRenderer(this);
	}

	Ridge::ImGuiRenderer* GLContext::CreateImGuiRendererImpl()
{
		return new GLImGuiRenderer();
	}

	Ridge::ShaderProgram* GLContext::CreateShaderProgramImpl()
{
		return new GLShaderProgram();
	}

	bool GLContext::LoadGLExtensions()
	{
		_wglGetExtentionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");

		if (_wglGetExtentionsStringARB == nullptr) {
			RIDGE_CORE_WARN("wglGetExtensionsStringARB not supported!");
			return false;
		}

		if (WGLExtensionSupported("WGL_EXT_swap_control")) {
			_wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
			_wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");
			RIDGE_CORE_INFO("VSync supported");
		}

		return true;
	}

	bool GLContext::WGLExtensionSupported(const char* extensionName)
	{
		bool result = strstr(_wglGetExtentionsStringARB(m_deviceContext), extensionName) != nullptr;
		return result;
	}
} }