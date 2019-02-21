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
	initwindow(320, 190, "Output Sample");
	circle(100, 50, 40);


	U32		Result;
	ULONG	i = 0, deviceNumber = 0;
	// CString	strTemp;
	char dllversion[8];
	ULONG buffer;
	buffer = 10;

	UINT AxisNum;

	HAND	m_Devhand;
	HAND	m_Axishand[64];
	DEVLIST	m_avaDevs[MAX_DEVICES];

	m_Devhand = 0;

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
	printf("Open Device : %d \n" , m_Devhand );


	
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
	
	







	Result = Acm_DevOpen(m_dwDevNum, &m_Devhand);
	if (Result != SUCCESS)
	{
		//strString.Format("Open Device Failed With Error Code:%x", Result);
		//ShowMessages(strString, Result);
		printf("Open Device Failed With Error Code:%x \n", Result);
		system("pause");
		return 0;
	}

	printf("Open Device : %d \n", m_Devhand);
	
	ULONG m_devType = (m_dwDevNum & 0xff000000) >> 24;

	printf("==============> m_devType Code:%x \n", m_devType);

	UINT Ret = 0;
	double m_FSV = 0;
	for (UINT i = 0; i < 24; i++)
	{
		Ret = Acm_GetChannelProperty(m_Devhand, i, CFG_CH_DaqDoFsvEnable, &m_FSV);
		printf("CH- %3d : ", i);
		if (m_FSV == 1)
		{
			printf("ON  \n");
		}
		else
		{
			printf("OFF \n");
		}
	}


	/*
	UINT m_DOChanNum;
	m_DOChanNum = 128;
	UINT Ret = 0;
	bool isInitial = true;
	double m_FSV = 0;
	char subItems[4];
	for (UINT i = 0; i < m_DOChanNum; i++)
	{
		
		Ret = Acm_GetChannelProperty( m_Devhand, i, CFG_CH_DaqDoFsvEnable, &m_FSV);
		if (Ret != SUCCESS)
		{
			printf("Acm_GetChannelProperty Error Code: %x \n", Ret);

			//char ErrorMsg[100];
			//bool res = Acm_GetErrorMessage( Ret, ErrorMsg, 100);
			//if (res) printf("Error Message: %s \n", ErrorMsg);

			system("pause");
			return 0;
		}

		if (isInitial)
		{
			//subItems[0].Format("CH- %d", i);
			//m_listdo.InsertItem(i, subItems[0]);
			printf("CH- %3d : ", i);
		}
		if (m_FSV == 1)
		{
			//m_listdo.SetCheck(i, TRUE);
			printf("ON  \n");
		}
		else
		{
			//m_listdo.SetCheck(i, FALSE);
			printf("OFF \n");
		}

	}
	*/


	/*
	for (UINT i = 0; i < m_DOChanNum; i++)
	{
		Ret = Acm_SetChannelProperty(m_devhand, i, CFG_CH_DaqDoFsvEnable, m_FSV);
		if (Ret != SUCCESS)
		{

		}
	}
	*/











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