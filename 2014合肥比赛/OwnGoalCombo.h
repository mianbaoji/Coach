#if !defined(AFX_OWNGOALCOMBO_H__AFB535D2_CED2_4F92_B092_F6BAC17451FA__INCLUDED_)
#define AFX_OWNGOALCOMBO_H__AFB535D2_CED2_4F92_B092_F6BAC17451FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OwnGoalCombo.h : header file
//
#include "ClrStr.h"
#include "UPFullField.h"

/////////////////////////////////////////////////////////////////////////////
// COwnGoalCombo window

class COwnGoalCombo : public CComboBox
{
// Construction
public:
	COwnGoalCombo();

// Attributes
public:
	CClrStr* pDis;
	CUPFullField* pfield;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COwnGoalCombo)
	//}}AFX_VIRTUAL

// Implementation
public:
	void ChangeGoal();
	virtual ~COwnGoalCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(COwnGoalCombo)
	afx_msg void OnSelendok();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OWNGOALCOMBO_H__AFB535D2_CED2_4F92_B092_F6BAC17451FA__INCLUDED_)
