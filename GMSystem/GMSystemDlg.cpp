// GMSystemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GMSystem.h"
#include "GMSystemDlg.h"
#include "general.h"
#include "PPointOperation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGMSystemDlg dialog




CGMSystemDlg::CGMSystemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGMSystemDlg::IDD, pParent)
	, m_uGmSystemState(0)
{
	m_Devhand = 0;
	memset(m_Axishand,0,sizeof(m_Axishand));
	m_bServoOn =FALSE;
	m_bInit=FALSE;
	m_GMShand = 0;
	m_bGMSOpened = FALSE;
    m_CurrentRowCount =-1;
	//{{AFX_DATA_INIT(CE_GearDlg)

	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGMSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_AVLAIBLEDEVICE, m_AvailableDevice);
	DDX_Control(pDX, IDC_ETROW, m_CurrentRow);
	DDX_Control(pDX, IDC_ETSTATUS2, m_CurrentStatus);
	DDX_Control(pDX, IDC_ETSTATUS, m_CurrentCmd);
	DDX_Control(pDX, IDC_EDIT1, m_GCodeText);
	DDX_Control(pDX, IDC_ETROW2, m_Cmd);
	DDX_Control(pDX, IDC_BTN_OPEN, m_OpenBoard);
	DDX_Control(pDX, IDC_BTNOPEN, m_OpenGMS);
}

BEGIN_MESSAGE_MAP(CGMSystemDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_OPEN, CGMSystemDlg::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_CLOSE, CGMSystemDlg::OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BTN_LOADCFG, CGMSystemDlg::OnBnClickedBtnLoadcfg)
	ON_CBN_SELCHANGE(IDC_AVLAIBLEDEVICE, CGMSystemDlg::OnCbnSelchangeAvlaibledevice)
	ON_BN_CLICKED(IDC_BTNDOWNLOAD, CGMSystemDlg::OnBnClickedBtndownload)
	ON_BN_CLICKED(IDC_BTNUPLOAD, CGMSystemDlg::OnBnClickedBtnupload)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTNOPEN, CGMSystemDlg::OnBnClickedBtnopen)
	ON_BN_CLICKED(IDC_BTNCLOSE, CGMSystemDlg::OnBnClickedBtnclose)
	ON_BN_CLICKED(IDC_BTNDXF, CGMSystemDlg::OnBnClickedBtndxf)
	ON_BN_CLICKED(IDC_BTNEXECUTE, CGMSystemDlg::OnBnClickedBtnexecute)
	ON_BN_CLICKED(IDC_SEND, CGMSystemDlg::OnBnClickedSend)
	ON_BN_CLICKED(IDC_BTNSTOP, CGMSystemDlg::OnBnClickedBtnstop)
	ON_BN_CLICKED(IDC_BTNOPENFILE, CGMSystemDlg::OnBnClickedBtnopenfile)
	ON_BN_CLICKED(IDC_BTNPPOINT, &CGMSystemDlg::OnBnClickedBtnppoint)
END_MESSAGE_MAP()


// CGMSystemDlg message handlers

