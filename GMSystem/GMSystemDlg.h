// GMSystemDlg.h : header file
//

#pragma once
#include "advmotdrv.h"
#include "General.h"
#include "AdvMotApi.h"
#include "advmotapi.h"
#include "afxwin.h"

// CGMSystemDlg dialog
class CGMSystemDlg : public CDialog
{
// Construction
public:
	CGMSystemDlg(CWnd* pParent = NULL);	// standard constructor

	HAND	m_Devhand;
	DEVLIST	m_avaDevs[MAX_DEVICES];
	HAND	m_Axishand[64];
	USHORT	AxisArray[64];


	double	m_End[64];
	ULONG	m_ulAxisCount;

	BOOL	m_bServoOn;
	DWORD	m_dwDevNum;
	BOOL	m_bInit;

	HAND	m_GMShand;
	BOOL	m_bGMSOpened;
	CString m_GcodeFileName;
	UINT m_CurrentRowCount;
// Dialog Data
	enum { IDD = IDD_GMSYSTEM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnOpen();
public:
	afx_msg void OnBnClickedBtnClose();
public:
	afx_msg void OnBnClickedBtnLoadcfg();
public:
	afx_msg void OnBnClickedBtnServo();
public:
	afx_msg void OnCbnSelchangeAvlaibledevice();
public:
	afx_msg void OnBnClickedBtndownload();
public:
	afx_msg void OnBnClickedBtnupload();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	CComboBox m_AvailableDevice;
public:
	CButton m_btnServoOn;
public:
	afx_msg void OnBnClickedBtnopen();
public:
	afx_msg void OnBnClickedBtnclose();
public:
	afx_msg void OnBnClickedBtndxf();
public:
	afx_msg void OnBnClickedBtnexecute();
public:
	afx_msg void OnBnClickedSend();
public:
	afx_msg void OnBnClickedBtnstop();
public:
	CEdit m_CurrentRow;
public:
	CEdit m_CurrentStatus;
public:
	CEdit m_CurrentCmd;
public:
	CEdit m_GCodeText;
public:
	afx_msg void OnBnClickedBtnopenfile();
public:
	CEdit m_Cmd;
public:
	CButton m_OpenBoard;
public:
	CButton m_OpenGMS;
	unsigned int m_uGmSystemState;
public:
	afx_msg void OnBnClickedPpoint();
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnBnClickedBtnppoint();
};
