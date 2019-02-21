#if !defined(AFX_ETHCATDO_H__2AF8BAB9_55EF_4CC4_B288_A9FE82A3A414__INCLUDED_)
#define AFX_ETHCATDO_H__2AF8BAB9_55EF_4CC4_B288_A9FE82A3A414__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EthcatDo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEthcatDo dialog
#include "EthcatDoSetUp.h"

class CEthcatDo : public CDialog
{
	// Construction
public:
	void RefreshDOValue(bool IsInit);
    UCHAR m_bitData[128];
	CEthcatDoSetUp m_ethcatdosetup;
	void RefreshDOValue();
	void InitParam(ULONG);
	ULONG m_devhand;
	CEdit* newMyEdit(int,CRect);
	void OnButtonDoset();
	CButton* NewMyButton(int nID,CRect rect,int nStyle);
	ULONG m_DONumber;
	CEthcatDo(CWnd* pParent = NULL);   // standard constructor
	//	CButton *p_myDObtn;
	CEdit *p_myDOEdit[16];
	CEdit *p_myDOOutEdit[16];
    CButton *p_mybtn[128];
	CButton *pbtn;
	HBITMAP hBitmapRed;
	HBITMAP hBitmapGray;
	// Dialog Data
	//{{AFX_DATA(CEthcatDo)
	enum { IDD = IDD_ETHCATDO_DIALOG };
	CButton	m_dosetup;
	CStatic	m_lblzero;
	CStatic	m_lblthree;
	CStatic	m_lblport;
	CStatic	m_lblhex;
	CStatic	m_lblfour;
	CStatic	m_lblbit;
	//}}AFX_DATA
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEthcatDo)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CEthcatDo)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonDosetup();
	afx_msg void OnButtonDoSet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETHCATDO_H__2AF8BAB9_55EF_4CC4_B288_A9FE82A3A414__INCLUDED_)
