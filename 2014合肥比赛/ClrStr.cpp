// ClrStr.cpp : implementation file
//

#include "stdafx.h"
#include "MSLCoach.h"
#include "ClrStr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClrStr

CClrStr::CClrStr()
{
	VERIFY(font.CreateFont( 
		32, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_NORMAL, // nWeight 
		FALSE, // bItalic 
		FALSE, // bUnderline 
		0, // cStrikeOut 
		ANSI_CHARSET, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		CLIP_DEFAULT_PRECIS, // nClipPrecision 
		DEFAULT_QUALITY, // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		"Arial")); // lpszFacename
	
	strTxt.Format("Magenta");
	clrbk = RGB(255,0,255);
	clrtext = RGB(200,0,100);
}

CClrStr::~CClrStr()
{
}


BEGIN_MESSAGE_MAP(CClrStr, CStatic)
	//{{AFX_MSG_MAP(CClrStr)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClrStr message handlers

void CClrStr::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	m_bk.CreateCompatibleBitmap(&dc,100,100);

	dc.SetBkMode(TRANSPARENT);

	dc.SelectObject(&m_bk);

		CRect rc;
		GetClientRect(rc);
		CBrush bkcolor(clrbk);
		dc.FillRect(rc,&bkcolor);

		CFont* def_font = dc.SelectObject(&font); 
		dc.SetTextColor(clrtext);

		dc.TextOut(2,2,strTxt);
	
		dc.SelectObject(def_font); 

		m_bk.DeleteObject();
	// Do not call CStatic::OnPaint() for painting messages
}
