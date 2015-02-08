// MSLCoachDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MSLCoach.h"
#include "MSLCoachDlg.h"
#include "MSL_Protocol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef struct  
{	
	char RefBoxIP[16];
	UINT Name;
	AgCnt CreditCnt[5];
}CoachSet;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMSLCoachDlg dialog

CMSLCoachDlg::CMSLCoachDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMSLCoachDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMSLCoachDlg)
	m_strRefIP = _T("192.168.1.200");
	m_nRefPort = 28097;
	m_nDefAng = 0;
	m_fSelAngle = 0.0;
	m_nDefDistance = 200;
	m_nDistOfMultiDef = 250;
	m_fKField = 1.0f;
	m_bObst_Org = FALSE;
	m_nConf1_atk = 2;
	m_nConf1_def = 3;
	m_nConf2_atk1 = 2;
	m_nConf2_atk2 = 3;
	m_nConf1_DefAngle = 0;
	m_nConf1_DefDist = 150;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMSLCoachDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMSLCoachDlg)
	DDX_Control(pDX, IDC_PASSANDCATCH, m_cDisPassAndCatch);
	DDX_Control(pDX, IDC_TACLIST, m_cTacList);
	DDX_Control(pDX, IDC_CHECK_IPFILTER, m_cChkIpFilter);
	DDX_Control(pDX, IDC_CREDITLIST, m_cCreditList);
	DDX_Control(pDX, IDC_CNTLIST, m_cCntList);
	DDX_Control(pDX, IDC_AGINFOLIST, m_cAlist);
	DDX_Control(pDX, IDC_CAMPDIS, m_cCampDis);
	DDX_Control(pDX, IDC_CAMPCOMBO, m_cCamp);
	DDX_Control(pDX, IDC_SELNUM, m_cSelNum);
	DDX_Control(pDX, IDC_SENDLIST, m_cSendList);
	DDX_Control(pDX, IDC_REFLIST, m_cRefList);
	DDX_Control(pDX, IDC_FULLFIELD, m_cFullField);
	DDX_Text(pDX, IDC_REFIP, m_strRefIP);
	DDX_Text(pDX, IDC_REFPORT, m_nRefPort);
	DDX_Text(pDX, IDC_DEFANG, m_nDefAng);

	DDX_Text(pDX, IDC_midpassx, test_midpass_x);
	DDX_Text(pDX, IDC_midpassy, test_midpass_y);

	DDX_Text(pDX, IDC_MOVETOANGLE, m_fSelAngle);
	DDX_Text(pDX, IDC_DEFDIST, m_nDefDistance);
	DDX_Text(pDX, IDC_DISTMDEF, m_nDistOfMultiDef);
	DDX_Text(pDX, IDC_KFIELD, m_fKField);
	DDX_Check(pDX, IDC_CHECK_OBST_ORG, m_bObst_Org);
	DDX_Text(pDX, IDC_CONF1_ATK, m_nConf1_atk);
	DDX_Text(pDX, IDC_CONF1_DEF, m_nConf1_def);
	DDX_Text(pDX, IDC_CONF2_ATK1, m_nConf2_atk1);
	DDX_Text(pDX, IDC_CONF2_ATK2, m_nConf2_atk2);
	DDX_Text(pDX, IDC_CONF1_DEFANG, m_nConf1_DefAngle);
	DDX_Text(pDX, IDC_CONF1_DEFDIST, m_nConf1_DefDist);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMSLCoachDlg, CDialog)
	//{{AFX_MSG_MAP(CMSLCoachDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECTREF, OnConnectref)
	ON_BN_CLICKED(IDC_AGENTATTACK, OnAgentattack)
	ON_BN_CLICKED(IDC_AGENTSTOP, OnAgentstop)
	ON_BN_CLICKED(IDC_AGKICKOFF, OnAgkickoff)
	ON_BN_CLICKED(IDC_REFKICKOFF, OnRefkickoff)
	ON_BN_CLICKED(IDC_KO_START, OnKoStart)
	ON_BN_CLICKED(IDC_AGGOALKEEP, OnAggoalkeep)
	ON_BN_CLICKED(IDC_AGKO_SL, OnAgkoSl)
	ON_BN_CLICKED(IDC_AGKO_SL_START, OnAgkoSlStart)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_C_FK, OnCFk)
	ON_BN_CLICKED(IDC_C_GK, OnCGk)
	ON_BN_CLICKED(IDC_C_TI, OnCTi)
	ON_BN_CLICKED(IDC_C_P, OnCP)
	ON_BN_CLICKED(IDC_C_CK, OnCCk)
	ON_BN_CLICKED(IDC_M_KO, OnMKo)
	ON_BN_CLICKED(IDC_M_FK, OnMFk)
	ON_BN_CLICKED(IDC_M_GK, OnMGk)
	ON_BN_CLICKED(IDC_M_TI, OnMTi)
	ON_BN_CLICKED(IDC_M_P, OnMP)
	ON_BN_CLICKED(IDC_M_CK, OnMCk)
	ON_BN_CLICKED(IDC_REF_DB, OnRefDb)
	ON_BN_CLICKED(IDC_REF_START, OnRefStart)
	ON_BN_CLICKED(IDC_REF_STOP, OnRefStop)
	ON_BN_CLICKED(IDC_DEFENCE, OnDefence)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_PRODEF, OnProdef)
	ON_BN_CLICKED(IDC_ATTENTION, OnAttention)
	ON_BN_CLICKED(IDC_UPDATEANG, OnUpdateang)
	ON_BN_CLICKED(IDC_ASEARCHBALL, OnAsearchball)
	ON_BN_CLICKED(IDC_AGKICKOUT, OnAgkickout)
	ON_BN_CLICKED(IDC_MULTIDEF, OnMultidef)
	ON_BN_CLICKED(IDC_MULTIATK, OnMultiatk)
	ON_BN_CLICKED(IDC_MULTITEST, OnMultitest)
	ON_BN_CLICKED(IDC_READY, OnReady)
	ON_BN_CLICKED(IDC_SCANAGENT, OnScanagent)
	ON_BN_CLICKED(IDC_CHANGEGOAL, OnChangegoal)
	ON_BN_CLICKED(IDC_HFLIP, OnHflip)
	ON_BN_CLICKED(IDC_VFLIP, OnVflip)
	ON_BN_CLICKED(IDC_EXPAND, OnExpand)
	ON_BN_CLICKED(IDC_ADDALLTOCREDIT, OnAddalltocredit)
	ON_BN_CLICKED(IDC_MOVETOTEST, OnMovetotest)
	ON_BN_CLICKED(IDC_CLEARALLCREDIT, OnClearallcredit)
	ON_BN_CLICKED(IDC_CLEARCREDIT, OnClearcredit)
	ON_NOTIFY(NM_CLICK, IDC_CREDITLIST, OnClickCreditlist)
	ON_BN_CLICKED(IDC_ADDTOCREDIT, OnAddtocredit)
	ON_BN_CLICKED(IDC_CHECK_IPFILTER, OnCheckIpfilter)
	ON_BN_CLICKED(IDC_UPDATEK, OnUpdatek)
	ON_BN_CLICKED(IDC_PR_OUT, OnPrOut)
	ON_BN_CLICKED(IDC_PR_READY, OnPrReady)
	ON_BN_CLICKED(IDC_SETPLAYERINPONT, OnSetplayerinpont)
	ON_BN_CLICKED(IDC_CHECK_OBST_ORG, OnCheckObstOrg)
	//ON_MESSAGE(WM_DLGDBGCLOSED, OnDlgDbgClosing)
	ON_BN_CLICKED(IDC_DISDLGDBG, OnDisdlgdbg)
	ON_BN_CLICKED(IDC_TCDEMO, OnTcdemo)
	ON_BN_CLICKED(IDC_AG_CATCH, OnAgCatch)
	ON_BN_CLICKED(IDC_COVERTHEBALL, OnCovertheball)
	ON_BN_CLICKED(IDC_DISCONNECTREF, OnDisconnectref)
	ON_BN_CLICKED(IDC_OBSTCONFIG, OnObstconfig)
	ON_BN_CLICKED(IDC_ALLFLIP, OnAllflip)
	ON_BN_CLICKED(IDC_CONF1, OnConf1)
	ON_BN_CLICKED(IDC_CONF2, OnConf2)
	ON_BN_CLICKED(IDC_OUTFIELD, OnOutfield)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_TCDEMO2, &CMSLCoachDlg::OnBnClickedTcdemo2)
	ON_BN_CLICKED(IDC_TCDEMO3, &CMSLCoachDlg::OnBnClickedTcdemo3)
	ON_STN_CLICKED(IDC_FULLFIELD, &CMSLCoachDlg::OnStnClickedFullfield)
	ON_EN_CHANGE(IDC_CONF1_DEFANG, &CMSLCoachDlg::OnEnChangeConf1Defang)
	ON_EN_CHANGE(IDC_midpassy, &CMSLCoachDlg::OnEnChangemidpassy)
	ON_LBN_SELCHANGE(IDC_SENDLIST, &CMSLCoachDlg::OnLbnSelchangeSendlist)
	ON_STN_CLICKED(IDC_PASSANDCATCH, &CMSLCoachDlg::OnStnClickedPassandcatch)
	ON_BN_CLICKED(IDC_BUTTON3, &CMSLCoachDlg::OnAgentDefend)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMSLCoachDlg message handlers

