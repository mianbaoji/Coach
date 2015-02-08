// ColorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MSLCoach.h"
#include "ColorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorDlg dialog


CColorDlg::CColorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CColorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_cCampBox.pDis = &m_cCampDis;
	m_nName = 0;
}


void CColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorDlg)
	DDX_Control(pDX, IDC_CAMPDIS, m_cCampDis);
	DDX_Control(pDX, IDC_CAMPCOMBO, m_cCampBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColorDlg, CDialog)
	//{{AFX_MSG_MAP(CColorDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorDlg message handlers

void CColorDlg::OnOK() 
{
	// TODO: Add extra validation here
	m_nName = m_cCampBox.GetCurSel();
	CDialog::OnOK();
}

BOOL CColorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_cCampBox.SetCurSel(m_nName);
	
	int ind = m_cCampBox.GetCurSel();
	switch(ind)
	{
	case CYAN:		
		m_cCampDis.strTxt = "Cyan";
		m_cCampDis.clrbk = RGB(0,0,255);
		m_cCampDis.clrtext = RGB(200,200,255);
		m_cCampDis.Invalidate();
		break;
		
	case MAGENTA:
		m_cCampDis.strTxt = "Magenta";
		m_cCampDis.clrbk = RGB(255,0,255);
		m_cCampDis.clrtext = RGB(0,0,255);
		m_cCampDis.Invalidate();
		break;
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
