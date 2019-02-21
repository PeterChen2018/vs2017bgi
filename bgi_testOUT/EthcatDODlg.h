// AIODlg.h : header file
//


#if !defined(AFX_AIODLG_H__D5EC82A6_1F5C_4A3B_A183_350240F510EC__INCLUDED_)
#define AFX_AIODLG_H__D5EC82A6_1F5C_4A3B_A183_350240F510EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "AdvMotDrv.h"
#include "General.h"
#include "AdvMotApi.h"
#include<windows.h>
#include<winver.h>
#ifndef ETHAN_CAT_AO
#define ETHAN_CAT_AO
#include "EthcatDoValue.h"
#endif  

/////////////////////////////////////////////////////////////////////////////
// CAIODlg dialog

class CAIODlg : public CDialog
{
// Construction
public:
	CEthcatDo *m_ethcatdo;
	CAIODlg(CWnd* pParent = NULL);	// standard constructor  
	HAND	m_Devhand;
	DEVLIST	m_avaDevs[MAX_DEVICES];
    void ShowMessages(CString DetailMessage,U32 errorCode);
	void CloseBoardOrForm();
	U32  GetDevNum(U32 DevType, U32 BoardID, U32 MasterRingNo, U32 SlaveBoardID);
	ULONG GetDevCfgDllDrvVer( PVOID Value, ULONG *Length); //Get ADVMOT.dll driver version number
// Dialog Data
	//{{AFX_DATA(CAIODlg)
	enum { IDD = IDD_DO_DIALOG };
	CTabCtrl	m_tabaio;
	CComboBox	m_AvailableDevice;
	DWORD m_dwDevNum;
	BOOL m_bInit;
	BOOL m_isStart;
	bool IsInit;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAIODlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAIODlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnButton2();
	afx_msg void OnSelchangeAvlaibledevice();
	afx_msg void OnButtonAiset();
	afx_msg void OnBtnOpenBoard();
	afx_msg void OnBtnCloseBoard();
	afx_msg void OnSelchangeTabAio(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AIODLG_H__D5EC82A6_1F5C_4A3B_A183_350240F510EC__INCLUDED_)
