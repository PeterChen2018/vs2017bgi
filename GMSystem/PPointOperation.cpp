// PPointOperation.cpp : implementation file
//

#include "stdafx.h"
#include "GMSystem.h"
#include "PPointOperation.h"


// CPPointOperation dialog

IMPLEMENT_DYNAMIC(CPPointOperation, CDialog)

CPPointOperation::CPPointOperation(CWnd* pParent /*=NULL*/)
	: CDialog(CPPointOperation::IDD, pParent)
	, m_Point_X(0)
	, m_Point_Y(0)
	, m_Point_Z(0)
	, m_Point_R(0)
	, m_PointID(0)
{

}

CPPointOperation::CPPointOperation(CWnd* pParent, HAND GmcHandle, U32 AxisCount)
: CDialog(CPPointOperation::IDD, pParent), m_Point_X(0), m_Point_Y(0), m_Point_Z(0), m_Point_R(0), m_PointID(0)
{
	m_GMShand=GmcHandle;
	m_AxisCount=AxisCount;
}

CPPointOperation::~CPPointOperation()
{
}

void CPPointOperation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Point_X, m_Point_X);
	DDX_Text(pDX, IDC_EDIT_Point_Y, m_Point_Y);
	DDX_Text(pDX, IDC_EDIT_Point_Z, m_Point_Z);
	DDX_Text(pDX, IDC_EDIT_Point_R, m_Point_R);
	DDX_Text(pDX, IDC_EDIT_Point_ID, m_PointID);
	DDV_MinMaxUInt(pDX, m_PointID, 0, 9999);
	DDX_Control(pDX, IDC_LIST_POINT, m_PointList);
	DDX_Control(pDX, IDC_EDIT_Point_X, m_Ctrl_Point_X);
	DDX_Control(pDX, IDC_EDIT_Point_Y, m_Ctrl_Point_Y);
	DDX_Control(pDX, IDC_EDIT_Point_Z, m_Ctrl_Point_Z);
	DDX_Control(pDX, IDC_EDIT_Point_R, m_Ctrl_Point_R);
	DDX_Control(pDX, IDC_EDIT_Point_ID, m_Ctrl_PointID);
}


BEGIN_MESSAGE_MAP(CPPointOperation, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_LoadPoint, &CPPointOperation::OnBnClickedButtonLoadpoint)
	ON_BN_CLICKED(IDC_BUTTON_SavePoint, &CPPointOperation::OnBnClickedButtonSavepoint)
	ON_BN_CLICKED(IDC_BUTTON_ResetPoint, &CPPointOperation::OnBnClickedButtonResetpoint)
	ON_BN_CLICKED(IDC_BUTTON_SetPoint, &CPPointOperation::OnBnClickedButtonSetpoint)
END_MESSAGE_MAP()


// CPPointOperation message handlers

BOOL CPPointOperation::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_PointList.InsertColumn(0,"Point");
	m_PointList.InsertColumn(1,"Value");
	m_PointList.SetColumnWidth(0,60);
	m_PointList.SetColumnWidth(1,250);

	return TRUE;
}

void CPPointOperation::OnBnClickedButtonLoadpoint()
{
	// TODO: Add your control notification handler code here
	U32 Result;
	CString strString;

	CFileDialog  fileDialog(TRUE,
		".csv",
		NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
		"Text file(.csv)|*.csv||",this);
	if (fileDialog.DoModal() != IDOK)
		return;

	strString= fileDialog.GetPathName();

	Result=Acm_GmLoadPoint(m_GMShand,strString.GetBuffer(strString.GetLength()));
	if(Result != SUCCESS)
	{
		strString.Format("Load point Failed with Error Code:%xh",Result);
		MessageBox(strString,"G Code System",MB_OK);
		return;
	}	

	RefreshPpointValues();
}

void CPPointOperation::OnBnClickedButtonSavepoint()
{
	// TODO: Add your control notification handler code here
	U32 Result;
	CString strString;

	CFileDialog  fileDialog(FALSE,
		".csv",
		NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"Text file(.csv)|*.csv||",this);
	if (fileDialog.DoModal() != IDOK)
		return;

	strString= fileDialog.GetPathName();

	Result=Acm_GmSavePoint(m_GMShand,strString.GetBuffer(strString.GetLength()));
	if(Result != SUCCESS)
	{
		strString.Format("Save point Failed with Error Code:%xh",Result);
		MessageBox(strString,"G Code System",MB_OK);
		return;
	}	
}

void CPPointOperation::OnBnClickedButtonResetpoint()
{
	// TODO: Add your control notification handler code here
	U32 Result;
	CString strString;

	Result=Acm_GmResetPoint(m_GMShand);
	if(Result != SUCCESS)
	{
		strString.Format("Reset point Failed with Error Code:%xh",Result);
		MessageBox(strString,"G Code System",MB_OK);
		return;
	}	

	RefreshPpointValues();
}

void CPPointOperation::OnBnClickedButtonSetpoint()
{
	// TODO: Add your control notification handler code here
	U32 Result;
	CString strString;
	double PointArray[4];
	U32 uPointID=0;
	double dPointX,dPointY,dPointZ,dPointR=0;

	UpdateData(TRUE);
	
	m_Ctrl_Point_X.GetWindowText(strString);
	dPointX=atof(strString);

	m_Ctrl_Point_Y.GetWindowText(strString);
	dPointY=atof(strString);

	m_Ctrl_Point_Z.GetWindowText(strString);
	dPointZ=atof(strString);

	m_Ctrl_Point_R.GetWindowText(strString);
	dPointR=atof(strString);

	m_Ctrl_PointID.GetWindowText(strString);
	uPointID=atol(strString);

	PointArray[0]=dPointX;
	PointArray[1]=dPointY;
	PointArray[2]=dPointZ;
	PointArray[3]=dPointR;

	Result=Acm_GmSetPoint(m_GMShand,uPointID,PointArray);
	if(Result != SUCCESS)
	{
		strString.Format("Set point Failed with Error Code:%xh",Result);
		MessageBox(strString,"G Code System",MB_OK);
		return;
	}	

	RefreshPpointValues();
}

void CPPointOperation::RefreshPpointValues()
{
	U32 Result;
	double PointArray[4];
	CString strPointIndex = "";
	CString strPointValue = "";
	CString strPoint="";
	U32 ItemIndex=0;

	m_PointList.DeleteAllItems();
	for(U32 PointID=0;PointID<10000;PointID++)
	{
		Result = Acm_GmGetPoint(m_GMShand,PointID, PointArray);
		if(Result==SUCCESS)
		{
			strPointIndex.Format(_T("P%d"),PointID);
			strPointValue="";
			for(U32 i=0;i<m_AxisCount;i++)
			{
				strPoint.Format("%lf",PointArray[i]);
				strPointValue+=strPoint+CString("  ");
			}
			m_PointList.InsertItem(ItemIndex,"Point");
			m_PointList.SetItemText(ItemIndex,0,strPointIndex);
			m_PointList.SetItemText(ItemIndex,1,strPointValue);
			ItemIndex++;
		}
	}
}

