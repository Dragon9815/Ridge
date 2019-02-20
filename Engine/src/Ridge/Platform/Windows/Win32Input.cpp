#include "RidgePCH.h"
#include "Win32Input.h"

#include "Ridge/Input/Key.h"
#include "Ridge/Input/Input.h"

#include "Ridge/Events/KeyEvent.h"
#include "Ridge/Events/MouseEvent.h"

namespace Ridge {

	Input* Input::s_instance = new Win32Input();

	bool Win32Input::InitializeImpl()
	{
		return InitializeKeyMappings();;
	}

	void Win32Input::ShutdownImpl()
	{

	}

	bool Win32Input::InitializeKeyMappings() {
		// Set all values to KeyCode::Unknown (= 0)
		memset(winToKeyMapping, 0, 0x200 * sizeof(KeyCode));

		winToKeyMapping[0x00B] = KeyCode::N0;
		winToKeyMapping[0x002] = KeyCode::N1;
		winToKeyMapping[0x003] = KeyCode::N2;
		winToKeyMapping[0x004] = KeyCode::N3;
		winToKeyMapping[0x005] = KeyCode::N4;
		winToKeyMapping[0x006] = KeyCode::N5;
		winToKeyMapping[0x007] = KeyCode::N6;
		winToKeyMapping[0x008] = KeyCode::N7;
		winToKeyMapping[0x009] = KeyCode::N8;
		winToKeyMapping[0x00A] = KeyCode::N9;

		winToKeyMapping[0x01E] = KeyCode::A;
		winToKeyMapping[0x030] = KeyCode::B;
		winToKeyMapping[0x02E] = KeyCode::C;
		winToKeyMapping[0x020] = KeyCode::D;
		winToKeyMapping[0x012] = KeyCode::E;
		winToKeyMapping[0x021] = KeyCode::F;
		winToKeyMapping[0x022] = KeyCode::G;
		winToKeyMapping[0x023] = KeyCode::H;
		winToKeyMapping[0x017] = KeyCode::I;
		winToKeyMapping[0x024] = KeyCode::J;
		winToKeyMapping[0x025] = KeyCode::K;
		winToKeyMapping[0x026] = KeyCode::L;
		winToKeyMapping[0x032] = KeyCode::M;
		winToKeyMapping[0x031] = KeyCode::N;
		winToKeyMapping[0x018] = KeyCode::O;
		winToKeyMapping[0x019] = KeyCode::P;
		winToKeyMapping[0x010] = KeyCode::Q;
		winToKeyMapping[0x013] = KeyCode::R;
		winToKeyMapping[0x01F] = KeyCode::S;
		winToKeyMapping[0x014] = KeyCode::T;
		winToKeyMapping[0x016] = KeyCode::U;
		winToKeyMapping[0x02F] = KeyCode::V;
		winToKeyMapping[0x011] = KeyCode::W;
		winToKeyMapping[0x02D] = KeyCode::X;
		winToKeyMapping[0x015] = KeyCode::Y;
		winToKeyMapping[0x02C] = KeyCode::Z;

		winToKeyMapping[0x028] = KeyCode::Apostrophe;
		winToKeyMapping[0x02B] = KeyCode::Backslash;
		winToKeyMapping[0x033] = KeyCode::Comma;
		winToKeyMapping[0x00D] = KeyCode::Equal;
		winToKeyMapping[0x029] = KeyCode::GraveAccent;
		winToKeyMapping[0x01A] = KeyCode::LeftBracket;
		winToKeyMapping[0x00C] = KeyCode::Minus;
		winToKeyMapping[0x034] = KeyCode::Period;
		winToKeyMapping[0x01B] = KeyCode::RightBracket;
		winToKeyMapping[0x027] = KeyCode::Semicolon;
		winToKeyMapping[0x035] = KeyCode::Slash;
		winToKeyMapping[0x056] = KeyCode::World2;

		winToKeyMapping[0x00E] = KeyCode::Backspace;
		winToKeyMapping[0x153] = KeyCode::Delete;
		winToKeyMapping[0x14F] = KeyCode::End;
		winToKeyMapping[0x01C] = KeyCode::Enter;
		winToKeyMapping[0x001] = KeyCode::Escape;
		winToKeyMapping[0x147] = KeyCode::Home;
		winToKeyMapping[0x152] = KeyCode::Insert;
		winToKeyMapping[0x15D] = KeyCode::Menu;
		winToKeyMapping[0x151] = KeyCode::PageDown;
		winToKeyMapping[0x149] = KeyCode::PageUp;
		winToKeyMapping[0x045] = KeyCode::Pause;
		winToKeyMapping[0x146] = KeyCode::Pause;
		winToKeyMapping[0x039] = KeyCode::Space;
		winToKeyMapping[0x00F] = KeyCode::Tab;
		winToKeyMapping[0x03A] = KeyCode::CapsLock;
		winToKeyMapping[0x145] = KeyCode::NumLock;
		winToKeyMapping[0x046] = KeyCode::ScrollLock;

		winToKeyMapping[0x03B] = KeyCode::F1;
		winToKeyMapping[0x03C] = KeyCode::F2;
		winToKeyMapping[0x03D] = KeyCode::F3;
		winToKeyMapping[0x03E] = KeyCode::F4;
		winToKeyMapping[0x03F] = KeyCode::F5;
		winToKeyMapping[0x040] = KeyCode::F6;
		winToKeyMapping[0x041] = KeyCode::F7;
		winToKeyMapping[0x042] = KeyCode::F8;
		winToKeyMapping[0x043] = KeyCode::F9;
		winToKeyMapping[0x044] = KeyCode::F10;
		winToKeyMapping[0x057] = KeyCode::F11;
		winToKeyMapping[0x058] = KeyCode::F12;
		winToKeyMapping[0x064] = KeyCode::F13;
		winToKeyMapping[0x065] = KeyCode::F14;
		winToKeyMapping[0x066] = KeyCode::F15;
		winToKeyMapping[0x067] = KeyCode::F16;
		winToKeyMapping[0x068] = KeyCode::F17;
		winToKeyMapping[0x069] = KeyCode::F18;
		winToKeyMapping[0x06A] = KeyCode::F19;
		winToKeyMapping[0x06B] = KeyCode::F20;
		winToKeyMapping[0x06C] = KeyCode::F21;
		winToKeyMapping[0x06D] = KeyCode::F22;
		winToKeyMapping[0x06E] = KeyCode::F23;
		winToKeyMapping[0x076] = KeyCode::F24;

		winToKeyMapping[0x038] = KeyCode::LeftAlt;
		winToKeyMapping[0x01D] = KeyCode::LeftControl;
		winToKeyMapping[0x02A] = KeyCode::LeftShift;
		winToKeyMapping[0x15B] = KeyCode::LeftSuper;
		winToKeyMapping[0x137] = KeyCode::PrintScreen;
		winToKeyMapping[0x138] = KeyCode::RightAlt;
		winToKeyMapping[0x11D] = KeyCode::RightControl;
		winToKeyMapping[0x036] = KeyCode::RightShift;
		winToKeyMapping[0x15C] = KeyCode::RightSuper;
		winToKeyMapping[0x150] = KeyCode::Down;
		winToKeyMapping[0x14B] = KeyCode::Left;
		winToKeyMapping[0x14D] = KeyCode::Right;
		winToKeyMapping[0x148] = KeyCode::Up;

		winToKeyMapping[0x052] = KeyCode::Keypad0;
		winToKeyMapping[0x04F] = KeyCode::Keypad1;
		winToKeyMapping[0x050] = KeyCode::Keypad2;
		winToKeyMapping[0x051] = KeyCode::Keypad3;
		winToKeyMapping[0x04B] = KeyCode::Keypad4;
		winToKeyMapping[0x04C] = KeyCode::Keypad5;
		winToKeyMapping[0x04D] = KeyCode::Keypad6;
		winToKeyMapping[0x047] = KeyCode::Keypad7;
		winToKeyMapping[0x048] = KeyCode::Keypad8;
		winToKeyMapping[0x049] = KeyCode::Keypad9;
		winToKeyMapping[0x04E] = KeyCode::KeypadAdd;
		winToKeyMapping[0x053] = KeyCode::KeypadDecimal;
		winToKeyMapping[0x135] = KeyCode::KeypadDivide;
		winToKeyMapping[0x11C] = KeyCode::KeypadEnter;
		winToKeyMapping[0x059] = KeyCode::KeypadEqual;
		winToKeyMapping[0x037] = KeyCode::KeypadMultiply;
		winToKeyMapping[0x04A] = KeyCode::KeypadSubtract;

		return true;
	}

