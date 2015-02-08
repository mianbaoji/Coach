// MSLCoachDlg.h : header file
//

#if !defined(AFX_MSLCOACHDLG_H__3773455F_D2F0_4C28_A2E9_1C55D32DD558__INCLUDED_)
#define AFX_MSLCOACHDLG_H__3773455F_D2F0_4C28_A2E9_1C55D32DD558__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UPFullField.h"
#include "RefClient.h"
#include "NumDisplay.h"
#include "CampCBox.h"
#include "OwnGoalCombo.h"
#include "ClrStr.h"
#include "CoachUDP.h"
#include "ColorDlg.h"
#include "MulticastSocket.h"
#include "DlgRefInfo.h"
#include "DlgDebug.h"

#define EXETIMER 1
#define REFCNT	2
#define INITEVENT	10
#define DISUPDATE	11

#define WM_DLGDBGCLOSED WM_USER + 2000
/////////////////////////////////////////////////////////////////////////////
// CMSLCoachDlg dialog

class CMSLCoachDlg : public CDialog
{
// Construction
public:
	void LoadRefInfo();
	void SaveRefInfo();
	CMSLCoachDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMSLCoachDlg)
	enum { IDD = IDD_MSLCOACH_DIALOG };
	CStatic	m_cDisPassAndCatch;
	CListCtrl	m_cTacList;
	CButton	m_cChkIpFilter;
	CListCtrl	m_cCreditList;
	CListCtrl	m_cCntList;
	CListCtrl	m_cAlist;
	CClrStr	m_cCampDis;
	CCampCBox	m_cCamp;
	CNumDisplay	m_cSelNum;
	CListBox	m_cSendList;
	CListBox	m_cRefList;
	CUPFullField	m_cFullField;
	CString	m_strRefIP;
	int		m_nRefPort;
	int		m_nDefAng;
	double	m_fSelAngle;
	int		m_nDefDistance;
	int		m_nDistOfMultiDef;
	float	m_fKField;
	BOOL	m_bObst_Org;
	int		m_nConf1_atk;
	int		m_nConf1_def;
	int		m_nConf2_atk1;
	int		m_nConf2_atk2;
	int		m_nConf1_DefAngle;
	int		m_nConf1_DefDist;
	int     test_midpass_x;
	int		test_midpass_y;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMSLCoachDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int i;
	void m_DisplayCredit();
	void m_EnableVisibleChildren();
	void m_ExpandDialog();
	HICON m_hIcon;
	CRefClient m_refclient;

	//09
	CCoachUDP m_CoachUdp;
	int m_nCreditSel;

	//expand
	BOOL m_bExpandRight;//记录扩展或收缩状状态

	CColorDlg m_clrdlg;

	CMulticastSocket m_refUDP;
	CDlgRefInfo* m_pDlgRef;
	CDlgDebug* m_pDlgDbg;

	// Generated message map functions
	//{{AFX_MSG(CMSLCoachDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnConnectref();
	afx_msg void OnAgentattack();
	afx_msg void OnAgentstop();
	afx_msg void OnAgkickoff();
	afx_msg void OnRefkickoff();
	afx_msg void OnKoStart();
	afx_msg void OnAggoalkeep();
	afx_msg void OnAgkoSl();
	afx_msg void OnAgkoSlStart();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCFk();
	afx_msg void OnCGk();
	afx_msg void OnCTi();
	afx_msg void OnCP();
	afx_msg void OnCCk();
	afx_msg void OnMKo();
	afx_msg void OnMFk();
	afx_msg void OnMGk();
	afx_msg void OnMTi();
	afx_msg void OnMP();
	afx_msg void OnMCk();
	afx_msg void OnRefDb();
	afx_msg void OnRefStart();
	afx_msg void OnRefStop();
	afx_msg void OnDefence();
	afx_msg void OnClose();
	afx_msg void OnProdef();
	afx_msg void OnAttention();
	afx_msg void OnUpdateang();
	afx_msg void OnAsearchball();
	afx_msg void OnAgkickout();
	afx_msg void OnMultidef();
	afx_msg void OnMultiatk();
	afx_msg void OnMultitest();
	afx_msg void OnReady();
	afx_msg void OnScanagent();
	afx_msg void OnChangegoal();
	afx_msg void OnHflip();
	afx_msg void OnVflip();
	afx_msg void OnExpand();
	afx_msg void OnAddalltocredit();
	afx_msg void OnMovetotest();
	afx_msg void OnClearallcredit();
	afx_msg void OnClearcredit();
	afx_msg void OnClickCreditlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAddtocredit();
	afx_msg void OnCheckIpfilter();
	afx_msg void OnUpdatek();
	afx_msg void OnPrOut();
	afx_msg void OnPrReady();
	afx_msg void OnSetplayerinpont();
	afx_msg void OnCheckObstOrg();
	afx_msg void OnDlgDbgClosing(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDisdlgdbg();
	afx_msg void OnTcdemo();
	afx_msg void OnAgCatch();
	afx_msg void OnCovertheball();
	afx_msg void OnDisconnectref();
	afx_msg void OnObstconfig();
	afx_msg void OnAllflip();
	afx_msg void OnConf1();
	afx_msg void OnConf2();
	afx_msg void OnOutfield();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedTcdemo2();
	afx_msg void OnBnClickedTcdemo3();
	afx_msg void OnStnClickedFullfield();
	afx_msg void OnEnChangeConf1Defang();
	afx_msg void OnEnChangemidpassy();
	afx_msg void OnLbnSelchangeSendlist();
	
	afx_msg void OnStnClickedPassandcatch();
	
	afx_msg void OnAgentDefend();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSLCOACHDLG_H__3773455F_D2F0_4C28_A2E9_1C55D32DD558__INCLUDED_)