BOOL CMSLCoachDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	

	//ref 
	//m_refclient.m_pShowList = &m_cRefList;
	m_refclient.pfield = &m_cFullField;
	//m_refclient.pGoalCombo = &m_cOwnGoalCombo;

	//game
	m_cFullField.pSndList = &m_cSendList;
	m_cFullField.pAgInfo = &m_cAlist;

	//将决策对象里的对队员发送指令指针指向实际通讯对象
	for (int i=0;i<7;i++)
	{
		m_cFullField.pToAgent[i] = &m_CoachUdp.m_ToAgent[i];
	}
	//CoachUDP 与队员通讯
	m_CoachUdp.pfield = &m_cFullField;
	m_CoachUdp.Listen(20090);

	//获取控件指针，选中目标时显示队员号码
	m_cFullField.pNum = &m_cSelNum;
		
	//////////////////////////////////////////////////////////////////////////
	//队员列表显示
	m_cAlist.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	CString strname[8];
	strname[0]="ID";
	strname[1]="STATUS";
	strname[2]="X";
	strname[3]="Y";
	strname[4]="BALL";
	strname[5]="reply";
	strname[6]="Right";

	/*for(int i=0;i<5;i++)
	{
		m_cAlist.InsertColumn(i,strname[i],LVCFMT_LEFT,70);
	}*/
	
	m_cAlist.InsertColumn(0,strname[0],LVCFMT_LEFT,30);
	m_cAlist.InsertColumn(1,strname[1],LVCFMT_LEFT,100);
	m_cAlist.InsertColumn(2,strname[2],LVCFMT_LEFT,50);
	m_cAlist.InsertColumn(3,strname[3],LVCFMT_LEFT,50);
	m_cAlist.InsertColumn(4,strname[4],LVCFMT_LEFT,100);
	m_cAlist.InsertColumn(5,strname[5],LVCFMT_LEFT,50);
	m_cAlist.InsertColumn(6,strname[6],LVCFMT_LEFT,80);

	//初始化队员列表
	CString str;
	for (i=1;i<7;i++)
	{
		str.Format("%d",i);
		m_cAlist.InsertItem(i-1,str);
		//show id
		m_cAlist.SetItemText(i,0,str);

		m_cAlist.SetItemText(i-1,1,"OFFLINE");
	}
	//////////////////////////////////////////////////////////////////////////
	//连接列表初始化
	m_cCntList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	strname[0]="IP";
	strname[1]="ID";
	strname[2]="PackCnt";
	
	m_cCntList.InsertColumn(0,strname[0],LVCFMT_LEFT,100);
	m_cCntList.InsertColumn(1,strname[1],LVCFMT_LEFT,30);
	m_cCntList.InsertColumn(2,strname[2],LVCFMT_LEFT,60);
