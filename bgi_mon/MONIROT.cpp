#include "graphics.h"
#include "advmotdrv.h"
#include "General.h"
#include "AdvMotApi.h"

#include "bitmap.h"


// https://msdn.microsoft.com/en-us/library/windows/desktop/ms633559(v=vs.85).aspx
// int CALLBACK WinMain

// int main()
// int APIENTRY WinMain
//int CALLBACK WinMain
//(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)

int main()
{
	initwindow(800, 600, "MONITOR V1.0");
	circle(100, 50, 40);

	Get_BMP16(0, 0, (char *) "MONIT_E.BMP", 1);    // 英文畫面

	//system("pause");
	while (!kbhit())
	{
		delay(200);
	}
	return 0;
}