	bool Win32Input::GetKeyStateImpl(KeyCode key)
	{
		//RIDGE_TRACE("Get state: k={0}, s={1}", key, keyState[key]);
		return keyState[key];
	}

	bool Win32Input::GetMouseStateImpl(MouseButton button)
	{
		return mouseState[button];
	}

	Vector2i Win32Input::GetMousePosImpl()
	{
		return mousePos;
	}

	void Win32Input::SetKeyState(KeyCode key, bool pressed)
	{
		keyState[key] = pressed;
		RIDGE_TRACE("Set state: k={0}, s={1}", key, pressed);
	}

	void Win32Input::SetMouseButtonState(MouseButton button, bool pressed)
	{
		mouseState[button] = pressed;
	}

	void Win32Input::SetMousePos(const Vector2i& pos)
	{
		mousePos = pos;
	}

	KeyCode Win32Input::TranslateKey(WPARAM wParam, LPARAM lParam)
	{
		// Windows sends Left Control and Right Alt message for the Right Alt key, so we need to check if 
		// this is the case and ignore the Left Control message
		if (wParam == VK_CONTROL) {
			MSG next;
			DWORD time;

			// We don't care about the Right Control key
			if (lParam & 0x01000000)
				return KeyCode::RightControl;

			time = GetMessageTime();

			// Ignore message, if next message is the Right Alt key
			if (PeekMessage(&next, NULL, 0, 0, PM_NOREMOVE))
			{
				if (next.message == WM_KEYDOWN || next.message == WM_SYSKEYDOWN ||
					next.message == WM_KEYUP || next.message == WM_SYSKEYUP)
				{
					if (next.wParam == VK_MENU && (next.lParam & 0x01000000) && next.time == time)
					{
						return KeyCode::Invalid;
					}
				}
			}

			return KeyCode::LeftControl;
		}

		
		const int scancode = HIWORD(lParam) & 0x1FF;
		RIDGE_TRACE("Translated key from 0x{0:X} to 0x{1:X}", scancode, winToKeyMapping[scancode]);
		return winToKeyMapping[scancode];
	}
}