// 	for (i=1;i<3;i++)
// 	{
// 		m_cCntList.InsertItem(i-1," ");
//	}
	m_CoachUdp.m_pCntList = &m_cCntList;

	//////////////////////////////////////////////////////////////////////////
	//连接列表初始化
	m_cCreditList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	strname[0]="IP";
	
	m_cCreditList.InsertColumn(0,strname[0],LVCFMT_LEFT,100);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//策略列表初始化
	m_cTacList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	strname[0]="TID";
	strname[1]="Ctrl";
	strname[2]="AgentID";
	
	m_cTacList.InsertColumn(0,strname[0],LVCFMT_LEFT,30);
	m_cTacList.InsertColumn(1,strname[1],LVCFMT_LEFT,60);
	m_cTacList.InsertColumn(2,strname[2],LVCFMT_LEFT,60);

	for (i=0;i<AG_NUM;i++)
	{		
		m_cTacList.InsertItem(i,"");
	}

	m_cFullField.pTacInfo = &(m_cTacList);
	//////////////////////////////////////////////////////////////////////////

	//timer
	SetTimer(EXETIMER,100,NULL);

	//refinfo dlg
	m_pDlgRef = new CDlgRefInfo;
	//m_pDlgRef->m_hParent = this->GetSafeHwnd();	
	m_pDlgRef->Create(IDD_DLGREFINFO,this);	//创建非模态对话框
	//m_dlgView->CenterWindow(this);			//居中显示
	m_pDlgRef->ShowWindow(SW_HIDE);		//显示非模态对话框

	//expand flag
// 	m_bExpandRight = TRUE;
// 	m_ExpandDialog();
// 	if (m_bExpandRight == TRUE)
// 	{
// 		GetDlgItem(IDC_EXPAND)->SetWindowText("收起<<");
// 	} 
// 	else
// 	{
// 		GetDlgItem(IDC_EXPAND)->SetWindowText("展开>>");
// 	}

	//ref ip 
	LoadRefInfo();
	m_nCreditSel = 5;

	//credit
	if (m_CoachUdp.m_bEnIPFilter == true)
	{
		m_cChkIpFilter.SetCheck(1);
	} 
	else
	{		
		m_cChkIpFilter.SetCheck(0);
	}

	//field k
	m_fKField = m_cFullField.m_fKField;

	//m_clrdlg.DoModal();
	
	m_cCamp.pref = &m_refclient;
	m_cCamp.pDis = &m_cCampDis;
	m_cCamp.SetCurSel(m_clrdlg.m_nName);

	//保存选择
	SaveRefInfo();
	//display
