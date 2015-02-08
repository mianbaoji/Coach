// OwnGoalCombo.cpp : implementation file
//

#include "stdafx.h"
#include "MSLCoach.h"
#include "OwnGoalCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COwnGoalCombo

COwnGoalCombo::COwnGoalCombo()
{
}

COwnGoalCombo::~COwnGoalCombo()
{
}


BEGIN_MESSAGE_MAP(COwnGoalCombo, CComboBox)
	//{{AFX_MSG_MAP(COwnGoalCombo)
	ON_CONTROL_REFLECT(CBN_SELENDOK, OnSelendok)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COwnGoalCombo message handlers

void COwnGoalCombo::OnSelendok() 
{
	// TODO: Add your control notification handler code here
	int index = GetCurSel();
	switch(index)
	{
	case 0:
		pDis->strTxt = "Blue";
		pDis->clrbk = RGB(0,0,255);
		pDis->clrtext = RGB(200,200,255);
		pDis->Invalidate();
		break;

	case 1:
		pDis->strTxt = "Yellow";
		pDis->clrbk = RGB(255,255,0);
		pDis->clrtext = RGB(0,0,255);
		pDis->Invalidate();
		break;
	}
	if (pfield != NULL)
	{
		pfield->ChangeOwnGoal(index);
	}
}

void COwnGoalCombo::ChangeGoal()
{
	int curindex = GetCurSel();
	switch(curindex)
	{
	case 0:
		SetCurSel(1);
		break;

	case 1:
		SetCurSel(0);
		break;
	}
	OnSelendok();
}
