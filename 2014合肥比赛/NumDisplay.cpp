// NumDisplay.cpp : implementation file
//

#include "stdafx.h"
#include "MSLCoach.h"
#include "NumDisplay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNumDisplay

CNumDisplay::CNumDisplay()
{
	Number = 0;
	VERIFY(font.CreateFont( 
		120, // nHeight 
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
}

CNumDisplay::~CNumDisplay()
{
}


BEGIN_MESSAGE_MAP(CNumDisplay, CStatic)
	//{{AFX_MSG_MAP(CNumDisplay)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNumDisplay message handlers

void CNumDisplay::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	m_bk.CreateCompatibleBitmap(&dc,100,100);

	dc.SetBkMode(TRANSPARENT);

	dc.SelectObject(&m_bk);

		CRect rc;
		GetClientRect(rc);
		CBrush bkcolor(RGB(245,245,245));
		dc.FillRect(rc,&bkcolor);
		
	if (Number < 1 )
	{
		/*CRect rc;
		GetClientRect(rc);
		CBrush bkcolor(RGB(245,245,245));
		dc.FillRect(rc,&bkcolor);*/
	}
	else
	{
		tstr.Format("%d",Number);

		CFont* def_font = dc.SelectObject(&font); 

		dc.TextOut(0,0,tstr);
	
		dc.SelectObject(def_font); 
	}

	m_bk.DeleteObject();
	
	// Do not call CStatic::OnPaint() for painting messages
}