// 	m_cCampDis.strTxt = "Cyan";
// 	m_cCampDis.clrbk = RGB(0,0,255);
// 	m_cCampDis.clrtext = RGB(200,200,255);
// 	
// 	m_cCamp.pDis = &m_cCampDis;
// 	m_cCamp.pref = &m_refclient;
// 	m_cCamp.SetCurSel(0);
	m_refUDP.plist = &m_cRefList;
	m_refUDP.pEdit = &(m_pDlgRef->m_cRefInfoDis);
	m_refUDP.pOldRef = &m_refclient;
	m_refUDP.pField = &m_cFullField;
//	OnConnectref();

	GetDlgItem(IDC_REF_STOP)->SetFocus();

	SetTimer(INITEVENT,500,NULL);
	SetTimer(DISUPDATE,30,NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMSLCoachDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMSLCoachDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMSLCoachDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMSLCoachDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(EXETIMER);
	if (m_pDlgRef != NULL)
	{
		delete m_pDlgRef;
	}
	CDialog::OnClose();
}

void CMSLCoachDlg::OnConnectref() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
 	BOOL bCntSuc = m_refclient.ConnectTo(m_strRefIP,m_nRefPort);
 	if (bCntSuc == TRUE)
 	{
 		SaveRefInfo();
 	}
 	SetTimer(REFCNT,15000,NULL);

	//////////////////////////////////////////////////////////////////////////
	/*UpdateData();
	m_refUDP.SetRefBoxIP(m_strRefIP);
	BOOL resJoin = m_refUDP.JoinGroup("230.0.0.1",30000,0,0);
	if (resJoin == FALSE)
	{
	m_cRefList.AddString("组播频道加入失败！！");
	}
	else
	{
	SaveRefInfo();
	(CWnd*)GetDlgItem(IDC_CONNECTREF)->EnableWindow(FALSE);
	}*/
	//////////////////////////////////////////////////////////////////////////
}

void CMSLCoachDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case EXETIMER:
		m_cFullField.Exec();
		m_CoachUdp.UpdateCntList();
		break;

	case REFCNT:
		OnConnectref();
		break;

	case INITEVENT:
		//OnExpand();
		KillTimer(INITEVENT);
		break;

	case DISUPDATE:
		m_cFullField.Invalidate(FALSE);
		break;

	default:
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CMSLCoachDlg::OnAgentattack() 
{
	// TODO: Add your control notification handler code here
	m_cFullField.ManiMode();
	if (m_cSelNum.Number < 1)
	{
		m_cSendList.AddString("没有选择目标机器");
		m_cSendList.SetCurSel(m_cSendList.GetCount() - 1);
		return;
	}
	m_cFullField.AChangeStatus(m_cSelNum.Number,CTRL_ATTACK,0);
}

void CMSLCoachDlg::OnAgentstop() 
{
	// TODO: Add your control notification handler code here
	if (m_cSelNum.Number < 1)
	{
		m_cSendList.AddString("没有选择目标机器");
		m_cSendList.SetCurSel(m_cSendList.GetCount() - 1);
		return;
	}
	m_cFullField.TestMode();
	m_cFullField.AChangeStatus(m_cSelNum.Number,CTRL_STOP,0);
}

void CMSLCoachDlg::OnAgkickoff() 
{
	// TODO: Add your control notification handler code here
	//m_cFullField.REF_KickOff();
	m_cFullField.ManiMode();
	m_cFullField.AChangeStatus(m_cFullField.SelID,CTRL_KICKOFF_PRIM_READY,0);
}

void CMSLCoachDlg::OnRefkickoff() 
{
	// TODO: Add your control notification handler code here
	m_refclient.Parse(COMM_KICKOFF_CYAN);
}

void CMSLCoachDlg::OnKoStart() 
{
	// TODO: Add your control notification handler code here
	m_cFullField.ManiMode();
	m_cFullField.AChangeStatus(m_cFullField.SelID,CTRL_KICKOFF_PRIM,0);
}

void CMSLCoachDlg::OnAggoalkeep() 
{
	// TODO: Add your control notification handler code here
	m_cFullField.ManiMode();
	m_cFullField.AChangeStatus(m_cFullField.SelID,CTRL_GOALKEEP,0);
}

void CMSLCoachDlg::OnAgkoSl() 
{
	// TODO: Add your control notification handler code here
	m_cFullField.ManiMode();
	m_cFullField.AChangeStatus(m_cFullField.SelID,CTRL_KICKOFF_SLAVE_READY,0);
}

void CMSLCoachDlg::OnAgkoSlStart() 
{
	// TODO: Add your control notification handler code here
	m_cFullField.ManiMode();
	m_cFullField.AChangeStatus(m_cFullField.SelID,CTRL_KICKOFF_SLAVE,0);
	
}

void CMSLCoachDlg::OnDefence() 
{
	// TODO: Add your control notification handler code here
	m_cFullField.ManiMode();
	UpdateData();
	m_cFullField.AChangeStatus(m_cFullField.SelID,CTRL_DEFENCE,m_nDefAng,m_nDefDistance);
}

void CMSLCoachDlg::OnCFk() 
{
	// TODO: Add your control notification handler code here
	m_refclient.Parse(COMM_FREEKICK_CYAN);
}

