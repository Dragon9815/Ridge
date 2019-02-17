#pragma once

#include "RidgePCH.h"
#include "Ridge/Core/Layer.h"

#include "Ridge/Events/ApplicationEvent.h"
#include "Ridge/Events/KeyEvent.h"
#include "Ridge/Events/MouseEvent.h"

#include "ImGuiRenderer.h"

namespace Ridge {
	class RIDGE_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer(const std::string& name = "ImGui Layer");
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(double deltaTime) override;
		void OnRender(double deltaTime) override;
		void OnEvent(Event& event) override;
	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnMouseKeyPressedEvent(KeyPressedEvent& e);
		bool OnMouseKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnMouseKeyTypedEvent(CharTypedEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);

	private:
		float m_time = 0;

		std::unique_ptr<ImGuiRenderer> m_imguiRenderer;
	};
}