#pragma once

#include "Ridge/Input/Input.h"
#include "Ridge/Math/Vector2.h"

namespace Ridge {
	class Win32Input : public Input
	{
	public:
		static Win32Input* GetInstance() { return static_cast<Win32Input*>(s_instance); }

		void SetKeyState(KeyCode key, bool pressed);
		void SetMouseButtonState(MouseButton button, bool pressed);
		void SetMousePos(const Vector2i& pos);

		KeyCode TranslateKey(WPARAM wParam, LPARAM lParam);
	protected:
		virtual bool InitializeImpl() override;
		virtual void ShutdownImpl() override;

		virtual bool GetKeyStateImpl(KeyCode key) override;
		virtual bool GetMouseStateImpl(MouseButton button) override;
		virtual Vector2i GetMousePosImpl() override;

	private:
		bool InitializeKeyMappings();

	private:
		// Windows scancodes are 9 bits wide (0x000 - 0x1FF)
		KeyCode		winToKeyMapping[0x200];

		bool		keyState[KeyCode::LENGTH];
		bool		mouseState[MouseButton::MB_LENGTH];
		Vector2i	mousePos;
	};
}
