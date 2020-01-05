#pragma once
#include <windows.h>
#include <vector>
#include <map>
class Inputs
{
public:
	Inputs();

	static void RightClick();
	static void LeftClick();
	static void MouseMoveOffset(int x, int y, double xOffset, double yOffset);
	static void DeltaMouseMove(int dx, int dy, int speed);
	static void MouseMove(int x, int y);
	static void Keyboard(char c);
	static int getVirtualKeyCode(char c);
	static void findFile(const std::string& name, std::vector<std::string>& output);
};

