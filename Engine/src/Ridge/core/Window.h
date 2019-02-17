#pragma once

#include "RidgePCH.h"

#include "Ridge/Events/Event.h"

namespace Ridge
{
	struct WindowProps
	{
		std::string Title;
		unsigned int ClientWidth;
		unsigned int ClientHeight;

		WindowProps(const std::string& title = "Ridge Engine",
					unsigned int clientWidth = 1280,
					unsigned int clientHeight = 720)
			: Title(title), ClientWidth(clientWidth), ClientHeight(clientHeight)
		{ }
	};

	class RIDGE_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		virtual ~Window() { }

		virtual bool Init() = 0;

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual void SetVisibility(bool shown) = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}