#if !defined(AFX_CAMPCBOX_H__048373D2_7560_4FED_BB79_6BFB5B05E7F0__INCLUDED_)
#define AFX_CAMPCBOX_H__048373D2_7560_4FED_BB79_6BFB5B05E7F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CampCBox.h : header file
//
#include "ClrStr.h"
#include "RefClient.h"

/////////////////////////////////////////////////////////////////////////////
// CCampCBox window

class CCampCBox : public CComboBox
{
// Construction
public:
	CCampCBox();

// Attributes
public:	
	int iIconX, iIconY;		//SMALL ICONµÄ´óÐ¡³ß´ç
	CClrStr* pDis;
	CRefClient* pref;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCampCBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SaveCampInfo();
	virtual ~CCampCBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCampCBox)
	afx_msg void OnSelendok();
	afx_msg void OnSelchange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAMPCBOX_H__048373D2_7560_4FED_BB79_6BFB5B05E7F0__INCLUDED_)
