/*************************************************
  http://robot.up-tech.com
  Author: 张万杰      Version: 1.0       Date: 2008/10/5
  Email:  zwj_uptech@126.com
  Description:    // 字符显示类
  Others:         
*************************************************/
#if !defined(AFX_CLRSTR_H__82F6969C_F797_420C_B2FC_FEBC43BE8F1D__INCLUDED_)
#define AFX_CLRSTR_H__82F6969C_F797_420C_B2FC_FEBC43BE8F1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClrStr.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CClrStr window

class CClrStr : public CStatic
{
// Construction
public:
	CClrStr();

// Attributes
public:
	CString strTxt;
	COLORREF clrbk;
	COLORREF clrtext;

protected:
	CBitmap m_bk;
	CFont font; 

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClrStr)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CClrStr();

	// Generated message map functions
protected:
	//{{AFX_MSG(CClrStr)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLRSTR_H__82F6969C_F797_420C_B2FC_FEBC43BE8F1D__INCLUDED_)
