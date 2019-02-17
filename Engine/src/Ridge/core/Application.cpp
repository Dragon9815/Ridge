#include "RidgePCH.h"
#include "Application.h"

#include "Ridge/Graphics/Context.h"

#include <gl/gl.h>

namespace Ridge {
	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		RIDGE_CORE_ASSERT(!s_instance, "Only one instance of Application can exists!");
		s_instance = this;

		m_window = std::unique_ptr<Window>(Window::Create());
		m_window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		if (!m_window->Init()) {
			RIDGE_CORE_ERROR("Failed to initalize Window!");
			return;
		}
		m_window->SetVSync(false);

		m_renderer = std::unique_ptr<Renderer>(Context::CreateRenderer());
		m_frameTimer = std::unique_ptr<Timer>(Timer::Create());
	}

	Application::~Application()
	{
	}

	bool Application::OnInit()
	{
		m_window->SetVisibility(true);
		//glClearColor(0.3f, 0.3f, 0.7f, 1.0f);
		//m_renderer->SetClearColor(0.3f, 0.3f, 0.7f, 1.0f);

		return true;
	}

	void Application::OnUpdate(double deltaTime)
	{
		m_window->OnUpdate();

		for (auto layer : m_layerStack) {
			layer->OnUpdate(deltaTime);
		}
	}

	void Application::OnRender(double deltaTime) 
	{
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		for (auto layer : m_layerStack) {
			layer->OnRender(deltaTime);
		}
	}
	
	void Application::OnShutdown() 
	{
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		RIDGE_CORE_TRACE(e.ToString());
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));

		m_renderer->OnEvent(e);

		for (auto it = m_layerStack.end(); it != m_layerStack.begin(); ) {
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_layerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_layerStack.PushOverlay(overlay);
	}

	bool Application::OnWindowClose(WindowCloseEvent & e)
	{
		m_running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent & e)
	{
		//GLCall(glViewport(0, 0, e.GetWidth(), e.GetHeight()));
		return false;
	}

	void Application::Run()
	{
		m_running = OnInit();
		m_frameTimer->Reset();

		while (m_running) {
			double deltaTime = m_frameTimer->ElapsedMillis();
			m_frameTimer->Reset();
			m_framerate = 1000 / deltaTime;

			OnUpdate(deltaTime);

			m_renderer->PrepareFrame();
			OnRender(deltaTime);
			Context::SwapBuffers();
		}

		OnShutdown();
	}
}
