#if !defined(AFX_DLGREFINFO_H__10B06522_6D42_4FE2_90AF_F566FE211EFE__INCLUDED_)
#define AFX_DLGREFINFO_H__10B06522_6D42_4FE2_90AF_F566FE211EFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRefInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgRefInfo dialog

class CDlgRefInfo : public CDialog
{
// Construction
public:
	CDlgRefInfo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgRefInfo)
	enum { IDD = IDD_DLGREFINFO };
	CRichEditCtrl	m_cRefInfoDis;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRefInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRefInfo)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGREFINFO_H__10B06522_6D42_4FE2_90AF_F566FE211EFE__INCLUDED_)
