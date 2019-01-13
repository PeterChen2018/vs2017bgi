#include "graphics.h"

// https://msdn.microsoft.com/en-us/library/windows/desktop/ms633559(v=vs.85).aspx
// int CALLBACK WinMain

// int main()
// int APIENTRY WinMain
int CALLBACK WinMain
(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	initwindow(400, 300, "First Sample");
	circle(100, 50, 40);
	while (!kbhit())
	{
		delay(200);
	}
	return 0;
}