BOOL CGMSystemDlg::OnInitDialog()
{
	U32		Result;
	ULONG	i=0,deviceNumber=0;
	CString	strTemp;

	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	Result = Acm_GetAvailableDevs(m_avaDevs,MAX_DEVICES,&deviceNumber);
	if (Result!=SUCCESS)
	{
		MessageBox("no available device in system","G Code System",MB_OK);
		return TRUE;
	}

	m_AvailableDevice.Clear();
	for (i=0;i<deviceNumber;i++)
	{
		m_AvailableDevice.AddString(m_avaDevs[i].szDeviceName);
	}

	m_AvailableDevice.SetCurSel(0);
	m_dwDevNum=m_avaDevs[0].dwDeviceNum;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGMSystemDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	
	
	CDialog::OnSysCommand(nID, lParam);
	
}
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGMSystemDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGMSystemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CGMSystemDlg::OnBnClickedBtnOpen()
{
	// TODO: Add your control notification handler code here
	U32		Result;
	ULONG	retry = 0, SlaveOnRing0 = 0, SlaveOnRing1 = 0;
    bool	rescan = false;
	int		i=0;
	ULONG	slaveDevs[16];
	CString strString;
	ULONG	buffLen=0,AxisPerDev=0,AxisNumber=0;

	UpdateData(TRUE);

	if (m_bInit == FALSE)
	{
		do
		{
			Result = Acm_DevOpen(m_dwDevNum,&m_Devhand);
			if (Result != SUCCESS)
			{
				strString.Format("Open Device Failed With Error Code:%x",Result);
				MessageBox(strString,"G Code System",MB_OK);
				retry++;
				rescan = true;
				if (retry > 10)
					return;
				Sleep(1000);
			}
			else
			{
				//User must check the slave count on each ring match the actual connection.
				//We recommend using following code that was marked to check connection status.
				//The example expect there is one slave on Motion ring and user does not connect any slave on IO ring.
				/*rescan = false;
				Result = Acm_GetU32Property(m_Devhand, FT_MasCyclicCnt_R0, &SlaveOnRing0);
				Result = Acm_GetU32Property(m_Devhand, FT_MasCyclicCnt_R1, &SlaveOnRing1);
				if (SlaveOnRing0 != 1 || SlaveOnRing1 != 0)
				{
					AfxMessageBox("Retrieved the slave states do not match the actual connection.",MB_OK);
					Acm_DevReOpen(m_Devhand);
					Acm_DevClose(&m_Devhand);
					Sleep(1000);
					retry = 0;
					rescan = true;
				}*/
			}
		} while (rescan == true);
		m_bInit = TRUE; 
		m_OpenBoard.SetWindowText("Close Board");
	}
	else
	{
		if(m_bGMSOpened)
		{
			KillTimer(1);
			Acm_GmClose(m_GMShand);
			m_GMShand= 0;
			m_OpenGMS.SetWindowText("Open GMS");
			m_bGMSOpened = FALSE;
		}
		//Close Device
		Acm_DevClose(&m_Devhand);
		m_Devhand = 0;
		m_OpenBoard.SetWindowText("Open Board");
		m_bInit = FALSE;
	}
	
}

void CGMSystemDlg::OnBnClickedBtnClose()
{
	// TODO: Add your control notification handler code here
	//Stop Every Axes
	if (m_bInit == TRUE)
	{
		//KillTimer(1);
		if(m_bGMSOpened)
		{
			Acm_GmClose(m_GMShand);
		}		
		//Close Device
		Acm_DevClose(&m_Devhand);
		
		
		m_bInit = FALSE;
	}
}

void CGMSystemDlg::OnBnClickedBtnLoadcfg()
{
	// TODO: Add your control notification handler code here
	U32 Result;
	CString strString;

	CFileDialog  fileDialog(TRUE,
		"cfg",
		NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
		"Config file(*.cfg)|*.cfg||",this);
	if (fileDialog.DoModal() != IDOK)
		return;

	strString= fileDialog.GetPathName();
	Result=Acm_DevLoadConfig(m_Devhand,strString.GetBuffer(strString.GetLength()));

	if(Result != SUCCESS)
	{
		strString.Format("Load config Failed with Error Code:%xh",Result);
		MessageBox(strString,"G Code System",MB_OK);
		return;
	}	
}

void CGMSystemDlg::OnBnClickedBtnServo()
{
	// TODO: Add your control notification handler code here
	UINT AxisNum;
	U32 Result;
	CString   strString;
	//Check the servoOno flag to decide if turn on or turn off the ServoOn output.
	if (m_bServoOn==false)
	{
		for(AxisNum=0;AxisNum<m_ulAxisCount;AxisNum++)
		{
			Result=Acm_AxSetSvOn(m_Axishand[AxisNum],1);
			if(Result != SUCCESS)
			{
				strString.Format("Servo On Failed with Error Code:%x",Result);
				MessageBox(strString,"G Code System",MB_OK);
				return;
			}
			m_bServoOn=true;
			m_btnServoOn.SetWindowText("Servo Off");
		}
	}
	else
	{
		for(AxisNum=0;AxisNum<m_ulAxisCount;AxisNum++)
		{
			Result=Acm_AxSetSvOn(m_Axishand[AxisNum],0);
			if(Result != SUCCESS)
			{
				strString.Format("Servo Off Failed with Error Code:%xh",Result);
				MessageBox(strString,"G Code System",MB_OK);
				return;
			}
			m_bServoOn=false;
			m_btnServoOn.SetWindowText("Servo On");
		}
	}	
}

void CGMSystemDlg::OnCbnSelchangeAvlaibledevice()
{
	// TODO: Add your control notification handler code here
	m_dwDevNum=m_avaDevs[m_AvailableDevice.GetCurSel()].dwDeviceNum;
}



