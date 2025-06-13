#include "Display.h"

namespace Display {
	int WINDOW_WIDTH = 0;
	int WINDOW_HEIGHT = 0;
}

int Display::GetWindowWidth()
{
	return WINDOW_WIDTH;
}

int Display::GetWindowHeight()
{
	return  WINDOW_HEIGHT;
}

void Display::SetWindowSize(int width, int height)
{
	WINDOW_HEIGHT = height;
	WINDOW_WIDTH = width;
}
