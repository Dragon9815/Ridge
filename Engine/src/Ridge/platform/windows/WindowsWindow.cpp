#include "WindowsWindow.h"

#include "glfw\glfw3.h"

namespace Ridge
{
	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps & props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		
	}

	void WindowsWindow::Init(const WindowProps & props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
	}

	void WindowsWindow::Shutdown()
	{ }

	void WindowsWindow::OnUpdate()
	{ }

	void WindowsWindow::SetVSync(bool enabled)
	{ }

	bool WindowsWindow::IsVSync() const
	{
		return false;
	}
	
}