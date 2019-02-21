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
	initwindow(640, 380, "First Sample");
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
	
	ULONG	m_ulCurAxis1;
	m_ulCurAxis1 = 0;
	
	Result = Acm_AxResetError( m_Axishand[m_ulCurAxis1] );
	if (Result != SUCCESS)
	{
		//strString.Format("Reset axis's state failed with error Code: %x", Result);
		//ShowMessages(strString, Result);
		printf("Reset axis's state failed with error Code: %x \n", Result);
		system("pause");
		return 0;
	}
	printf("Reset axis's state \n");
	
	F64 CurPos;
	U16 State;
	U32 Status;

	printf("\n");
	// Get the axis's current state
	Result = Acm_AxGetState(m_Axishand[m_ulCurAxis1], &State);
	if (Result == SUCCESS)
	{
		switch (State)
		{
		case 0:
			//strTemp.Format("STA_AX_DISABLE");
			printf("STA_AX_DISABLE \n");
			break;
		case 1:
			//strTemp.Format("STA_AX_READY");
			printf("STA_AX_READY   \n");
			break;
		case 2:
			//strTemp.Format("STA_AX_STOPPING");
			printf("STA_AX_STOPPING   \n");
			break;
		case 3:
			//strTemp.Format("STA_AX_ERROR_STOP");
			printf("STA_AX_ERROR_STOP   \n");
			break;
		case 4:
			//strTemp.Format("STA_AX_HOMING");
			printf("STA_AX_HOMING   \n");
			break;
		case 5:
			//strTemp.Format("STA_AX_PTP_MOT");
			printf("STA_AX_PTP_MOT   \n");
			break;
		case 6:
			//strTemp.Format("STA_AX_CONTI_MOT");
			printf("STA_AX_CONTI_MOT   \n");
			break;
		case 7:
			//strTemp.Format("STA_AX_SYNC_MOT");
			printf("STA_AX_SYNC_MOT   \n");
			break;
		default:
			printf("???  \n");
			break;
		}
		//m_editXState.SetWindowText(strTemp);
	}
	printf("\n");
	Sleep(7000);




	Result = Acm_AxMoveVel(m_Axishand[m_ulCurAxis1], 0);
	if (Result != SUCCESS)
	{
		//strString.Format("Move Failed With Error Code: %x", Result);
		//ShowMessages(strString, Result);
		printf("Move Failed With Error Code: %x \n ", Result);
		system("pause");
		return 0;
	}
	printf("Movement \n");
	Sleep(5000);
	//To command axis to decelerate to stop.		
	Result = Acm_AxStopDec(m_Axishand[m_ulCurAxis1]); 
	if (Result != SUCCESS)
	{
		//strString.Format("Axis To Stop Immediately Failed  With Error Code: %x", Result);
		//ShowMessages(strString, Result);
		printf("Axis To Stop Immediately Failed  With Error Code: %x", Result);
		system("pause");
		return 0;
	}
	printf("Stop \n");
	Sleep(3000);

	printf("\n");
	// Get the axis's current state
	Result = Acm_AxGetState(m_Axishand[m_ulCurAxis1], &State);
	if (Result == SUCCESS)
	{
		switch (State)
		{
		case 0:
			//strTemp.Format("STA_AX_DISABLE");
			printf("STA_AX_DISABLE \n");
			break;
		case 1:
			//strTemp.Format("STA_AX_READY");
			printf("STA_AX_READY   \n");
			break;
		case 2:
			//strTemp.Format("STA_AX_STOPPING");
			printf("STA_AX_STOPPING   \n");
			break;
		case 3:
			//strTemp.Format("STA_AX_ERROR_STOP");
			printf("STA_AX_ERROR_STOP   \n");
			break;
		case 4:
			//strTemp.Format("STA_AX_HOMING");
			printf("STA_AX_HOMING   \n");
			break;
		case 5:
			//strTemp.Format("STA_AX_PTP_MOT");
			printf("STA_AX_PTP_MOT   \n");
			break;
		case 6:
			//strTemp.Format("STA_AX_CONTI_MOT");
			printf("STA_AX_CONTI_MOT   \n");
			break;
		case 7:
			//strTemp.Format("STA_AX_SYNC_MOT");
			printf("STA_AX_SYNC_MOT   \n");
			break;
		default:
			printf("???  \n");
			break;
		}
		//m_editXState.SetWindowText(strTemp);
	}
	printf("\n");








	//To command axis to make a never ending movement 
	// with a specified velocity
	// 0: Positive direction 1: Negative direction
	Result = Acm_AxMoveVel(m_Axishand[m_ulCurAxis1] ,1 );
	if (Result != SUCCESS)
	{
		//strString.Format("Move Failed With Error Code: %x", Result);
		//ShowMessages(strString, Result);
		printf("Move Failed With Error Code: %x \n ", Result);
		system("pause");
		return 0;
	}
	printf("Movement \n");
	Sleep(3000);
	//To command axis to decelerate to stop.		
	Result = Acm_AxStopDec(m_Axishand[m_ulCurAxis1]);
	if (Result != SUCCESS)
	{
		//strString.Format("Axis To Stop Immediately Failed  With Error Code: %x", Result);
		//ShowMessages(strString, Result);
		printf("Axis To Stop Immediately Failed  With Error Code: %x", Result);
		system("pause");
		return 0;
	}
	printf("Stop \n");
	Sleep(3000);

	printf("\n");
	// Get the axis's current state
	Result = Acm_AxGetState(m_Axishand[m_ulCurAxis1], &State);
	if (Result == SUCCESS)
	{
		switch (State)
		{
		case 0:
			//strTemp.Format("STA_AX_DISABLE");
			printf("STA_AX_DISABLE \n");
			break;
		case 1:
			//strTemp.Format("STA_AX_READY");
			printf("STA_AX_READY   \n");
			break;
		case 2:
			//strTemp.Format("STA_AX_STOPPING");
			printf("STA_AX_STOPPING   \n");
			break;
		case 3:
			//strTemp.Format("STA_AX_ERROR_STOP");
			printf("STA_AX_ERROR_STOP   \n");
			break;
		case 4:
			//strTemp.Format("STA_AX_HOMING");
			printf("STA_AX_HOMING   \n");
			break;
		case 5:
			//strTemp.Format("STA_AX_PTP_MOT");
			printf("STA_AX_PTP_MOT   \n");
			break;
		case 6:
			//strTemp.Format("STA_AX_CONTI_MOT");
			printf("STA_AX_CONTI_MOT   \n");
			break;
		case 7:
			//strTemp.Format("STA_AX_SYNC_MOT");
			printf("STA_AX_SYNC_MOT   \n");
			break;
		default:
			printf("???  \n");
			break;
		}
		//m_editXState.SetWindowText(strTemp);
	}
	printf("\n");
	

	


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

	Sleep(1000);
	Sleep(1000);
	Sleep(1000);

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