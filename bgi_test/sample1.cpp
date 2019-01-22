#include "graphics.h"
#include "advmotdrv.h"
#include "General.h"
#include "AdvMotApi.h"


// https://msdn.microsoft.com/en-us/library/windows/desktop/ms633559(v=vs.85).aspx
// int CALLBACK WinMain

// int main()
// int APIENTRY WinMain
//int CALLBACK WinMain
//(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)

int main()
{
	initwindow(400, 300, "First Sample");
	circle(100, 50, 40);


	U32		Result;
	ULONG	i = 0, deviceNumber = 0;
	// CString	strTemp;
	char dllversion[8];
	ULONG buffer;
	buffer = 10;

	HAND	m_Devhand;
	HAND	m_Axishand[64];
	DEVLIST	m_avaDevs[MAX_DEVICES];


	// Get the list of available device numbers and names of devices, of which driver has been loaded successfully 
	//If you have two/more board,the device list(m_avaDevs) may be changed when the slot of the boards changed,for example:m_avaDevs[0].szDeviceName to PCI-1245
	//m_avaDevs[1].szDeviceName to PCI-1245L,changing the slot£¬Perhaps the opposite
	Result = Acm_GetAvailableDevs(m_avaDevs, MAX_DEVICES, &deviceNumber);
	if (Result != SUCCESS)
	{
		//strTemp.Format("Get Device Numbers Failed With Error Code: %x", Result);
		//ShowMessages(strTemp, Result);
		//return TRUE;
		printf("Get Device Numbers Failed With Error Code: %x", Result);
	}
	printf("Code: %x\n", Result);

	system("pause");
	while (!kbhit())
	{
		delay(200);
	}
	return 0;
}