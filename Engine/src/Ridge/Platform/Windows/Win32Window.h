#pragma once

#include "RidgePCH.h"

#include "Ridge/core/Window.h"
#include "Ridge/Input/Key.h"

#include <glad/glad.h>
#include <gl/wglext.h>

namespace Ridge
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		~WindowsWindow();

		bool Init() override;

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_clientWidth; }
		inline unsigned int GetHeight() const override { return m_clientHeight; }

		void SetEventCallback(const EventCallbackFn& callback) override;
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		virtual void SetVisibility(bool shown) override;

	private:
		bool InitWindow();
		void Shutdown();

	private:
		const char* const CLASS_NAME = "RIDGEWNDCLASS";

		std::string m_title;
		unsigned int m_clientWidth, m_clientHeight;

		WindowProps m_windowProps;

		DWORD m_windowStyle;
		HINSTANCE m_hInstance;
		//HDC m_deviceContext;
		//HGLRC m_renderContext;
		HWND m_hWnd;

		EventCallbackFn m_eventCallback;

		bool m_canVSync;
	};

	WindowsWindow* g_pWindow;
}