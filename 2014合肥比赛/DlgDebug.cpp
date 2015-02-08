// DlgDebug.cpp : implementation file
//

#include "stdafx.h"
#include "MSLCoach.h"
#include "DlgDebug.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDebug dialog


CDlgDebug::CDlgDebug(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDebug::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDebug)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
//	pFilter = NULL;
}


void CDlgDebug::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDebug)
	DDX_Control(pDX, IDC_SLIDER_TH, m_cSldThresdHold);
	DDX_Control(pDX, IDC_EDIT_TH, m_cEditThreshold);
	DDX_Control(pDX, IDC_PROCVIEW, m_cProcView);
	DDX_Control(pDX, IDC_VIEW, m_cDbgView);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDebug, CDialog)
	//{{AFX_MSG_MAP(CDlgDebug)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_TH, OnCustomdrawSliderTh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDebug message handlers

BOOL CDlgDebug::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_cSldThresdHold.SetRangeMax(255);
	m_cSldThresdHold.SetRangeMin(1);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDebug::OnCustomdrawSliderTh(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
// 	if (pFilter != NULL)
// 	{
// 		pFilter->m_nThreshold = m_cSldThresdHold.GetPos();
// 	}
	CString strTxt;
	strTxt.Format("%d",m_cSldThresdHold.GetPos());
	m_cEditThreshold.SetWindowText(strTxt);
	*pResult = 0;
}
