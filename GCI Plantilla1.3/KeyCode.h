#ifndef _KEYCODE_
#define _KEYCODE_
#include <Windows.h>

class KeyBoard
{
public:
	enum Keys
	{
		Clic_Izq = 0x01, Clic_Der,
		Clic_Middle = 0x04,
		Backspace = 0x08, Tab,
		Clear = 0x0C, Enter,
		Shift = 0x10, Control, Alt,
		Escape = 0x1B,
		Space = 0x20, PageUp, PageDown, End, Home, Left, Up, Right, Down,
		Zero = 0x30, One, Two, Three, Four, Five, Six, Seven, Eight, Nine,
		A = 0x41, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		NumPad0 = 0x60, NumPad1, NumPad2, NumPad3, NumPad4, NumPad5, NumPad6, NumPad7, NumPad8, NumPad9,
		F1 = 0x70, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12
	};
	enum XBOX_Button
	{
		DPAD_UP = 0, DPAD_DOWN, DPAD_LEFT, DPAD_RIGHT, BACK, LEFT_THUMB, RIGHT_THUMB, LT, RT, XBOX_A, XBOX_B, XBOX_X, XBOX_Y
	};
};

#endif