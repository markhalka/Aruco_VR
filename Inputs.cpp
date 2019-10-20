#include "Inputs.h"



Inputs::Inputs()
{
}


Inputs::~Inputs()
{
}

void Inputs::Keyboard(char c) {
	INPUT input;
	WORD vkey = 0x53; // see link below
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = MapVirtualKey(vkey, MAPVK_VK_TO_VSC);
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;
	input.ki.wVk = vkey;
	input.ki.dwFlags = 0; // there is no KEYEVENTF_KEYDOWN
	SendInput(1, &input, sizeof(INPUT));

	input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(INPUT));
}

void Inputs::MouseMove(int x, int y)
{
	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.mouseData = 0;
	input.mi.dx = x * (65536.0f / GetSystemMetrics(SM_CXSCREEN)); //x being coord in pixels
	input.mi.dy = y * (65536.0f / GetSystemMetrics(SM_CYSCREEN)); //y being coord in pixels
	input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
	SendInput(1, &input, sizeof(input));
}

void Inputs::DeltaMouseMove(int dx, int dy, int speed) {
	if (dx == 0 && dy == 0)
		return;
	POINT p;
	if (GetCursorPos(&p))
	{
		int changeX = dx / speed == 0 ? dx % speed : dx / speed;
		int changeY = dy / speed == 0 ? dy % speed : dy / speed;

		MouseMove(p.x + dx, p.y + dy);
		DeltaMouseMove(dx - changeX, dy - changeY, speed);

		//cursor position now in p.x and p.y
	}
}

void Inputs::MouseMoveOffset(int x, int y, double xOffset, double yOffset)
{
	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.mouseData = 0;
	input.mi.dx = x * (65536.0f / GetSystemMetrics(SM_CXSCREEN)) * xOffset; //x being coord in pixels
	input.mi.dy = y * (65536.0f / GetSystemMetrics(SM_CYSCREEN)) * yOffset; //y being coord in pixels
	input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
	SendInput(1, &input, sizeof(input));
}

void Inputs::LeftClick()
{
	INPUT    Input = { 0 };
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	::SendInput(1, &Input, sizeof(INPUT));
	::ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	::SendInput(1, &Input, sizeof(INPUT));
}

void Inputs::RightClick()
{
	INPUT    Input = { 0 };
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	::SendInput(1, &Input, sizeof(INPUT));
	::ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	::SendInput(1, &Input, sizeof(INPUT));
}

