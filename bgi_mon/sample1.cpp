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
	initwindow(320, 190, "G code Sample");
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

	memset(m_Axishand, 0, sizeof(m_Axishand));

	DWORD	m_dwDevNum;
	ULONG	m_ulAxisCount;

	// Get the list of available device numbers and names of devices, of which driver has been loaded successfully 
	//If you have two/more board,the device list(m_avaDevs) may be changed when the slot of the boards changed,for example:m_avaDevs[0].szDeviceName to PCI-1245
	//m_avaDevs[1].szDeviceName to PCI-1245L,changing the slot£¬Perhaps the opposite
	Result = Acm_GetAvailableDevs(m_avaDevs, MAX_DEVICES, &deviceNumber);
	if (Result != SUCCESS)
	{
		//strTemp.Format("Get Device Numbers Failed With Error Code: %x", Result);
		//ShowMessages(strTemp, Result);
		//return TRUE;
		printf("Get Device Numbers Failed With Error Code: %x \n", Result);
		system("pause");
		return 0;
	}
	printf("Result Code: %x\n", Result);
	printf("deviceNumber: %d\n", deviceNumber);
	
/*typedef struct tagPT_DEVLIST
{
    DWORD   dwDeviceNum;
    char	szDeviceName[50];    	
    SHORT   nNumOfSubdevices;	
} DEVLIST, *LPDEVLIST;
*/

	for (i = 0; i < deviceNumber; i++)
	{
		printf("device: %s \n", m_avaDevs[i].szDeviceName);
		printf("device: %d \n", m_avaDevs[i].dwDeviceNum);
	}
	m_dwDevNum = m_avaDevs[0].dwDeviceNum;
	

	// Open Device
	Result = Acm_DevOpen(m_dwDevNum, &m_Devhand);
	if (Result != SUCCESS)
	{
		//strString.Format("Open Device Failed With Error Code:%x", Result);
		//ShowMessages(strString, Result);
		printf("Open Device Failed With Error Code: %x \n", Result);
		system("pause");
		return 0;
	}
	printf("Open Device \n");

	ULONG	AxesPerDev, AxisNumber;

	m_ulAxisCount = 0;
	//FT_DevAxesCount:Get axis number of this device.
	Result = Acm_GetU32Property(m_Devhand, FT_DevAxesCount, &AxesPerDev);
	if (Result != SUCCESS)
	{
		//strString.Format("Get Device Axes Number Failed With Error Code: %x", Result);
		//ShowMessages(strString, Result);
		printf("Get Device Axes Number Failed With Error Code: %x \n", Result);
		system("pause");
		return 0;
	}
	m_ulAxisCount = AxesPerDev;
	
	printf("Get axis number of this device: %d \n" , m_ulAxisCount);

	for (AxisNumber = 0; AxisNumber < m_ulAxisCount; AxisNumber++)
	{
		//Open every Axis and get the each Axis Handle
		//And Initial property for each Axis 					
		//Open Axis 
		//The second parameter is the position of this axis.
		//The first axis is 0, the second one is 1 and so on.
		Result = Acm_AxOpen(m_Devhand, (USHORT)AxisNumber, &m_Axishand[AxisNumber]);
		//Another way to open axis is "Acm_AxOpenbyID" .
		//The second parameter of Acm_AxOpenbyID is the EtherCAT Slave ID
		if (Result != SUCCESS)
		{
			//strString.Format("Open Axis Failed With Error Code: %x", Result);
			//ShowMessages(strString, Result);
			printf("Open Axis Failed With Error Code: %x \n", Result);
			system("pause");
			return 0;
		}
		// strTemp.Format("%d-Axis", AxisNumber);
		// m_cmbAxisSel1.AddString(strTemp);
		printf("%d - Axis \n", AxisNumber);

		//Set command position for the specified axis
		Acm_AxSetCmdPosition( m_Axishand[AxisNumber], 0 );
	}

	printf("Open every Axis and get the each Axis Handle \n");

	
	UINT AxisNum;
	//Set servo Driver ON,1: On
	for (AxisNum = 0; AxisNum < m_ulAxisCount; AxisNum++)
	{
		printf("Servo ON Axis: %d \n", Result);
		Result = Acm_AxSetSvOn( m_Axishand[AxisNum], 1 );
		if (Result != SUCCESS)
		{
			//strString.Format("Servo On Failed With Error Code: %x", Result);
			//ShowMessages(strString, Result);
			printf("Servo On Failed With Error Code: %x \n", Result);
			system("pause");
			return 0;
		}
	}
	printf("SERVER ON \n");
	

	HAND	m_GMShand;
	m_GMShand = 0;

	Result = Acm_GmOpen(m_Devhand, &m_GMShand);
	if (Result != SUCCESS)
	{
		//strString.Format("Open G Code System fail. error code:%xh", Result);
		//MessageBox(strString, "G Code System", MB_OK);
		printf("Open G Code System fail. error code:%xh \n", Result);
		system("pause");
		return 0;
	}

	printf("Open GMSystem \n");



	/*
	FILE * pFile;
	pFile = fopen("myfile.txt", "w");
	if (pFile != NULL)
	{
		fputs("G01 X20000\n", pFile);
		fclose(pFile);
	}
	*/

	
	U32 ErrorRow = 0;
	
	char m_GcodeFileName[] = "D:\\vs2017bgi\\bgi_test3\\myfile.txt";
	//char m_GcodeFileName[] = "D:\\vs2017bgi\\bgi_test3\\test5.ncd";
	char* pFilePath = (char*)(LPCTSTR)m_GcodeFileName;
	Result = Acm_GmLoadJob(m_GMShand, pFilePath, &ErrorRow);

	//Result = Acm_GmLoadJob(m_GMShand, "D:\\vs2017bgi\\bgi_test3\\myfile.txt", &ErrorRow);
	if (Result != SUCCESS)
	{
		//strString.Format("Down load ncd file error. error code:%xh,error row:%d", Result, ErrorRow);
		//MessageBox(strString, "G Code System", MB_OK);
		printf("Down load ncd file error. error code:%xh,error row:%d \n", Result, ErrorRow);
		system("pause");
		return 0;
	}
	printf("Download \n");
	

	
	Result = Acm_GmCommand(m_GMShand, "XQ");
	if (Result != SUCCESS)
	{
		//strString.Format("Execute G Code System failed. error code:%xh", Result);
		//MessageBox(strString, "G Code System", MB_OK);
		printf("Execute G Code System failed. error code:%xh \n", Result);
		system("pause");
		return 0;
	}
	printf("Execute G code \n");


	Sleep(15000);

	Result = Acm_GmClose(m_GMShand);
	if (Result != SUCCESS)
	{
		//strString.Format("Close G Code System fail. error code:%xh", Result);
		//MessageBox(strString, "G Code System", MB_OK);
		printf("Close G Code System fail. error code:%xh \n", Result);
		system("pause");
		return 0;
	}
	m_GMShand = 0;

	printf("Close GMSystem \n");



	for (AxisNum = 0; AxisNum < m_ulAxisCount; AxisNum++)
	{
		printf("Servo OFF Axis: %d \n", Result);
		//Set servo Driver OFF,0: Off
		Result = Acm_AxSetSvOn(m_Axishand[AxisNum], 0);
		if (Result != SUCCESS)
		{
			//strString.Format("Servo Off Failed With Error Code: %x", Result);
			//ShowMessages(strString, Result);
			printf("Servo Off Failed With Error Code: %x \n ", Result);
			system("pause");
			return 0;
		}
	}

	printf("SERVER OFF \n");

	Sleep(3000);

	USHORT usAxisState[64];

	for (AxisNum = 0; AxisNum < m_ulAxisCount; AxisNum++)
	{
		// Get the axis's current state
		Acm_AxGetState(m_Axishand[AxisNum], &usAxisState[AxisNum]);
		if (usAxisState[AxisNum] == STA_AX_ERROR_STOP)
		{
			//Reset the axis' state. 
			//If the axis is in ErrorStop state, the state will be changed to Ready after calling this function
			Acm_AxResetError(m_Axishand[AxisNum]);
		}
		//To command axis to decelerate to stop.
		Acm_AxStopDec(m_Axishand[AxisNum]);
	}
	for (AxisNum = 0; AxisNum < m_ulAxisCount; AxisNum++)
	{
		//Close Axes
		Acm_AxClose(&m_Axishand[AxisNum]);
	}
	//Close Device
	Acm_DevClose(&m_Devhand);

	printf("Close Device \n");

	system("pause");
	while (!kbhit())
	{
		delay(200);
	}
	return 0;
}