void CMSLCoachDlg::OnCGk() 
{
	// TODO: Add your control notification handler code here
	m_refclient.Parse(COMM_GOALKICK_CYAN);
}

void CMSLCoachDlg::OnCTi() 
{
	// TODO: Add your control notification handler code here
	m_refclient.Parse(COMM_THROWIN_CYAN);
}

void CMSLCoachDlg::OnCP() 
{
	// TODO: Add your control notification handler code here
	m_refclient.Parse(COMM_PENALTY_CYAN);
}

void CMSLCoachDlg::OnCCk() 
{
	// TODO: Add your control notification handler code here
	m_refclient.Parse(COMM_CORNER_CYAN);
}

void CMSLCoachDlg::OnMKo() 
{
	// TODO: Add your control notification handler code here
	m_refclient.Parse(COMM_KICKOFF_MAGENTA);
}

void CMSLCoachDlg::OnMFk() 
{
	// TODO: Add your control notification handler code here
	m_refclient.Parse(COMM_FREEKICK_MAGENTA);
}

void CMSLCoachDlg::OnMGk() 
{
	// TODO: Add your control notification handler code here
	m_refclient.Parse(COMM_GOALKICK_MAGENTA);
}

void CMSLCoachDlg::OnMTi() 
{
	// TODO: Add your control notification handler code here
	m_refclient.Parse(COMM_THROWIN_MAGENTA);
}

void CMSLCoachDlg::OnMP() 
{
	// TODO: Add your control notification handler code here
	m_refclient.Parse(COMM_PENALTY_MAGENTA);
}

void CMSLCoachDlg::OnMCk() 
{
	// TODO: Add your control notification handler code here
	m_refclient.Parse(COMM_CORNER_MAGENTA);
}

void CMSLCoachDlg::OnRefDb() 
{
	// TODO: Add your control notification handler code here
	m_refclient.Parse(COMM_DROPPED_BALL);
}

void CMSLCoachDlg::OnRefStart() 
{
	// TODO: Add your control notification handler code here
	m_refclient.Parse(COMM_START);
}

