#if !defined(AFX_DLGDEBUG_H__04AE02B7_E03F_4BA7_BEA7_FE772218CC6E__INCLUDED_)
#define AFX_DLGDEBUG_H__04AE02B7_E03F_4BA7_BEA7_FE772218CC6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDebug.h : header file
//
#include "DebugView.h"
//#include "2DFilter.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgDebug dialog

class CDlgDebug : public CDialog
{
// Construction
public:
	CDlgDebug(CWnd* pParent = NULL);   // standard constructor

//	C2DFilter* pFilter;

// Dialog Data
	//{{AFX_DATA(CDlgDebug)
	enum { IDD = IDD_DLG_DEBUG };
	CSliderCtrl	m_cSldThresdHold;
	CEdit	m_cEditThreshold;
	CStatic	m_cProcView;
	CStatic	m_cDbgView;
	//}}AFX_DATA
		

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDebug)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDebug)
	virtual BOOL OnInitDialog();
	afx_msg void OnCustomdrawSliderTh(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDEBUG_H__04AE02B7_E03F_4BA7_BEA7_FE772218CC6E__INCLUDED_)