void CGMSystemDlg::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	U32 Result;
	CString strString;
	UpdateData(TRUE);
	U32 uCurrentRow;
	U32 uCurrentStatus;
	if (m_bInit&&m_bGMSOpened)
	{
		
		Result = Acm_GmGetCurrentRow(m_GMShand, &uCurrentRow);
		if(Result != SUCCESS)
		{
			strString.Format("Get Current Row Error.error code:%xh",Result);
			m_CurrentRow.SetWindowText(strString);
			return;
		}
// 		else
// 		{
// 			strString.Format("%d",uCurrentRow);
// 			m_CurrentRow.SetWindowText(strString);
// 		}

		Result = Acm_GmGetState(m_GMShand, &uCurrentStatus);

		if(Result != SUCCESS)
		{
			strString.Format("Get Current status Error.error code:%xh",Result);
			m_CurrentRow.SetWindowText(strString);
			return;
		}
		else
		{
			m_uGmSystemState = uCurrentStatus;
			switch(uCurrentStatus)
			{
			case 0:
				m_CurrentStatus.SetWindowText("Stopping");
				m_CurrentRowCount = -1;
				break;
			case 1:
				m_CurrentStatus.SetWindowText("UnReady");
				m_CurrentRowCount = -1;
				break;
			case 2:
				m_CurrentStatus.GetWindowText(strString);
				if (strString !="Ready")
				{
					m_CurrentStatus.SetWindowText("Ready");	
					m_CurrentRowCount = -1;
				}	
				break;
			case 3:
				m_CurrentStatus.SetWindowText("Executing");
				if (m_CurrentRowCount != uCurrentRow)
				{
					m_CurrentRowCount = uCurrentRow;
				}
				break;
			case 4:
				break;
			case 5:
				m_CurrentStatus.SetWindowText("Pause");
				if (m_CurrentRowCount != uCurrentRow)
				{
                   m_CurrentRowCount = uCurrentRow;
				}				
				break;
			case 6:
				m_CurrentStatus.SetWindowText("Error");
				if (m_CurrentRowCount != uCurrentRow)
				{
					m_CurrentRowCount = uCurrentRow;
				}
				break;
			default:
				strString.Format("Ready");
				break;
			}
			//m_CurrentStatus.SetWindowText(strString);
		}
		if (m_CurrentRowCount == -1)
		{
			m_CurrentRow.SetWindowText("0");
		}
		else
		{
			strString.Format("%d",m_CurrentRowCount +1);
    		m_CurrentRow.SetWindowText(strString);
		}
		char chCurrentCmd[100];
		U32 length=100;
		chCurrentCmd[0]=0;
		if(m_uGmSystemState == 3)
		{
		
			Result = Acm_GmGetCurrentCommand(m_GMShand, chCurrentCmd,&length);
			if(Result != SUCCESS)
			{
				strString.Format("Get Current Cmd Error.error code:%xh",Result);
				m_CurrentCmd.SetWindowText(strString);
				return;
			}
			else
			{
				m_CurrentCmd.SetWindowText(chCurrentCmd);
			}
		}
		else
			m_CurrentCmd.SetWindowText("");

	}

	UpdateData(FALSE);
	CDialog::OnTimer(nIDEvent);
}


void CGMSystemDlg::OnBnClickedBtnopen()
{
	// TODO: Add your control notification handler code here
	U32 Result;
	CString strString;

	if(m_bGMSOpened == FALSE)
	{
		Result = Acm_GmOpen(m_Devhand, &m_GMShand);
		if(Result != SUCCESS)
		{
			strString.Format("Open G Code System fail. error code:%xh",Result);
			MessageBox(strString,"G Code System",MB_OK);
			return;
		}
		SetTimer(1, 200,NULL);
		m_OpenGMS.SetWindowText("Close GMSystem");
		m_bGMSOpened = TRUE;
		//Enable timer to check command counter and so on
	}
	else
	{
		KillTimer(1);
		Result = Acm_GmClose(m_GMShand);
		if(Result != SUCCESS)
		{
			strString.Format("Close G Code System fail. error code:%xh",Result);
			MessageBox(strString,"G Code System",MB_OK);
			return;
		}
		m_GMShand= 0;
		m_OpenGMS.SetWindowText("Open GMSystem");
		m_bGMSOpened = FALSE;
	}
}

