#include "RidgePCH.h"
#include "ImGuiLayer.h"

#include <imgui.h>
#include <glad/glad.h>

#include "Ridge/Input/Input.h"
#include "Ridge/Core/Application.h"
#include "Ridge/Graphics/Context.h"

namespace Ridge {
	ImGuiLayer::ImGuiLayer(const std::string& name)
		: Layer(name)
	{
		m_imguiRenderer = std::unique_ptr<ImGuiRenderer>(Context::CreateImGuiRenderer());
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

#define SET_KEY(imgui_key, ridge_key) io.KeyMap[imgui_key] = static_cast<int>(ridge_key)

		SET_KEY(ImGuiKey_Tab, KeyCode::Tab);
		SET_KEY(ImGuiKey_LeftArrow, KeyCode::Left);
		SET_KEY(ImGuiKey_RightArrow, KeyCode::Right);
		SET_KEY(ImGuiKey_UpArrow, KeyCode::Up);
		SET_KEY(ImGuiKey_DownArrow, KeyCode::Down);
		SET_KEY(ImGuiKey_PageUp, KeyCode::PageUp);
		SET_KEY(ImGuiKey_PageDown, KeyCode::PageDown);
		SET_KEY(ImGuiKey_Home, KeyCode::Home);
		SET_KEY(ImGuiKey_End, KeyCode::End);
		SET_KEY(ImGuiKey_Insert, KeyCode::Insert);
		SET_KEY(ImGuiKey_Delete, KeyCode::Delete);
		SET_KEY(ImGuiKey_Backspace, KeyCode::Backspace);
		SET_KEY(ImGuiKey_Space, KeyCode::Space);
		SET_KEY(ImGuiKey_Enter, KeyCode::Enter);
		SET_KEY(ImGuiKey_Escape, KeyCode::Escape);
		SET_KEY(ImGuiKey_A, KeyCode::A);
		SET_KEY(ImGuiKey_C, KeyCode::C);
		SET_KEY(ImGuiKey_V, KeyCode::V);
		SET_KEY(ImGuiKey_X, KeyCode::X);
		SET_KEY(ImGuiKey_Y, KeyCode::Y);
		SET_KEY(ImGuiKey_Z, KeyCode::Z);

		m_imguiRenderer->Init();

		Application& app = Application::Get();
		io.DisplaySize = ImVec2(static_cast<float>(app.GetWindow().GetWidth()), static_cast<float>(app.GetWindow().GetHeight()));
	}

	void ImGuiLayer::OnDetach()
	{
		m_imguiRenderer->Shutdown();
	}

	void ImGuiLayer::OnUpdate(double deltaTime)
	{
		
	}

	void ImGuiLayer::OnRender(double deltaTime)
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();

		io.DeltaTime = deltaTime;

		m_imguiRenderer->NewFrame();
		ImGui::NewFrame();


		const float DISTANCE = 10.0f;
		static int corner = 0;
		ImVec2 window_pos = ImVec2((corner & 1) ? ImGui::GetIO().DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? ImGui::GetIO().DisplaySize.y - DISTANCE : DISTANCE);
		ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
		if (corner != -1)
			ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		ImGui::SetNextWindowBgAlpha(0.3f); // Transparent background
		if (ImGui::Begin("Example: Simple Overlay", NULL, (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
		{
			ImGui::Text("Simple overlay\n" "in the corner of the screen.\n" "(right-click to change position)");
			ImGui::Text("Framerate: %d", static_cast<int>(app.GetFramerate()));
			ImGui::Text("FrameTime: %f ms", deltaTime);
			if (Input::IsKeyDown(KeyCode::Space)) {
				RIDGE_INFO("Space down");
				ImGui::Text("Space down!");
			}
			/*ImGui::Separator();
			if (ImGui::IsMousePosValid())
				ImGui::Text("Mouse Position: (%.1f,%.1f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
			else
				ImGui::Text("Mouse Position: <invalid>");*/
			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::MenuItem("Custom", NULL, corner == -1)) corner = -1;
				if (ImGui::MenuItem("Top-left", NULL, corner == 0)) corner = 0;
				if (ImGui::MenuItem("Top-right", NULL, corner == 1)) corner = 1;
				if (ImGui::MenuItem("Bottom-left", NULL, corner == 2)) corner = 2;
				if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
				ImGui::EndPopup();
			}
		}
		ImGui::End();

		//static bool show = true;
		//ImGui::ShowDemoWindow(&show);
		ImGui::Render();

		// Prevent backspace from removing more than on character per KeyPressedEvent
		io.KeysDown[KeyCode::Backspace] = false;

		m_imguiRenderer->RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnEvent(Event& event)
	{
#undef BIND_EVENT_FN
#define BIND_EVENT_FN(func) std::bind(&ImGuiLayer::##func, this, std::placeholders::_1)

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(OnMouseMovedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OnMouseScrolledEvent));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(OnMouseKeyPressedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(OnMouseKeyReleasedEvent));
		dispatcher.Dispatch<CharTypedEvent>(BIND_EVENT_FN(OnMouseKeyTypedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResizeEvent));
	}

	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent & e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[static_cast<int>(e.GetMouseButton())] = true;

		return false;
	}

	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent & e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[static_cast<int>(e.GetMouseButton())] = false;

		return false;
	}

	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent & e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());

		return false;
	}

	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent & e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += e.GetXOffset();
		io.MouseWheel += e.GetYOffset();

		return false;
	}

	bool ImGuiLayer::OnMouseKeyPressedEvent(KeyPressedEvent & e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKey().keyCode] = true;

		io.KeyCtrl = io.KeysDown[KeyCode::LeftControl] || io.KeysDown[KeyCode::RightControl];
		io.KeyShift = io.KeysDown[KeyCode::LeftShift] || io.KeysDown[KeyCode::RightShift];
		io.KeyAlt = io.KeysDown[KeyCode::LeftAlt] || io.KeysDown[KeyCode::RightAlt];
		io.KeySuper = io.KeysDown[KeyCode::LeftSuper] || io.KeysDown[KeyCode::RightSuper];

		return false;
	}

	bool ImGuiLayer::OnMouseKeyReleasedEvent(KeyReleasedEvent & e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKey().keyCode] = false;

		return false;
	}

	bool ImGuiLayer::OnMouseKeyTypedEvent(CharTypedEvent & e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddInputCharacter(static_cast<ImWchar>(e.GetTypedChar()));

		return false;
	}

	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent & e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(static_cast<float>(e.GetWidth()), static_cast<float>(e.GetHeight()));
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

		return false;
	}
}