void CMSLCoachDlg::OnRefStop() 
{
	// TODO: Add your control notification handler code here
	m_refclient.Parse(COMM_STOP);
}
BOOL CMSLCoachDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message==WM_KEYDOWN)	//判断是否为按键消息
	{
		
		switch (pMsg->wParam)		//根据虚拟键表值来判断按下的是什么键
		{
		case VK_UP:			//按下前键
			break;

		case VK_DOWN:		//按下后键
			break;

		case VK_LEFT:		//按下左键
			break;

		case VK_RIGHT:		//按下右键
			break;

		case VK_SHIFT:
			m_refclient.Parse(COMM_STOP);
			break;
		case VK_CONTROL:
			//AfxMessageBox("ctrl down!");
			m_cFullField.m_bSetPlayerIn = true;
			m_cFullField.Invalidate();
			break;

		case 87:
			/*/AfxMessageBox(_T("w按下"));
			break;*/
		case 83:
			//AfxMessageBox(_T("s按下"));
			OnAgentstop();
			break;
		case 65:
			//AfxMessageBox(_T("a按下"));
			OnAgentattack();
			break;
		case 67:
			//AfxMessageBox(_T("c按下"));
			OnAgCatch();
			break;
		case 68:
			/*/AfxMessageBox(_T("d按下"));
			break;*/
		case 81:	
			break;

		case 90:	//z		缩小
			m_cFullField.Active();
			break;
			
		case 88:	//x		放大
			m_refclient.Parse(COMM_START);
			break;
			
		default:
			break;
		}

		if (m_cFullField.m_eStage == 22)
		{
			//sTestPassCatch
			if (pMsg->wParam > 0x30  && pMsg->wParam < 0x36)
			{
				m_cFullField.m_nPassPlayer = 0;
				m_cFullField.m_nCatchPlayer = (pMsg->wParam & 0x0f);
				m_cFullField.m_bTaskPassCatch_kick = false;
				m_cFullField.Invalidate(FALSE);
			}
		}
	}

	if (pMsg->message==WM_KEYUP)	//判断是否为按键弹起消息
	{
		switch (pMsg->wParam)		//根据虚拟键表值来判断按下的是什么键
		{
		case VK_CONTROL:
			//AfxMessageBox("ctrl UP!");
			m_cFullField.m_bSetPlayerIn = false;
			m_cFullField.Invalidate();
			break;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CMSLCoachDlg::OnProdef() 
{
	// TODO: Add your control notification handler code here
	
	m_cFullField.ManiMode();
	m_cFullField.AChangeStatus(m_cFullField.SelID,CTRL_PRODEF,0);
}

void CMSLCoachDlg::OnAttention() 
{
	// TODO: Add your control notification handler code here
	m_cFullField.ManiMode();
	m_cFullField.AChangeStatus(m_cFullField.SelID,CTRL_ATTENTIONBALL,0);
	
}

void CMSLCoachDlg::OnUpdateang() 
{
	// TODO: Add your control notification handler code here
	m_cFullField.ManiMode();
	UpdateData();
	m_cFullField.m_SelAng = m_fSelAngle;
}

void CMSLCoachDlg::OnAsearchball() 
{
	// TODO: Add your control notification handler code here
	m_cFullField.ManiMode();
	m_cFullField.AChangeStatus(m_cFullField.SelID,CTRL_SEARCHBALL,0);
	
}

void CMSLCoachDlg::OnAgkickout() 
{
	// TODO: Add your control notification handler code here
	//m_cFullField.ManiMode();
	m_cFullField.KickAgent(m_cFullField.SelID);
}

void CMSLCoachDlg::OnMultidef() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_cFullField.Test_Multidef(m_nDistOfMultiDef);
	
}

void CMSLCoachDlg::OnMultiatk() 
{
	// TODO: Add your control notification handler code here
	m_cFullField.Test_PressDef();
}

void CMSLCoachDlg::OnMultitest() 
{
	// TODO: Add your control notification handler code here
	//m_cFullField.Test_checkeveryone();
	m_cFullField.Test_PassCatch();
}

void CMSLCoachDlg::OnReady() 
{
	// TODO: Add your control notification handler code here
	m_refclient.Parse(COMM_PARKING);
}

void CMSLCoachDlg::OnScanagent() 
{
	// TODO: Add your control notification handler code here
	m_CoachUdp.m_ag_Scan();
}

void CMSLCoachDlg::OnChangegoal() 
{
	// TODO: Add your control notification handler code here
	m_cFullField.Field_Change();
}

void CMSLCoachDlg::OnHflip() 
{
	// TODO: Add your control notification handler code here
	m_cFullField.Display_HFlip();
}

void CMSLCoachDlg::OnVflip() 
{
	// TODO: Add your control notification handler code here
	m_cFullField.Display_VFlip();
}

void CMSLCoachDlg::m_ExpandDialog()
{
	//参数nResourceID表示分割线的ID
	//参数bexpand为TRUE时表示要扩展对话框，否则缩减对话框
	//参数inmode表示扩展的方向
	static CRect rcLarge;
	static CRect rcSmall;

	//static CRgn rgnSmall;

	if(rcLarge.IsRectNull())
	{
		
		GetWindowRect(rcLarge);
	}

	//获得一个完整的窗体尺寸	
	rcSmall=rcLarge;
	//扩展的方向
	if (m_bExpandRight == FALSE)
	{
		rcSmall.right -= 270;
		//rgnSmall.CreateRectRgn(rcSmall.left,rcSmall.top,rcSmall.right,rcSmall.bottom);
	}
//	SetWindowRgn(rgnSmall,TRUE);
	SetWindowPos(NULL,100,0,rcSmall.Width(),
		rcSmall.Height(),
		SWP_NOMOVE|SWP_NOZORDER);
	//MoveWindow(rcSmall);
	m_EnableVisibleChildren();
}

void CMSLCoachDlg::m_EnableVisibleChildren()
{
	CWnd* pWndCtl=GetWindow(GW_CHILD);
	CRect rcTest;
	CRect rcControl;
	//记录各控件的位置和大小
	CRect rcShow;
	//记录目前显示对话框的位置和大小
	GetWindowRect(rcShow);
	while(pWndCtl!=NULL)
	{
		pWndCtl->GetWindowRect(rcControl);
		if(rcTest.IntersectRect(rcShow,rcControl))
			pWndCtl->EnableWindow(TRUE);
		else
			pWndCtl->EnableWindow(FALSE);
		pWndCtl=pWndCtl->GetWindow(GW_HWNDNEXT);
	}
}

void CMSLCoachDlg::OnExpand() 
{
	// TODO: Add your control notification handler code here
	m_bExpandRight=!m_bExpandRight;
	m_ExpandDialog();//扩展或收缩对话框

	if (m_bExpandRight == TRUE)
	{
		GetDlgItem(IDC_EXPAND)->SetWindowText("收起<<");
	} 
	else
	{
		GetDlgItem(IDC_EXPAND)->SetWindowText("展开>>");

		if (m_pDlgRef != NULL)
		{
			static CRect mainRect , rect;

			GetWindowRect(&mainRect);
			m_pDlgRef->GetWindowRect(&rect);

			rect.right = mainRect.right + rect.Width();
			rect.left = mainRect.right;
			rect.top = mainRect.top;
			rect.bottom = mainRect.bottom;

			m_pDlgRef->MoveWindow(rect,FALSE);
			m_pDlgRef->ShowWindow(SW_SHOW);		//显示非模态对话框
			this->GetFocus();	
		}
	}
}

void CMSLCoachDlg::SaveRefInfo()
{
	CoachSet setting;
	strcpy(setting.RefBoxIP,m_strRefIP.GetBuffer(m_strRefIP.GetLength()));
	setting.Name = m_cCamp.GetCurSel();

	for (int i=0;i<5;i++)
	{
		setting.CreditCnt[i] = m_CoachUdp.m_creditCnt[i];
	}

	CFile mFile("Ref.Cfg", CFile::modeWrite|CFile::modeCreate); 
	mFile.Write(&setting,sizeof(setting)); 
	mFile.Flush(); 
	mFile.Close();
}

void CMSLCoachDlg::LoadRefInfo()
{
	CFileFind finder;
	if(finder.FindFile("Ref.Cfg") == FALSE)
		return;

	CoachSet setting;
	CFile mFile("Ref.Cfg",CFile::modeRead); 

	if(mFile.GetLength() != sizeof(setting)) 
	return; 

	mFile.Read(&setting,sizeof(setting)); 
	mFile.Close(); 

	//赋值
	GetDlgItem(IDC_REFIP)->SetWindowText(setting.RefBoxIP);
	m_cCamp.SetCurSel(setting.Name);
	m_clrdlg.m_nName = setting.Name;

	//色标
	int logInd = GetPrivateProfileInt("Coach Config", "Camp", 0, "./Coach.ini");
	m_cCamp.SetCurSel(logInd);

	int ind = m_cCamp.GetCurSel();
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

	m_refclient.Self = ind;

	for (int i=0;i<5;i++)
	{
		m_CoachUdp.m_creditCnt[i] = setting.CreditCnt[i];
	}

	m_DisplayCredit();
}


void CMSLCoachDlg::m_DisplayCredit()
{
	m_cCreditList.DeleteAllItems();
	CString strT;
	for (int i=0;i<5;i++)
	{	
		strT.Format("%d.%d.%d.%d",m_CoachUdp.m_creditCnt[i].ip[0],m_CoachUdp.m_creditCnt[i].ip[1],m_CoachUdp.m_creditCnt[i].ip[2],m_CoachUdp.m_creditCnt[i].ip[3]);
		m_cCreditList.InsertItem(i,strT);
		m_cCreditList.SetItemText(i,0,strT);
	}
}

void CMSLCoachDlg::OnAddalltocredit() 
{
	// TODO: Add your control notification handler code here
	m_CoachUdp.AllCredit();
	m_DisplayCredit();
	SaveRefInfo();
}

void CMSLCoachDlg::OnMovetotest() 
{
	// TODO: Add your control notification handler code here
	if (m_cFullField.bMovetoTest == true)
	{
		m_cFullField.bMovetoTest = false;
	} 
	else
	{
		m_cFullField.bMovetoTest = true;
	}
}

void CMSLCoachDlg::OnClearallcredit() 
{
	// TODO: Add your control notification handler code here
	for (int i=0;i<5;i++)
	{
		m_CoachUdp.ClearCredit(i);
	}
	m_DisplayCredit();
}

void CMSLCoachDlg::OnClearcredit() 
{
	// TODO: Add your control notification handler code here
	m_CoachUdp.ClearCredit(m_nCreditSel);
	m_DisplayCredit();
}

void CMSLCoachDlg::OnClickCreditlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != -1)
	{	
		m_nCreditSel = pNMListView->iItem;
	}
	*pResult = 0;
}

