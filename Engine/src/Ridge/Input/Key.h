#pragma once

#include "Ridge/Common.h"

/*#define RG_KEY_UNKNOWN -1
#define RG_KEY_SPACE 32
#define RG_KEY_APOSTROPHE 39
#define RG_KEY_COMMA 44
#define RG_KEY_MINUS 45
#define RG_KEY_PERIOD 46
#define RG_KEY_SLASH 47
#define RG_KEY_0 48
#define RG_KEY_1 49
#define RG_KEY_2 50
#define RG_KEY_3 51
#define RG_KEY_4 52
#define RG_KEY_5 53
#define RG_KEY_6 54
#define RG_KEY_7 55
#define RG_KEY_8 56
#define RG_KEY_9 57
#define RG_KEY_SEMICOLON 59
#define RG_KEY_EQUAL 61
#define RG_KEY_A 65
#define RG_KEY_B 66
#define RG_KEY_C 67
#define RG_KEY_D 68
#define RG_KEY_E 69
#define RG_KEY_F 70
#define RG_KEY_G 71
#define RG_KEY_H 72
#define RG_KEY_I 73
#define RG_KEY_J 74
#define RG_KEY_K 75
#define RG_KEY_L 76
#define RG_KEY_M 77
#define RG_KEY_N 78
#define RG_KEY_O 79
#define RG_KEY_P 80
#define RG_KEY_Q 81
#define RG_KEY_R 82
#define RG_KEY_S 83
#define RG_KEY_T 84
#define RG_KEY_U 85
#define RG_KEY_V 86
#define RG_KEY_W 87
#define RG_KEY_X 88
#define RG_KEY_Y 89
#define RG_KEY_Z 90

#define RG_KEY_LEFT_BRACKET 91
#define RG_KEY_BACKSLASH 92
#define RG_KEY_RIGHT_BRACKET 93
#define RG_KEY_GRAVE_ACCENT 96
#define RG_KEY_WORLD1 161
#define RG_KEY_WORLD2 162
#define RG_KEY_ESCAPE 256
#define RG_KEY_ENTER 257
#define RG_KEY_TAB 258
#define RG_KEY_BACKSPACE 259
#define RG_KEY_INSERT 260
#define RG_KEY_DELETE 261
#define RG_KEY_RIGHT 262
#define RG_KEY_LEFT 263
#define RG_KEY_DOWN 264
#define RG_KEY_UP 265
#define RG_KEY_PAGE_UP 266
#define RG_KEY_PAGE_DOWN 267
#define RG_KEY_HOME 268
#define RG_KEY_END 269
#define RG_KEY_CAPS_LOCK 280
#define RG_KEY_SCROLL_LOCK 281
#define RG_KEY_NUM_LOCK 282
#define RG_KEY_PRINT_SCREEN 283
#define RG_KEY_PAUSE 284

#define RG_KEY_F1 290
#define RG_KEY_F2 291
#define RG_KEY_F3 292
#define RG_KEY_F4 293
#define RG_KEY_F5 294
#define RG_KEY_F6 295
#define RG_KEY_F7 296
#define RG_KEY_F8 297
#define RG_KEY_F9 298
#define RG_KEY_F10 299
#define RG_KEY_F11 300
#define RG_KEY_F12 301
#define RG_KEY_F13 302
#define RG_KEY_F14 303
#define RG_KEY_F15 304
#define RG_KEY_F16 305
#define RG_KEY_F17 306
#define RG_KEY_F18 307
#define RG_KEY_F19 308
#define RG_KEY_F20 309
#define RG_KEY_F21 310
#define RG_KEY_F22 311
#define RG_KEY_F23 312
#define RG_KEY_F24 313
#define RG_KEY_F25 314

#define RG_KEY_KP_0 320
#define RG_KEY_KP_1 321
#define RG_KEY_KP_2 322
#define RG_KEY_KP_3 323
#define RG_KEY_KP_4 324
#define RG_KEY_KP_5 325
#define RG_KEY_KP_6 326
#define RG_KEY_KP_7 327
#define RG_KEY_KP_8 328
#define RG_KEY_KP_9 329
#define RG_KEY_KP_DECIMAL 330
#define RG_KEY_KP_DIVIDE 331
#define RG_KEY_KP_MULTIPLY 332
#define RG_KEY_KP_SUBTRACT 333
#define RG_KEY_KP_ADD 334
#define RG_KEY_KP_ENTER 335

#define RG_KEY_LEFT_SHIFT 340
#define RG_KEY_LEFT_CONTROL 341
#define RG_KEY_LEFT_ALT 342
#define RG_KEY_LEFT_SUPER 343

#define RG_KEY_RIGHT_SHIFT 344
#define RG_KEY_RIGHT_CONTROL 345
#define RG_KEY_RIGHT_ALT 346
#define RG_KEY_RIGHT_SUPER 347
#define RG_KEY_MENU 348
#define RG_KEY_LAST RG_KEY_MENU*/

namespace Ridge {
	enum KeyCode {
		 Unknown = 0,
		 A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		 N0, N1, N2, N3, N4, N5, N6, N7, N8, N9,
		 Space, Apostrophe, Comma, Minus, Period, Slash, Semicolon, Equal, LeftBracket, Backslash, RightBracket, GraveAccent, 
		 World1, World2,
		 Escape, Enter, Tab, Backspace, Insert, Delete, Right, Left, Down, Up, PageUp, PageDown, Home, End, 
		 CapsLock, ScrollLock, NumLock, PrintScreen, Pause,
		 F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24,
		 Keypad0, Keypad1, Keypad2, Keypad3, Keypad4, Keypad5, Keypad6, Keypad7, Keypad8, Keypad9,
		 KeypadDecimal, KeypadDivide, KeypadMultiply, KeypadSubtract, KeypadAdd, KeypadEnter, KeypadClear, KeypadEqual,
		 LeftShift, LeftControl, LeftAlt, LeftSuper, RightShift, RightControl, RightAlt, RightSuper, Menu,
		 Invalid
	};

	class RIDGE_API Key 
	{
	public:
		Key(KeyCode keyCode, int scancode) 
			: keyCode(keyCode), scancode(scancode) {}

		KeyCode keyCode;
		int scancode;
	};

	inline std::ostream& operator<<(std::ostream& os, const Key& e)
	{
		return os << "Key{kc=" << (int)e.keyCode << ",sc=" << std::hex << e.scancode << "}" << std::dec;
	}
}

