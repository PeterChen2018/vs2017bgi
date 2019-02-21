#if !defined(AFX_ETHCATDOSETUP_H__178EED50_DE9C_4A04_BDFF_B41455811988__INCLUDED_)
#define AFX_ETHCATDOSETUP_H__178EED50_DE9C_4A04_BDFF_B41455811988__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EthcatDoSetUp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEthcatDoSetUp dialog
class CEthcatDo;
class CEthcatDoSetUp : public CDialog
{
// Construction
public:
	bool IsApply;
	void IsInit();
	void InitParam(ULONG,ULONG);
	ULONG m_devhand;
	void RefreshDOSetup(bool isInitial);
	CEthcatDoSetUp(CWnd* pParent = NULL);   // standard constructor
    UINT m_DOChanNum;
// Dialog Data
	//{{AFX_DATA(CEthcatDoSetUp)
	enum { IDD = IDD_ETHCATDOSETUP_DIALOG };
	CListCtrl	m_listdo;
	CComboBox	m_cmbchanindex;
	BOOL	m_checkfsv;
	BOOL	m_checkallfollowch0;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEthcatDoSetUp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEthcatDoSetUp)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonApply();
	afx_msg void OnSelchangeComboChanindex();
	afx_msg void OnClickListDo(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETHCATDOSETUP_H__178EED50_DE9C_4A04_BDFF_B41455811988__INCLUDED_)
