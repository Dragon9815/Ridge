#include "RidgePCH.h"
#include "Win32Window.h"
#include "Win32Input.h"

#include "Ridge/Events/ApplicationEvent.h"
#include "Ridge/Events/KeyEvent.h"
#include "Ridge/Events/MouseEvent.h"
#include "Ridge/Graphics/Context.h"

#include <glad/glad.h>
#include <GL/wglext.h>

namespace Ridge
{
	LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (g_pWindow)
			return g_pWindow->MsgProc(hWnd, msg, wParam, lParam);
		else
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps & props)
		: m_windowProps(props)
	{
		g_pWindow = this;

		m_title = props.Title;
		m_clientWidth = props.ClientWidth;
		m_clientHeight = props.ClientHeight;

		m_hWnd = NULL;
		m_hInstance = GetModuleHandle(NULL);
		m_windowStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME;
	}

	WindowsWindow::~WindowsWindow()
	{
		
	}

	bool WindowsWindow::Init()
	{
		if (!InitWindow())
			return false;

		if (!Context::Create(m_windowProps, m_hWnd))
			return false;

		return true;
	}

	bool WindowsWindow::InitWindow()
	{
		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof(WNDCLASSEX));
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.hInstance = m_hInstance;
		wc.lpfnWndProc = MainWndProc;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
		wc.lpszClassName = CLASS_NAME;
		wc.lpszMenuName = NULL;

		if (!RegisterClassEx(&wc)) {
			RIDGE_CORE_ERROR("Failed to register window class!");
			return false;
		}

		// Adjust window rect for client size
		RECT r = { 0, 0, (LONG)m_clientWidth, (LONG)m_clientHeight };
		AdjustWindowRect(&r, m_windowStyle, FALSE);
		int width = r.right - r.left;
		int height = r.bottom - r.top;
		int x = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
		int y = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;

		m_hWnd = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			CLASS_NAME,
			m_title.c_str(),
			m_windowStyle,
			x, y, width, height,
			NULL, NULL, m_hInstance, NULL);

		if (m_hWnd == NULL) {
			RIDGE_CORE_ERROR("Failed to create window!");
			return false;
		}

		return true;
	}

	void WindowsWindow::Shutdown()
	{ 
		ShowWindow(m_hWnd, SW_HIDE);
		DestroyWindow(m_hWnd);
	}

	void WindowsWindow::OnUpdate()
	{ 
		MSG msg = { 0 };
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				WindowCloseEvent event;
				m_eventCallback(event);
				return;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void WindowsWindow::SetEventCallback(const EventCallbackFn & callback)
	{
		m_eventCallback = callback;
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		/*if (m_canVSync) {
			_wglSwapIntervalEXT(enabled ? 1 : 0);
		}*/
	}

	bool WindowsWindow::IsVSync() const
	{
		//return m_canVSync && _wglGetSwapIntervalEXT() != 0;
		return false;
	}

	LRESULT WindowsWindow::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		static int keyRepeats[0x200] = { 0 };

		switch (msg) {
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			KeyCode keyCode = Win32Input::GetInstance()->TranslateKey(wParam, lParam);
			int scancode = (lParam >> 16) & 0x1FF;
			bool released = ((lParam >> 31) & 1); // true = released, false = pressed

			if (keyCode == KeyCode::Invalid)
				return 0;

			Win32Input::GetInstance()->SetKeyState(keyCode, !released);

			if (released) {
				keyRepeats[scancode] = 0;
				KeyReleasedEvent event(Key(keyCode, scancode));
				m_eventCallback(event);
			}
			else {
				keyRepeats[scancode] += static_cast<int>(LOWORD(lParam)); 
				KeyPressedEvent event(Key(keyCode, scancode), keyRepeats[scancode]);
				m_eventCallback(event);
			}

			return 0;
		}

		case WM_CHAR:
		case WM_SYSCHAR:
		case WM_UNICHAR:
		{
			if (msg == WM_UNICHAR && wParam == UNICODE_NOCHAR) {
				// WM_UNICHAR is not sent by Windows, but is sent by some
				// third-party input method engine
				// Returning TRUE here announces support for this message
				return TRUE;
			}

			CharTypedEvent event(static_cast<int>(wParam));
			m_eventCallback(event);
			return 0;
		}

		case WM_MOUSEMOVE:
		{
			Win32Input::GetInstance()->SetMousePos(Vector2i(LOWORD(lParam), HIWORD(lParam)));
			//win32_mouseMove({ LOWORD(lParam), HIWORD(lParam) });
			MouseMovedEvent event(LOWORD(lParam), HIWORD(lParam));
			m_eventCallback(event);
			return 0;
		}

		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_XBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_XBUTTONUP:
		{
			MouseButton button;

			if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP)
				button = RD_MOUSE_LEFT;
			else if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONUP)
				button = RD_MOUSE_RIGHT;
			else if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONUP)
				button = RD_MOUSE_MIDDLE;
			else if (LOWORD(wParam) == XBUTTON1)
				button = RD_MOUSE_X1;
			else
				button = RD_MOUSE_X2;

			if (msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_XBUTTONDOWN) {
				Win32Input::GetInstance()->SetMouseButtonState(button, true);
				MouseButtonPressedEvent event(button);
				m_eventCallback(event);
			}
			else {
				Win32Input::GetInstance()->SetMouseButtonState(button, false);
				MouseButtonReleasedEvent event(button);
				m_eventCallback(event);
			}

			return 0;
		}

		case WM_MOUSEHWHEEL:
		case WM_MOUSEWHEEL:
		{
			float delta = GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA;

			float deltaX = 0.0f, deltaY = 0.0f;
			if (msg == WM_MOUSEWHEEL) {
				deltaY = delta;
			}
			else {
				deltaX = delta;
			}

			MouseScrolledEvent event(deltaX, deltaY);
			m_eventCallback(event);

			return 0;
		}

		case WM_SIZE:
		case WM_SIZING:
		{
			RECT r;
			GetClientRect(m_hWnd, &r);
			m_clientWidth = r.right - r.left;
			m_clientHeight = r.bottom - r.top;

			WindowResizeEvent event(m_clientWidth, m_clientHeight);
			m_eventCallback(event);
			return 0;
		}

		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}

		
	}

	void WindowsWindow::SetVisibility(bool shown)
	{
		ShowWindow(m_hWnd, shown ? SW_SHOW : SW_HIDE);
	}

	/*bool WindowsWindow::WGLExtensionSupported(const char* extensionName)
	{
		bool result = strstr(_wglGetExtentionsStringARB(m_deviceContext), extensionName) != nullptr;
		return result;
	}*/
}