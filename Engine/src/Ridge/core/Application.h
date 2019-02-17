#pragma once

#include "Ridge/Common.h"

#include "Ridge/Core/Window.h"
#include "Ridge/Core/LayerStack.h"

#include "Ridge/Events/ApplicationEvent.h"
#include "Ridge/Events/KeyEvent.h"
#include "Ridge/Events/MouseEvent.h"

#include "Ridge/Graphics/Renderer.h"
#include "Ridge/Graphics/Shader.h"

#include "Ridge/Util/Timer.h"

namespace Ridge {
	class RIDGE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		virtual bool OnInit();
		virtual void OnUpdate(double deltaTime);
		virtual void OnRender(double deltaTime);
		virtual void OnShutdown();

		virtual void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline float GetFramerate() { return m_framerate; }

		inline Window& GetWindow() { return *m_window; }
		inline static Application& Get() { return *s_instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		bool m_running;
		float m_framerate;

		std::unique_ptr<Window> m_window;
		std::unique_ptr<Renderer> m_renderer;
		std::unique_ptr<Timer> m_frameTimer;
		
		LayerStack m_layerStack;

		static Application* s_instance;
	};
}