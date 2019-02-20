#pragma once

#include "Key.h"
#include "Mouse.h"

#include "Ridge/Math/Vector2.h"

namespace Ridge {
	class RIDGE_API Input {
	public:
		static bool Initalize() { return s_instance->InitializeImpl(); }
		static void Shutdown() { s_instance->ShutdownImpl(); }

		static bool IsKeyDown(KeyCode key) {
			bool state = s_instance->GetKeyStateImpl(key);
			return state; 
		}
		static bool IsKeyUp(KeyCode key) { return !s_instance->GetKeyStateImpl(key); }

		static bool IsMouseDown(MouseButton button) { return s_instance->GetMouseStateImpl(button); }
		static bool IsMouseUp(MouseButton button) { return !s_instance->GetMouseStateImpl(button); }

		static Vector2i GetMousePos() { return s_instance->GetMousePosImpl(); }
	protected:
		virtual bool InitializeImpl() = 0;
		virtual void ShutdownImpl() = 0;

		virtual bool GetKeyStateImpl(KeyCode key) = 0;
		virtual bool GetMouseStateImpl(MouseButton button) = 0;
		virtual Vector2i GetMousePosImpl() = 0;

	protected:
		static Input* s_instance;
	};
}