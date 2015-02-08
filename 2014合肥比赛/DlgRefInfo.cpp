// DlgRefInfo.cpp : implementation file
//

#include "stdafx.h"
#include "MSLCoach.h"
#include "DlgRefInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRefInfo dialog


CDlgRefInfo::CDlgRefInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRefInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRefInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgRefInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRefInfo)
	DDX_Control(pDX, IDC_REFINFODIS, m_cRefInfoDis);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRefInfo, CDialog)
	//{{AFX_MSG_MAP(CDlgRefInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRefInfo message handlers

BOOL CDlgRefInfo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	AfxInitRichEdit();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
