// AIO.h : main header file for the AIO application
//

#if !defined(AFX_AIO_H__7B8F2A44_D759_4BC3_8000_66DBD96C7B69__INCLUDED_)
#define AFX_AIO_H__7B8F2A44_D759_4BC3_8000_66DBD96C7B69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAIOApp:
// See AIO.cpp for the implementation of this class
//

class CAIOApp : public CWinApp
{
public:
	CAIOApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAIOApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAIOApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AIO_H__7B8F2A44_D759_4BC3_8000_66DBD96C7B69__INCLUDED_)
