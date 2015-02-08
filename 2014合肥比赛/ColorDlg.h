#if !defined(AFX_COLORDLG_H__899B9FEA_AADF_419B_A8A7_029F70CFC43D__INCLUDED_)
#define AFX_COLORDLG_H__899B9FEA_AADF_419B_A8A7_029F70CFC43D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "CampCBox.h"
#include "ClrStr.h"
// ColorDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorDlg dialog

class CColorDlg : public CDialog
{
// Construction
public:
	CColorDlg(CWnd* pParent = NULL);   // standard constructor
	UINT m_nName;

// Dialog Data
	//{{AFX_DATA(CColorDlg)
	enum { IDD = IDD_COLORDLG };
	CClrStr	m_cCampDis;
	CCampCBox	m_cCampBox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CColorDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORDLG_H__899B9FEA_AADF_419B_A8A7_029F70CFC43D__INCLUDED_)