void CMSLCoachDlg::OnAddtocredit() 
{
	// TODO: Add your control notification handler code here
	//先清空信任列表
	for (int i=0;i<5;i++)
	{
		m_CoachUdp.ClearCredit(i);
	}
	//添加已选择项至信任列表
	int nIndex = 0;
	POSITION pos = m_cCntList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		while (pos)
		{
			int nItem = m_cCntList.GetNextSelectedItem(pos);
			m_CoachUdp.AddCredit(nItem,nIndex);
			nIndex ++;
		}
	}
	m_DisplayCredit();
}

void CMSLCoachDlg::OnCheckIpfilter() 
{
	// TODO: Add your control notification handler code here
	if (m_cChkIpFilter.GetCheck() == 0)
	{
		m_CoachUdp.m_bEnIPFilter = false;
	} 
	else
	{
		//AfxMessageBox("check");
		m_CoachUdp.m_bEnIPFilter = true;
	}
}

void CMSLCoachDlg::OnUpdatek() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	m_cFullField.m_fKField = m_fKField;

}

void CMSLCoachDlg::OnPrOut() 
{
	// TODO: Add your control notification handler code here
	m_cFullField.PR_Out(m_cFullField.SelID);
}

void CMSLCoachDlg::OnPrReady() 
{
	// TODO: Add your control notification handler code here	
	m_cFullField.PR_Ready(m_cFullField.SelID);
}

void CMSLCoachDlg::OnSetplayerinpont() 
{
	// TODO: Add your control notification handler code here
	m_cFullField.m_bSetPlayerIn = true;
}

void CMSLCoachDlg::OnCheckObstOrg() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (m_bObst_Org == TRUE)
	{
		m_cFullField.m_bObst_Org = true;
	} 
	else
	{
		m_cFullField.m_bObst_Org = false;
	}
}

void CMSLCoachDlg::OnDlgDbgClosing(WPARAM wParam, LPARAM lParam)
{
	;
}

