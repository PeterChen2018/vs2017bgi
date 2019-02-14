#pragma once
#include "AdvMotApi.h"
#include "afxcmn.h"
#include "afxwin.h"

// CPPointOperation dialog

class CPPointOperation : public CDialog
{
	DECLARE_DYNAMIC(CPPointOperation)
private:
	void RefreshPpointValues();
public:
	CPPointOperation(CWnd* pParent = NULL);   // standard constructor
	CPPointOperation(CWnd* pParent, HAND GmcHandle,U32 AxisCount);
	virtual ~CPPointOperation();

	HAND	m_GMShand;
	U32 m_AxisCount;

// Dialog Data
	enum { IDD = IDD_DIALOG_PPOINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonLoadpoint();
public:
	afx_msg void OnBnClickedButtonSavepoint();
public:
	afx_msg void OnBnClickedButtonResetpoint();
public:
	afx_msg void OnBnClickedButtonSetpoint();
public:
	double m_Point_X;
public:
	double m_Point_Y;
public:
	double m_Point_Z;
public:
	double m_Point_R;
public:
	UINT m_PointID;
public:
	CListCtrl m_PointList;
public:
	CEdit m_Ctrl_Point_X;
public:
	CEdit m_Ctrl_Point_Y;
public:
	CEdit m_Ctrl_Point_Z;
public:
	CEdit m_Ctrl_Point_R;
public:
	CEdit m_Ctrl_PointID;
};
