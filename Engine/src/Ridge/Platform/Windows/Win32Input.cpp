#include "RidgePCH.h"
#include "Ridge/Input/Key.h"

namespace Ridge {
	// Windows scancodes are 9 bits wide (0x000 - 0x1FF)
	KeyCode translationTable[0x200];

	void InitializeTranslationTable() {
		// Set all values to KeyCode::Unknown (= 0)
		memset(translationTable, 0, 0x200 * sizeof(KeyCode));

		translationTable[0x00B] = KeyCode::N0;
		translationTable[0x002] = KeyCode::N1;
		translationTable[0x003] = KeyCode::N2;
		translationTable[0x004] = KeyCode::N3;
		translationTable[0x005] = KeyCode::N4;
		translationTable[0x006] = KeyCode::N5;
		translationTable[0x007] = KeyCode::N6;
		translationTable[0x008] = KeyCode::N7;
		translationTable[0x009] = KeyCode::N8;
		translationTable[0x00A] = KeyCode::N9;

		translationTable[0x01E] = KeyCode::A;
		translationTable[0x030] = KeyCode::B;
		translationTable[0x02E] = KeyCode::C;
		translationTable[0x020] = KeyCode::D;
		translationTable[0x012] = KeyCode::E;
		translationTable[0x021] = KeyCode::F;
		translationTable[0x022] = KeyCode::G;
		translationTable[0x023] = KeyCode::H;
		translationTable[0x017] = KeyCode::I;
		translationTable[0x024] = KeyCode::J;
		translationTable[0x025] = KeyCode::K;
		translationTable[0x026] = KeyCode::L;
		translationTable[0x032] = KeyCode::M;
		translationTable[0x031] = KeyCode::N;
		translationTable[0x018] = KeyCode::O;
		translationTable[0x019] = KeyCode::P;
		translationTable[0x010] = KeyCode::Q;
		translationTable[0x013] = KeyCode::R;
		translationTable[0x01F] = KeyCode::S;
		translationTable[0x014] = KeyCode::T;
		translationTable[0x016] = KeyCode::U;
		translationTable[0x02F] = KeyCode::V;
		translationTable[0x011] = KeyCode::W;
		translationTable[0x02D] = KeyCode::X;
		translationTable[0x015] = KeyCode::Y;
		translationTable[0x02C] = KeyCode::Z;

		translationTable[0x028] = KeyCode::Apostrophe;
		translationTable[0x02B] = KeyCode::Backslash;
		translationTable[0x033] = KeyCode::Comma;
		translationTable[0x00D] = KeyCode::Equal;
		translationTable[0x029] = KeyCode::GraveAccent;
		translationTable[0x01A] = KeyCode::LeftBracket;
		translationTable[0x00C] = KeyCode::Minus;
		translationTable[0x034] = KeyCode::Period;
		translationTable[0x01B] = KeyCode::RightBracket;
		translationTable[0x027] = KeyCode::Semicolon;
		translationTable[0x035] = KeyCode::Slash;
		translationTable[0x056] = KeyCode::World2;

		translationTable[0x00E] = KeyCode::Backspace;
		translationTable[0x153] = KeyCode::Delete;
		translationTable[0x14F] = KeyCode::End;
		translationTable[0x01C] = KeyCode::Enter;
		translationTable[0x001] = KeyCode::Escape;
		translationTable[0x147] = KeyCode::Home;
		translationTable[0x152] = KeyCode::Insert;
		translationTable[0x15D] = KeyCode::Menu;
		translationTable[0x151] = KeyCode::PageDown;
		translationTable[0x149] = KeyCode::PageUp;
		translationTable[0x045] = KeyCode::Pause;
		translationTable[0x146] = KeyCode::Pause;
		translationTable[0x039] = KeyCode::Space;
		translationTable[0x00F] = KeyCode::Tab;
		translationTable[0x03A] = KeyCode::CapsLock;
		translationTable[0x145] = KeyCode::NumLock;
		translationTable[0x046] = KeyCode::ScrollLock;

		translationTable[0x03B] = KeyCode::F1;
		translationTable[0x03C] = KeyCode::F2;
		translationTable[0x03D] = KeyCode::F3;
		translationTable[0x03E] = KeyCode::F4;
		translationTable[0x03F] = KeyCode::F5;
		translationTable[0x040] = KeyCode::F6;
		translationTable[0x041] = KeyCode::F7;
		translationTable[0x042] = KeyCode::F8;
		translationTable[0x043] = KeyCode::F9;
		translationTable[0x044] = KeyCode::F10;
		translationTable[0x057] = KeyCode::F11;
		translationTable[0x058] = KeyCode::F12;
		translationTable[0x064] = KeyCode::F13;
		translationTable[0x065] = KeyCode::F14;
		translationTable[0x066] = KeyCode::F15;
		translationTable[0x067] = KeyCode::F16;
		translationTable[0x068] = KeyCode::F17;
		translationTable[0x069] = KeyCode::F18;
		translationTable[0x06A] = KeyCode::F19;
		translationTable[0x06B] = KeyCode::F20;
		translationTable[0x06C] = KeyCode::F21;
		translationTable[0x06D] = KeyCode::F22;
		translationTable[0x06E] = KeyCode::F23;
		translationTable[0x076] = KeyCode::F24;

		translationTable[0x038] = KeyCode::LeftAlt;
		translationTable[0x01D] = KeyCode::LeftControl;
		translationTable[0x02A] = KeyCode::LeftShift;
		translationTable[0x15B] = KeyCode::LeftSuper;
		translationTable[0x137] = KeyCode::PrintScreen;
		translationTable[0x138] = KeyCode::RightAlt;
		translationTable[0x11D] = KeyCode::RightControl;
		translationTable[0x036] = KeyCode::RightShift;
		translationTable[0x15C] = KeyCode::RightSuper;
		translationTable[0x150] = KeyCode::Down;
		translationTable[0x14B] = KeyCode::Left;
		translationTable[0x14D] = KeyCode::Right;
		translationTable[0x148] = KeyCode::Up;

		translationTable[0x052] = KeyCode::Keypad0;
		translationTable[0x04F] = KeyCode::Keypad1;
		translationTable[0x050] = KeyCode::Keypad2;
		translationTable[0x051] = KeyCode::Keypad3;
		translationTable[0x04B] = KeyCode::Keypad4;
		translationTable[0x04C] = KeyCode::Keypad5;
		translationTable[0x04D] = KeyCode::Keypad6;
		translationTable[0x047] = KeyCode::Keypad7;
		translationTable[0x048] = KeyCode::Keypad8;
		translationTable[0x049] = KeyCode::Keypad9;
		translationTable[0x04E] = KeyCode::KeypadAdd;
		translationTable[0x053] = KeyCode::KeypadDecimal;
		translationTable[0x135] = KeyCode::KeypadDivide;
		translationTable[0x11C] = KeyCode::KeypadEnter;
		translationTable[0x059] = KeyCode::KeypadEqual;
		translationTable[0x037] = KeyCode::KeypadMultiply;
		translationTable[0x04A] = KeyCode::KeypadSubtract;
	}

	bool InitPlatformInput() {
		Ridge::InitializeTranslationTable();
		return true;
	}
}