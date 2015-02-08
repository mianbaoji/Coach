// CampCBox.cpp : implementation file
//

#include "stdafx.h"
#include "MSLCoach.h"
#include "CampCBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCampCBox

CCampCBox::CCampCBox()
{
	pref = NULL;
}

CCampCBox::~CCampCBox()
{
}


BEGIN_MESSAGE_MAP(CCampCBox, CComboBox)
	//{{AFX_MSG_MAP(CCampCBox)
	ON_CONTROL_REFLECT(CBN_SELENDOK, OnSelendok)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCampCBox message handlers

void CCampCBox::OnSelendok() 
{
	// TODO: Add your control notification handler code here
	int ind = GetCurSel();
	switch(ind)
	{
	case CYAN:
		pDis->strTxt = "Cyan";
		pDis->clrbk = RGB(0,0,255);
		pDis->clrtext = RGB(200,200,255);
		pDis->Invalidate();
		break;

	case MAGENTA:
		pDis->strTxt = "Magenta";
		pDis->clrbk = RGB(255,0,255);
		pDis->clrtext = RGB(0,0,255);
		pDis->Invalidate();
		break;
	}
	SaveCampInfo();

	if (pref != NULL)
	{
		pref->Self = ind;
	}
}

void CCampCBox::OnSelchange() 
{
	// TODO: Add your control notification handler code here

}


void CCampCBox::SaveCampInfo()
{
	CString strValue;
	
	//camp
	int ind = GetCurSel();
	strValue.Format("%d", ind);
	BOOL res = WritePrivateProfileString("Coach Config", "Camp", strValue, "./Coach.ini");
}
