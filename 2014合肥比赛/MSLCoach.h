// MSLCoach.h : main header file for the MSLCOACH application
//

#if !defined(AFX_MSLCOACH_H__9D755D70_759D_4645_920A_5DEB9D7BF0B2__INCLUDED_)
#define AFX_MSLCOACH_H__9D755D70_759D_4645_920A_5DEB9D7BF0B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMSLCoachApp:
// See MSLCoach.cpp for the implementation of this class
//

class CMSLCoachApp : public CWinApp
{
public:
	CMSLCoachApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMSLCoachApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMSLCoachApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSLCOACH_H__9D755D70_759D_4645_920A_5DEB9D7BF0B2__INCLUDED_)