void CMSLCoachDlg::OnDisdlgdbg() 
{
	// TODO: Add your control notification handler code here
// 	m_dlgMInfo = new CDlgMotorInfo(this);
// 	m_dlgMInfo->Create(IDD_DLG_MOTORINFO,this);
// 	
// 	m_dlgMInfo->m_hParent = this->GetSafeHwnd();
// 	m_cmd.pMotorDis = &(m_dlgMInfo->m_cMotorInfo);
// 	m_cmd.pIODis = &(m_dlgMInfo->m_cIOInfo);
// 	m_cmd.pADDis = &(m_dlgMInfo->m_cADInfo);
// 	m_cmd.CmdShow_Rec = &(m_dlgMInfo->m_cMInfo_List);
// 	
// 	m_dlgMInfo->CenterWindow(this);
// 	m_dlgMInfo->ShowWindow(SW_SHOW);
}

void CMSLCoachDlg::OnTcdemo() 
{
	// TODO: Add your control notification handler code here
	m_cFullField.TC_Demo1();
}

void CMSLCoachDlg::OnAgCatch() 
{
	// TODO: Add your control notification handler code here
	m_cFullField.midcatchmode();
	if (m_cSelNum.Number < 1)
	{
		m_cSendList.AddString("没有选择目标机器");
		m_cSendList.SetCurSel(m_cSendList.GetCount() - 1);
		return;
	}
	UpdateData();
//	m_cFullField.AChangeMidPassStatus////(m_cSelNum.Number,CTRL_MID_CATCH,test_midpass_x,test_midpass_y,10,0//////////////////////////////////////////////////////////////////////////
//*Cal_Angle_Mid_Catch()*/);
	m_cFullField.midcatch_n = m_cSelNum.Number;
	m_cFullField.midcatch_x = test_midpass_x;
	m_cFullField.midcatch_y = test_midpass_y;
	m_cFullField.midcatch_b = 10;
	CString info;
	info.Format("中场传球位置 %d     %d\n", test_midpass_x,test_midpass_y);
	m_cFullField.printf(info);
}

void CMSLCoachDlg::OnCovertheball() 
{
	// TODO: Add your control notification handler code here
	
}

void CMSLCoachDlg::OnDisconnectref() 
{
	// TODO: Add your control notification handler code here
	BOOL res = m_refUDP.LeaveGroup();
	if (res == TRUE)
	{
		m_cRefList.AddString("成功离开组播频道！！");
		(CWnd*)GetDlgItem(IDC_CONNECTREF)->EnableWindow(TRUE);
	}
}

void CMSLCoachDlg::OnObstconfig() 
{
	// TODO: Add your control notification handler code here	
	//障碍物debug
	m_pDlgDbg = new CDlgDebug(this);
	m_pDlgDbg->Create(IDD_DLG_DEBUG,this);
	m_pDlgDbg->ShowWindow(SW_SHOW);		//显示非模态对话框
// 	m_cFullField.m_obstFilter.pView = &(m_pDlgDbg->m_cDbgView);
// 	m_cFullField.m_obstFilter.pProcView = &(m_pDlgDbg->m_cProcView);
// 	m_pDlgDbg->pFilter = &(m_cFullField.m_obstFilter);
	
	this->GetFocus();
}

void CMSLCoachDlg::OnAllflip() 
{
	// TODO: Add your control notification handler code here
	OnHflip();
	OnVflip();
}

void CMSLCoachDlg::OnConf1() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_cFullField.ManiMode();
	m_cFullField.AChangeStatus(m_nConf1_atk,CTRL_ATTACK,0);
	m_cFullField.AChangeStatus(m_nConf1_def,CTRL_DEFENCE,m_nConf1_DefAngle,m_nConf1_DefDist);
}

void CMSLCoachDlg::OnConf2() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_cFullField.ManiMode();
	m_cFullField.AChangeStatus(m_nConf2_atk1,CTRL_ATTACK,0);
	m_cFullField.AChangeStatus(m_nConf2_atk2,CTRL_ATTACK,0);
}

void CMSLCoachDlg::OnOutfield() 
{
	// TODO: Add your control notification handler code here
	m_cFullField.Leave();
}


void CMSLCoachDlg::OnBnClickedTcdemo2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_cFullField.TC_Demo2012();
}


void CMSLCoachDlg::OnBnClickedTcdemo3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_cFullField.isStart=true;
}


void CMSLCoachDlg::OnStnClickedFullfield()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CMSLCoachDlg::OnEnChangeConf1Defang()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CMSLCoachDlg::OnEnChangemidpassy()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CMSLCoachDlg::OnLbnSelchangeSendlist()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CMSLCoachDlg::OnAgentDefend()
{
	// TODO: 在此添加控件通知处理程序代码
	m_cFullField.ManiMode();
	if (m_cSelNum.Number < 1)
	{
		m_cSendList.AddString("没有选择目标机器");
		m_cSendList.SetCurSel(m_cSendList.GetCount() - 1);
		return;
	}
	m_cFullField.AFollowPosition(m_cSelNum.Number,CTRL_FOLLOWPOSITION,0,0);
}


void CMSLCoachDlg::OnStnClickedPassandcatch()
{
	// TODO: 在此添加控件通知处理程序代码
}





