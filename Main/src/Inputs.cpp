#include "Inputs.h"



Inputs::Inputs()
{

//	specialCharacters.insert(std::pair<int, char>(VK_SPACE, ' '));
	//specialCharacters.insert(std::pair<int, char>(VK_SPACE, ' '));


}

void Inputs::findFile(const std::string& name, std::vector<std::string>& output)
{
	std::string pattern(name);
	pattern.append("\\*");
	WIN32_FIND_DATA data;
	HANDLE hFind;
	if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) { //check to make sure this looks for it not the whole file name
		do {
			output.push_back(data.cFileName);
		} while (FindNextFile(hFind, &data) != 0);
		FindClose(hFind);
	}
}



std::string characters = "abcdefghijklmnopqrstuvwxyz";
std::map<int, char> specialCharacters{ std::pair<int, char>(VK_SPACE, ' '),std::pair<int, char>(VK_CONTROL, '1') };


int Inputs::getVirtualKeyCode(char c) {
	int pos = -1;
	if ((pos = characters.find(c)) != std::string::npos) {
		return 65 + pos;
	}

	if ((pos = specialCharacters.at(c)) >= 0) {
		return pos;
	}
	
		return -1;

}
void Inputs::Keyboard(char c) {
	INPUT input;

	WORD vkey = getVirtualKeyCode(c);
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = MapVirtualKey(vkey, MAPVK_VK_TO_VSC);
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;
	input.ki.wVk = vkey;
	input.ki.dwFlags = 0; 
	SendInput(1, &input, sizeof(INPUT));
	input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(INPUT));
}

void Inputs::MouseMove(int x, int y)
{

	INPUT input;
	memset(&input, 0, sizeof(INPUT));
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
		MouseMove(p.x + dx, p.y + dy);
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