void CGMSystemDlg::OnBnClickedBtnclose()
{
	// TODO: Add your control notification handler code here
	U32 Result;
	CString strString;

	if(m_bGMSOpened == TRUE)
	{
		Result = Acm_GmClose(m_GMShand);
		if(Result != SUCCESS)
		{
			strString.Format("Close G Code System fail. error code:%xh",Result);
			MessageBox(strString,"G Code System",MB_OK);
			return;
		}
		KillTimer(1);
		m_bGMSOpened = FALSE;
	}
	else
	{

	}
}

void CGMSystemDlg::OnBnClickedBtndownload()
{
	// TODO: Add your control notification handler code here
	U32 Result;
	U32 ErrorRow=0;
	CString strFilePathName;
	CString strString;
	CFile  file; 
	UpdateData( TRUE ) ;
	if(m_GcodeFileName.IsEmpty())
	{
		CFileDialog  fileDlg(FALSE,
			"cfg",
			NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
			"CNC files(*.ncd)|*.ncd",this);		
		if ( fileDlg.DoModal() == IDOK )
		{
			m_GcodeFileName = fileDlg.GetPathName();
		} 
	}
	if(m_GcodeFileName.IsEmpty())
	{
		strString.Format("Download G Code fail.filename is empty");
		MessageBox(strString,"G Code System",MB_OK);
		return;
	}
	//save the file
	file.Open(m_GcodeFileName,CFile::modeWrite|CFile::modeCreate); 
	if(file!=NULL)
	{
		m_GCodeText.GetWindowText(strString);
		//file.WriteString(strString);
		file.Write(strString,strString.GetLength());
		file.Close();
	}
	if (m_uGmSystemState != 2)
	{
		strString.Format("Can not download the job file when the system is not ready;");
		MessageBox(strString,"G Code System",MB_OK);
		return;
	}

	char* pFilePath=(char*)(LPCTSTR)m_GcodeFileName;

	Result = Acm_GmLoadJob(m_GMShand, pFilePath, &ErrorRow);

	if(Result != SUCCESS)
	{
		strString.Format("Down load ncd file error. error code:%xh,error row:%d",Result,ErrorRow);
		MessageBox(strString,"G Code System",MB_OK);
		return;
	}	
}

void CGMSystemDlg::OnBnClickedBtnupload()
{
	// TODO: Add your control notification handler code here
	U32 Result;
	CString strString;

    CString *ss;
	char chGCodeBuffer[9999];
	
	U32 uCodeLength=9999;

	Result = Acm_GmUploadJob(m_GMShand,chGCodeBuffer , &uCodeLength);
	if(Result != SUCCESS)
	{
		if (Result == Gm_NotEnoughMemory)
		{
           //chGCodeBuffer.AllocBuffer(uCodeLength + 1);
		}
		strString.Format("Upload ncd file error. error code:%xh",Result);
		MessageBox(strString,"G Code System",MB_OK);
		return;
	}

 	///Result = Acm_GmUploadJob(m_GMShand,chGCodeBuffer, &uCodeLength);
 //	if(Result != SUCCESS)
 //	{
 //		strString.Format("Upload Gcode error. error code:%xh",Result);
//		MessageBox(strString,"G Code System",MB_OK);
 //		return;
 //	}
	
	m_GCodeText.SetWindowText(chGCodeBuffer);	
	UpdateData(FALSE);
}


void CGMSystemDlg::OnBnClickedBtnopenfile()
{
	// TODO: Add your control notification handler code here
	U32 Result;
	CString strString;
	CString strFilePathName;

	CFileDialog fileDlg(TRUE,
		"cfg",
		NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
		"CNC files(*.ncd)|*.ncd;*.nc",this);

	if (fileDlg.DoModal() == IDOK)
	{
		CStdioFile  file; 

		strFilePathName = fileDlg.GetPathName();
		file.Open(strFilePathName,CFile::modeRead); 

		//file.Read(buffer,1000);
		//m_GCodeText.SetWindowText(buffer);

		//m_GCodeText.Clear();
		m_GCodeText.SetWindowText("");
		//UpdateData(FALSE);
		m_GCodeText.SetSel(-1, 0); 
		while(file.ReadString(strString))
		{	
			strString+="\r\n";			
			m_GCodeText.ReplaceSel(strString);
			//m_GCodeText.SetSel(-1, 0); 
			//m_GCodeText.ReplaceSel("\n");
		}
		m_GcodeFileName = strFilePathName;
		//
	
	}
	UpdateData(FALSE);
}

void CGMSystemDlg::OnBnClickedBtndxf()
{
	// TODO: Add your control notification handler code here
	U32 Result;
	CString strString;

	U32 Length = 0;
	double DXFProportion = 0.2;
	CHAR GCodeText[9999];


	CFileDialog fileDlg(TRUE,
		"cfg",
		NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
		"dxf files(*.dxf)|*.dxf",this);


	if (fileDlg.DoModal() == IDOK)
	{
		CFile file; 
		strString = fileDlg.GetPathName();

		Length = 9999;
		//Result=Acm_GmLoadDXF(m_GMShand, strString.GetBuffer(strString.GetLength()), DXFProportion,GCodeText, &Length);
		Result=Acm_GmDxfToGCodeText(strString.GetBuffer(strString.GetLength()), DXFProportion,GCodeText, &Length);
		if (Result != 0)
		{
			strString.Format("Load DXF failed with ErrorCode:%xh",Result);
			MessageBox(strString,"G Code System",MB_OK);
			return;
		}

		strString = "HF 1200\r\n";              //1200mm/Min 1200/60=20mm/s
		strString = strString + "LF 120\r\n";   //120 mm/Min 120/60=2mm/s
		strString = strString + "F 600\r\n";    //600 mm/Min  10mm/s
		strString = strString + "AC 180000\r\n";//180000mm/Min 180000/60/60=50mm/s 
		strString = strString + "DC 180000\r\n";//180000mm/Min 180000/60/60=50mm/s 
		strString = strString + "G0 X0 Y0\r\n"; //Move to (0,0)
		strString = strString + "G90\r\n";      //Absolute coordinate mode
		strString = strString + "G62\r\n";      //Correct positioning mode
		strString = strString + GCodeText; 
		strString = strString + "M30";
		
		m_GCodeText.SetWindowText(strString);
		m_GcodeFileName ="";
	}
	UpdateData(FALSE);
}

void CGMSystemDlg::OnBnClickedBtnexecute()
{
	// TODO: Add your control notification handler code here
	U32 Result;
	CString strString;

	if(m_bGMSOpened == TRUE)
	{
		Result = Acm_GmCommand(m_GMShand,"XQ");
		if(Result != SUCCESS)
		{
			strString.Format("Execute G Code System failed. error code:%xh",Result);
			MessageBox(strString,"G Code System",MB_OK);
			return;
		}
	}
	else
	{
		strString.Format("Open G Code System first.");
		MessageBox(strString,"G Code System",MB_OK);
		return;
	}
}



void CGMSystemDlg::OnBnClickedBtnstop()
{
	// TODO: Add your control notification handler code here
	U32 Result;
	CString strString;
	if(m_bGMSOpened == TRUE)
	{
		Result = Acm_GmCommand(m_GMShand,"ST");
		if(Result != SUCCESS)
		{
			strString.Format("Execute G Code System fail. error code:%xh",Result);
			MessageBox(strString,"G Code System",MB_OK);
			return;
		}
	}
	else
	{
		strString.Format("Open G Code System fisrt.");
		MessageBox(strString,"G Code System",MB_OK);
		return;
	}
}

void CGMSystemDlg::OnBnClickedSend()
{
	// TODO: Add your control notification handler code here
	U32 Result;
	CString strString;
	UpdateData(TRUE);
	if(m_bGMSOpened == TRUE)
	{
		m_Cmd.GetWindowText (strString);
		Result = Acm_GmCommand(m_GMShand,strString.GetBuffer(strString.GetLength()));
		if(Result != SUCCESS)
		{
			strString.Format("Send G Code Cmd fail. error code:%xh",Result);
			MessageBox(strString,"G Code System",MB_OK);
			return;
		}
	}
}


void CGMSystemDlg::OnBnClickedBtnppoint()
{
	// TODO: Add your control notification handler code here
	//Modal 
	/*CPPointOperation pPointDlg=new CPPointOperation();
	pPointDlg.Create(IDD_DIALOG_PPOINT);
	pPointDlg.DoModal();*/

	//Non_Modal
	CPPointOperation *pPointDlg = new CPPointOperation(NULL,m_GMShand,4);
	pPointDlg->Create(IDD_DIALOG_PPOINT); 
	pPointDlg->ShowWindow(SW_SHOWNORMAL);  
}
