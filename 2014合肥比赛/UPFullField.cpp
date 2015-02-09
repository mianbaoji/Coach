// UPFullField.cpp : implementation file
//

#include "stdafx.h"
#include "MSLCoach.h"
#include "UPFullField.h"
#include "MSL_Protocol.h"
#include <math.h>
#include<fstream>
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUPFullField

CUPFullField::CUPFullField()
{
	//zbw dig_in
	goalx = -1;
	goaly = -1;
	goalangle = 180 ;
	//zbw predict ball
	lpballjiao=0;
	lpballchang=0;
	tpballjiao=0;
	tpballchang=0;
	//
	isStart = false;
	//m_BkBmp.LoadBitmap(IDB_BKFIELD);
	srand((unsigned)time(NULL));
	m_nAgentNum = 0;
	m_iEvaluteFrame=0;
	m_px = m_py = NULL;
	m_pcnt = 0;

	for (int i=0;i<7;i++)
	{
		//agents
		m_agents[i].ID = i;
		m_agents[i].foundball = false;
		m_agents[i].dist_ball = 10000;
		m_agents[i].online = false;
		m_agents[i].allocated = false;
		//balls
		m_balls[i].ID = i;
		m_balls[i].updated = false;
		m_balls[i].x = 0;
		m_balls[i].y = 0;
		//preballs
		m_preballs[i].ID = i;
		m_preballs[i].jiao = 0;
		m_preballs[i].chang = 0;
		//dig_in
		m_follow[i].x=-1;
		m_follow[i].y=-1;
		m_follow[i].size=-1;
		m_follow[i].believe=false;

		m_memstatus[i] = 0;
	}
	m_nAtkDly = 0;

	m_trueball.x = m_trueball.y = 900;
	m_Foreball.x = m_Foreball.y = 600;
	m_lastball = m_trueball;
	m_bFoundball = false;	//初始化为未发现球

	//sel
	SelID = 0;
	pNum = NULL;
	m_SelAng = 0;

	//game
	for (i=0;i<7;i++)
	{
		pToAgent[i] = NULL;
	}
	pSndList = NULL;

	//owngoal
	OwnGoalColor = BLUEGOAL;
	m_bField = false;

	//tactic
	for (i=1;i<7;i++)
	{
		m_tacctrl[i].target_x = 500;
		m_tacctrl[i].target_y = 600;
		m_tacctrl[i].target_angle = 0;
		m_tacctrl[i].targetGoal = 1;
		m_tacctrl[i].re_ballx = 900;
		m_tacctrl[i].re_bally = 600;
	}
	m_Pos_Back.x = 400;
	m_Pos_Back.y = 600;
	m_bHalfready = false;

	//debug
	m_bDebug = true;

	m_eStage = sMani;

	for (i =0;i<7;i++)
	{
		m_recvcnt[i] = 0;
	}
	m_chkagnt = 0;

	//status
	VERIFY(font.CreateFont( 
		16, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_BLACK, // nWeight 
		FALSE, // bItalic 
		FALSE, // bUnderline 
		0, // cStrikeOut 
		ANSI_CHARSET, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		CLIP_DEFAULT_PRECIS, // nClipPrecision 
		DEFAULT_QUALITY, // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		"Arial")); // lpszFacename
	m_strCurSt.Format("比赛尚未开始");

	//status
	VERIFY(txtfont.CreateFont( 
		14, // nHeight 
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

	GetCurTime();

	pAgInfo = NULL;

	//翻转
	m_bHFlip = false;
	m_bVFlip = false;

	//multiDef
	m_nDistOfMultiDef = 250;

	//kick off
	m_curKOPrim = 0;
	m_curKOSlave = 0;

	bMovetoTest = false;
	nMovetoAng = 0;
	m_fKField = 1;

	m_eLastStage = sStop;
	m_nAtkNotMove = 20;
	m_bakAtkAgent.ID = 0;
	m_nNumOfAtk = 1;

	bCntRefbox = false;

	//atk switch delay
	for (i=0;i<7;i++)
	{
		m_nSWAtkDly[i] = ATK_SW_DLY;
		m_nObstMsgCnt[i] = 0;
	}

	for(i=0;i<(EACH_AG_OBST_NUM*AG_NUM+1);i++)
	{
		m_cObst[i].valid = false;
		m_tacObst[i].valid = false;
		m_tmpObst[i].valid = false;
	}

	for(i=0;i<AG_NUM;i++)
	{
		m_BufBall[i].SetSize(BALLBUF_LEN);
		m_BufObst[i].SetSize(OBSTBUF_LEN);
	}
	//m_BufBall[1].AddPos(1,1);

	//传接球
	m_bTaskPassCatch = false;
	m_bTaskPassCatch_kick = false;
	m_nCatchPlayer = 0;
	m_nPassPlayer = 0;

	//mid传接球
	m_bTaskMidPassCatch = false;
	m_bTaskMidPassCatch_kick = false;
	m_isMidPasserCannotTakeBall = false;
	m_MidPasserCannotTakeBallCount = 0;
	m_nMidCatchPlayer = 0;
	m_nMidPassPlayer = 0;

	//3meter pass catch [wzc]
	HoldBallFlag =0;
	MemHoldBallFlag=0;
	InitialHoldBall.x=0;
	InitialHoldBall.y=0;
	DistToInitialHoldBallPoint=0;
	HoldBallBelieve=0;
	m_3meterPassBelieve=0;
	Last3meterCatcher=0;
	Last3meterPasser=0;
	m_bTask3MeterPass=false; 
	m_bTask3MeterPass_Kick=false;
	Default_start_delay=0;
	Attack_Convert_Flag=0;
	Attack_Convert_Counter=0;
	m_n3mCatchPlayer = 0;
	m_n3mPassPlayer = 0;
	Flag333=0;

	for (i=0;i<AG_NUM;i++)
	{
		m_prRightOfPlayer[i] = PLAYER_RIGHT_INFIELD;
		m_prBeInPoint[i] = 0;
	}

	m_fpPlayerIn.x = 900;
	m_fpPlayerIn.y = -20;
	m_bSetPlayerIn = false;
	m_nTimeToBeIn = 0;

	m_bObst_Org = false;

//	m_obstFilter.CreatBuf(180,120);

	m_nTCFindBall = 0;

	m_nAgCanBeInPenaltyArea = 0;
	m_bCatchDelay = false;

	m_nLogTacIndex = 0;

	m_bTCArrived = false;

	m_b10SecEnd = false;

	m_nPassEndDelay = PASS_END_DELAY;

	m_BallLogAt10SecEnd.x = 900;
	m_BallLogAt10SecEnd.y = 600;

	 final_mid_pass_x = 1800;
	 final_mid_pass_y = 900;
	 final_mid_pass_believe = 0;
}

CUPFullField::~CUPFullField()
{
	m_Clearpa();
	m_Clearpb();
	m_asc.Unlock();
	m_bsc.Unlock();
}

BEGIN_MESSAGE_MAP(CUPFullField, CStatic)
	//{{AFX_MSG_MAP(CUPFullField)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUPFullField message handlers

void CUPFullField::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC dcMemory;
	dcMemory.CreateCompatibleDC(&dc);

	m_BkBmp.DeleteObject();
	if (m_bHFlip == false)
	{
		m_BkBmp.LoadBitmap(IDB_L2RBKFIELD);
	} 
	else
	{
		m_BkBmp.LoadBitmap(IDB_R2LBKFIELD);
	}

	dcMemory.SelectObject(&m_BkBmp);
	dcMemory.SetBkMode(TRANSPARENT);
	CPen* oldpen;
	CPen path(PS_DOT,1,RGB(255,255,0));

	//绘制场线采集点
// 	if (m_px != NULL && m_py != NULL)
// 	{
// 		for (int i=0;i<m_pcnt;i++)
// 		{	
// 			dcMemory.SetPixel(m_Point_Field2Img(m_px[i],m_py[i]),RGB(255,255,0));
// 		}
// 	}
	//绘制PlayerIn上场的位置	
	CPoint coodinate;
	coodinate = m_Point_Field2Img(m_fpPlayerIn.x,m_fpPlayerIn.y);
	CPen redpen(PS_SOLID,1,RGB(255,0,0));
	CPen yellow(PS_SOLID,2,RGB(255,255,0));
	oldpen = dcMemory.SelectObject(&yellow);
	dcMemory.MoveTo(coodinate.x-8,coodinate.y);
	dcMemory.LineTo(coodinate.x+8,coodinate.y);
	dcMemory.MoveTo(coodinate.x,coodinate.y-8);
	dcMemory.LineTo(coodinate.x,coodinate.y+8);
	dcMemory.SelectObject(oldpen);	
	dcMemory.SetTextColor(RGB(255,255,0));
	CFont* oldfont = dcMemory.SelectObject(&font);
	if (m_bSetPlayerIn == true)
	{
		dcMemory.TextOut(380,320,"设置入场点");
	}
	dcMemory.SelectObject(oldfont);

	//障碍物
	CString aNum;
	CBrush black(RGB(0,0,0));	
	CBrush* oldbrush = dcMemory.SelectObject(&black);
	dcMemory.SetTextColor(RGB(150,150,150));
	int nNumObst;
	nNumObst = EACH_AG_OBST_NUM*(AG_NUM+1);
	int i;
	if (m_bObst_Org == true)
	{
		for (i=0;i<nNumObst;i++)
		{
			if (m_cObst[i].valid == true)
			{
				coodinate = m_Point_Field2Img(m_cObst[i].x,m_cObst[i].y);
				dcMemory.Ellipse(coodinate.x-7,coodinate.y-7,coodinate.x+7,coodinate.y+7);
				//aNum.Format("%d",m_cObst[i].ID);
				aNum.Format("%d(%d)",m_cObst[i].ID,m_cObst[i].size);
				dcMemory.TextOut(coodinate.x-4,coodinate.y-8,aNum);
			}
		}
	} 
	else
	{
		for (i=0;i<nNumObst;i++)
		{
			if (m_tacObst[i].valid == true)
			{
				coodinate = m_Point_Field2Img(m_tacObst[i].x,m_tacObst[i].y);
				dcMemory.Ellipse(coodinate.x-7,coodinate.y-7,coodinate.x+7,coodinate.y+7);
				//aNum.Format("%d",m_tacObst[i].ID);			
				//aNum.Format("%d(%d)",m_tacObst[i].ID,m_tacObst[i].DistToOwner/*m_tacObst[i].size*/);
				aNum.Format("%d(%d)",m_tacObst[i].ID,m_tacObst[i].size);
				dcMemory.TextOut(coodinate.x-4,coodinate.y-8,aNum);
			}
		}	
	}
	dcMemory.SelectObject(oldbrush);

	//绘制Agnet
	CBrush blue(RGB(0,0,255));
	CPoint direct;
	double dAngle = 0;
	CPen dpen(PS_SOLID,3,RGB(255,0,100));
	//uagent* t = m_pa;

	CPen outline(PS_SOLID,1,RGB(0,0,0));
	//m_asc.Lock();
	for (i=1;i<7;i++)
	{
		if (m_agents[i].online == false)
		{
			continue;
		}
		coodinate = m_Point_Field2Img(m_agents[i].x,m_agents[i].y);
		dcMemory.SelectObject(&blue);
		
		//绘制路径
		if ((m_agents[i].x != m_agents[i].moveto_x || m_agents[i].y != m_agents[i].moveto_y) && m_agents[i].status == CTRL_MOVETO )
		{
			oldpen = dcMemory.SelectObject(&path);
			CPoint moveto = m_Point_Field2Img(m_agents[i].moveto_x,m_agents[i].moveto_y);
			dcMemory.MoveTo(coodinate);
			dcMemory.LineTo(moveto);
			dcMemory.SelectObject(oldpen);
		}

		//如果是点选目标，则将外围线设为黄色
		if (m_agents[i].ID == SelID)
		{
			oldpen = dcMemory.SelectObject(&yellow);
		}
		else
		{
			oldpen = dcMemory.SelectObject(&outline);
		}
		dcMemory.Ellipse(coodinate.x-8,coodinate.y-8,coodinate.x+8,coodinate.y+8);
		dcMemory.SelectObject(oldpen);
		
		//根据翻转情况计算朝向角度
		dAngle = m_agents[i].angle;
		if (m_bHFlip == true)
		{
			dAngle = 180 - dAngle;
		}

		if (m_bVFlip == true)
		{
			dAngle = -dAngle;
		}
		direct.x = (long)(12*cos(dAngle*3.14/180));
		direct.y = (long)(-12*sin(dAngle*3.14/180));
		direct += coodinate;
		oldpen = dcMemory.SelectObject(&dpen);
		dcMemory.MoveTo(coodinate);
		dcMemory.LineTo(direct);

		//根据翻转情况计算速度方向
// 		if (m_agents[i].V > 0)
// 		{
// 			dAngle = m_agents[i].V_Angle;
// 			if (m_bHFlip == true)
// 			{
// 				dAngle = 180 - dAngle;
// 			}
// 			
// 			if (m_bVFlip == true)
// 			{
// 				dAngle = -dAngle;
// 			}
// 			direct.x = (long)(m_agents[i].V*cos(dAngle*3.14/180))/10;
// 			direct.y = (long)(-m_agents[i].V*sin(dAngle*3.14/180))/10;
// 			direct += coodinate;
// 			dcMemory.SelectObject(&redpen);
// 			dcMemory.MoveTo(coodinate);
// 			dcMemory.LineTo(direct);
// 		}

		dcMemory.SelectObject(oldpen);
		aNum.Format("%d",m_agents[i].ID);
		dcMemory.SetTextColor(RGB(0,255,255));
		dcMemory.TextOut(coodinate.x-4,coodinate.y-8,aNum);

		//显示状态
		aNum = GetCTRLString(m_agents[i].status);
		oldfont = dcMemory.SelectObject(&txtfont);
		dcMemory.SetTextColor(RGB(255,255,255));
// 		if (aNum == "进攻")
// 		{
// 			dcMemory.SetTextColor(RGB(200,0,0));
// 		}
		dcMemory.TextOut(coodinate.x-10,coodinate.y-20,aNum);
		dcMemory.SelectObject(oldfont);

		//test 显示进攻效用
// 		dcMemory.SetTextColor(RGB(255,255,0));
// 		aNum.Format("%d",m_Perform_ATK(&m_agents[i]));
// 		dcMemory.TextOut(coodinate.x+10,coodinate.y,aNum);
	}

	//绘制传球路径
	CPen passPath(PS_DOT,1,RGB(255,100,100));
	if (m_bTaskPassCatch == true)
	{
		oldpen = dcMemory.SelectObject(&passPath);
		coodinate = m_Point_Field2Img(m_agents[m_nPassPlayer].x,m_agents[m_nPassPlayer].y);
		direct = m_Point_Field2Img(m_agents[m_nCatchPlayer].x,m_agents[m_nCatchPlayer].y);

		dcMemory.MoveTo(coodinate);
		dcMemory.LineTo(direct);
			
		dcMemory.SelectObject(oldpen);
	}
	//中场传球路径[yzz]
	if (m_bTaskMidPassCatch == true)
	{
		oldpen = dcMemory.SelectObject(&passPath);
		coodinate = m_Point_Field2Img(m_agents[m_nMidPassPlayer].x,m_agents[m_nMidPassPlayer].y);
		direct = m_Point_Field2Img(m_agents[m_nMidCatchPlayer].x,m_agents[m_nMidCatchPlayer].y);

		dcMemory.MoveTo(coodinate);
		dcMemory.LineTo(direct);

		dcMemory.SelectObject(oldpen);
	}


	//绘制Ball	
	CBrush orange(RGB(255,155,0));
	oldbrush = dcMemory.SelectObject(&orange);
	dcMemory.SetTextColor(RGB(0,0,0));
	for (i=1;i<7;i++)
	{
		if (m_agents[i].foundball == true)
		{
			//m_bsc.Lock();
			coodinate = m_Point_Field2Img(m_balls[i].x,m_balls[i].y);
			dcMemory.Ellipse(coodinate.x-7,coodinate.y-7,coodinate.x+7,coodinate.y+7);
 			aNum.Format("%d",m_balls[i].ID);
 			dcMemory.TextOut(coodinate.x-4,coodinate.y-8,aNum);
			//m_bsc.Unlock();
		}

	}

	//trueball
	if (m_trueball.x>-100 && m_trueball.y > -100)
	{
		CBrush red(RGB(255,0,0));	
		oldbrush = dcMemory.SelectObject(&red);
		coodinate = m_Point_Field2Img(m_trueball.x,m_trueball.y);
		dcMemory.Ellipse(coodinate.x-7,coodinate.y-7,coodinate.x+7,coodinate.y+7);

		//////////////////////////////////////////////////////////////////////////
// 		CString strTmp;
// 		strTmp.Format("(%d,%d)",
// 			m_Loca_CalAngle(m_trueball.x,m_trueball.y,0,600),
// 			m_Loca_CalAngle(m_trueball.x,m_trueball.y,1800,600)
// 			);
// 		dcMemory.TextOut(coodinate.x+10,coodinate.y+10,strTmp);
		//////////////////////////////////////////////////////////////////////////
	}
	dcMemory.SelectObject(oldbrush);

	//////////////////////////////////////////////////////////////////////////
	/*/ foreball
	if (m_Foreball.x>-100 && m_Foreball.y > -100)
	{
		CBrush green(RGB(0,100,0));	
		oldbrush = dcMemory.SelectObject(&green);
		coodinate = m_Point_Field2Img(m_Foreball.x,m_Foreball.y);
		dcMemory.Ellipse(coodinate.x-7,coodinate.y-7,coodinate.x+7,coodinate.y+7);
	}
	dcMemory.SelectObject(oldbrush);
	/////////////////////////////////////////////////////////////////////////*/

	//status
	dcMemory.SelectObject(&font);
	if (m_strCurSt.FindOneOf("对") == 0)
	{
		dcMemory.SetTextColor(RGB(200,0,0));
	}
	else
	{
		dcMemory.SetTextColor(RGB(0,0,200));
	}
	dcMemory.TextOut(200,10,m_strCurSt);

	//atk
// 	if (m_nAtkNotMove <= 30)
// 	{
// 		aNum.Format("%d号%d秒未移动",m_bakAtkAgent.ID,m_nAtkNotMove);
// 		dcMemory.SetTextColor(RGB(200,0,0));
// 		dcMemory.TextOut(200,320,aNum);
// 	}

	//refbox
	if (bCntRefbox == true)
	{
		dcMemory.SetTextColor(RGB(0,0,200));
		dcMemory.TextOut(200,320,"裁判盒已连接");
	}
	else
	{
		dcMemory.SetTextColor(RGB(250,0,0));
		dcMemory.TextOut(170,320,"注意：未连接裁判盒！");
	}

	//传接球
	if (m_bTaskPassCatch == true)
	{	
		dcMemory.SetTextColor(RGB(255,255,0));
		CString strTask;
		if (m_bTaskPassCatch_kick == false)
		{
			if (m_nPassPlayer == 0 || m_nCatchPlayer == 0)
			{
				strTask.Format("传球任务启动！");
			}
			else
			{
				strTask.Format("%d号向%d号传球！",m_nPassPlayer,m_nCatchPlayer);
			}
		} 
		else
		{
			strTask.Format("球已传出！");
		}
		dcMemory.TextOut(320,10,strTask);
	} 
	//mid传球
	if (m_bTaskMidPassCatch == true)//[yzz]
	{	
		dcMemory.SetTextColor(RGB(255,255,0));
		CString strTask;
		if (m_bTaskMidPassCatch_kick == false)
		{
			if (m_nMidPassPlayer == 0 || m_nMidCatchPlayer == 0)
			{
				strTask.Format("mid传球任务启动！");
			}
			else
			{
				strTask.Format("%d号向%d号传球！",m_nMidPassPlayer,m_nMidCatchPlayer);
			}
		} 
		else
		{
			strTask.Format("球已传出！");
		}
		dcMemory.TextOut(320,10,strTask);
	} 

	//离场时间统计
	bool bSomeInfoToDisp;
	bSomeInfoToDisp = false;
	CString strTimeInfo;
	if (m_nTimeToBeIn > 0)
	{
		strTimeInfo = "";
		for (i=0;i<AG_NUM;i++)
		{
			if (m_prRightOfPlayer[i] == PLAYER_RIGHT_OUT)
			{
				aNum.Format("%d号 ",i);
				strTimeInfo += aNum;
				bSomeInfoToDisp = true;
			}
		}

		aNum.Format("还有%d秒入场；",m_nTimeToBeIn);
		strTimeInfo += aNum;
	}

	for (i=0;i<AG_NUM;i++)
	{
		if (m_prRightOfPlayer[i] == PLAYER_RIGHT_WAITFOEPOINT)
		{
			aNum.Format("%d号 移动到匹配点；",i);
			strTimeInfo += aNum;
			bSomeInfoToDisp = true;
		}

		if (m_prRightOfPlayer[i] == PLAYER_RIGHT_READY)
		{
			aNum.Format("%d号 等待入场时机；",i);
			strTimeInfo += aNum;
			bSomeInfoToDisp = true;
		}
	}

	if (bSomeInfoToDisp == true)
	{
		dcMemory.SetTextColor(RGB(255,255,0));
		dcMemory.TextOut(5,320,strTimeInfo);
	}

	dc.BitBlt(0, 0, 493, 346, &dcMemory, 0, 0, SRCCOPY);
	dcMemory.DeleteDC();
	
	// Do not call CStatic::OnPaint() for painting messages
}

void CUPFullField::m_Clearpa()
{
// 	uagent* t = NULL;
// 	while (m_pa != NULL)
// 	{
// 		t = m_pa;
// 		m_pa = t->next;
// 		delete t;
// 	}
// 	m_nAgentNum = 0;
}
void CUPFullField::SetHoldBallInfo(UINT inID, int inhb)
{
	if (inID <0 || inID>6)
	{
		return;
	}
	if (inhb==1)
	{
		m_agents[inID].isAgHoldBall=true;
	}
	else
	{
		m_agents[inID].isAgHoldBall=false;
	}

}
void CUPFullField::SetAgent(UINT inID, int inx, int iny, double inangle,int inV,int inV_Angle,UINT inStatus,bool infb,int inhb)
{
	if (inID <0 || inID>6)
	{
		return;
	}

	m_asc.Lock();
	m_agents[inID].x = inx;
	m_agents[inID].y = iny;
	m_agents[inID].angle = inangle;
	m_agents[inID].V = inV;
	m_agents[inID].V_Angle = inV_Angle;
	m_agents[inID].status = inStatus;
	m_agents[inID].foundball = infb;
	if (inhb==1)
	m_agents[inID].isAgHoldBall = true;
	else 
		m_agents[inID].isAgHoldBall = false;
	
	// [wzc] & [cyg] 
	/*if (inhb==1)
	{
		m_agents[inID].isAgHoldBall=true;
	}
	else
	{
		m_agents[inID].isAgHoldBall=false;
	}*/
	// [wzc] & [cyg] 

	if (m_agents[inID].online == false)
	{
		m_nAgentNum ++;
		//对新连上的队员发送匹配点位置
		//pToAgent[inID]->SendInitPos(m_fpPlayerIn.x,m_fpPlayerIn.y,0);
		pToAgent[inID]->SetInitPos(m_fpPlayerIn.x,m_fpPlayerIn.y,0);
		//给上线的机器发一条停止指令
		AChangeStatus(inID,CTRL_STOP,0);
		m_agents[inID].online = true;
	}
	m_recvcnt[inID] ++;
	m_asc.Unlock();
	
	//判断是否WaitForPoint
	if (m_prRightOfPlayer[inID] == PLAYER_RIGHT_WAITFOEPOINT)
	{
		if (m_CalDist(m_agents[inID].x,m_agents[inID].y,m_fpPlayerIn.x,m_fpPlayerIn.y) < PLAYER_RIGHT_RANGETOPOINT)
		{
			m_prBeInPoint[inID] ++;
			if (m_prBeInPoint[inID] > 100)
			{
				m_prRightOfPlayer[inID] = PLAYER_RIGHT_INFIELD;
				m_prBeInPoint[inID] = 0;
			}
		}
		else
		{
			m_prBeInPoint[inID] = 0;
		}
	}

// 	if (infb == false)
// 	{
// 		m_ballshot[inID].Ball_Shot_Determine((float)m_agents[inID].x,(float)m_agents[inID].y,0,0,0);
// 	}

//	Invalidate();
}

//DEL CPoint CUPFullField::m_CalPoint(int inx, int iny)
//DEL {
//DEL 	CPoint p;
//DEL 	p.x = inx*400/1800 + 48;
//DEL 	p.y = 305 - iny*266/1200;
//DEL 	//限一下值
//DEL 	if (p.x > 490)
//DEL 	{
//DEL 		p.x = 490;
//DEL 	}
//DEL 	if (p.x <0)
//DEL 	{
//DEL 		p.x =0;
//DEL 	}
//DEL 	if (p.y < 0)
//DEL 	{
//DEL 		p.y = 0;
//DEL 	}
//DEL 	if (p.y > 340)
//DEL 	{
//DEL 		p.y = 340;
//DEL 	}
//DEL 	return p;
//DEL }

void CUPFullField::SetPoints(int *inx, int *iny, int inNum)
{
	if (inx == NULL || iny == NULL)
	{
		return;
	}
	m_px = inx;
	m_py = iny;
	m_pcnt = inNum;

//	Invalidate();
}

void CUPFullField::RemovePoints()
{
	m_px = NULL;
	m_py = NULL;
	m_pcnt = 0;

//	Invalidate();
}
//preballs
void CUPFullField::SetPreball(UINT inID, float jiao,float chang)
{
	if (m_agents[inID].status != ROBST_ERR)
	{
		m_bsc.Lock();
		m_preballs[inID].jiao = jiao*1.0/100;
		m_preballs[inID].chang = chang*1.0/100;
		m_bsc.Unlock();
	}
}
void CUPFullField::SetBall(UINT inID, int inx, int iny)
{
	if (m_agents[inID].status != ROBST_ERR)
	{
		m_bsc.Lock();
		m_balls[inID].x = inx;
		m_balls[inID].y = iny;
		m_balls[inID].updated = true;
		m_bsc.Unlock();
	}
	else
	{
		m_balls[inID].updated = false;
	}
	//m_BufBall[inID].AddPos(inx,iny);
// 	int res = m_ballshot[inID].Ball_Shot_Determine((float)m_agents[inID].x,(float)m_agents[inID].y,(float)inx,(float)iny,(float)1000);
// 	BallShotRes(res,m_ballshot[inID].Shot_ball_point_X,m_ballshot[inID].Shot_ball_point_Y);

// 	uball* tb = m_pb;
// 	while (tb != NULL)
// 	{
// 		if (tb->ID == inID)
// 		{
// 			tb->x = inx;
// 			tb->y = iny;
// 			tb->updated = true;
// 			return;
// 		}
// 		tb =tb->next;
// 	}
// 	tb = new uball;
// 	tb->ID = inID;
// 	tb->x = inx;
// 	tb->y = iny;
// 	tb->updated = true;
// 	tb->next = m_pb;
// 	m_pb = tb;

//	Invalidate();
}

void CUPFullField::m_Clearpb()
{
// 	uball* tb = NULL;
// 	while (m_pb != NULL)
// 	{
// 		tb = m_pb;
// 		m_pb = tb->next;
// 		delete tb;
// 	}
}

void CUPFullField::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint tpoint;
	if (m_bSetPlayerIn == true)
	{
		//更新匹配起始点
		tpoint = m_Point_Img2Field(point.x,point.y);
		m_fpPlayerIn.x = tpoint.x;
		m_fpPlayerIn.y = tpoint.y;
		for (int i=1;i<AG_NUM;i++)
		{
			if (m_agents[i].online == true)
			{
				if (m_eStage == sMani || m_agents[i].status == ROBST_ERR)
				{
					pToAgent[i]->SendInitPos(m_fpPlayerIn.x,m_fpPlayerIn.y,0);
				} 
				else
				{
					pToAgent[i]->SetInitPos(m_fpPlayerIn.x,m_fpPlayerIn.y,0);
				}
			}
		}
		m_bSetPlayerIn = false;
	}
	else
	{	
		//判断点选范围
		int rag = 10;
		for (int i=0;i<7;i++)
		{
			//m_asc.Lock();
			tpoint = m_Point_Field2Img(m_agents[i].x,m_agents[i].y);
			if (point.x>tpoint.x-rag && point.x<tpoint.x+rag && point.y>tpoint.y-rag && point.y<tpoint.y+rag)
			{
				SelID = m_agents[i].ID;
// 				m_self = tpoint;
// 				m_sTp = m_self;
				if (pNum != NULL)
				{
					pNum->Number = SelID;
					pNum->Invalidate();
				}
				//m_asc.Unlock();
				Invalidate();
				return;
			}
			//m_asc.Unlock();
		}

		//计算移动目标点
		if (SelID != 0)
		{
			//移动
			if (m_eStage == sStop)
			{
				ManiMode();
			}
			/*/////////////////////////////////////////////////////////////////////////
			int tx,ty;
			tx = (point.x-48)*18/4;
			ty = (305-point.y)*1200/266;
			AMoveTo(SelID,tx,ty,(int)m_SelAng);
			//直接更新显示出移动目标点
			m_agents[SelID].moveto_x = tx;
			m_agents[SelID].moveto_y = ty;
			/////////////////////////////////////////////////////////////////////////*/
			CPoint targ = m_Point_Img2Field(point.x,point.y);
			if (bMovetoTest == false)
			{
				AMoveTo(SelID,targ.x,targ.y,(int)m_SelAng);
			} 
			else
			{
				AMoveTo(SelID,targ.x,targ.y,nMovetoAng);
				nMovetoAng += 140;
				if (nMovetoAng > 360)
				{
					nMovetoAng = 10;
				}
			}
			//直接更新显示出移动目标点
			m_agents[SelID].moveto_x = targ.x;
			m_agents[SelID].moveto_y = targ.y;
			//Invalidate();
		}
	
		if (m_bSetPlayerIn == true)
		{
			m_fpPlayerIn = m_Point_Img2Field(point.x,point.y);
			m_bSetPlayerIn = false;
		}
		Invalidate();
	}
	CStatic::OnLButtonDown(nFlags, point);
}

void CUPFullField::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SelID = 0;
	if (pNum != NULL)
	{
		pNum->Number = SelID;
		pNum->Invalidate();
	}
	Invalidate();
	CStatic::OnRButtonDown(nFlags, point);
}

void CUPFullField::RemoveAgent(int inID)
{
	m_asc.Lock();
	if (m_agents[inID].online == true)
	{
		m_nAgentNum --;
	}
	m_agents[inID].online = false;
	m_agents[inID].foundball = false;
	m_asc.Unlock();
// 	uagent* t = m_pa;
// 	uagent* pre = t;
// 	while (t != NULL)
// 	{
// 		if (t->ID == inID)
// 		{
// 			pre->next = t->next;
// 			if (t == m_pa)
// 			{
// 				m_pa = t->next;
// 			}
// 			delete t;
// 			t = pre->next;
// 			m_nAgentNum --;
// 			Invalidate();
// 			return;
// 		}
// 		pre = t;
// 		t = t->next;
// 	}
}

void CUPFullField::RemoveBall(UINT inID)
{
// 	uball* tb = m_pb;
// 	uball* pre = tb;
// 	while (tb != NULL)
// 	{
// 		if (tb->ID == inID)
// 		{
// 			pre->next = tb->next;
// 			if (tb == m_pb)
// 			{
// 				m_pb = NULL;
// 			}
// 			delete tb;
// 			Invalidate();
// 			return;
// 		}
// 		pre = tb;
// 		tb = tb->next;
// 	}
}

uagent* CUPFullField::GetAgent(UINT inID)
{
	if (inID < 1 || inID > 6)
	{
		return NULL;
	}

	if (m_agents[inID].online == false)
	{
		return NULL;
	}

	return &m_agents[inID];
// 	uagent* temp = m_pa;
// 	while (temp != NULL)
// 	{
// 		if (temp->ID == inID)
// 		{
// 			return temp;
// 		}
// 		temp = temp->next;
// 	}
// 	return NULL;
}

void CUPFullField::AMoveTo(int ID, int inx, int iny, int inAngle)
{
	if (ID <0 || ID > 6)
	{
		return;
	}

	//发送指令数据
	if (pToAgent[ID] != NULL)
	{
		//如果他距离移动点小于死区范围，则发送stop指令
// 		if (
// 			m_CalDist(m_agents[ID].x,m_agents[ID].y,inx,iny)<MOVETO_DEADZONE && 
// 			(int)abs((int)(m_agents[ID].angle) - inAngle) < MOVETO_ANGLE_DEADZONE
// 			)
// 		{
// 			//处于调节死区，则STOP
// 			AChangeStatus(ID,CTRL_STOP,inAngle);
// 		} 
// 		else
// 		{
// 			pToAgent[ID]->MoveCmd(inx,iny,inAngle,0,m_bField);
// 			//Display
// 			m_ShowTacInfo(CTRL_MOVETO,ID);
// 		}
		//////////////////////////////////////////////////////////////////////////
		pToAgent[ID]->MoveCmd(inx,iny,inAngle,0,m_bField);
		//zbw pre ball
		//pToAgent[ID]->Predictball(tpballjiao,tpballchang);
		//if(goalx!=-1&&goaly!=-1)
		//pToAgent[ID]->FollowPosition(goalx,m_follow[ID].size);
		//Display
		m_ShowTacInfo(CTRL_MOVETO,ID);
		//////////////////////////////////////////////////////////////////////////
		//如果该队员之前处于进攻状态，则缓期切换其状态
// 		if (m_tacagent[ID].status == CTRL_ATTACK)
// 		{
// 			if (m_nSWAtkDly[ID] > 0)
// 			{
// 				m_nSWAtkDly[ID]--;
// 				AChangeStatus(ID,CTRL_ATTACK,0,0);
// 			} 
// 			else
// 			{
// 				pToAgent[ID]->MoveCmd(inx,iny,inAngle,0,m_bField);
// 				m_ResetAtkDly(ID);
// 			}
// 		} 
// 		else
// 		{
// 			//如果他距离移动点小于死区范围，则发送stop指令
// 			if (
// 				m_CalDist(m_agents[ID].x,m_agents[ID].y,inx,iny)<MOVETO_DEADZONE && 
// 				(int)abs((int)(m_agents[ID].angle) - inAngle) < MOVETO_ANGLE_DEADZONE
// 				)
// 			{
// 				//处于调节死区，则STOP
// 				AChangeStatus(ID,CTRL_STOP,inAngle);
// 			} 
// 			else
// 			{
// 				pToAgent[ID]->MoveCmd(inx,iny,inAngle,0,m_bField);
// 				//Display
// 				m_ShowTacInfo(CTRL_MOVETO,ID);
// 			}
// 		}
		//pToAgent[ID]->MoveCmd(inx,iny,inAngle,0,m_bField);
		//标记该agent已经分配角色
		m_tacagent[ID].allocated = true;
	}

	//更新显示
	m_agents[ID].moveto_x = inx;
	m_agents[ID].moveto_y = iny;
}

void CUPFullField::Exec()
{
	
	m_EliminateFalseBall();	//球信息融合
	
	//更新球的参考位置信息
	int i;
	for(i=0;i<7;i++)
	{
		pToAgent[i]->SetRefBall(m_trueball.x,m_trueball.y);
	}

	GetCurTime();			//获取系统时间

	m_chkagnt ++;			//计数，计数值到阈值后检查agents返回的数据包
	if (m_chkagnt > 10)		//每执行10次exec进行一次agents返回数据包统计
	{
		m_CheckAgentReply();
		m_chkagnt = 0;
	}

	
	//m_agents数据稳定了，将他们复制到m_tacagent,后面直接对m_tacagent进行操作
	m_asc.Lock();
	for (i=1;i<7;i++)
	{
		if (m_agents[i].online == true && m_prRightOfPlayer[i] == PLAYER_RIGHT_INFIELD && m_agents[i].status != ROBST_ERR)
		{
			memcpy(&m_tacagent[i],&m_agents[i],sizeof(uagent));
		}
		else
		{
			//掉线、被发出场外，该agent都被认为不在线,但参数还是复制一下
			memcpy(&m_tacagent[i],&m_agents[i],sizeof(uagent));
			m_tacagent[i].online = false;

			if (m_prRightOfPlayer[i] == PLAYER_RIGHT_OUT)
			{
				//被判离场的，解除电机紧锁
				AChangeStatus(i,CTRL_IDLE,0);
			}
		}
	}
	m_asc.Unlock();

	//障碍物信息
	m_ObstFusion();
	m_EliminateFalseObst(); //障碍物信息融合,选出最大障碍物[zbw]
	//////////////////////////////////////////////////////////////////////////
	//为各队员分发传感器信息
// 	int obst_num;
// 	int tmpNum;
// 	obst_num = EACH_AG_OBST_NUM*AG_NUM+1;
// 	for (i=1;i<AG_NUM;i++)
// 	{
// 		if (m_agents[i].online == true)
// 		{
// 			tmpNum = 0;
// 			pToAgent[i]->Clear_Obst();
// 			for(int j=0;j<obst_num;j++)
// 			{
// 				if (m_tacObst[j].valid == true)
// 				{
// 					if (m_tacObst[j].ID != (UINT)abs(i))
// 					{
// 						pToAgent[i]->AddObst(m_tacObst[j].x,m_tacObst[j].y,m_tacObst[j].size);
// 						tmpNum ++;
// 					}
// 				}
// 			}
// 			//pToAgent[i]->Send_Obst();
// 			//////////////////////////////////////////////////////////////////////////
// 			// 			CString str;
// 			// 			str.Format("dbg 向%d号机发送%d个障碍物信息",i,tmpNum);
// 			// 			printf(str);
// 			//////////////////////////////////////////////////////////////////////////
// 		}
// 	}
	//////////////////////////////////////////////////////////////////////////
	
	//在队员间分享信息
	//m_TeammateShare();
	//////////////////////////////////////////////////////////////////////////

	NodeInput* behaviorTreeInput;
	behaviorTreeInput = new NodeInput(pToAgent, m_agents, m_tacagent);
	if (m_eStage == sMani)
	{
		//手动控制模式
	}
	if (m_eStage==midcatch)
	{
		AChangeMidPassStatus(midcatch_n,CTRL_MID_CATCH,midcatch_x,midcatch_y,midcatch_b,0);
	}
	if (m_eStage == sEnter)
	{
		//入场
		Enter();
	}

	if (m_eStage == sOut)
	{
		Leave();
	}

	if (m_eStage == sAUTO)
	{
		//常规比赛状态
		Tac_Default();
	}

	if (m_eStage == sPenaltyReady)
	{
		//预备点球
		Ready_Penalty();
	}
	
	if (m_eStage == sStart)
	{
		//这时候一旦有队员切换到进攻状态，立刻开始自主决策
		for (int i=0;i<7;i++)
		{
			if (m_tacagent[i].online == true && m_tacagent[i].status == CTRL_ATTACK)
			{
				Active();
				CString aa;
				aa.Format("%d----------------------",i);
				printf(aa);
				return;
			}
		}

		switch(m_eLastStage)
		{
		case sKickOffReady:
		case sFreekickReady:
		case sGoalkickReady:
		case sThrowinReady:
		case sCornerKickReady:
			DefaultSTART();
			return;
			break;

		case sAntiKickOffReady:
		case sAntiFreekickReady:
		case sAntiGoalkickReady:
		case sAntiThrowinReady:
		case sAntiCornerKickReady:
		//	printf("未实现！");
			break;

		default:
			break;
		}

		//能执行到这里，都不是我方开球，应该是对方开球，下面的操作是保证至少有一名观察球的队员
		//对方开球Start，但是我方球员还未到位（常见于一些新手裁判）
		int atballID = 0;
	
		m_TacAction(m_tacagent);
	}

	if (m_eStage == sStop)
	{
		for (int i=0;i<7;i++)
		{
			m_tacctrl[i].ctrl = CTRL_STOP;
		}
		m_TacAction(m_tacagent);
	}

	if (m_eStage == sCornerKickReady)
	{
		Ready_CornerKick();
	}

	if (m_eStage == sKickOffReady)
	{
		Ready_KickOff();
	}
	
	if (m_eStage == sFreekickReady)
	{
		Ready_Freekick();
	}
	
	
	if (m_eStage == sGoalkickReady)
	{
		Ready_Goalkick();
	}

	if (m_eStage == sThrowinReady)
	{
		Ready_Throwin();
	}
	
	if (m_eStage == sDroppedBallReady)
	{
		DropBall_Ready(TAC_DIST_DROPBALL);
	}

	if (m_eStage == sPenaltyReady)
	{
		Ready_Penalty();
	}

	//////////////////////////////////////////////////////////////////////////
	if (m_eStage == sAntiKickOffReady)
	{
		Ready_Anti_Kickoff();
	}
	if (m_eStage == sAntiFreekickReady)
	{
		Ready_Anti_Freekick();
	}
	if (m_eStage == sAntiGoalkickReady)
	{
		Ready_Anti_Goalkick();
	}
	if (m_eStage == sAntiThrowinReady)
	{
		Ready_Anti_Throwin();
	}
	if (m_eStage == sAntiCornerKickReady)
	{
		Ready_Anti_CornerKick();
	}
	//////////////////////////////////////////////////////////////////////////
	if (m_eStage == sAntiPenaltyReady)
	{
		Ready_Anti_Penalty();
	}

	//Test
	if (m_eStage == sTestMultiDef)
	{
		Tac_DefTest(m_nDistOfMultiDef);
		//DefaultActive(m_nDistOfMultiDef);
	}

	if (m_eStage == sTestPressDef)
	{
		Test_PressDef();
	}

	if (m_eStage == sTestPassCatch)
	{
		Test_PassCatch();
	}

	/************************************************************************/
	/*                     2010国内技术挑战赛                               */
	/************************************************************************/
	if (m_eStage == sTechChallenge)
	{
		//寻找适合的进攻队员
		CPoint atkpnt;
		atkpnt.x = 1400;
		atkpnt.y = 600;
		int atkid = 0;
		
		m_tacctrl[1].ctrl = CTRL_GOALKEEP;

		if (m_bTCArrived == false)
		{
			atkid = m_Ag_GetNearBy(atkpnt.x,atkpnt.y,m_tacagent);
			if (atkid == 0)
			{
				return;
			}
			m_tacctrl[2].ctrl = CTRL_MOVETO;
			m_tacctrl[2].target_x = atkpnt.x;
			m_tacctrl[2].target_y = atkpnt.y;

			//debug
			CString info;
			info.Format("atkid %d now(%d,%d) target(%d,%d)",atkid,m_tacagent[atkid].x,m_tacagent[atkid].y,atkpnt.x,atkpnt.y);
			printf(info);

			if (m_CalDist(m_tacagent[atkid].x,m_tacagent[atkid].y,atkpnt.x,atkpnt.y) < 50)
			{
				m_bTCArrived = true;
			}
		}
		else
		{
			m_tacctrl[2].ctrl = CTRL_ATTACK;
		}

		for (int i=3;i<AG_NUM;i++)
		{
			m_tacctrl[i].ctrl = CTRL_STOP;
		}
		
		m_TacAction(m_tacagent);
	}
	/************************************************************************/
	/*                     2012国内技术挑战赛                               */
	/************************************************************************/
	if (m_eStage == sTechChallenge2012)
	{
		CString info;
		
		for(int i=1;i<7;i++)
		{
			m_tacctrl[i].ctrl=CTRL_STOP;
		}
		switch(techStep2012)
		{
			case s_start:
				info.Format("s_start\n");
				printf(info);
			if (this->GetStart()==true )//如果没有看到球
			{
				techStep2012=s_search1;
			}
			break;
			case s_search1:
				info.Format("s_search1\n");
				printf(info);

				if (m_bFoundball==false)
				{
					m_tacctrl[2].ctrl=CTRL_SEARCHBALL;
					m_tacctrl[3].ctrl=CTRL_SEARCHBALL;
					m_tacctrl[4].ctrl=CTRL_SEARCHBALL;
					if (m_tacagent[4].x>850)
					{
						m_tacctrl[4].ctrl=CTRL_STOP;
					}
				}
				else
				{
					m_tacctrl[2].ctrl=CTRL_ATTACK;//4号机应该避让球。
					m_Fix_GetOutOfTheWayOfATK();
				}
				
				if (m_bSomeoneHoldBall()==true)
				{
					techStep2012=s_run1;
				}
			break;
			case s_run1:
				info.Format("s_run1\n");
				printf(info);
				techStep2012=s_pass1;
				m_bTaskMidPassCatch=true;
				break;
			case s_pass1:
				info.Format("s_pass1\n");
				printf(info);
				info.Format("before passcatch=%d, passkick=%d\n", m_bTaskMidPassCatch, m_bTaskMidPassCatch_kick);
				printf(info);
			//	m_tacctrl[3].ctrl = CTRL_STOP;
				GoPass(2,4,1);//0622
				m_Fix_GetOutOfTheWayOfATK();
				if (m_bTaskMidPassCatch==false)
				{
					techStep2012 = s_run2;
					m_bTaskMidPassCatch=true;
				}
				break;
			case s_run2:
				m_tacctrl[2].ctrl = CTRL_STOP;
				info.Format("s_run2\n");
				printf(info);
				techStep2012=s_pass2;
				m_bTaskMidPassCatch=true;
				break;
			case s_pass2:
				info.Format("s_pass2\n");
				printf(info);
				info.Format("before passcatch=%d, passkick=%d\n", m_bTaskMidPassCatch, m_bTaskMidPassCatch_kick);
				printf(info);
				GoPass(4,3,0);
				m_Fix_GetOutOfTheWayOfATK();
				if (m_bTaskMidPassCatch==false)
				{
					techStep2012 = s_kick;
					m_bTaskMidPassCatch=true;
				}
				break;
			case s_kick:
				info.Format("s_kick\n");
				printf(info);
				/*if (m_bSomeoneHoldBall()==true)
				{
					info.Format("true\n");
					printf(info);
					m_tacctrl[3].ctrl = CTRL_ATTACK;
				}*/
				m_tacctrl[3].ctrl = CTRL_ATTACK;
				break;
		}
		m_TacAction(m_tacagent);
	}
	if (m_eStage == sTestATKCover)
	{
		//护球测试
		
	}
	
	m_ShowAgentsInfo();

	//debug
// 	CString st;发送
// 	st.Format("m_eStage = %d",m_eStage);
// 	printf(st);
}

void CUPFullField::AChangeStatus(int inID, UINT inCtrlCode, int inAng)
{
	if (inID<0 || inID>6)
	{
		return;
	}

	if (pToAgent[inID] != NULL)
	{
		pToAgent[inID]->CtrlCmd(inCtrlCode,m_trueball.x,m_trueball.y,inAng,0,m_bField);
		//zbw dig_in
		//if(goalx!=-1&&goaly!=-1)
		//pToAgent[inID]->FollowPosition(goalx,m_follow[inID].size);
		//标记该agent已经分配角色
		m_tacagent[inID].allocated = true;
	}

// 	if (inCtrlCode == CTRL_SEARCHBALL)
// 	{
// 		CString str;
// 		str.Format("ID: %d SERCHBALL",inID);
// 		printf(str);
// 	}
// 	CString str;
// 	str.Format("%d号机  行为%d",inID,inCtrlCode);
// 	printf(str);

	//Display
	m_ShowTacInfo(inCtrlCode,inID);
}
void CUPFullField::m_EliminateFalseObst()
{
//zbw dig_in
	int best=0;
	goalx =-1;
	goaly =-1;
	
	for(i=0;i<7;i++){
	if(m_agents[i].online == true&&m_follow[i].size > m_follow[best].size)
		best=i;
	}
	goalx = m_follow[best].x;
	goaly = m_follow[best].y;
	/*
	for (i=1;i<EACH_AG_OBST_NUM*(AG_NUM+1);i++)
	{
		if (m_tacObst[i].valid == true&&m_tacObst[i].size>m_tacObst[i].size)
		{
		best=i;
		}
	}
	goalx =m_tacObst[best].x;
	goaly =m_tacObst[best].y;
	*/
	
}
void CUPFullField::m_EliminateFalseBall()
{
	//检查所有球员返回的球信息
	m_bFoundball = false;
	m_asc.Lock();
	m_bsc.Lock();
	for (int i=1;i<7;i++)
	{
		if (m_agents[i].online == true && m_balls[i].updated == true)
		{	
			//将太明显的场地外干扰去除
			if (m_balls[i].x < -150 || m_balls[i].x > 1950 || m_balls[i].y < -150 || m_balls[i].y > 1350)
			{
				m_agents[i].foundball = false;
				m_balls[i].updated = false;
				continue;
			}

			//计算该在线队员到球的距离
			m_agents[i].dist_ball = (m_agents[i].x - m_balls[i].x)*(m_agents[i].x - m_balls[i].x) + (m_agents[i].y - m_balls[i].y)*(m_agents[i].y - m_balls[i].y);
			//m_agents[i].dist_ball = sqrt(m_agents[i].dist_ball);//yz 10.19
			//若在线的队员里有人更新了球坐标，则开启球判别
			

			if (m_bFoundball == false)
			{
				m_bFoundball = true;
			}
		}
	}
	m_asc.Unlock();
	m_bsc.Unlock();

	if (m_bFoundball == false)
	{
		return;	//都没发现球，没话说了。。。
	}
	
	//寻找离观察者最近的球坐标作为真球坐标
	int x = m_lastball.x;
	int y = m_lastball.y;
	//zbw predict ball
	float jiao = lpballjiao;
	float chang = lpballchang;
	//
	int min_dist = 0x7fffffff;
	m_asc.Lock();
	m_bsc.Lock();
	for (i=1;i<7;i++)
	{
		if (m_agents[i].online == true && m_balls[i].updated == true)
		{
			//判断该队员与球距离是否最小
			if (m_agents[i].dist_ball < min_dist)
			{
				x = m_balls[i].x;
				y = m_balls[i].y;
				min_dist = m_agents[i].dist_ball;
				//
				jiao = m_preballs[i].jiao;
				chang = m_preballs[i].chang;
				//
			}
		}
		m_balls[i].updated = false;
	}
	m_asc.Unlock();
	m_bsc.Unlock();
	
	if (x < 100 || x > 2000 || y <100 || y > 1500)
	{
		m_trueball.x = m_lastball.x;
		m_trueball.y = m_lastball.y;
		tpballjiao=lpballjiao;
		tpballchang= lpballchang;
	}
	
	m_trueball.x = x;
	m_trueball.y = y;
	tpballjiao = jiao;
	tpballchang = chang;
	//test
//	m_Ball_Forecast();

	m_lastball.x = x;
	m_lastball.y = y;
	  
	lpballjiao = jiao;
	lpballchang = chang;

	//更新每个机器人与球的距离
	m_asc.Lock();
	for (i=1;i<7;i++)
	{
		m_agents[i].dist_ball = m_CalDist(m_agents[i].x,m_agents[i].y,m_trueball.x,m_trueball.y);
	}
	m_asc.Unlock();
	
//	Invalidate();
}

void CUPFullField::printf(CString instr)
{
	if (pSndList != NULL)
	{
		if (pSndList->GetCount() > 10000)
		{
			pSndList->ResetContent();
		}
		pSndList->AddString(instr);
		pSndList->SetCurSel(pSndList->GetCount()-1);
	}
}

int CUPFullField::m_CalDist(int inx1, int iny1, int inx2, int iny2)
{
	UINT ret = (inx1-inx2)*(inx1-inx2)+(iny1-iny2)*(iny1-iny2);
	ret = (int)sqrt(static_cast<float>(ret));
	return ret;
}

void CUPFullField::GetCurTime()
{
	m_critiTime.Lock();
	SYSTEMTIME st;
	GetLocalTime(&st);
	m_strTime.Format("%2d:%2d:%2d",st.wHour,st.wMinute,st.wSecond);
	m_critiTime.Unlock();
}

//策略执行部分，动态角色分配就在这里面实现
void CUPFullField::m_TacAction(uagent* inlist)
{
	//////////////////////////////////////////////////////////////////////////
	//09年 加入效用考核的角色分配：由高到低优先级遍历角色队列，依次为每个角色物色最佳的队员，直到所有队员分配完成
	bool bNoOneToAlloc = false;
	int tempdistance;
	//先将所有球员的角色分配标记清掉
	int ai = 1;
	for (ai=1;ai<6;ai++)
	{
		if (m_tacagent[ai].status != CTRL_KICKOFF_SLAVE)
		{
			m_tacagent[ai].allocated = false;
		} 
		else
		{
			m_tacagent[ai].allocated = true;
		}
	}
	//开始分配角色
	int bestID;
	bestID = 0;

	//开球行为用到的
	CPoint tmp;
	CPoint tmpball;
	int tmpangle;
	bool bflip = false;
	int tmpDist;
	int nNoLoop;

	//传接球
	CPoint posPass;

	CString strTmp;
	CString str;
// 	for (int i=1;i<6;i++)
// 	{
// 		str.Format(" %d",m_tacctrl[i].ctrl);
// 		strTmp += str;
// 	}
// 	printf(strTmp);

	
	//printf("CUPFullField::m_TacAction");
// 	if (pTacInfo != NULL)
// 	{
// 		pTacInfo->DeleteAllItems();
// 	}
	m_nLogTacIndex = 0;
	CString info;

	//mc 2012.8.5
	//if (m_bTaskMidPassCatch ==true && m_bTaskMidPassCatch_kick==true)//[yzz]test 先暂停传球者的行动，之后改为让他防守，即延迟传球和接球者的行为
	//{
	//	m_tacagent[m_nMidPassPlayer].allocated=true;
	//	AChangeStatus(m_nMidPassPlayer,CTRL_STOP,0,0);
	//}

	for (int ti=1;ti<6;ti++)
	{
		TRACE("%d %d\n",ti,m_tacctrl[ti].ctrl);
		if(m_eStage==sTechChallenge2012 && ti==1)
		{
			 continue;
		}
		switch(m_tacctrl[ti].ctrl)
		{
			
		/*守门员的挑选*/
		case CTRL_GOALKEEP:
			//1号球员优先
			if (m_tacagent[1].online == true)
			{
				if (m_CalDist(m_tacagent[1].x,m_tacagent[1].y,0,600) < 300)
				{
					AChangeStatus(1,CTRL_GOALKEEP,0,0);
				} 
				else
				{
					AMoveTo(1,0,600,0);
				}
			}
			else
			{
				//1号球员不在，由离球门最近的队员担当守门员(暂不实现，得考虑1号球员通讯暂时中断的情形)
			}
			break;
		case CTRL_MID_PASS:
			bestID = 0;
			for (int i=1;i<7;i++)
			{
				if (m_tacagent[i].online==true && m_tacagent[i].status==CTRL_MID_PASS)
				{
					bestID = i;
					m_tacagent[i].allocated=true;
					break;
				}
			}
			if (m_eStage==sTechChallenge2012)
			{
				bestID= m_nMidPassPlayer;
			}
			if (bestID == 0)
			{
				//已经没有球员可分配了
				bNoOneToAlloc = true;
				break;
			}
			m_nMidPassPlayer = bestID;
			//CString info;
			info.Format("分配中场传球角色id=%d \n",bestID);
			printf(info);
			m_memstatus[bestID] = CTRL_MID_PASS;
			AChangeMidPassStatus(bestID,CTRL_MID_PASS,final_mid_pass_x,final_mid_pass_y,final_mid_pass_believe);
			break;
		case CTRL_MID_CATCH:
			bestID = 0;
			if (m_eStage==sTechChallenge2012)
			{
				bestID= ti;
			}
			else
			bestID = TheBestMidCatch(m_tacagent,final_mid_pass_x,final_mid_pass_y);
			if (bestID == 0)
			{
				//已经没有球员可分配了
				bNoOneToAlloc = true;
				info.Format("没有队员可以分配\n",bestID);
				printf(info);
				break;
			}
			m_nMidCatchPlayer = bestID;
			
			info.Format("分配中场接球角色id=%d \n",bestID);
			printf(info);
			m_memstatus[bestID] = CTRL_MID_CATCH;
			AChangeMidPassStatus(bestID,CTRL_MID_CATCH,final_mid_pass_x,final_mid_pass_y,final_mid_pass_believe,0/*Cal_Angle_Mid_Catch()*/);
			break;
		case CTRL_3M_PASS:
		
			bestID = 0;
			info.Format("3米传球的Pass角色id=%d \n",m_n3mPassPlayer);
			printf(info);
			info.Format("传球朝向)%d,%d)\n",kongx,kongy);
			printf(info);
			bestID = m_n3mPassPlayer;
			m_tacagent[bestID].allocated=true;
			Last3meterPasser=bestID;
		
			//////////////////////////////////////////////////////////////////////////
			//跑位修正,检测其与传球点的距离，太远的话先移动过去
			//printf("//跑位修正,检测其与传球点的距离，太远的话先移动过去");
			//posPass = m_Loca_Ray(m_balls[m_nPassPlayer].x,m_balls[m_nPassPlayer].y,m_agents[m_nCatchPlayer].x,m_agents[m_nCatchPlayer].y,100*m_fKField);
			//posPass = m_Loca_Rotate(posPass.x,posPass.y,m_balls[m_nPassPlayer].x,m_balls[m_nPassPlayer].y,180);
			//int angPass;
			//angPass = m_Loca_CalAngle(posPass.x,posPass.y,m_balls[m_nPassPlayer].x,m_balls[m_nPassPlayer].y);
			//if (m_CalDist(m_agents[m_nPassPlayer].x,m_agents[m_nPassPlayer].y,posPass.x,posPass.y) > 300)	//与移动目标点距离
			//{
			//	AMoveTo(m_nPassPlayer,posPass.x,posPass.y,angPass);
			//	break;
			//}
			//////////////////////////////////////////////////////////////////////////
		//	printf("	APass(bestID,m_tacagent[m_nCatchPlayer].x,m_tacagent[m_nCatchPlayer].y);");
			m_memstatus[bestID] = CTRL_3M_PASS;
			
			APass_Normal(bestID,kongx,kongy);
			//APass_Normal(bestID,1100,900);
			//AChangeMidPassStatus(bestID,CTRL_MID_PASS,final_mid_pass_x,final_mid_pass_y,final_mid_pass_believe);
			break;
		case CTRL_PASS_MOVE:
			bestID = 0;
			//挑选剩下的离移动点最近的队员
			bestID = m_n3mCatchPlayer;
			if (m_n3mCatchPlayer >0 && m_n3mCatchPlayer <6)
			{
				Last3meterCatcher=m_n3mCatchPlayer;
			}
			m_tacagent[bestID].allocated=true;
			m_memstatus[m_n3mCatchPlayer] = CTRL_PASS_MOVE;
			if (bestID == 0)
			{
				//已经没有球员可分配了
				bNoOneToAlloc = true;
				break;
			}
			info.Format("准备接球的机器是：%d",bestID);
			printf(info);
			AMoveTo(bestID,m_tacctrl[ti].target_x,m_tacctrl[ti].target_y,m_tacctrl[ti].target_angle);
			break;
		case CTRL_3M_CATCH:
			//事先指定接球队员
			if (m_n3mCatchPlayer == 0)
			{
				for (int i=2;i<AG_NUM;i++)
				{
					if (m_tacagent[i].online == true && m_tacagent[i].allocated == false && i!=m_n3mPassPlayer)
					{
						m_n3mCatchPlayer = i;
						break;
					}
				}
			}
			if (m_n3mCatchPlayer >0 && m_n3mCatchPlayer <6)
			{
				Last3meterCatcher=m_n3mCatchPlayer;
			}
			else
			{
				//info.Format("3M error !!!!!!!!!!!!!!!!!");
			}
			m_memstatus[m_n3mCatchPlayer] = CTRL_3M_CATCH;

			m_tacagent[m_n3mCatchPlayer].allocated=true;

			tempdistance = ( m_agents[m_n3mCatchPlayer].x - m_trueball.x ) * ( m_agents[m_n3mCatchPlayer].x - m_trueball.x ) + ( m_agents[m_n3mCatchPlayer].y - m_trueball.y ) *( m_agents[m_n3mCatchPlayer].y - m_trueball.y );
			tempdistance = sqrt(tempdistance);

			if (tempdistance < 80)
			{
				m_bClear3meterPassInfo();
				Attack_Convert_Flag=0;
				//Last3meterCatcher=0;
				//Last3meterPasser=0;
				m_bTask3MeterPass=false;
				m_bTask3MeterPass_Kick=false;
				info.Format("球接近了，转进攻！！！ \n");
				printf(info);
				
				break ;
			} 
			
			
			if (tempdistance < 100)
			{
				Flag333=1;
			}
			if (Flag333==1 && tempdistance > 150)
			{
				Flag333=0;
				m_bClear3meterPassInfo();
				Attack_Convert_Flag=0;
				Last3meterCatcher=0;
				Last3meterPasser=0;
				m_bTask3MeterPass=false;
				m_bTask3MeterPass_Kick=false;
				m_n3mCatchPlayer=0;
				m_n3mPassPlayer=0;
				info.Format("球接近又弹开了，转进攻！！！ \n");
				printf(info);
				//Active();
				break;
			}
			


			if (m_agents[m_n3mPassPlayer].x > m_trueball.x + 150 )
			{
				m_bClear3meterPassInfo();
				Attack_Convert_Flag=0;
				Last3meterCatcher=0;
				Last3meterPasser=0;
				m_bTask3MeterPass=false;
				m_bTask3MeterPass_Kick=false;
				m_n3mCatchPlayer=0;
				m_n3mPassPlayer=0;
				info.Format("球反向运动了，转进攻！！！ \n");
				printf(info);
				//Active();
				break;
			}



			//if (m_agents[m_n3mCatchPlayer].x < m_trueball.x )
			//{
			//	m_bClear3meterPassInfo();
			//	Attack_Convert_Flag=0;
			//	Last3meterCatcher=0;
			//	Last3meterPasser=0;
			//	m_bTask3MeterPass=false;
			//	m_bTask3MeterPass_Kick=false;
			//	m_n3mCatchPlayer=0;
			//	m_n3mPassPlayer=0;
			//	info.Format("球越过了接球队员，转进攻！！！ \n");
			//	printf(info);
			//	//Active();
			//	break;
			//}


			
			if (Attack_Convert_Counter>60)//5秒还未接到球
			{
				m_bClear3meterPassInfo();
				Attack_Convert_Flag=0;
				Last3meterCatcher=0;
				Last3meterPasser=0;
				m_bTask3MeterPass=false;
				m_bTask3MeterPass_Kick=false;
				m_n3mCatchPlayer=0;
				m_n3mPassPlayer=0;
				info.Format("3米接球超时，转进攻 \n");
				printf(info);
				//Active(); 
				break ;
			}
			
			
			
			info.Format("分配Catch角色id=%d \n",m_n3mCatchPlayer);
			printf(info);
			info.Format("接球位置(%d,%d)\n",m_tacagent[m_n3mCatchPlayer].x,m_tacagent[m_n3mCatchPlayer].y );
			printf(info);
			ACatch_Normal(m_n3mCatchPlayer,m_agents[m_n3mPassPlayer].x,m_agents[m_n3mPassPlayer].y);
			break;
		/*进攻球员的挑选*/
		case CTRL_ATTACK:
			//对方10秒未开球的话，特殊处理：先跑到不宜出线的位置
			if(m_eStage==sTechChallenge2012)
			{
				bestID = ti;
				AChangeStatus(bestID,CTRL_ATTACK,0,0);
			}
			else
			if (m_b10SecEnd == false)
			{
				bestID = 0;
				bestID = TheBestAtk(m_tacagent);
				if (bestID == 0)
				{
					//已经没有球员可分配了
					bNoOneToAlloc = true;
					break;
				}
				//CString info;
				info.Format("分配进攻角色id=%d \n",bestID);
				printf(info);
				AChangeStatus(bestID,CTRL_ATTACK,0,0);
			} 
			else
			{	
				//m_b10SecEnd == true 对方10秒未动
// 				tmp = m_Loca_Ray(m_trueball.x,m_trueball.y,1600,600,100);
// 				tmp = m_Loca_Rotate(tmp.x,tmp.y,m_trueball.x,m_trueball.y,180);
// 				tmpangle = m_Loca_CalAngle(tmp.x,tmp.y,1600,600);
// 				bestID = m_Ag_GetNearBy(tmp.x,tmp.y,m_tacagent);
// 				AMoveTo(bestID,tmp.x,tmp.y,tmpangle);
// 				if (m_CalDist(tmp.x,tmp.y,m_tacagent[bestID].x,m_tacagent[bestID].y) < 50)
// 				{
// 					m_b10SecEnd = false;
// 				} 
// 				if (m_CalDist(m_trueball.x,m_trueball.y,m_BallLogAt10SecEnd.x,m_BallLogAt10SecEnd.y) > 150)
// 				{
// 					 m_b10SecEnd = false;
// 				}
// 				printf("对方10秒未动");
				//////////////////////////////////////////////////////////////////////////
				bestID = 0;
				bestID = TheBestAtk(m_tacagent);
				if (bestID == 0)
				{
					//已经没有球员可分配了
					bNoOneToAlloc = true;
					break;
				}
				AChangeStatus(bestID,CTRL_ATTACK,0,0);
				//////////////////////////////////////////////////////////////////////////
			}
			//////////////////////////////////////////////////////////////////////////
			//传球延时
			if (m_nPassEndDelay > 0)
			{
				if (bestID == m_nPassPlayer)
				{
					//传球延时后被切换成进攻，则延时记录消零
					m_nPassPlayer = 0;
					m_nPassEndDelay = 0;
				}
			}
			//////////////////////////////////////////////////////////////////////////
			break;

		/*定点移动*/
		case CTRL_MOVETO:
			bestID = 0;
			if(m_eStage==sTechChallenge2012)
			{
				AMoveTo(ti,m_tacctrl[ti].target_x,m_tacctrl[ti].target_y,m_tacctrl[ti].target_angle);
			}
			if (ti == 1)
			{
				AMoveTo(1,m_tacctrl[ti].target_x,m_tacctrl[ti].target_y,m_tacctrl[ti].target_angle);
				continue;
			}
			//挑选剩下的离移动点最近的队员
			bestID = m_Ag_GetNearBy(m_tacctrl[ti].target_x,m_tacctrl[ti].target_y,m_tacagent);
			if (bestID == 0)
			{
				//已经没有球员可分配了
				bNoOneToAlloc = true;
				break;
			}
			//////////////////////////////////////////////////////////////////////////
			//传球延时
			if (bestID == m_nPassPlayer)
			{
				if (m_nPassEndDelay > 0)
				{
					m_nPassEndDelay --;
					AChangeStatus(bestID,CTRL_STOP,0,0);
					break;
				}
				else
				{
					//延时结束，清空记录
					//m_nPassPlayer = 0;
					m_nPassEndDelay=0;//[yzz]进行修改，因为会在发球、点球时，覆盖传球和接球人，别的行为应该也有这个问题
				}
			}
			
			
			//////////////////////////////////////////////////////////////////////////
			AMoveTo(bestID,m_tacctrl[ti].target_x,m_tacctrl[ti].target_y,m_tacctrl[ti].target_angle);
			break;

		/*定距离防守*/
		case CTRL_DEFENCE:
			//先维护防守队员稳定性
			bestID = 0;
//			int i;
// 			for (i=0;i<AG_NUM;i++)
// 			{
// 				if (m_tacagent[i].online == true && m_tacagent[i].allocated == false)
// 				{
// 					if (m_tacagent[i].status == CTRL_DEFENCE && m_tacagent[i].logDefAngle == m_tacctrl[ti].def_angle)
// 					{
// 						bestID = i;
// 						break;
// 					}
// 				}
// 			}
			if (bestID <= 0)
			{
				//如果之前没有以这个角度防守的队员，则挑选离防守点最近的队员
				tmp = m_Loca_Ray(m_trueball.x,m_trueball.y,0,600,m_tacctrl[ti].def_dist);
				if (m_tacctrl[ti].def_angle != 0)
				{
					tmp = m_Loca_Rotate(tmp.x,tmp.y,m_trueball.x,m_trueball.y,m_tacctrl[ti].def_angle);
				}
				bestID = m_Ag_GetNearBy(tmp.x,tmp.y,m_tacagent);
			}
			
			if (bestID == 0)
			{
				//已经没有球员可分配了
				bNoOneToAlloc = true;
				break;
			}
			//////////////////////////////////////////////////////////////////////////
			//传球延时
			if (bestID == m_nPassPlayer )
			{
				if (m_nPassEndDelay > 0)
				{
					m_nPassEndDelay --;
					AChangeStatus(bestID,CTRL_STOP,0,0);
					break;
				}
				else
				{
					//m_nPassEndDelay=0;
					//延时结束，清空记录
					m_nPassPlayer = 0;
				}
			}
			//////////////////////////////////////////////////////////////////////////
			AChangeStatus(bestID,m_tacctrl[ti].ctrl,m_tacctrl[ti].def_angle,m_tacctrl[ti].def_dist);
			break;

		/*高级防守*/
		case CTRL_PRODEF:
			//挑选剩下的离球最近的队员
			bestID = m_Ag_GetNearBy(m_trueball.x,m_trueball.y,m_tacagent);
			if (bestID == 0)
			{
				//已经没有球员可分配了
				bNoOneToAlloc = true;
				break;
			}

			//计算防守的距离
			tmpDist = m_CalDist(m_trueball.x,m_trueball.y,m_tacagent[bestID].x,m_tacagent[bestID].y);
			if (tmpDist > (m_tacctrl[ti].def_dist+100))
			{
				tmpDist = 300;
			} 
			else
			{
				tmpDist = m_tacctrl[ti].def_dist;
			}

			//计算移动点坐标
			tmp = m_Loca_Ray(m_trueball.x,m_trueball.y,0,600,(int)(tmpDist*m_fKField));
			m_tacctrl[ti].target_x = tmp.x;
			m_tacctrl[ti].target_y = tmp.y;

			//////////////////////////////////////////////////////////////////////////
			if (m_Check_MovetoOurPenaltyArea(m_tacctrl[ti]) == true)
			{
				m_Fix_MovetoOurPenaltyArea(&m_tacctrl[ti]);
			}

			if (m_Check_MoveOutOffFrame(m_tacctrl[ti]) == true)
			{
				m_Fix_MoveOutOffFrame(&m_tacctrl[ti]);
			}
			//////////////////////////////////////////////////////////////////////////
			
			AMoveTo(bestID,m_tacctrl[ti].target_x,m_tacctrl[ti].target_y,0);
			break;

		/*传球*/
		case CTRL_PASS:
			//////////////////////////////////////////////////////////////////////////
			//开球后的避让修正
			if (m_bTaskPassCatch == true && m_bTaskPassCatch_kick == true)
			{
				//printf("if (m_bTaskPassCatch == true && m_bTaskPassCatch_kick == true)");
				if (m_nPassPlayer > 0 && m_nCatchPlayer >0 && m_nPassPlayer != m_nCatchPlayer)
				{
					//printf("if (m_nPassPlayer > 0 && m_nCatchPlayer >0 && m_nPassPlayer != m_nCatchPlayer)");
					//传球者尽量不挡住接球者射门视野
					tmp.x = m_agents[m_nPassPlayer].x;
					tmp.y = m_agents[m_nPassPlayer].y;
					//////////////////////////////////////////////////////////////////////////
					/*/退避方式一
					if (m_agents[m_nPassPlayer].y > 600)
					{
						tmp.y = 0;
					} 
					else
					{
						tmp.y = 1200;
					}
					/////////////////////////////////////////////////////////////////////////*/
					//退避方式二
// 					if (m_agents[m_nPassPlayer].x < m_balls[m_nPassPlayer].x)
// 					{	
// 						//向对方半场传球
// 						tmp.x = m_trueball.x;
// 						
// 					}
// 					else
// 					{
// 						//向己方半场传球
// 						tmp.x = m_balls[m_nPassPlayer].x - 200;
// 					}
// 
// 					if (m_trueball.y > 600)
// 					{					
// 						tmp.y = 1250;
// 					} 
// 					else
// 					{
// 						tmp.y = -50;
// 					}

					//////////////////////////////////////////////////////////////////////////
					if (m_agents[m_nPassPlayer].x < m_balls[m_nPassPlayer].x)
					{	
						//向对方半场传球
						AChangeStatus(m_nPassPlayer,CTRL_STOP,0,0);
						break;
					}
					else
					{
						//向己方半场传球，冲向对方防守位置
						tmp = m_Loca_Ray(m_trueball.x,m_trueball.y,1700,600,500);
						//避免冲进禁区
// 						if (tmp.x > 1700)
// 						{
// 							if (tmp.y > 225 && tmp.y < 975)
// 							{
// 								if (tmp.y > 600)
// 								{
// 									tmp.y = 1000;
// 								}
// 								else
// 								{
// 									tmp.y < 200;
// 								}
// 							}
// 						}
					}

					//////////////////////////////////////////////////////////////////////////
// 					int angToGoal,angToPass;
// 					angToGoal = m_Loca_CalAngle(m_agents[m_nCatchPlayer].x,m_agents[m_nCatchPlayer].y,1800,600);
// 					angToPass = m_Loca_CalAngle(m_agents[m_nCatchPlayer].x,m_agents[m_nCatchPlayer].y,m_agents[m_nPassPlayer].x,m_agents[m_nPassPlayer].y);
// 					if (angToPass > angToGoal)
// 					{
// 						tmp = m_Loca_Ray(m_agents[m_nCatchPlayer].x,m_agents[m_nCatchPlayer].y,1800,600,300*m_fKField);
// 						tmp = m_Loca_Rotate(tmp.x,tmp.y,m_agents[m_nCatchPlayer].x,m_agents[m_nCatchPlayer].y,-45);
// 					} 
// 					else
// 					{
// 						tmp = m_Loca_Ray(m_agents[m_nCatchPlayer].x,m_agents[m_nCatchPlayer].y,1800,600,300*m_fKField);
// 						tmp = m_Loca_Rotate(tmp.x,tmp.y,m_agents[m_nCatchPlayer].x,m_agents[m_nCatchPlayer].y,45);
// 					}
// 					if (m_CalDist(tmp.x,tmp.y,0,0) > 10000)
// 					{
// 						printf("pass errr!");
// 					}
					tmpangle = m_Loca_CalAngle(tmp.x,tmp.y,m_trueball.x,m_trueball.y);
					AMoveTo(m_nPassPlayer,tmp.x,tmp.y,tmpangle);
					break;
				}
			}
			//////////////////////////////////////////////////////////////////////////
			//如果之前指定传球队员，则延续角色稳定性
			//printf("//如果之前指定传球队员，则延续角色稳定性");
			bestID = 0;
			info.Format("分配时 他的Pass角色id=%d \n",m_nPassPlayer);
			printf(info);
			if (m_nPassPlayer>0)
			{
				bestID = m_nPassPlayer;
			}
			else
			{
				bestID = TheBestAtk(m_tacagent);
				if (bestID == 0)
				{
					//已经没有球员可分配了
					bNoOneToAlloc = true;
					break;
				}
				m_nPassPlayer = bestID;
			}
			//////////////////////////////////////////////////////////////////////////
			//跑位修正,检测其与传球点的距离，太远的话先移动过去
			//printf("//跑位修正,检测其与传球点的距离，太远的话先移动过去");
			posPass = m_Loca_Ray(m_balls[m_nPassPlayer].x,m_balls[m_nPassPlayer].y,m_agents[m_nCatchPlayer].x,m_agents[m_nCatchPlayer].y,100*m_fKField);
			posPass = m_Loca_Rotate(posPass.x,posPass.y,m_balls[m_nPassPlayer].x,m_balls[m_nPassPlayer].y,180);
			int angPass;
			angPass = m_Loca_CalAngle(posPass.x,posPass.y,m_balls[m_nPassPlayer].x,m_balls[m_nPassPlayer].y);
			if (m_CalDist(m_agents[m_nPassPlayer].x,m_agents[m_nPassPlayer].y,posPass.x,posPass.y) > 300)	//与移动目标点距离
			{
				AMoveTo(m_nPassPlayer,posPass.x,posPass.y,angPass);
				break;
			}
			//////////////////////////////////////////////////////////////////////////
		//	printf("	APass(bestID,m_tacagent[m_nCatchPlayer].x,m_tacagent[m_nCatchPlayer].y);");
			info.Format("分配Pass角色id=%d \n",bestID);
			printf(info);
			APass(bestID,m_tacagent[m_nCatchPlayer].x,m_tacagent[m_nCatchPlayer].y);
		//	printf("break");
			break;

		/*接球*/
		case CTRL_CATCH:
			//事先指定接球队员
			if (m_nCatchPlayer == 0)
			{
				for (int i=2;i<AG_NUM;i++)
				{
					if (m_tacagent[i].online == true && m_tacagent[i].allocated == false && i!=m_nPassPlayer)
					{
						m_nCatchPlayer = i;
						break;
					}
				}
			}
			//时间未到的时候会一直break;
			if (m_bCatchDelay == true)
			{
				printf("接球延时中");
				AChangeStatus(m_nCatchPlayer,CTRL_STOP,0,0);
				break;
			}
			//实在找不到能接球的人话，传球者就自己切到进攻
			if (m_nCatchPlayer == 0)
			{
				
				info.Format("实在找不到能接球的人话，传球者就自己切到进攻!");
				printf(info);
				AChangeStatus(m_nCatchPlayer,CTRL_ATTACK,0,0);
				
			}
			else
			{		
			info.Format("分配Catch角色id=%d \n",m_nCatchPlayer);
				printf(info);
				ACatch(m_nCatchPlayer,m_tacagent[m_nPassPlayer].x,m_tacagent[m_nPassPlayer].y);
			}
			break;
		/*****环绕球*****/
		case CTRL_AROUNDBALL:
			//根据trueball计算大概的移动目标点
			tmp = m_Loca_Ray(m_trueball.x,m_trueball.y,m_tacctrl[ti].target_x,m_tacctrl[ti].target_y,m_tacctrl[ti].def_dist);
			tmp = m_Loca_Rotate(tmp.x,tmp.y,m_trueball.x,m_trueball.y,m_tacctrl[ti].def_angle);			
			//针对出界情况进行修正
			
			//上边界
			nNoLoop = 100;
			while (tmp.y > 1300)
			{
				tmp = m_Loca_Rotate(tmp.x,tmp.y,m_trueball.x,m_trueball.y,-10);
				nNoLoop -- ;
				if (nNoLoop < 0)
				{
					break;
				}
			}
			
			//下边界
			nNoLoop = 100;
			while (tmp.y < -50)
			{
				tmp = m_Loca_Rotate(tmp.x,tmp.y,m_trueball.x,m_trueball.y,10);
				nNoLoop -- ;
				if (nNoLoop < 0)
				{
					break;
				}
			}
			
			//左边界(己方)
			nNoLoop = 100;
			while (tmp.x < -50 || (tmp.x < 250 && tmp.y < 925 && tmp.y > 275))
			{
				//如果是禁区，考虑禁区内受优待的名额
				if (
					(tmp.x < 250 && tmp.y < 925 && tmp.y > 275) &&	//跑禁区里了禁区范围
					(m_nAgCanBeInPenaltyArea > 0)					//禁区优待名额还没用完
					)
				{
					if (m_trueball.x > 225 && m_trueball.y < 900 && m_trueball.y > 300)//球位置禁区右边
					{
						tmp.x = 200;
						tmp.y = m_trueball.y;
					}
					else
					{
						//m_trueball.x < 225
						if (m_trueball.y > 925)
						{
							tmp.x = m_trueball.x;
							tmp.y = 900;
						}
						if (m_trueball.y < 275)
						{
							tmp.x = m_trueball.x;
							tmp.y = 300;
						}
						if (m_trueball.y > 925 && m_trueball.x > 225)
						{
							tmp.x = 200;
							tmp.y = 900;
						}
						if (m_trueball.y <300 && m_trueball.x > 225)
						{
							tmp.x = 200;
							tmp.y = 300;
						}
					}
					break;	//优待名额，不用再旋转了
				}
				
				//根据旋转中心位置来判断上下旋转
				if (m_trueball.y >600)
				{
					tmp = m_Loca_Rotate(tmp.x,tmp.y,m_trueball.x,m_trueball.y,-10);
				} 
				else
				{
					tmp = m_Loca_Rotate(tmp.x,tmp.y,m_trueball.x,m_trueball.y,10);
				}
				nNoLoop -- ;
				if (nNoLoop < 0)
				{
					break;
				}
			} 
			
			//右边界(对方)
			nNoLoop = 100;
			while (tmp.x > 1900 || (tmp.x > 1575 && tmp.y > 275 && tmp.y < 925))
			{
				//根据旋转中心位置来判断上下旋转
				if (m_trueball.y >600)
				{
					tmp = m_Loca_Rotate(tmp.x,tmp.y,m_trueball.x,m_trueball.y,10);
				} 
				else
				{
					tmp = m_Loca_Rotate(tmp.x,tmp.y,m_trueball.x,m_trueball.y,-10);
				}
				nNoLoop -- ;
				if (nNoLoop < 0)
				{
					break;
				}
			} 

			//////////////////////////////////////////////////////////////////////////
			//!!避开己方队友，尚未测试
// 			nNoLoop = 100;
// 			int nDistToPeer;
// 			nDistToPeer = 0;
// 			while(nDistToPeer < 100)
// 			{
// 				//遍历agent，计算距离，只要与任何一名队员距离过近，都要想办法偏离
// 				for (int i=1;i<AG_NUM;i++)
// 				{
// 					if (m_agents[i].online == true)
// 					{
// 						nDistToPeer = m_CalDist(tmp.x,tmp.y,m_agents[i].x,m_agents[i].y);
// 						if (nDistToPeer < 100)
// 						{
// 							//根据旋转中心位置来判断上下旋转
// 							if (m_trueball.y >600)
// 							{
// 								tmp = m_Loca_Rotate(tmp.x,tmp.y,m_trueball.x,m_trueball.y,-10);
// 							} 
// 							else
// 							{
// 								tmp = m_Loca_Rotate(tmp.x,tmp.y,m_trueball.x,m_trueball.y,10);
// 							}
// 						}
// 					}
// 				}
// 			}
			//////////////////////////////////////////////////////////////////////////

			/************************************************************************/
			/*            下面才是真正开始计算单个球员的位置                        */
			/************************************************************************/
			//寻找合适的队员
			bestID = m_Ag_GetNearBy(tmp.x,tmp.y,m_tacagent);
			if (bestID <= 0 || bestID >AG_NUM)
			{
				//找不到合适的队员，算了
				break;
			}
			//如果该队员看不到球，则以全局球坐标为准
			if (m_agents[bestID].foundball == false)
			{
				m_balls[bestID].x = m_trueball.x;
				m_balls[bestID].y = m_trueball.y;
			}
			bestID = m_Ag_GetNearBy(tmp.x,tmp.y,m_tacagent);
			if (bestID == 0)
			{
				//已经没有球员可分配了
				bNoOneToAlloc = true;
				break;
			}
			tmp = m_Loca_Ray(m_balls[bestID].x,m_balls[bestID].y,m_tacctrl[ti].target_x,m_tacctrl[ti].target_y,m_tacctrl[ti].def_dist);
			tmp = m_Loca_Rotate(tmp.x,tmp.y,m_balls[bestID].x,m_balls[bestID].y,m_tacctrl[ti].def_angle);
			//上边界
			nNoLoop = 50;
			while (tmp.y > 1300)
			{
				tmp = m_Loca_Rotate(tmp.x,tmp.y,m_balls[bestID].x,m_balls[bestID].y,-10);
				nNoLoop -- ;
				if (nNoLoop < 0)
				{
					break;
				}
			}
			
			//下边界
			nNoLoop = 50;
			while (tmp.y < -50)
			{
				tmp = m_Loca_Rotate(tmp.x,tmp.y,m_balls[bestID].x,m_balls[bestID].y,10);
				nNoLoop -- ;
				if (nNoLoop < 0)
				{
					break;
				}
			}
			
			//左边界(己方)
			nNoLoop = 100;
			while (tmp.x < -50 || (tmp.x < 250 && tmp.y < 925 && tmp.y > 275))
			{
				//如果是禁区，考虑禁区内受优待的名额
				if (
					(tmp.x < 250 && tmp.y < 925 && tmp.y > 275) &&	//跑禁区里了禁区范围
					(m_nAgCanBeInPenaltyArea > 0)					//禁区优待名额还没用完
					)
				{
					if (m_balls[bestID].x > 225 && m_balls[bestID].y < 900 && m_balls[bestID].y > 300)//球位置禁区右边
					{
						tmp.x = 200;
						tmp.y = m_balls[bestID].y;
					}
					else
					{
						//m_balls[bestID].x < 225
						if (m_balls[bestID].y > 925)
						{
							tmp.x = m_balls[bestID].x;
							tmp.y = 900;
						}
						if (m_balls[bestID].y < 275)
						{
							tmp.x = m_balls[bestID].x;
							tmp.y = 300;
						}
						if (m_balls[bestID].y > 925 && m_balls[bestID].x > 225)
						{
							tmp.x = 200;
							tmp.y = 900;
						}
						if (m_balls[bestID].y <300 && m_balls[bestID].x > 225)
						{
							tmp.x = 200;
							tmp.y = 300;
						}
					}
					m_nAgCanBeInPenaltyArea --;
					break;	//优待名额，不用再旋转了
				}
				
				//根据旋转中心位置来判断上下旋转
				if (m_trueball.y >600)
				{
					tmp = m_Loca_Rotate(tmp.x,tmp.y,m_balls[bestID].x,m_balls[bestID].y,-10);
				} 
				else
				{
					tmp = m_Loca_Rotate(tmp.x,tmp.y,m_balls[bestID].x,m_balls[bestID].y,10);
				}
				nNoLoop -- ;
				if (nNoLoop < 0)
				{
					break;
				}
			} 
			
			//右边界
			nNoLoop = 100;
			while (tmp.x > 1900|| (tmp.x > 1575 && tmp.y > 275 && tmp.y < 925))
			{
				//根据旋转中心位置来判断上下旋转
				if (m_trueball.y >600)
				{
					tmp = m_Loca_Rotate(tmp.x,tmp.y,m_balls[bestID].x,m_balls[bestID].y,10);
				} 
				else
				{
					tmp = m_Loca_Rotate(tmp.x,tmp.y,m_balls[bestID].x,m_balls[bestID].y,-10);
				}
				nNoLoop -- ;
				if (nNoLoop < 0)
				{
					break;
				}
			}
			//////////////////////////////////////////////////////////////////////////
			//!!避开己方队友，尚未测试
 		//	nNoLoop = 100;
 		//	nDistToPeer = 0;
			//int ddd=10;
 		//	while((nDistToPeer < 100) && (ddd-->0))
 		//	{
 		//		//遍历agent，计算距离，只要与任何一名队员距离过近，都要想办法偏离
 		//		for (int i=1;i<AG_NUM;i++)
 		//		{
 		//			if (m_agents[i].online == true)
 		//			{
 		//				nDistToPeer = m_CalDist(tmp.x,tmp.y,m_agents[i].x,m_agents[i].y);
 		//				if (nDistToPeer < 100)
 		//				{
 		//					//根据旋转中心位置来判断上下旋转
 		//					if (m_balls[bestID].y >600)
 		//					{
 		//						tmp = m_Loca_Rotate(tmp.x,tmp.y,m_balls[bestID].x,m_balls[bestID].y,-10);
 		//					} 
 		//					else
 		//					{
 		//						tmp = m_Loca_Rotate(tmp.x,tmp.y,m_balls[bestID].x,m_balls[bestID].y,10);
 		//					}
 		//				}
 		//			}
 		//		}
 		//	}
			//////////////////////////////////////////////////////////////////////////
			
			tmpangle = m_Loca_CalAngle(tmp.x,tmp.y,m_balls[bestID].x,m_balls[bestID].y);
			AMoveTo(bestID,tmp.x,tmp.y,tmpangle);
			break;

		/*****堵截球*****/
		case CTRL_BLOCK:
			bestID = 0;
			//挑选剩下的离球最近的队员
			bestID = m_Ag_GetNearBy(m_trueball.x,m_trueball.y,m_tacagent);
			if (bestID == 0)
			{
				//已经没有球员可分配了
				bNoOneToAlloc = true;
				break;
			}
			
			//计算堵截的距离
			tmpDist = m_CalDist(m_trueball.x,m_trueball.y,m_tacagent[bestID].x,m_tacagent[bestID].y);
			if (tmpDist > (m_tacctrl[ti].def_dist+100))
			{
				tmpDist = 300;
			} 
			else
			{
				tmpDist = m_tacctrl[ti].def_dist;
			}
			
			//计算移动点坐标
			tmp = m_Loca_Ray(m_Foreball.x,m_Foreball.y,m_trueball.x,m_trueball.y,(int)(tmpDist*m_fKField));
			tmp = m_Loca_Rotate(tmp.x,tmp.y,m_Foreball.x,m_Foreball.y,180);

			m_tacctrl[ti].target_x = tmp.x;
			m_tacctrl[ti].target_y = tmp.y;
			
			//////////////////////////////////////////////////////////////////////////
			if (m_Check_MovetoOurPenaltyArea(m_tacctrl[ti]) == true)
			{
				m_Fix_MovetoOurPenaltyArea(&m_tacctrl[ti]);
			}
			
			if (m_Check_MoveOutOffFrame(m_tacctrl[ti]) == true)
			{
				m_Fix_MoveOutOffFrame(&m_tacctrl[ti]);
			}
			//////////////////////////////////////////////////////////////////////////
			
			AMoveTo(bestID,m_tacctrl[ti].target_x,m_tacctrl[ti].target_y,0);
			break;

		/****进攻跑位****/
		case CTRL_SHIFTATK:
			//挑选剩下的离球最近的队员
			bestID = m_Ag_GetNearBy(m_trueball.x,m_trueball.y,m_tacagent);
			if (bestID == 0)
			{
				//已经没有球员可分配了
				bNoOneToAlloc = true;
				break;
			}
			
			//计算防守的距离
			tmpDist = m_CalDist(m_Foreball.x,m_Foreball.y,m_tacagent[bestID].x,m_tacagent[bestID].y);
			if (tmpDist > (m_tacctrl[ti].def_dist+100))
			{
				tmpDist = 400;
			} 
			else
			{
				tmpDist = m_tacctrl[ti].def_dist;
			}
			
			//计算移动点坐标
			tmp = m_Loca_Ray(m_Foreball.x,m_Foreball.y,1800,600,(int)(tmpDist*m_fKField));
			tmp = m_Loca_Rotate(tmp.x,tmp.y,m_Foreball.x,m_Foreball.y,180); 
			m_tacctrl[ti].target_x = tmp.x;
			m_tacctrl[ti].target_y = tmp.y;
			
			//////////////////////////////////////////////////////////////////////////
			if (m_Check_MovetoOurPenaltyArea(m_tacctrl[ti]) == true)
			{
				m_Fix_MovetoOurPenaltyArea(&m_tacctrl[ti]);
			}
			
			if (m_Check_MoveOutOffFrame(m_tacctrl[ti]) == true)
			{
				m_Fix_MoveOutOffFrame(&m_tacctrl[ti]);
			}
			//////////////////////////////////////////////////////////////////////////
			
			AMoveTo(bestID,m_tacctrl[ti].target_x,m_tacctrl[ti].target_y,0);
			break;
         //紧盯防守角色，需要离目标敌人近的角色
		case CTRL_FOLLOWPOSITION:
			bestID = m_Ag_GetStatus(CTRL_FOLLOWPOSITION,m_tacagent,false);	//尽量维持角色稳定性,寻找未分配的该角色
			if (bestID > 0)
			{
				//直接切换
				AFollowPosition(bestID,CTRL_FOLLOWPOSITION,m_tacctrl[ti].target_x,m_tacctrl[ti].target_y);
			}
			else
			{
				bestID = m_Ag_GetNearBy(goalx,goaly,m_tacagent);  //距离目标点近的优先
				AFollowPosition(bestID,CTRL_FOLLOWPOSITION,0,0);
			}
		/*球观察角色，需要离球近的角色*/
		case CTRL_ATTENTIONBALL:
			bestID = m_Ag_GetStatus(CTRL_ATTENTIONBALL,m_tacagent,false);	//尽量维持角色稳定性,寻找未分配的该角色
			if (bestID > 0)
			{
				//直接切换
				AChangeStatus(bestID,CTRL_ATTENTIONBALL,m_tacctrl[ti].target_x,m_tacctrl[ti].target_y);
			}
			else
			{
				bestID = m_Ag_GetNearBy(m_trueball.x,m_trueball.y,m_tacagent);  //距离目标点近的优先
				AChangeStatus(bestID,CTRL_ATTENTIONBALL,0,0);
			}

			//////////////////////////////////////////////////////////////////////////
// 			if (bestID != 0 && bestID == m_Ag_GetMoveTo(m_tacctrl[ti].target_x,m_tacctrl[ti].target_y,m_tacagent))
// 			{
// 				//直接切换
// 				AChangeStatus(bestID,CTRL_ATTENTIONBALL,m_tacctrl[ti].target_x,m_tacctrl[ti].target_y);			
// 			}
// 			else
// 			{
// 				//没有人之前扮演过此角色，开始拆选合格人选
// 				bestID = m_Ag_GetNearBy(m_tacctrl[ti].target_x,m_tacctrl[ti].target_y,m_tacagent);  //距离目标点近的优先
// 				if (m_CalDist(m_tacagent[bestID].x,m_tacagent[bestID].y,m_tacctrl[ti].target_x,m_tacctrl[ti].target_y) > 50*m_fKField)
// 				{
// 					//距离较大时，移动过去
// 					AMoveTo(bestID,m_tacctrl[ti].target_x,m_tacctrl[ti].target_y,m_tacctrl[ti].target_angle);
// 					// 					m_strTmp.Format("%d号距离观察点较远，移动过去",bestID);
// 					// 					printf(m_strTmp);
// 					break;
// 				}
// 				else
// 				{
// 					//距离够近了,切换到
// 					AChangeStatus(bestID,CTRL_ATTENTIONBALL,0,0);
// 				}
// 			}
			//////////////////////////////////////////////////////////////////////////

			break;

		case CTRL_KICKOFF_PRIM_READY:
			//1、moveto 方式
			//维持角色稳定性
			if (
				m_curKOPrim != 0 && 
				m_tacagent[m_curKOPrim].online == true
				)
			{
				bestID = m_curKOPrim;
			} 
			else
			{
				//找出离球最近的队员
				bestID = m_Ag_GetNearBy(m_trueball.x,m_trueball.y,m_tacagent);
				
			}
			//尽量以其自己看到的球作为移动基准，避免间距过大
			if (m_tacagent[bestID].foundball == true)
			{
				tmpball.x = m_balls[bestID].x;
				tmpball.y = m_balls[bestID].y;
			} 
			else
			{
				tmpball = m_trueball;
			}

			//先假设球在上
			if (tmpball.y < 500)
			{
				tmpball.y = 1200 - tmpball.y;
				bflip = true;
			}

			//开球进攻队员-球-对方球门 = 120度
			tmp = m_Loca_Ray(tmpball.x,tmpball.y,1600,600,(int)(150*m_fKField));			//最后一个参数是距离
			tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,120);
			tmpangle = m_Loca_CalAngle(tmp.x,tmp.y,1800,600);

			//若球在下方，则坐标上下翻转
			if (bflip == true)
			{
				tmp.y = 1200 - tmp.y;
				tmpangle = -tmpangle;
			}
			AMoveTo(bestID,tmp.x,tmp.y,tmpangle);
			m_curKOPrim = bestID;
			break;

			/*/2、发控制号的方式
			bestID = m_Ag_GetStatus(CTRL_KICKOFF_PRIM_READY,m_tacagent);	//尽量维持角色稳定性
			if (bestID != 0)
			{
				AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
				break;
			}*/

		case CTRL_KICKOFF_SLAVE_READY:
			//1、moveto 方式
			//维持角色稳定性
			if (
				m_curKOSlave != 0 && 
				m_curKOSlave != m_curKOPrim && 
				m_tacagent[m_curKOSlave].online == true
				)
			{
				bestID = m_curKOSlave;
			} 
			else
			{
				//找出离球最近的角色
				bestID = m_Ag_GetNearBy(m_trueball.x,m_trueball.y,m_tacagent);
// 				while(bestID == m_curKOPrim)
// 				{
// 					bestID = m_Ag_GetNearBy(m_trueball.x,m_trueball.y,m_tacagent);
// 					if (bestID < 2)
// 					{
// 						break;
// 					}
// 				}
// 				if (bestID < 2)
// 				{
// 					break;
// 				}
			}
			//尽量以其自己看到的球作为移动基准，避免间距过大
			if (m_tacagent[bestID].foundball == true)
			{
				tmpball.x = m_balls[bestID].x;
				tmpball.y = m_balls[bestID].y;
			} 
			else
			{
				tmpball = m_trueball;
			}

			//先假设球在上
			if (tmpball.y < 500)
			{
				tmpball.y = 1200 - tmpball.y;
				bflip = true;
			}

			//触球队员-球-对方球门 = -120度
			tmp = m_Loca_Ray(tmpball.x,tmpball.y,1600,600,(int)(100*m_fKField));	//最后一个参数是距离
			tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,-120);
			tmpangle = m_Loca_CalAngle(tmp.x,tmp.y,tmpball.x,tmpball.y)+120;

			//若球在下方，则坐标上下翻转
			if (bflip == true)
			{
				tmp.y = 1200 - tmp.y;
				tmpangle = -tmpangle;
			}
			AMoveTo(bestID,tmp.x,tmp.y,tmpangle);
			m_curKOSlave = bestID;
			break;

			/*/2、发控制号的方式
			bestID = m_Ag_GetStatus(CTRL_KICKOFF_SLAVE_READY,m_tacagent);	//尽量维持角色稳定性
			if (bestID != 0)
			{
				AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
				break;
			}*/

		case CTRL_FREEKICK_PRIM_READY:
			bestID = m_Ag_GetStatus(CTRL_FREEKICK_PRIM_READY,m_tacagent);	//尽量维持角色稳定性
			if (bestID != 0)
			{
				AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
			}
			break;

		case CTRL_FREEKICK_SLAVE_READY:
			bestID = m_Ag_GetStatus(CTRL_FREEKICK_SLAVE_READY,m_tacagent);	//尽量维持角色稳定性
			if (bestID != 0)
			{
				AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
				break;
			}
			break;

		case CTRL_GOALKICK_PRIM_READY:
			bestID = m_Ag_GetStatus(CTRL_GOALKICK_PRIM_READY,m_tacagent);	//尽量维持角色稳定性
			if (bestID != 0)
			{
				AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
				break;
			}
			break;

		case CTRL_GOALKICK_SLAVE_READY:
			bestID = m_Ag_GetStatus(CTRL_GOALKICK_SLAVE_READY,m_tacagent);	//尽量维持角色稳定性
			if (bestID != 0)
			{
				AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
				break;
			}
			break;

		case CTRL_THROWIN_PRIM_READY:
			bestID = m_Ag_GetStatus(CTRL_THROWIN_PRIM_READY,m_tacagent);	//尽量维持角色稳定性
			if (bestID != 0)
			{
				AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
				break;
			}
			break;

		case CTRL_THROWIN_SLAVE_READY:
			bestID = m_Ag_GetStatus(CTRL_THROWIN_SLAVE_READY,m_tacagent);	//尽量维持角色稳定性
			if (bestID != 0)
			{
				AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
				break;
			}
			break;

		case CTRL_CORNERKICK_PRIM_READY:
			bestID = m_Ag_GetStatus(CTRL_CORNERKICK_PRIM_READY,m_tacagent);	//尽量维持角色稳定性
			if (bestID != 0)
			{
				AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
				break;
			}
			break;

		case CTRL_CORNERKICK_SLAVE_READY:
			bestID = m_Ag_GetStatus(CTRL_CORNERKICK_SLAVE_READY,m_tacagent);	//尽量维持角色稳定性
			if (bestID != 0)
			{
				AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
				break;
			}

			//挑选剩下的离球最近的队员
			bestID = m_Ag_GetNearBy(m_trueball.x,m_trueball.y,m_tacagent);
			if (bestID == 0)
			{
				//已经没有球员可分配了
				bNoOneToAlloc = true;
				break;
			}
			AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
			break;

		//////////////////////////////////////////////////////////////////////////
		//带有继承性质的角色
		//////////////////////////////////////////////////////////////////////////
		/*主开球队员*/
		case CTRL_KICKOFF_PRIM:
			//该行为ready状态者优先
			bestID = m_Ag_GetStatus(CTRL_KICKOFF_PRIM_READY,m_tacagent);
			if (bestID == 0)
			{
				//处于该状态者其次
				bestID = m_Ag_GetStatus(CTRL_KICKOFF_PRIM,m_tacagent);
				if (bestID == 0)
				{
					if (m_curKOPrim != 0)
					{
						bestID = m_curKOPrim;
					}
					else
					{
						//都没有就挑选离球最近的角色
						bestID = m_Ag_GetNearBy(m_trueball.x,m_trueball.y,m_tacagent);
					}
					if (bestID == 0)
					{
						//已经没有球员可分配了
						bNoOneToAlloc = true;
						break;
					}
				}
			}	
			AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
			break;

		/*辅助开球队员*/
		case CTRL_KICKOFF_SLAVE:
			//该行为ready状态者优先
			bestID = m_Ag_GetStatus(CTRL_KICKOFF_SLAVE_READY,m_tacagent);
			if (bestID == 0)
			{
				//处于该状态者其次
				bestID = m_Ag_GetStatus(CTRL_KICKOFF_SLAVE,m_tacagent);
				if (bestID == 0)
				{
					if (m_curKOSlave != 0)
					{
						bestID = m_curKOSlave;
					}
					else
					{
						//都没有就挑选离球最近的角色
						bestID = m_Ag_GetNearBy(m_trueball.x,m_trueball.y,m_tacagent);
					}
					if (bestID == 0)
					{
						//已经没有球员可分配了
						bNoOneToAlloc = true;
						break;
					}
				}
			}	
			AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
			break;


		/*主罚任意球队员*/
		case CTRL_FREEKICK_PRIM:
			//该行为ready状态者优先
			bestID = m_Ag_GetStatus(CTRL_FREEKICK_PRIM_READY,m_tacagent);
			if (bestID == 0)
			{
				//处于该状态者其次
				bestID = m_Ag_GetStatus(CTRL_FREEKICK_PRIM,m_tacagent);
				if (bestID == 0)
				{
					//都没有就挑选离球最近的角色
					bestID = m_Ag_GetNearBy(m_trueball.x,m_trueball.y,m_tacagent);
					if (bestID == 0)
					{
						//已经没有球员可分配了
						bNoOneToAlloc = true;
						break;
					}
				}
			}	
			AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
			break;


		/*辅助任意球队员*/
		case CTRL_FREEKICK_SLAVE:
			//该行为ready状态者优先
			bestID = m_Ag_GetStatus(CTRL_FREEKICK_SLAVE_READY,m_tacagent);
			if (bestID == 0)
			{
				//处于该状态者其次
				bestID = m_Ag_GetStatus(CTRL_FREEKICK_SLAVE,m_tacagent);
				if (bestID == 0)
				{
					//都没有就挑选离球最近的角色
					bestID = m_Ag_GetNearBy(m_trueball.x,m_trueball.y,m_tacagent);
					if (bestID == 0)
					{
						//已经没有球员可分配了
						bNoOneToAlloc = true;
						break;
					}
				}
			}	
			AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
			break;


		/*主罚球门球队员*/
		case CTRL_GOALKICK_PRIM:
			//该行为ready状态者优先
			bestID = m_Ag_GetStatus(CTRL_GOALKICK_PRIM_READY,m_tacagent);
			if (bestID == 0)
			{
				//处于该状态者其次
				bestID = m_Ag_GetStatus(CTRL_GOALKICK_PRIM,m_tacagent);
				if (bestID == 0)
				{
					//都没有就挑选离球最近的角色
					bestID = m_Ag_GetNearBy(m_trueball.x,m_trueball.y,m_tacagent);
					if (bestID == 0)
					{
						//已经没有球员可分配了
						bNoOneToAlloc = true;
						break;
					}
				}
			}	
			AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
			break;


		/*辅助球门球队员*/
		case CTRL_GOALKICK_SLAVE:
			//该行为ready状态者优先
			bestID = m_Ag_GetStatus(CTRL_GOALKICK_SLAVE_READY,m_tacagent);
			if (bestID == 0)
			{
				//处于该状态者其次
				bestID = m_Ag_GetStatus(CTRL_GOALKICK_SLAVE,m_tacagent);
				if (bestID == 0)
				{
					//都没有就挑选离球最近的角色
					bestID = m_Ag_GetNearBy(m_trueball.x,m_trueball.y,m_tacagent);
					if (bestID == 0)
					{
						//已经没有球员可分配了
						bNoOneToAlloc = true;
						break;
					}
				}
			}	
			AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
			break;


		/*主罚边线球队员*/
		case CTRL_THROWIN_PRIM:
			//该行为ready状态者优先
			bestID = m_Ag_GetStatus(CTRL_THROWIN_PRIM_READY,m_tacagent);
			if (bestID == 0)
			{
				//处于该状态者其次
				bestID = m_Ag_GetStatus(CTRL_THROWIN_PRIM,m_tacagent);
				if (bestID == 0)
				{
					//都没有就挑选离球最近的角色
					bestID = m_Ag_GetNearBy(m_trueball.x,m_trueball.y,m_tacagent);
					if (bestID == 0)
					{
						//已经没有球员可分配了
						bNoOneToAlloc = true;
						break;
					}
				}
			}	
			AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
			break;


		/*辅助边线球队员*/
		case CTRL_THROWIN_SLAVE:
			//该行为ready状态者优先
			bestID = m_Ag_GetStatus(CTRL_THROWIN_SLAVE_READY,m_tacagent);
			if (bestID == 0)
			{
				//处于该状态者其次
				bestID = m_Ag_GetStatus(CTRL_THROWIN_SLAVE,m_tacagent);
				if (bestID == 0)
				{
					//都没有就挑选离球最近的角色
					bestID = m_Ag_GetNearBy(m_trueball.x,m_trueball.y,m_tacagent);
					if (bestID == 0)
					{
						//已经没有球员可分配了
						bNoOneToAlloc = true;
						break;
					}
				}
			}	
			AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
			break;


		/*主罚角球队员*/
		case CTRL_CORNERKICK_PRIM:
			//该行为ready状态者优先
			bestID = m_Ag_GetStatus(CTRL_CORNERKICK_PRIM_READY,m_tacagent);
			if (bestID == 0)
			{
				//处于该状态者其次
				bestID = m_Ag_GetStatus(CTRL_CORNERKICK_PRIM,m_tacagent);
				if (bestID == 0)
				{
					//都没有就挑选离球最近的角色
					bestID = m_Ag_GetNearBy(m_trueball.x,m_trueball.y,m_tacagent);
					if (bestID == 0)
					{
						//已经没有球员可分配了
						bNoOneToAlloc = true;
						break;
					}
				}
			}	
			AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
			break;


		/*辅助角球队员*/
		case CTRL_CORNERKICK_SLAVE:
			//该行为ready状态者优先
			bestID = m_Ag_GetStatus(CTRL_CORNERKICK_SLAVE_READY,m_tacagent);
			if (bestID == 0)
			{
				//处于该状态者其次
				bestID = m_Ag_GetStatus(CTRL_CORNERKICK_SLAVE,m_tacagent);
				if (bestID == 0)
				{
					//都没有就挑选离球最近的角色
					bestID = m_Ag_GetNearBy(m_trueball.x,m_trueball.y,m_tacagent);
					if (bestID == 0)
					{
						//已经没有球员可分配了
						bNoOneToAlloc = true;
						break;
					}
				}
			}	
			AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
			break;

		//////////////////////////////////////////////////////////////////////////	
		/*进攻掩护*/
		case CTRL_ATKCOVER:
			//1、寻找离球最近的的队员做掩护队员（进攻队员已经被挑走，不必担心）
			bestID = m_Ag_GetNearBy(m_trueball.x,m_trueball.y,m_tacagent);
			if (bestID <= 0)
			{
				break;
			}
			//2、复制掩护球员的障碍物信息，以供判断敌人
			memcpy(m_tmpAgObst,(char*)&(m_cObst[bestID*EACH_AG_OBST_NUM]),EACH_AG_OBST_NUM*sizeof(obstacle));
			//3、寻找障碍物中不是己方队员，且距离较近的障碍物
			int nTmpOb;
			nTmpOb = 0;
			while(nTmpOb >= 0)
			{
				nTmpOb = m_Obst_NearBy(m_trueball.x,m_trueball.y,m_tmpAgObst,EACH_AG_OBST_NUM);
				if (nTmpOb >=0 )
				{
					//用agents里的队员与获取的障碍物进行比对，排除己方球员
					for(int i=0;i<AG_NUM;i++)
					{
						if (i != m_tmpAgObst[nTmpOb].ID)
						{
							if (m_CalDist(m_tmpAgObst[nTmpOb].x,m_tmpAgObst[nTmpOb].y,m_agents[i].x,m_agents[i].y) < 100)
							{
								//nTmpOb障碍物与某个队员距离低于1米，己方队员的嫌疑比较大
								m_tmpAgObst[nTmpOb].valid = false;
								break;
							}
						}
					}
					//如果使能，说明此障碍物为有效目标
					if (m_tmpAgObst[nTmpOb].valid == true)
					{
						break;
					}
				}
			}

			//有效的敌方目标，干扰他
			if (nTmpOb >= 0)
			{
				tmp = m_Loca_Ray(m_tmpAgObst[nTmpOb].x,m_tmpAgObst[nTmpOb].y,m_trueball.x,m_trueball.y,50);
				//计算实际的防守距离，避免过小
				tmpDist = m_CalDist(tmp.x,tmp.y,m_trueball.x,m_trueball.y);
				if (tmpDist < m_tacctrl[ti].def_dist)
				{
					//防守距离太小，需要延长
					tmp = m_Loca_Ray(m_trueball.x,m_trueball.y,tmp.x,tmp.y,m_tacctrl[ti].def_dist);
				}
				tmpangle = m_Loca_CalAngle(tmp.x,tmp.y,m_trueball.x,m_trueball.y);
				AMoveTo(bestID,tmp.x,tmp.y,tmpangle);
			}
			else
			{
				//找不到障碍物，则定点跑位
				if (m_trueball.y > 600)
				{
					tmp.y = m_trueball.y - 200;
					//判断是否已经有队员在那，避免堆到一起了
					for (int i=1;i<AG_NUM;i++)
					{
						if (m_agents[i].online == true)
						{
							int x = 100;
							//要去的点已经有人了，围球旋转寻找落脚点
							while (m_CalDist(m_agents[i].x,m_agents[i].y,tmp.x,tmp.y) < 100)
							{
								tmp = m_Loca_Rotate(tmp.x,tmp.y,m_trueball.x,m_trueball.y,-10);
								x--;
								if (x<0)
								{
									break;
								}
							}
						}
					}
				} 
				else
				{
					tmp.y = m_trueball.y + 200;
					//判断是否已经有队员在那，避免堆到一起了
					for (int i=1;i<AG_NUM;i++)
					{
						if (m_agents[i].online == true)
						{
							int x = 100;
							//要去的点已经有人了，围球旋转寻找落脚点
							while (m_CalDist(m_agents[i].x,m_agents[i].y,tmp.x,tmp.y) < 100)
							{
								tmp = m_Loca_Rotate(tmp.x,tmp.y,m_trueball.x,m_trueball.y,10);
								x--;
								if (x<0)
								{
									break;
								}
							}
						}
					}
				} 
				if (m_trueball.x > 300)
				{	
					tmp.x = m_trueball.x;
				} 
				else
				{	
					tmp.x = m_trueball.x + 300;
				}
				AMoveTo(bestID,tmp.x,tmp.y,90);
			}
			break;

		/**技术挑战赛找球**/
		case TC_FINDBALL:
			//己方半场的非守门员队员
			bestID = m_Ag_GetNearBy(0,600,m_tacagent);
			if (bestID == 1)
			{
				bestID = m_Ag_GetNearBy(0,600,m_tacagent);
			}
			if (bestID <= 0)
			{
				break;
			}
			switch(m_nTCFindBall)
			{
			case 0:
				AMoveTo(bestID,200,1000,0);
				if (m_CalDist(m_agents[bestID].x,m_agents[bestID].y,200,1000) < 100)
				{
					m_nTCFindBall ++;
				}
				break;

			case 1:
				AMoveTo(bestID,700,1000,0);
				if (m_CalDist(m_agents[bestID].x,m_agents[bestID].y,700,1000) < 100)
				{
					m_nTCFindBall ++;
				}
				break;

			case 2:
				AMoveTo(bestID,700,200,0);
				if (m_CalDist(m_agents[bestID].x,m_agents[bestID].y,700,200) < 100)
				{
					m_nTCFindBall ++;
				}
				break;

			case 3:
				AMoveTo(bestID,200,200,0);
				if (m_CalDist(m_agents[bestID].x,m_agents[bestID].y,200,200) < 100)
				{
					m_nTCFindBall ++;
				}
				break;

			default:
				m_nTCFindBall = 0;
				break;
			}
			break;

		/*不敏感的角色*/
		case CTRL_SEARCHBALL:
			strTmp.Format("搜球 tac%d ctrl%d",ti,m_tacctrl[ti].ctrl);
			//printf(strTmp);
			if (m_eStage==sTechChallenge2012)
			{
				bestID = ti;
			}
			else
			bestID = m_Ag_GetNearBy(900,600,m_tacagent);
			if (bestID >0)
			{
				AChangeStatus(bestID,CTRL_SEARCHBALL,0);
			}
			break;

		default:
// 			CString str;
// 			str.Format("不明行为 tac%d ctrl%d",ti,m_tacctrl[ti].ctrl);
		//	printf(str);
			//剩下的队员依次分配
			if (m_eStage==sTechChallenge2012)
			{
				AChangeStatus(ti,m_tacctrl[ti].ctrl,0,0);
				break;
			}
			
			for (int t=1;t<6;t++)
			{
				if (m_tacagent[t].online == true && m_tacagent[t].allocated == false)
				{
					//////////////////////////////////////////////////////////////////////////
					//传球延时
					if (t == m_nPassPlayer)
					{
						if (m_nPassEndDelay > 0)
						{
							m_nPassEndDelay --;
							AChangeStatus(t,CTRL_STOP,0,0);
							break;
						}
						else
						{
							//延时结束，清空记录
							m_nPassPlayer = 0;
						}
					}
					//////////////////////////////////////////////////////////////////////////
					AChangeStatus(t,m_tacctrl[ti].ctrl,0,0);
					break;
				}
			}
			break;
		}

	//修正moveto
	//m_Fix_KeepDistance(m_agents,100,300);
		//info.Format("Pass角色id=%d  catch id=%d \n",m_nPassPlayer,m_nCatchPlayer);
		//printf(info);
		//如果已经无队员可以分配了，跳出循环
		if (bNoOneToAlloc == true)
		{
			break;
		}
	}
	}
	

	//////////////////////////////////////////////////////////////////////////


/************************************************************************/
/*                              开球                                    */
/************************************************************************/
void CUPFullField::REF_KickOff()
{
	m_eStage = sKickOffReady;
	Ready_KickOff();
}

void CUPFullField::START_Kickoff()
{
	m_strCurSt.Format("我方开球开始！");
	m_eLastStage = m_eStage;
	DefaultSTART();
	
	Tac_Wait(8);	//8秒后强制开始自主决策
}

/************************************************************************/
/*                             任意球                                   */
/************************************************************************/
void CUPFullField::REF_Freekick()
{
	m_eStage = sFreekickReady;
	Ready_Freekick();
}

void CUPFullField::START_Freekick()
{
	m_strCurSt.Format("我方任意球开始！");
	m_eLastStage = m_eStage;
	DefaultSTART();
	
	Tac_Wait(8);	//8秒后强制开始自主决策
}

/************************************************************************/
/*                             球门球                                   */
/************************************************************************/
void CUPFullField::REF_Goalkick()
{
	m_eStage = sGoalkickReady;
	Ready_Goalkick();
}

void CUPFullField::START_Goalkick()
{
	m_strCurSt.Format("我方球门球开始！");
	m_eLastStage = m_eStage;
	DefaultSTART();
	
	Tac_Wait(8);	//8秒后强制开始自主决策
}

/************************************************************************/
/*                              界外球                                  */
/************************************************************************/
void CUPFullField::REF_Throwin()
{
	m_eStage = sThrowinReady;
	Ready_Throwin();
}

void CUPFullField::START_Throwin()
{
	m_strCurSt.Format("我方界外球开始！");
	m_eLastStage = m_eStage;
	DefaultSTART();
	
	Tac_Wait(8);	//8秒后强制开始自主决策
}

/************************************************************************/
/*                              角球                                    */
/************************************************************************/
void CUPFullField::REF_CornerKick()
{
	m_eStage = sCornerKickReady;
	Ready_CornerKick();
}

void CUPFullField::START_CornerKick()
{
	m_strCurSt.Format("我方角球开始！");
	m_eLastStage = m_eStage;
	if (m_bFoundball == true)
	{
		DefaultSTART();
	} 
	else
	{
		printf("角球开始时球失踪！");
		DefaultSTART();
	}
	m_eStage = sStart;
	Tac_Wait(8);	//8秒后强制开始自主决策

}

/************************************************************************/
/*                               点球/罚球                              */
/************************************************************************/
void CUPFullField::REF_Penalty()
{
	m_eStage = sPenaltyReady;
	Ready_Penalty();
}

void CUPFullField::START_Penalty()
{
	m_strCurSt.Format("我方点球开始！");
	m_eStage = sMani;

	m_tacctrl[1].ctrl = CTRL_MOVETO;
	m_tacctrl[1].target_x = 10;
	m_tacctrl[1].target_y = 600;
	m_tacctrl[1].target_angle = 0;
	
	m_tacctrl[2].ctrl = CTRL_ATTACK;
	
	//其他队员跑动到指定点
	for (int i=3;i<7;i++)
	{
		m_tacctrl[i].ctrl = CTRL_STOP;
		m_tacctrl[i].target_angle = 90;	
	}
	//其他
	
	m_tacctrl[3].target_x = 300;
	m_tacctrl[3].target_y = 100;
	
	m_tacctrl[4].target_x = 500;
	m_tacctrl[4].target_y = 100;
	
	m_tacctrl[5].target_x = 800;
	m_tacctrl[5].target_y = 100;
	
	m_tacctrl[6].target_x = 100;
	m_tacctrl[6].target_y = 100;
	
	m_TacAction(m_tacagent);
}

/************************************************************************/
/*                                争球                                  */
/************************************************************************/
void CUPFullField::REF_DroppedBall()
{
	m_eStage = sDroppedBallReady;
	Ready_DroppedBall();
}

void CUPFullField::START_DroppedBall()
{
	m_strCurSt.Format("争球开始！");
	Active();
}

//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/*                               对方开球                               */
/************************************************************************/
void CUPFullField::REF_Anti_Kickoff()
{
	m_eStage = sAntiKickOffReady;
	//Default_Anti_Ready(TAC_DIST_ANTI+10);
	Ready_Anti_Kickoff();
}

void CUPFullField::START_Anti_Kickoff()
{
	m_strCurSt.Format("对方开球开始！");
	m_eLastStage = m_eStage;
	Default_Anti_START();
}

/************************************************************************/
/*                               对方任意球                               */
/************************************************************************/
void CUPFullField::REF_Anti_Freekick()
{
	m_eStage = sAntiFreekickReady;
	Default_Anti_Ready(TAC_DIST_ANTI);
	//Ready_Anti_Freekick();
}

void CUPFullField::START_Anti_Freekick()
{
	m_strCurSt.Format("对方任意球开始！");
	m_eLastStage = m_eStage;
	Default_Anti_START();
}

/************************************************************************/
/*                               对方球门球                               */
/************************************************************************/
void CUPFullField::REF_Anti_Goalkick()
{
	m_eStage = sAntiGoalkickReady;
	Default_Anti_Ready(TAC_DIST_ANTI);
	//Ready_Anti_Goalkick();
}

void CUPFullField::START_Anti_Goalkick()
{
	m_strCurSt.Format("对方球门球开始！");	
	m_eLastStage = m_eStage;
	Default_Anti_START();
}

/************************************************************************/
/*                               对方边线球                               */
/************************************************************************/
void CUPFullField::REF_Anti_Throwin()
{
	m_eStage = sAntiThrowinReady;
	Default_Anti_Ready(TAC_DIST_ANTI);
	//Ready_Anti_Throwin();
}

void CUPFullField::START_Anti_Throwin()
{
	m_strCurSt.Format("对方边线球开始！");	
	m_eLastStage = m_eStage;
	Default_Anti_START();
}

/************************************************************************/
/*                               对方角球                               */
/************************************************************************/
void CUPFullField::REF_Anti_CornerKick()
{
	m_eStage = sAntiCornerKickReady;
	Default_Anti_Ready(TAC_DIST_ANTI);
	//Ready_Anti_CornerKick();
}

void CUPFullField::START_Anti_CornerKick()
{
	m_strCurSt.Format("对方角球开始！");	
	m_eLastStage = m_eStage;
	Default_Anti_START();
}

/************************************************************************/
/*                               对方点球                               */
/************************************************************************/
void CUPFullField::REF_Anti_Penalty()
{
	m_eStage = sAntiPenaltyReady;
	Ready_Anti_Penalty();
}

void CUPFullField::START_Anti_Penalty()
{
	m_strCurSt.Format("对方点球开始！");
	m_eStage = sMani;
	//其他队员跑动到指定点
	for (int i=1;i<7;i++)
	{
		m_tacctrl[i].ctrl = CTRL_MOVETO;
		m_tacctrl[i].target_angle = 90;	
	}
	m_tacctrl[1].target_x = 10;
	m_tacctrl[1].target_y = 600;
	m_tacctrl[1].target_angle = 0;	
	
	//跑到左边
	m_tacctrl[2].target_x = 500;
	m_tacctrl[2].target_y = 100;
	
	//其他
	
	m_tacctrl[3].target_x = 400;
	m_tacctrl[3].target_y = 100;
	
	m_tacctrl[4].target_x = 300;
	m_tacctrl[4].target_y = 100;
	 
	m_tacctrl[5].target_x = 200;
	m_tacctrl[5].target_y = 100;
	
	m_tacctrl[6].target_x = 100;
	m_tacctrl[6].target_y = 100;

	m_TacAction(m_tacagent);
}

/************************************************************************/
/*                                STOP                                  */
/************************************************************************/
void CUPFullField::REF_Stop()
{
	m_strCurSt.Format("比赛暂停...");
	KillTimer(TAC_WAIT_TIMER);
	KillTimer(999);
	Tac_ReleaseKickoff();
	Disactive();
	PR_AllReadyCanBeIn();
	m_taskPassCatchEnd();
	m_b10SecEnd = false;

	m_bTaskMidPassCatch = false;//[yzz]此处应该为 中场传球 动作的结束函数
	m_bTaskMidPassCatch_kick = false;
	m_bTask3MeterPass=false; 
	m_bTask3MeterPass_Kick=false;
	m_bClear3meterPassInfo();
	Default_start_delay=0;
	Attack_Convert_Flag=0;
	Attack_Convert_Counter=0;

	m_tacctrl[1].ctrl = CTRL_GOALKEEP;
	for (int i=0;i<7;i++)
	{
		m_tacctrl[i].ctrl = CTRL_STOP;	
	}
	
	m_TacAction(m_tacagent);
}

void CUPFullField::ChangeOwnGoal(UINT inOwnGoal)
{
	OwnGoalColor = inOwnGoal;
	Invalidate();
}

//////////////////////////////////////////////////////////////////////////
//进攻优先
void CUPFullField::DefaultReady()
{
	m_nAgCanBeInPenaltyArea = 5;
	KillTimer(TAC_WAIT_TIMER);

	m_bTaskPassCatch = false;
	m_bTaskPassCatch_kick = false;
	m_nCatchPlayer = 0;
	m_nPassPlayer = 0;
	m_nPassEndDelay = 0;

	//2010年使用moveto策略
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;
	//其他队员跑动到指定点
	for (int i=2;i<6;i++)
	{
		m_tacctrl[i].ctrl = CTRL_MOVETO;
		m_tacctrl[i].target_angle = 0;	
	}
	//先假设球靠上，计算位置
	CPoint tmpball = m_trueball;
	if (tmpball.y < 600)
	{
		tmpball.y = 1200 - tmpball.y;
	}
	
	CPoint tmp;
	CPoint posCatch;
	CPoint posPass;
	int tmpAng;

	//备份球的横坐标（为设置死区用）
	int halfField = 500;
	if (abs(tmpball.x - halfField) < 20)
	{
		//死区内
		tmpball.x = halfField;
	} 
	//根据球所在的半场来确定开球策略
	if (tmpball.x < halfField)
	{
	 	
		//1、球处于我方半场，处理的时候尽量大脚转移
		//[yzz]对于接球者而言 为什么要是 1600,600？经验的误差校正？
		//接球进攻队员-球-对方球门 = 180度
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,1600,600,(int)(TAC_DIST_CATCH*m_fKField));			
		m_tacctrl[3].target_x = tmp.x;
		m_tacctrl[3].target_y = tmp.y;
		m_tacctrl[3].target_angle = m_Loca_CalAngle(tmp.x,tmp.y,tmpball.x,tmpball.y);
		posCatch.x = tmp.x;
		posCatch.y = tmp.y;
		if (m_eStage == sGoalkickReady)
		{
			tmp = m_Loca_Ray(tmpball.x,tmpball.y,1600,600,(int)(750*m_fKField));			
			m_tacctrl[3].target_x = tmp.x;
			m_tacctrl[3].target_y = tmp.y;
			m_tacctrl[3].target_angle = m_Loca_CalAngle(tmp.x,tmp.y,tmpball.x,tmpball.y);
			posCatch.x = tmp.x;
			posCatch.y = tmp.y;
		}
		//传球队员-球-对方球门 = 180度
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,1800,600,(int)(TAC_DIST_PASS*m_fKField));			//最后一个参数是距离
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,180);//绕到球后面
		m_tacctrl[2].target_x = tmp.x;
		m_tacctrl[2].target_y = tmp.y;
		m_tacctrl[2].target_angle = m_Loca_CalAngle(tmp.x,tmp.y,tmpball.x,tmpball.y);
		posPass.x = tmp.x;
		posPass.y = tmp.y;

		//有时候离球太近
		//tmp = m_Loca_Ray(m_tacctrl[3].target_x,m_tacctrl[3].target_y,0,600,(int)(400*m_fKField));
		//tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,30);
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,m_tacctrl[3].target_x,m_tacctrl[3].target_y,(int)(400*m_fKField));
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,-30);
		m_tacctrl[4].target_x = tmp.x;
		m_tacctrl[4].target_y = tmp.y;
		m_tacctrl[4].target_angle = m_Loca_CalAngle(tmp.x,tmp.y,tmpball.x,tmpball.y);
	} 
	else if ((m_eStage == sFreekickReady||m_eStage == sThrowinReady) && tmpball.x<=1200 && tmpball.x>=500)
	{
		
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,1600,600,(int)(TAC_DIST_CATCH*m_fKField));			//最后一个参数是距离
		m_tacctrl[3].target_x = tmp.x;
		m_tacctrl[3].target_y = tmp.y;
		m_tacctrl[3].target_angle = m_Loca_CalAngle(tmp.x,tmp.y,tmpball.x,tmpball.y);
		posCatch.x = tmp.x;
		posCatch.y = tmp.y;
		int n = 0;
		while((tmp.x<925) && (n++<5))
		{
			tmp = m_Loca_Ray(tmpball.x,tmpball.y,1600,600,(int)(TAC_DIST_CATCH*m_fKField+n*20));			//最后一个参数是距离
			m_tacctrl[3].target_x = tmp.x;
			m_tacctrl[3].target_y = tmp.y;
			m_tacctrl[3].target_angle = m_Loca_CalAngle(tmp.x,tmp.y,tmpball.x,tmpball.y);
			posCatch.x = tmp.x;
			posCatch.y = tmp.y;
		}

		//传球队员-球-对方球门 = 180度
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,1800,600,(int)(TAC_DIST_PASS*m_fKField));			//最后一个参数是距离
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,180);//绕到球后面
		m_tacctrl[2].target_x = tmp.x;
		m_tacctrl[2].target_y = tmp.y;
		m_tacctrl[2].target_angle = m_Loca_CalAngle(tmp.x,tmp.y,tmpball.x,tmpball.y);
		posPass.x = tmp.x;
		posPass.y = tmp.y;

		//有时候离球太近
		//tmp = m_Loca_Ray(m_tacctrl[3].target_x,m_tacctrl[3].target_y,0,600,(int)(400*m_fKField));
		//tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,30);
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,m_tacctrl[3].target_x,m_tacctrl[3].target_y,(int)(400*m_fKField));
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,-30);
		m_tacctrl[4].target_x = tmp.x;
		m_tacctrl[4].target_y = tmp.y;
		m_tacctrl[4].target_angle = m_Loca_CalAngle(tmp.x,tmp.y,tmpball.x,tmpball.y);
	}
	else
	{
		
		//2、球处于对方半场，争取直接射门	(tmpball.x > 500)
		//接球进攻队员-球-对方球门 = 180度 (计算tmp的意义在于判断实际移动目标点的大概位置)
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,1800,600,(int)(TAC_DIST_CATCH*m_fKField));			//最后一个参数是距离
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,140);
// 		m_tacctrl[3].target_x = tmp.x;
// 		m_tacctrl[3].target_y = tmp.y;
// 		m_tacctrl[3].target_angle = m_Loca_CalAngle(tmp.x,tmp.y,tmpball.x,tmpball.y);
		posCatch.x = tmp.x;
		posCatch.y = tmp.y;

		m_tacctrl[3].ctrl = CTRL_AROUNDBALL;
		m_tacctrl[3].target_x = 1800;
		m_tacctrl[3].target_y = 600;
		m_tacctrl[3].def_dist = (int)(TAC_DIST_CATCH*m_fKField);
		m_tacctrl[3].def_angle = 140;

		int nodead = 0;
		//接球队员位置修正,避免出界
		while (posCatch.y > 1000)
		{
			//角球修正
		// 	tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,-5);
			posCatch = m_Loca_Rotate(posCatch.x,posCatch.y,tmpball.x,tmpball.y,-5);
			m_tacctrl[3].def_angle += -5;	
// 			m_tacctrl[3].target_x = tmp.x;
// 			m_tacctrl[3].target_y = tmp.y;
			nodead ++;
			if (nodead > 100)
			{
				break;
			}
		}
	//	m_tacctrl[3].target_angle = m_Loca_CalAngle(tmp.x,tmp.y,tmpball.x,tmpball.y);

		//传球队员-球-接球队员 = 0度
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,posCatch.x,posCatch.y,(int)(TAC_DIST_PASS*m_fKField));	//最后一个参数是距离
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,180);
// 		m_tacctrl[2].target_x = tmp.x;
// 		m_tacctrl[2].target_y = tmp.y;
// 		m_tacctrl[2].target_angle = m_Loca_CalAngle(tmp.x,tmp.y,tmpball.x,tmpball.y);
		posPass.x = tmp.x;
		posPass.y = tmp.y;
	
		m_tacctrl[2].ctrl = CTRL_AROUNDBALL;
		m_tacctrl[2].target_x = m_tacctrl[3].target_x;
		m_tacctrl[2].target_y = m_tacctrl[3].target_y;
		m_tacctrl[2].def_dist = (int)(TAC_DIST_PASS*m_fKField);
		//m_tacctrl[2].def_angle = 0;
		m_tacctrl[2].def_angle = m_tacctrl[3].def_angle-180;
		
		if (tmpball.x > 1600)
		{
			//角球区域
			tmp = m_Loca_Ray(posCatch.x,posCatch.y,0,600,(int)(350*m_fKField));
			tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,-40);
		} 
		else
		{
			//tmp = m_Loca_Ray(posCatch.x,posCatch.y,0,600,(int)(100*m_fKField));
			tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)(550*m_fKField));
			tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,-45);
		}
		m_tacctrl[4].target_x = tmp.x;
		m_tacctrl[4].target_y = tmp.y;
		m_tacctrl[4].target_angle = m_Loca_CalAngle(tmp.x,tmp.y,tmpball.x,tmpball.y);
	}
	
	//修正跑动位置，避免不巧与球的距离太近
	if (m_CalDist(tmpball.x,tmpball.y,m_Pos_Back.x,m_Pos_Back.y) > 300)
	{		
		m_tacctrl[5].target_x = m_Pos_Back.x;
		m_tacctrl[5].target_y = m_Pos_Back.y;
	} 
	else
	{	
		m_tacctrl[5].target_x = m_Pos_Back.x;
		m_tacctrl[5].target_y = m_Pos_Back.y - 300;
	}
	
	//若球在下方，则坐标上下翻转
	if (m_trueball.y < 550)		//故意不设置为600，避免在中心附近开球时由于球的定位晃动造成的机器位置来回晃动
	{
		m_tacctrl[2].target_y = 1200 - m_tacctrl[2].target_y;
		m_tacctrl[2].target_angle = -m_tacctrl[2].target_angle;
		m_tacctrl[2].def_angle *=-1;
		m_tacctrl[3].target_y = 1200 - m_tacctrl[3].target_y;
		m_tacctrl[3].target_angle = -m_tacctrl[3].target_angle;
		m_tacctrl[3].def_angle *=-1;
		m_tacctrl[4].target_y = 1200 - m_tacctrl[4].target_y;
		m_tacctrl[4].target_angle = -m_tacctrl[4].target_angle;
		m_tacctrl[4].def_angle *=-1;
		m_tacctrl[5].target_y = 1200 - m_tacctrl[5].target_y;
		m_tacctrl[5].target_angle = -m_tacctrl[5].target_angle;
		m_tacctrl[5].def_angle *=-1;

		//posCatch 的翻转
		posCatch.y = 1200 - posCatch.y;
		posPass.y = 1200 - posPass.y;
	}

	//指定接球者ID
	//	m_nCatchPlayer = m_Ag_GetNearBy(m_tacctrl[3].target_x,m_tacctrl[3].target_y,m_agents);
	m_nCatchPlayer = m_Ag_GetNearBy(posCatch.x,posCatch.y,m_tacagent);
	m_nPassPlayer = m_Ag_GetNearBy(posPass.x,posPass.y,m_tacagent);

	//debug
	CString strTmp;
	strTmp.Format(
		"传球%d(%d,%d) 接球%d(%d,%d)",
		m_nPassPlayer,posPass.x,posPass.y,
		m_nCatchPlayer,posCatch.x,posCatch.y
		);
	printf(strTmp);

	//////////////////////////////////////////////////////////////////////////
	/*/09年 用决策号开球
	//关键角色分配
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;
	m_tacctrl[2].ctrl = CTRL_KICKOFF_SLAVE_READY;
	m_tacctrl[3].ctrl = CTRL_KICKOFF_PRIM_READY;

	for (int i=4;i<6;i++)
	{
		m_tacctrl[i].ctrl = CTRL_MOVETO;
		m_tacctrl[i].target_angle = 0;	
	}
	//修正跑动位置，避免不巧与球的距离太近
	if (m_CalDist(m_trueball.x,m_trueball.y,300,300) > 200)
	{
		m_tacctrl[4].target_x = 300;
		m_tacctrl[4].target_y = 300;
	} 
	else
	{
		m_tacctrl[4].target_x = m_Pos_Back.x;
		m_tacctrl[4].target_y = m_Pos_Back.y;
	}
	
	if (m_CalDist(m_trueball.x,m_trueball.y,300,900) > 200)
	{
		m_tacctrl[5].target_x = 300;
		m_tacctrl[5].target_y = 900;
	} 
	else
	{
		m_tacctrl[5].target_x = m_Pos_Back.x;
		m_tacctrl[5].target_y = m_Pos_Back.y;
	}

	//////////////////////////////////////////////////////////////////////////
	//09年使用moveto策略
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;
	//其他队员跑动到指定点
	for (int i=2;i<6;i++)
	{
		m_tacctrl[i].ctrl = CTRL_MOVETO;
		m_tacctrl[i].target_angle = 0;	
	}
	//先假设球靠上，计算位置
	CPoint tmpball = m_trueball;
	if (tmpball.y < 500)
	{
		tmpball.y = 1200 - tmpball.y;
	}

	{
		//触球队员-球-对方球门 = -120度
		CPoint tmp = m_Loca_Ray(tmpball.x,tmpball.y,1600,600,80*m_fKField);	//最后一个参数是距离
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,-120);
		m_tacctrl[2].target_x = tmp.x;
 		m_tacctrl[2].target_y = tmp.y;
		m_tacctrl[2].target_angle = m_Loca_CalAngle(tmp.x,tmp.y,tmpball.x,tmpball.y)+120;

		//开球进攻队员-球-对方球门 = 120度
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,1600,600,100*m_fKField);			//最后一个参数是距离
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,120);
		m_tacctrl[3].target_x = tmp.x;
 		m_tacctrl[3].target_y = tmp.y;
		m_tacctrl[3].target_angle = m_Loca_CalAngle(tmp.x,tmp.y,1800,600);
	}

	//若球在下方，则坐标上下翻转
	if (m_trueball.y < 500)
	{
		m_tacctrl[2].target_y = 1200 - m_tacctrl[2].target_y;
		m_tacctrl[2].target_angle = -m_tacctrl[2].target_angle;
		m_tacctrl[3].target_y = 1200 - m_tacctrl[3].target_y;
		m_tacctrl[3].target_angle = -m_tacctrl[3].target_angle;
	}

	//修正跑动位置，避免不巧与球的距离太近
	if (m_CalDist(m_trueball.x,m_trueball.y,300,300) > 200)
	{
		m_tacctrl[4].target_x = 300;
		m_tacctrl[4].target_y = 300;
	} 
	else
	{
		m_tacctrl[4].target_x = m_Pos_Back.x;
		m_tacctrl[4].target_y = m_Pos_Back.y;
	}
	
	if (m_CalDist(m_trueball.x,m_trueball.y,300,900) > 200)
	{
		m_tacctrl[5].target_x = 300;
		m_tacctrl[5].target_y = 900;
	} 
	else
	{
		m_tacctrl[5].target_x = m_Pos_Back.x;
		m_tacctrl[5].target_y = m_Pos_Back.y;
	}

	/////////////////////////////////////////////////////////////////////////*/

	//防止撞击
	//m_Fix_CollisionAvoid(m_tacctrl, 50);

	m_TacAction(m_tacagent);
}

void CUPFullField::DefaultSTART()
{
	//2010年,一旦有人进入进攻状态，则激活自主决策
	m_bClear3meterPassInfo();
	Default_start_delay=0;
	SetTimer(999,1000,NULL);
	m_bTaskPassCatch = true;
	m_bTask3MeterPass=false;// 防止意外进入3米传球决策 wzc
	int i;
	for (i=0;i<AG_NUM;i++)
	{
		if (m_tacagent[i].online == true)
		{
			if (m_tacagent[i].status == CTRL_ATTACK)
			{
				CString strTmp;
				printf("2\r\n2\r\n2\r\n2\r\n");
				strTmp.Format("进攻情况下转自主决策〉〉〉〉》〉《〈〈");
				printf(strTmp);
				Active();
				return;
			}
		}
	}
	CString strTmp;
	strTmp.Format("DefaultSTART: 传球%d  接球%d",m_nPassPlayer,m_nCatchPlayer);
	printf(strTmp);

	//传球任务下，一旦球接近接球队员，则激活自主决策
	if (m_bTaskPassCatch == true)
	{
		
		//一个进攻球员的情况下，需要修正传接球队员id
		int nOnlineAgents = 0;
		int nOnlineID = 0;
		for (int i=2;i<AG_NUM;i++)
		{
			if (m_tacagent[i].online == true)
			{
				nOnlineAgents ++;
				nOnlineID = i;
			}
		}
		
		if (nOnlineAgents <= 1)
		{
			//只有一名非守门员的球员,特殊处理
			printf("//只有一名非守门员的球员,特殊处理");
			if(nOnlineID > 0)
			{
				m_nPassPlayer = nOnlineID;

				if (m_trueball.x < 1500)
				{
					m_tacagent[0].x = m_tacagent[m_nPassPlayer].x + 200;
				}
				else
				{
					m_tacagent[0].x = 900;
				}
				if (m_trueball.y < 600)
				{
					m_tacagent[0].y = 1200;
				} 
				else
				{
					m_tacagent[0].y = 0;
				}
				m_nCatchPlayer = 0;
			}
		}
		else
		{
			//传球任务下，一旦球接近接球队员，则激活自主决策
			if (m_nCatchPlayer > 0)
			{
				//计算球到接球球员距离，看情况切换到自主决策
				if (m_CalDist(m_balls[m_nCatchPlayer].x,m_balls[m_nCatchPlayer].y,m_agents[m_nCatchPlayer].x,m_agents[m_nCatchPlayer].y) < 100)
				{
					CString strTmp;
					strTmp.Format("球接近了〉〉〉〉〉〉〈〈〈〈");
					printf(strTmp);
					printf("12\r\n12\r\n12\r\n12\r\n");
					Active();
					return;
				}
			}
			else
			{
				//接球队员是虚拟的，传球后直接进入自主决策
				if (m_bTaskPassCatch_kick == true)
				{
					printf("//接球队员是虚拟的，传球后直接进入自主决策");
					printf("14\r\n14\r\n14\r\n31\r\n");
					Active();
					return;
				}
			}
		}		
	}
	
			
	//修正自己传球给自己的情况
	if (m_nCatchPlayer == m_nPassPlayer)
	{
		//寻找离球最近的非传球队员做接球队员
		printf("m_nCatchPlayer == m_nPassPlayer,寻找离球最近的非传球队员做接球队员");

		memcpy(m_tmpAgents,m_agents,AG_NUM*sizeof(uagent));
		m_tmpAgents[m_nPassPlayer].online = false;
		m_tmpAgents[m_nPassPlayer].allocated = true;
		int tmpcatch = TheBestAtk(m_tmpAgents);
		while(tmpcatch > 0)	//tmpcatch > 0 说明还有队员可以分配角色
		{
			if (tmpcatch != m_nPassPlayer)
			{
				CString strInfo;
				strInfo.Format("找到接球队员%d,传球队员为%d",tmpcatch,m_nPassPlayer);
				printf(strInfo);
				m_nCatchPlayer = tmpcatch;
				break;
			}
			else
			{
				m_tmpAgents[tmpcatch].online = false;
				tmpcatch = TheBestAtk(m_tmpAgents);
			}
		}
		//还是找不到可以接球的队员，虚拟一个0号球员
		if (m_nCatchPlayer == m_nPassPlayer)
		{
			printf("还是找不到可以接球的队员，虚拟一个0号球员");
			if (m_trueball.x < 1500)
			{
				m_tacagent[0].x = m_tacagent[m_nPassPlayer].x + 200;
			}
			else
			{
				m_tacagent[0].x = 900;
			}
			if (m_trueball.y < 600)
			{
				m_tacagent[0].y = 1200;
			} 
			else
			{
				m_tacagent[0].y = 0;
			}
			m_nCatchPlayer = 0;
		}
	}
	if (m_bTaskPassCatch == true)
	{
		if (m_bTaskPassCatch_kick == false)
		{
			//角色分配
			m_tacctrl[1].ctrl = CTRL_GOALKEEP;
			m_tacctrl[2].ctrl = CTRL_PASS;
			
			//3~5号队员停止
			for (i=3;i<AG_NUM;i++)         //4号位设成接球
			{
				m_tacctrl[i].ctrl = CTRL_STOP;
			}
		} 
		else
		{
			//(m_bTaskPassCatch_kick == true)	球已经踢出的情况
			//角色分配
			m_tacctrl[1].ctrl = CTRL_GOALKEEP;	
			m_tacctrl[2].ctrl = CTRL_CATCH;
			m_tacctrl[3].ctrl = CTRL_PASS;
			
			//3~5号队员停止
			for (i=3;i<AG_NUM;i++)
			{
				m_tacctrl[i].ctrl = CTRL_STOP;
			}

			//接球队员不存在的情况，选定一个非传球队员成为进攻队员(一旦出现进攻队员，就会激活自主决策)
			if (m_agents[m_nCatchPlayer].online == false)
			{
				printf("接球队员不存在的情况，选定一个非传球队员成为进攻队员");
				
				m_tacctrl[2].ctrl = CTRL_PASS;	//这个CTRL_PASS是为了避免传球队员自己去接球
				m_tacctrl[3].ctrl = CTRL_ATTACK;
			}
		}
	}
		
	//printf("m_TacAction(m_tacagent)前");
	m_TacAction(m_tacagent);	
	//printf("m_TacAction(m_tacagent)后");

	//m_eLastStage = m_eStage;
	m_eStage = sStart;

	/*/////////////////////////////////////////////////////////////////////////
	//09年
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;

	//正常的开球动作
	m_tacctrl[2].ctrl = CTRL_KICKOFF_SLAVE;
	m_tacctrl[4].ctrl = CTRL_KICKOFF_PRIM;
	
	m_TacAction(m_tacagent);
	
	m_eLastStage = m_eStage;
	m_eStage = sStart;

	//根据场上队员数量来决定开球后切到进攻的时间，单个球员要将自己切到进攻，所以时间短一些
	int remain = 0;
	for (int i=2;i<7;i++)
	{
		if (m_tacagent[i].online == true)
		{
			remain ++;
		}
	}

	if (remain > 2)
	{
		//有超过两名球员，切换速度稍慢
		Tac_Wait(3);
	}
	else
	{
		//单个球员，则快切
		Tac_Wait(2);
	}
	/////////////////////////////////////////////////////////////////////////*/
}

//对方开球开始的默认处理方式
void CUPFullField::Default_Anti_START()
{
	//非守门队员的多少决定球观察员的位置
	int nAgOnline = 0;
	for (int i=2;i<AG_NUM;i++)
	{
		if (m_tacagent[i].online == true && m_tacagent[i].status != ROBST_ERR)
		{
			nAgOnline ++;
		}
	}

	//此时m_tacctrl[2]为0角度防守 3为边翼A 4为后卫 5为边翼B
	//球观察员
	switch(nAgOnline)
	{
	case 1:
		m_tacctrl[2].ctrl = CTRL_ATTENTIONBALL;
		break;

	case 2:
		m_tacctrl[2].ctrl = CTRL_ATTENTIONBALL;	
		m_tacctrl[3].ctrl = CTRL_ATTENTIONBALL;
		break;

	case 3:
		m_tacctrl[2].ctrl = CTRL_ATTENTIONBALL;
		m_tacctrl[3].ctrl = CTRL_ATTENTIONBALL;
		break;

	case 4:
		m_tacctrl[2].ctrl = CTRL_ATTENTIONBALL;
		m_tacctrl[3].ctrl = CTRL_ATTENTIONBALL;	
		m_tacctrl[5].ctrl = CTRL_ATTENTIONBALL;
		break;
	}
			
	m_TacAction(m_tacagent);

	m_eLastStage = m_eStage;
	m_eStage = sStart;

	if (m_eLastStage == sStart)
	{
		return;
	}

	Tac_Wait10Sec();
}

void CUPFullField::Default_Anti_Ready(int inDistFB)
{
	m_nAgCanBeInPenaltyArea = 1;	//禁区内能站一个人

	KillTimer(TAC_WAIT_TIMER);
	//对方开球，我方跑位应对
	//////////////////////////////////////////////////////////////////////////
	//10年策略
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;

	for (int i=2;i<7;i++)
	{
		m_tacctrl[i].ctrl = CTRL_MOVETO;
	}

	//先假设球位于中线上方，计算位置
	CPoint tmpball = m_trueball;
	if (m_trueball.y < 600)
	{
		tmpball.y = 1200 - tmpball.y;
	}
	
 	CPoint tmp;	
	////////////////////////////////////////////////////////////////////////////
	//备份球的横坐标（为设置死区用）
	static int lastballx = 200;
	int halfField = 200;
	if (abs(tmpball.x - halfField) < 50)
	{
		//死区内
		tmpball.x = lastballx;
	} 
	else
	{
		//死区外，备份
		lastballx = tmpball.x;
	}
	lastballx = tmpball.x;
	if (tmpball.x > halfField && tmpball.x < 650)
	{
		//球在我方半场中等距离时
		//在球到球门之间堵上一个
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)(325*m_fKField));
		m_tacctrl[2].target_x = tmp.x;
		m_tacctrl[2].target_y = tmp.y;

		

		int defAngle = -52;
		//边翼1
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)((inDistFB)*m_fKField));
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,defAngle);
		m_tacctrl[3].target_x = tmp.x;
		m_tacctrl[3].target_y = tmp.y;

		//////////////////////////////////////////////////////////////////////////
		m_tacctrl[3].ctrl = CTRL_AROUNDBALL;
		m_tacctrl[3].target_x = 0;
		m_tacctrl[3].target_y = 575;
		m_tacctrl[3].def_dist = (inDistFB)*m_fKField;
		m_tacctrl[3].def_angle = defAngle;
		//////////////////////////////////////////////////////////////////////////

		//!替换方案：用进攻掩护来防守对方接球队员
		// 		m_tacctrl[3].ctrl = CTRL_ATKCOVER;
		// 		m_tacctrl[3].def_dist = (inDistFB+50)*m_fKField; 

		//后卫
		m_tacctrl[4].target_x =425;
		m_tacctrl[4].target_y = 635;

		if (m_CalDist(m_tacctrl[4].target_x,m_tacctrl[4].target_y,tmpball.x,tmpball.y) < TAC_DIST_ANTI+300)
		{
			//!!!临时修改，后卫离球太近
			m_tacctrl[4].target_y = tmpball.y - TAC_DIST_ANTI+100;
		}

		//边翼2，靠边线的一侧，容易出界
		defAngle = 52;
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)((inDistFB)*m_fKField));
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,defAngle);
		while(tmp.y > 1000)
		{
			defAngle -= 5;
			tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)((inDistFB)*m_fKField));
			tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,defAngle);
		}
		m_tacctrl[5].target_x = tmp.x;
		m_tacctrl[5].target_y = tmp.y;

		//////////////////////////////////////////////////////////////////////////
		m_tacctrl[5].ctrl = CTRL_AROUNDBALL;
		m_tacctrl[5].target_x = 0;
		m_tacctrl[5].target_y = 600;
		m_tacctrl[5].def_dist = (inDistFB)*m_fKField;
		m_tacctrl[5].def_angle = defAngle;
		//////////////////////////////////////////////////////////////////////////

		if (m_tacctrl[2].target_x < 200)
		{
			//堵门队员退到禁区里了，应该是球门任意球，位置调整
			m_tacctrl[2].target_x = 200;	//禁区内队员前移

			//两翼稍微张大一些
			tmp = m_Loca_Rotate(m_tacctrl[3].target_x,m_tacctrl[3].target_y,tmpball.x,tmpball.y,-50);
			m_tacctrl[3].target_x = tmp.x;
			m_tacctrl[3].target_y = tmp.y;

			tmp = m_Loca_Rotate(m_tacctrl[5].target_x,m_tacctrl[5].target_y,tmpball.x,tmpball.y,20);
			m_tacctrl[5].target_x = tmp.x;
			m_tacctrl[5].target_y = tmp.y;
		}
	}
	if (tmpball.x > halfField)
	{
		//球在我方半场中等距离时
		//在球到球门之间堵上一个
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)(inDistFB*m_fKField));
		m_tacctrl[2].target_x = tmp.x;
		m_tacctrl[2].target_y = tmp.y;

		//////////////////////////////////////////////////////////////////////////
		m_tacctrl[2].ctrl = CTRL_AROUNDBALL;
		m_tacctrl[2].target_x = 0;
		m_tacctrl[2].target_y = 600;
		m_tacctrl[2].def_dist = inDistFB*m_fKField;
		m_tacctrl[2].def_angle = 0;
		//////////////////////////////////////////////////////////////////////////

		int defAngle = -52;
		//边翼1
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)((inDistFB)*m_fKField));
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,defAngle);
		m_tacctrl[3].target_x = tmp.x;
		m_tacctrl[3].target_y = tmp.y;

		//////////////////////////////////////////////////////////////////////////
		m_tacctrl[3].ctrl = CTRL_AROUNDBALL;
		m_tacctrl[3].target_x = 0;
		m_tacctrl[3].target_y = 600;
		m_tacctrl[3].def_dist = (inDistFB)*m_fKField;
		m_tacctrl[3].def_angle = defAngle;
		//////////////////////////////////////////////////////////////////////////

		//!替换方案：用进攻掩护来防守对方接球队员
// 		m_tacctrl[3].ctrl = CTRL_ATKCOVER;
// 		m_tacctrl[3].def_dist = (inDistFB+50)*m_fKField; 

		//后卫
		m_tacctrl[4].target_x = m_Pos_Back.x;
 		m_tacctrl[4].target_y = m_Pos_Back.y;

		if (m_CalDist(m_tacctrl[4].target_x,m_tacctrl[4].target_y,tmpball.x,tmpball.y) < TAC_DIST_ANTI+300)
		{
			//!!!临时修改，后卫离球太近
			m_tacctrl[4].target_y = tmpball.y - TAC_DIST_ANTI+100;
		}

		//边翼2，靠边线的一侧，容易出界
		defAngle = 52;
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)((inDistFB)*m_fKField));
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,defAngle);
		while(tmp.y > 1000)
		{
			defAngle -= 5;
			tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)((inDistFB)*m_fKField));
			tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,defAngle);
		}
		m_tacctrl[5].target_x = tmp.x;
		m_tacctrl[5].target_y = tmp.y;

		//////////////////////////////////////////////////////////////////////////
		m_tacctrl[5].ctrl = CTRL_AROUNDBALL;
		m_tacctrl[5].target_x = 0;
		m_tacctrl[5].target_y = 600;
		m_tacctrl[5].def_dist = (inDistFB)*m_fKField;
		m_tacctrl[5].def_angle = defAngle;
		//////////////////////////////////////////////////////////////////////////
		
		if (m_tacctrl[2].target_x < 200)
		{
			//堵门队员退到禁区里了，应该是球门任意球，位置调整
			m_tacctrl[2].target_x = 200;	//禁区内队员前移

			//两翼稍微张大一些
			tmp = m_Loca_Rotate(m_tacctrl[3].target_x,m_tacctrl[3].target_y,tmpball.x,tmpball.y,-50);
			m_tacctrl[3].target_x = tmp.x;
			m_tacctrl[3].target_y = tmp.y;

			tmp = m_Loca_Rotate(m_tacctrl[5].target_x,m_tacctrl[5].target_y,tmpball.x,tmpball.y,20);
			m_tacctrl[5].target_x = tmp.x;
			m_tacctrl[5].target_y = tmp.y;
		}
		
	}
	else//(tmpball.x < 200),应该是角球一类的
	{
		//球在己方近球门
		//先在球到球门之间堵上一个
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)(inDistFB*m_fKField));
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,-15);
		m_tacctrl[2].target_x = tmp.x;
		m_tacctrl[2].target_y = tmp.y;
		//m_tacctrl[2].target_angle = m_Loca_CalAngle(m_tacctrl[2].target_x,m_tacctrl[2].target_y,tmpball.x,tmpball.y);
		//////////////////////////////////////////////////////////////////////////
		m_tacctrl[2].ctrl = CTRL_AROUNDBALL;
		m_tacctrl[2].target_x = 0;
		m_tacctrl[2].target_y = 600;
		m_tacctrl[2].def_dist = (inDistFB)*m_fKField;
		m_tacctrl[2].def_angle = -15;
		//////////////////////////////////////////////////////////////////////////

		//在第一个防卫上旋转一个角度
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,-60);
		m_tacctrl[3].target_x = tmp.x;
		m_tacctrl[3].target_y = tmp.y;
		//m_tacctrl[3].ctrl = CTRL_MOVETO;
		//////////////////////////////////////////////////////////////////////////
		m_tacctrl[3].ctrl = CTRL_AROUNDBALL;
		m_tacctrl[3].target_x = 0;
		m_tacctrl[3].target_y = 600;
		m_tacctrl[3].def_dist = (inDistFB)*m_fKField;
		m_tacctrl[3].def_angle = -60;
		//////////////////////////////////////////////////////////////////////////

		//!替换方案：用进攻掩护来防守对方接球队员
// 		m_tacctrl[3].ctrl = CTRL_ATKCOVER;
// 		m_tacctrl[3].def_dist = (inDistFB+50)*m_fKField; 
		
		//后防，避免对方向另一侧转移
// 		m_tacctrl[4].target_x = m_Pos_Back.x;
// 		m_tacctrl[4].target_y = 1100;

		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)(inDistFB*m_fKField));
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,-15);
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,-90);
		m_tacctrl[4].target_x = tmp.x;
		m_tacctrl[4].target_y = tmp.y;
		//m_tacctrl[4].ctrl = CTRL_MOVETO;
		//////////////////////////////////////////////////////////////////////////
		m_tacctrl[4].ctrl = CTRL_AROUNDBALL;
		m_tacctrl[4].target_x = 0;
		m_tacctrl[4].target_y = 600;
		m_tacctrl[4].def_dist = (inDistFB)*m_fKField;
		m_tacctrl[4].def_angle = -90;
		//////////////////////////////////////////////////////////////////////////

		//后卫
		m_tacctrl[5].target_x = m_Pos_Back.x;
		m_tacctrl[5].target_y = m_Pos_Back.y;

		

		if (m_CalDist(m_tacctrl[5].target_x,m_tacctrl[5].target_y,tmpball.x,tmpball.y) < TAC_DIST_ANTI)
		{
			//!!!临时修改，后卫离球太近
			m_tacctrl[5].target_y = tmpball.y - TAC_DIST_ANTI;
		}
	}
		
	//如果球在中线下方，则将坐标上下翻转
	if (m_trueball.y < 550)		//故意不设置为600，避免在中心附近开球时由于球的定位晃动造成的机器位置来回晃动
	{
		m_tacctrl[2].target_y = 1200 - m_tacctrl[2].target_y;
		m_tacctrl[3].target_y = 1200 - m_tacctrl[3].target_y;		
		m_tacctrl[4].target_y = 1200 - m_tacctrl[4].target_y;
		m_tacctrl[5].target_y = 1200 - m_tacctrl[5].target_y;
		
		m_tacctrl[2].def_angle *=-1;
		m_tacctrl[3].def_angle *=-1;
		m_tacctrl[4].def_angle *=-1;
		m_tacctrl[5].def_angle *=-1;
	}

	//2010添加防守队员
	//Default_Anti_Ready_Add(inDistFB);

	//防止移动到场地外
	for (i=2;i<6;i++)
	{
		m_Fix_MoveOutOffFrame(&(m_tacctrl[i]));
	}

	//防止撞击
	m_Fix_CollisionAvoid(m_tacctrl, 50);

	//角度,都朝向球
	for (i=2;i<6;i++)
	{
		m_tacctrl[i].target_angle = m_Loca_CalAngle(m_tacctrl[i].target_x,m_tacctrl[i].target_y,m_trueball.x,m_trueball.y);
	}

	m_TacAction(m_tacagent);
}
//////////////////////////////////////////////////////////////////////////

void CUPFullField::Tac_Wait10Sec()
{
	printf("10秒计时开始...");
	SetTimer(TAC_WAIT_TIMER,12000,NULL);
	m_b10SecEnd = true;
	m_BallLogAt10SecEnd.x = m_trueball.x;
	m_BallLogAt10SecEnd.y = m_trueball.y;
//	SetTimer(TAC_HALFREADY,7000,NULL);
}

void CUPFullField::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case TAC_WAIT_TIMER:
		Tac_ReleaseKickoff();	//仅对开球时有用
		printf("1\r\n1\r\n1213213213213213\r\n1\r\n");
		Active();	//10秒过后，自主决策开始
		KillTimer(TAC_WAIT_TIMER);
		break;

	case PENALTY_PRE:
		m_tacctrl[1].ctrl = CTRL_ATTACK;
		m_TacAction(m_tacagent);
		KillTimer(PENALTY_PRE);
		Sleep(3000);
		m_eStage = sStop;
		break;

	case TAC_HALFREADY:
		m_bHalfready = true;
		KillTimer(TAC_HALFREADY);
		break;

	case PLAYOUT_COUNT:
		if (m_nTimeToBeIn <= 0)
		{
			KillTimer(PLAYOUT_COUNT);
			//自动使能Playin
			int i;
			for (i=0;i<AG_NUM;i++)
			{
				if (m_prRightOfPlayer[i] == PLAYER_RIGHT_OUT)
				{
					PR_Ready(i);
				}
			}
		}
		else
		{
			m_nTimeToBeIn --;
		}
		break;

	case TIMER_CATCH_DELAY:
		AChangeStatus(m_nCatchPlayer,CTRL_CATCH,0,0);
		KillTimer(TIMER_CATCH_DELAY);
		break;
	case 999://wzc
		Default_start_delay++;
		if (Default_start_delay>10)
		{
			Default_start_delay=10;
			KillTimer(999);
		}
		break;
	//case 998://wzc
		//Attack_Convert_Flag++;
		//break;
	default:
		break;
	}

	CStatic::OnTimer(nIDEvent);
}

void CUPFullField::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CStatic::OnClose();
}

void CUPFullField::Tac_Default()
{
	int px=1100,py=1000,believe=0;
	//////////////////////////////////////////////////////////////////////////
	//新的一帧
	kong();
	CString info;
 	//info.Format("传球%d 接球%d D%d",m_nPassPlayer,m_nCatchPlayer,m_nPassEndDelay);
 	//printf(info);
	if (Attack_Convert_Flag==1)
	{
		Attack_Convert_Counter++;
		if (Attack_Convert_Counter>60)
		{
			m_bTask3MeterPass=false;
			m_bTask3MeterPass_Kick=false;
			m_bClear3meterPassInfo();
			Attack_Convert_Flag=0;
			Active();
		}
	}
	else
	{
		Attack_Convert_Counter=0;
	}
	//////////////////////////////////////////////////////////////////////////
	bool bsomeoneonline = false;
	
	//复制球员信息
	for (int i=0;i<7;i++)
	{
		if (m_prRightOfPlayer[i] == PLAYER_RIGHT_INFIELD)
		{
			m_tacagent[i] = m_agents[i];
		} 
		else
		{
			m_tacagent[i].online = false;
		}

		if (m_tacagent[i].online == true)
		{
			bsomeoneonline = true;
		}
	}
	//3 meter pass judgement starts here [wzc]***********************************
	int AgWhoHoldBall;
	int MemAgWhoHoldBall;
	if (m_bSomeoneHoldBall_fromAG()==true)
	{
		HoldBallFlag=1;	
		info.Format("Catcher: %d\n",m_nAgentWhoHoldBall_fromAg());
		printf(info);
	}
	else 
	{
		HoldBallFlag=0;
		info.Format("Catcher:NULL!\n");
		printf(info);
	}
	AgWhoHoldBall=m_nAgentWhoHoldBall_fromAg();
	if (HoldBallFlag==1 && MemHoldBallFlag==0 && AgWhoHoldBall>0)
	{
		
		InitialHoldBall.x=m_agents[AgWhoHoldBall].x;
		InitialHoldBall.y=m_agents[AgWhoHoldBall].y;
		MemAgWhoHoldBall=AgWhoHoldBall;
		/*HoldBallBelieve++;
		if (HoldBallBelieve >4)
			HoldBallBelieve=4;*/
	}
	else if (HoldBallFlag==1 && MemHoldBallFlag ==1)
	{
		DistToInitialHoldBallPoint=sqrt(((m_agents[AgWhoHoldBall].x - InitialHoldBall.x) * (m_agents[AgWhoHoldBall].x - InitialHoldBall.x)) + ((m_agents[AgWhoHoldBall].y - InitialHoldBall.y) *(m_agents[AgWhoHoldBall].y - InitialHoldBall.y)) );
	/*	HoldBallBelieve++;
		if (HoldBallBelieve >4)
			HoldBallBelieve=4;*/
	}
	else if (HoldBallFlag ==0 && MemHoldBallFlag ==1)
	{
		/*HoldBallBelieve--;
		if (HoldBallBelieve<0)
		{
			m_bClear3meterPassInfo();
			m_bTask3MeterPass=false;
			Last3meterCatcher=0;
			Last3meterPasser=0;
		}*/
	}
	else 
	{
		/*HoldBallBelieve--;
		if (HoldBallBelieve<0)
		{*/
			m_bClear3meterPassInfo();
		//}
	}

	if (HoldBallFlag==1)
		MemHoldBallFlag=1;
	else
		MemHoldBallFlag=0;

	// 3 meter pass judgement ends here [wzc]************************************

	//没有球员在线，没必要进行决策
	if (bsomeoneonline == false)
	{
		return;
	}
	if (m_isMidPasserCannotTakeBall==true)//没必要停住，可以去防守
	{
		m_MidPasserCannotTakeBallCount++;
		if (m_MidPasserCannotTakeBallCount>10)
		{
			m_isMidPasserCannotTakeBall=false;
		}
	} 
	else
	{
		m_MidPasserCannotTakeBallCount=0;
	}
	for(i = 0; i < AG_NUM; i ++)
	{
		if(m_memstatus[i] == CTRL_MID_CATCH)
		{
			if(m_tacagent[i].status == CTRL_ATTACK)
			{
				m_bTaskMidPassCatch=false; 
				m_bTaskMidPassCatch_kick=false;
			}
		}	
		if(m_memstatus[i] == CTRL_MID_PASS)
		{
			if(m_tacagent[i].status == CTRL_ATTACK)
			{
				m_bTaskMidPassCatch=false; 
				m_bTaskMidPassCatch_kick=false;
			}
		}

		//3 meter pass judgement ,
		if(m_memstatus[i] == CTRL_3M_CATCH)
		{
			if(m_tacagent[i].status == CTRL_ATTACK)
			{
				m_bClear3meterPassInfo();
				Attack_Convert_Flag=0;
				m_bTask3MeterPass=false;
				m_bTask3MeterPass_Kick=false;
				Last3meterCatcher=0;
				Last3meterPasser=0;
				m_n3mCatchPlayer=0;
				m_n3mPassPlayer=0;
				info.Format("单体转进攻了！！！！");
		        printf(info);
				m_memstatus[i]=CTRL_ATTACK;
				
			}
		}	
		if(m_memstatus[i] == CTRL_3M_PASS)
		{
			if(m_tacagent[i].status == CTRL_STOP)
			{
				/*m_bClear3meterPassInfo();
				m_bTask3MeterPass=false;
				Last3meterCatcher=0;
				Last3meterPasser=0;*/
				m_memstatus[i] == CTRL_STOP;
				info.Format("球已经被踢出！！！！");
		        printf(info);
				m_bTask3MeterPass_Kick=true;
			}
		}
		if(m_memstatus[i] == CTRL_3M_PASS)
		{
			if( m_tacagent[i].status == CTRL_ATTACK)
			{
				m_bClear3meterPassInfo();
				Attack_Convert_Flag=0;
				Last3meterCatcher=0;
				Last3meterPasser=0;
				m_n3mCatchPlayer=0;
				m_n3mPassPlayer=0;
				m_bTask3MeterPass=false;
				m_bTask3MeterPass_Kick=false;
				m_memstatus[i] = CTRL_ATK_CAT_CONVERT ;
				info.Format("球被断了,自主决策启动！！！");
		        printf(info);
			}
		}	
		if(m_memstatus[i] ==CTRL_PASS_MOVE)
		{
			if(m_tacagent[i].status == CTRL_3M_CATCH)
			{
			
				m_bTask3MeterPass=false;
				m_bTask3MeterPass_Kick = true;
				
			}
		}	
	}
	
	//1、找不到球情况的决策
	if (m_bFoundball == false && m_bTaskMidPassCatch==false)//TEST 713
	{
		if(m_bTaskMidPassCatch==true)
		{
	//	m_bTaskMidPassCatch==false;
		//m_bTaskMidPassCatch_kick==false;
		CString info;
		info.Format("找不到球\n");
		printf(info);
		}
		//统计场上球员数目
		int nAgOnline = 0;
		for (int i=2;i<AG_NUM;i++)
		{
			if (m_tacagent[i].online == true)
			{
				nAgOnline ++;
			}
		}
		
		//关键角色分配
		m_tacctrl[1].ctrl = CTRL_GOALKEEP;
        m_tacctrl[3].ctrl = CTRL_MOVETO;
        m_tacctrl[3].target_x = 400;
        m_tacctrl[3].target_y = 450;
        //now
		m_tacctrl[2].ctrl =  CTRL_MOVETO;
		m_tacctrl[2].target_x = 325;
        m_tacctrl[2].target_y = 750;
		
        
        m_tacctrl[5].ctrl = CTRL_MOVETO;
        m_tacctrl[5].target_x = 600;
        m_tacctrl[5].target_y = 600;
      
		int p1,p2,p3=-100;
		BestPassPoint(&p1,&p2,&p3);//qu diao weizhi zhiliuxia zhang ai wu 
		//int num_2013=0;

		m_tacctrl[4].ctrl = CTRL_MOVETO;
		m_tacctrl[4].target_x = p1;
		m_tacctrl[4].target_y = p2;
		//m_tacctrl[4].target_angle = m_Loca_CalAngle(p1,p2,m_trueball.x,m_trueball.y);

        for (i=3;i<5;i++)
        {
            //m_tacctrl[i].target_angle = m_Loca_CalAngle(m_tacctrl[i].target_x,m_tacctrl[i].target_y,m_trueball.x,m_trueball.y);
            m_tacctrl[i].target_angle = 0;
        }
        
		m_TacAction(m_tacagent);
		return;
	}
	//****************************************************************************************
	//3 meter pass code starts here [wzc]*****************************************************
	if ((DistToInitialHoldBallPoint>(TAC_DIST_3MeterPassJudge)) ) // 防止意外： 开球5秒内不会进入3米传球
	{
			m_bTask3MeterPass=true;
	}
	else
	{
		m_bTask3MeterPass=false;
	}
	info.Format("新的一帧%d,,%d,,%d\n",m_bTask3MeterPass,m_bTask3MeterPass_Kick,DistToInitialHoldBallPoint);
	printf(info);
	if (m_bTask3MeterPass==true && m_bTask3MeterPass_Kick == false)
		{
		    m_bTaskMidPassCatch=false;
			CString info;
			info.Format("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
			printf(info);
			int k=m_nAgentWhoHoldBall_fromAg();
			int k1;
			if (k>0 && k<6)
			{
				Tac_3meterPass(k);
			}
			else
			{
				k=Last3meterPasser;
				k1=Last3meterCatcher;
				Tac_3meterPass(k);
			}
			m_Fix_GetOutOfTheWayOfATK();	
		    return ;
		}
	else if (m_bTask3MeterPass==false && m_bTask3MeterPass_Kick == true && Default_start_delay >=3)
	{
		CString info;
			info.Format("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&");
			printf(info);
		Tac_3meterPass_Kicked();
		
		return;
	}
	else if (m_bTask3MeterPass==false && m_bTask3MeterPass_Kick == false && m_n3mCatchPlayer==m_nAgentWhoHoldBall_fromAg())
	{
		// 3 meter pass success
		//Active();
		m_n3mCatchPlayer =0;
		CString info;
		info.Format("3米传球成功\n");
		m_bClear3meterPassInfo();
		printf(info);
		return;
	}
	else if (m_bTask3MeterPass == true && m_bTask3MeterPass_Kick == true)
	{
		CString info;
		info.Format("三米传球出现了错误情况！！！！！！！！！！！！！！！！\n");
		printf(info);
		m_bTask3MeterPass == false;
		return;
	}
	else
	{
	
	//3 meter pass code ends here [wzc]*******************************************************
	//****************************************************************************************



	//if ((m_bTaskMidPassCatch==true) &&( m_bTaskMidPassCatch_kick==true) && m_bSomeoneHoldBall()==true )//[yzz]对于接球者 应该回去接球
	//{
	//	m_bTaskMidPassCatch==false;
	//	m_bTaskMidPassCatch_kick==false;
	//	m_nMidPassCatchBelieve_last = 0;
	//	CString info;
	//	info.Format("中场传球结束\n");
	//	printf(info);
	//}
	//else if ((m_bTaskMidPassCatch==true) &&( m_bTaskMidPassCatch_kick==true))
	//{
	//if (m_MidPasserCannotTakeBallCount<=0)
	//	{
	//		m_isMidPasserCannotTakeBall = true;
	//		info.Format("传球者不追球!!!!\n");
	//		printf(info);
	//	}
	//	
	//	//如果传球动作成功，己方半场的球员不要来抢球,接球者继续 等待接球，不设进攻者,其余球员跑位， 当believe大于10 的时候，继续等待接球，>20（应计算平均接球时间，20为估计值）时，自主决策
	//	//if (m_nMidPassCatchBelieve_last >= 10 )//(m_nMidPassCatchBelieve_last<20) mc
	//	{
	//		info.Format("中场传球已经起脚!!!!\n");
	//		printf(info);
	//		Tac_MidPass_kicked(m_nMidCatchPlayer,m_posFinalMidPass.x,m_posFinalMidPass.y,m_nMidPassCatchBelieve_last);
	//		//Tac_MidPass_kicked(m_nMidCatchPlayer,m_posFinalMidPass.x,m_posFinalMidPass.y,++m_nMidPassCatchBelieve_last);
	//		return;
	//	}
	////	else
	//	{
	//		/*m_bTaskMidPassCatch==false;  mc
	//		m_bTaskMidPassCatch_kick==false;
	//		CString info;
	//		info.Format("中场传球超时\n");
	//		printf(info);*/
	//		//避免传球者去追球??
	//	}
	//}
	//else if ((m_bTaskMidPassCatch==true) &&( m_bTaskMidPassCatch_kick==false))
	//{
	//	
	//	if (m_bSomeoneHoldBall()==false)
	//	{
	//		info.Format("m_bSomeoneHoldBall()=false\n");
	//		printf(info);
	//		//传球过程中球被断了
	//		m_bTaskMidPassCatch = false;
	//	}
	//	else
	//	{

	//	//CString info;
	//	info.Format("中场传球启动\n");
	//	printf(info);
	//	//先固定 最佳传球点，测试完再 改 m_posBestMidPass
	//	//测试代码，如果接球者位置合适，believe+5,模拟 场面评估,对于传球者而言，此时 可以 开始射门计数
	//	//if (m_nMidPassCatchBelieve_last<=5)
	//	//{
	//	//	for (int i=2;i<AG_NUM;i++)
	//	//	{
	//	//		if (m_tacagent[i].online == true && m_CalDist(m_tacagent[i].x,m_tacagent[i].y,m_posBestMidPass_last.x,m_posBestMidPass_last.y)<100)
	//	//		{
	//	//			m_nMidPassCatchBelieve_last+=5;
	//	//		}
	//	//	}
	//	//}
	//	//CString info;
	//	believe = 0;
	//    BestPassPoint(&px,&py,&believe);//believe默认为0，>=10 开始准备起脚传球
	//
	//	m_posBestMidPass_last.x = px;
	//	m_posBestMidPass_last.y = py;
	//	m_nMidPassCatchBelieve_last = believe;
	//	Tac_MidPass(TAC_DIST_DEFFAR,px,py,believe);
	//	//球在我方半场时，避免出现进攻球员和我方挤堆的状况
	//	m_Fix_GetOutOfTheWayOfATK();		

	//	info.Format("x=%d,y=%d;Believe = %d\n",px,py,m_nMidPassCatchBelieve_last);
	//	printf(info);

	//	return;
	//	}
	//} 
	
	//2、有球员能看到球，先判断球的位置，后面的值为进行紧逼防守的分界值
	if (m_trueball.x < 900)//0改为900[yzz]
	{
		//球在我方半场
		//判断守门员是否控球，避免进攻球员与守门员进行无谓的争抢
		bool bgk = m_bGoalKeeperHoldBall();
		if (bgk == false)
		{
			
			//2.1守门员未带球(球在我方半场)
			if (m_bSomeoneHoldBall()==true)
			{
				CString info;
				info.Format("守门员未带球");
				Tac_NormalAttack();
			    return ;
			}
			else
			{
			
				Tac_PressDef(TAC_DIST_DEFCLOSE);
				int p1,p2,p3;
				//BestPassPoint(&p1,&p2,&p3);//qu diao weizhi zhiliuxia zhang ai wu 
				int num_2013=0;
				for (int i=1;i<7;i++)
				{
					if(m_tacagent[i].online==true)
					{
						num_2013++;
					}
				}
				/*if (num_2013>=4)
				{
					m_tacctrl[4].ctrl = CTRL_MOVETO;
					m_tacctrl[4].target_x = p1;
					m_tacctrl[4].target_y = p2;
					m_tacctrl[4].target_angle = m_Loca_CalAngle(p1,p2,m_trueball.x,m_trueball.y);
				}*/
				
				//printf("Tac_PressDef2(150)");
				//Tac_3Atk(200);
			} 		
//			else
//			{
//				//仅一名，说明我方队员在拿球
			//	DefaultActive(150);		//对擅长远射的队伍贴近一些防守
//				//Tac_ContainDef(150);		//三角围堵防守
//			
//				Tac_RoundedDef2(TAC_DIST_DEFFAR);		//使用defence的防守
//				
//
////						Tac_RoundedDef3(TAC_DIST_DEFCLOSE);		//使用defence的防守
//
//			}
			
		} 
		else
		{
			//2.2守门员带球时的策略(球在我方半场)
			m_tacctrl[1].ctrl = CTRL_GOALKEEP;


			//其他队员让出一条路
			for (int i=2;i<7;i++)
			{
				m_tacctrl[i].ctrl = CTRL_MOVETO;
				m_tacctrl[i].target_angle = 0;
			}

			m_tacctrl[2].target_x = 900;
			m_tacctrl[2].target_y = 400;

			m_tacctrl[3].target_x = 300;
			m_tacctrl[3].target_y = 1000;
			

			m_tacctrl[4].target_x = 500;
			m_tacctrl[4].target_y = 300;

			m_tacctrl[5].target_x = 500;
			m_tacctrl[5].target_y = 900;

			m_tacctrl[6].target_x = 300;
			m_tacctrl[6].target_y = 200;

			m_TacAction(m_tacagent);
		}

		//球在我方半场时，避免出现进攻球员和我方挤堆的状况
		m_Fix_GetOutOfTheWayOfATK();
	} 
	else
	{
		
		/**************************球在对方半场***************************/
		//printf("对方半场");
		if (m_bSomeoneHoldBall() == true) // 组织进攻
		{
			m_tacctrl[1].ctrl = CTRL_GOALKEEP;
			m_tacctrl[2].ctrl = CTRL_ATTACK;
			m_tacctrl[3].ctrl = CTRL_MOVETO;//wzc  随时准备接传来的球
			
			//一名队员护送，随时准备接球   wzc
			//kong();
			m_tacctrl[3].target_x = kongx;
			m_tacctrl[3].target_y = kongy;
			m_tacctrl[3].target_angle = m_Loca_CalAngle(m_tacctrl[3].target_x,m_tacctrl[3].target_y,m_trueball.x,m_trueball.y);

			for(int i = 4; i<=AG_NUM;i++)
			{
				m_tacctrl[i].ctrl = CTRL_MOVETO;
			}

			//动态站位防守 
			CPoint tmp;
			tmp = m_Loca_Ray(0,600,m_trueball.x,m_trueball.y,TAC_DYNAMIC_DEFDIST*m_fKField);
			m_tacctrl[4].target_x = tmp.x;
			m_tacctrl[4].target_y = tmp.y - 200*m_fKField;
			m_tacctrl[4].target_angle = m_Loca_CalAngle(m_tacctrl[4].target_x,m_tacctrl[4].target_y,m_trueball.x,m_trueball.y);

			tmp = m_Loca_Ray(0,600,m_trueball.x,m_trueball.y,TAC_DYNAMIC_DEFDIST*m_fKField);
			m_tacctrl[5].target_x = tmp.x;
			m_tacctrl[5].target_y = tmp.y + 200*m_fKField;
			m_tacctrl[5].target_angle = m_Loca_CalAngle(m_tacctrl[5].target_x,m_tacctrl[5].target_y,m_trueball.x,m_trueball.y);

			//当球到对方禁区内时，无论如何都要将防守队员拉出来
			if (m_trueball.x > 1600 && m_trueball.y < 925 && m_trueball.y > 275)
			{
				for (i=4;i<AG_NUM;i++)
				{
					m_tacctrl[i].def_dist = 200;
				}
			}

			m_TacAction(m_tacagent);
		}
		else
		{  // 我方无人持球  ， 尽一切力量拼抢  wzc
			m_tacctrl[1].ctrl = CTRL_GOALKEEP;
			m_tacctrl[2].ctrl = CTRL_ATTACK;
			m_tacctrl[3].ctrl = CTRL_ATTACK;
			

			//两名队员防守   wzc
			for (int i=4;i<AG_NUM;i++)
			{
				m_tacctrl[i].ctrl = CTRL_DEFENCE;
				if (m_bSomeoneHoldBall() == true)
				{
					m_tacctrl[i].def_dist = (int)(TAC_DIST_DEFFAR*m_fKField);
				}	 
				else
				{	
					m_tacctrl[i].def_dist = (int)(TAC_DIST_DEFCLOSE*m_fKField);
				}
			}
		
		
			/*
			if (m_trueball.y > 600)
			{
				m_tacctrl[4].def_angle = -45;
			} 
			else
			{
				m_tacctrl[4].def_angle = 45;
			}*/
			m_tacctrl[4].ctrl = CTRL_MOVETO;
			m_tacctrl[4].target_x = m_trueball.x - 200;
			if (m_tacctrl[4].target_x < 300)
			{
				m_tacctrl[4].target_x = 300;
			}
			//m_tacctrl[4].target_y = 1200-m_trueball.y;

			if (m_trueball.y > 600)
			{
				m_tacctrl[4].target_y = 300;
			} 
			else
			{
			m_tacctrl[4].target_y = 900;
			}

			m_tacctrl[4].target_angle = m_Loca_CalAngle(m_tacctrl[4].target_x,m_tacctrl[4].target_y,m_trueball.x,m_trueball.y);

			//////////////////////////////////////////////////////////////////////////
		// 		CPoint tmp;
	// 		//int tmpAngle;
// 		tmp = m_Loca_Ray(200,600,m_trueball.x,m_trueball.y,500);
// 		m_tacctrl[4].target_x = tmp.x;
// 		m_tacctrl[4].target_y = tmp.y;
// 		m_tacctrl[4].target_angle = m_Loca_CalAngle(tmp.x,tmp.y,m_trueball.x,m_trueball.y);
		//////////////////////////////////////////////////////////////////////////
		
			m_tacctrl[5].ctrl = CTRL_MOVETO;
			m_tacctrl[5].target_x = m_Pos_Back.x;
			if (m_trueball.y > 600)
			{
				m_tacctrl[5].target_y = 700;
			} 
			else
			{
				m_tacctrl[5].target_y = 500;
			}
			m_tacctrl[5].target_angle = m_Loca_CalAngle(m_tacctrl[5].target_x,m_tacctrl[5].target_y,m_trueball.x,m_trueball.y);
		
			//zbw dig_in
		
	
		


			//当球到对方禁区内时，无论如何都要将防守队员拉出来
			if (m_trueball.x > 1600 && m_trueball.y < 925 && m_trueball.y > 275)
			{
				for (i=4;i<AG_NUM;i++)
				{
					m_tacctrl[i].def_dist = 200;
				}
			}

			m_TacAction(m_tacagent);
		}
	}
}

}

void CUPFullField::Active()
{
	m_taskPassCatchEnd();
	m_eStage = sAUTO;
	m_curKOPrim = 0;
	m_curKOSlave = 0;
	//m_bHalfready = false;
//	Exec();
	printf("自主决策机制激活！");
	m_strCurSt.Format("自主决策状态");
}

void CUPFullField::Disactive()
{
	m_eStage = sStop;
	m_bClear3meterPassInfo();
	m_bTask3MeterPass=false;
	printf("自主决策暂停");
}

void CUPFullField::AChangeStatus(int inID, UINT inCtrlCode,double inAngle, int inDist)
{
	if (inID<0 || inID>6)
	{
		return;
	}
// 	if (inCtrlCode == CTRL_ATTENTIONBALL)
// 	{
// 		m_strTmp.Format("%d号分配为观察角色",inID);
// 		printf(m_strTmp);
// 	}
// 	CString str;
// 		str.Format("%d号机  行为%d",inID,inCtrlCode);
// 	printf(str);

	if (pToAgent[inID] != NULL)
	{
		//防守角度记录下来
		if (inCtrlCode == CTRL_DEFENCE)
		{	
			m_agents[inID].logDefAngle = inAngle;
		}
		pToAgent[inID]->CtrlCmd(inCtrlCode,m_trueball.x,m_trueball.y,(int)inAngle,inDist,m_bField);
		//zbw pre ball
		//pToAgent[inID]->Predictball(tpballjiao,tpballchang);
		//pToAgent[inID]->FollowPosition(m_follow[inID].x,m_follow[inID].y);
		//
        m_tacagent[inID].allocated = true;
	}	
	
	//Display
	m_ShowTacInfo(inCtrlCode,inID);

}
//dig_in
void CUPFullField::AFollowPosition(int inID, UINT inCtrlCode,double inAngle, int inDist)
{
	if (inID<0 || inID>6)
	{
		return;
	}

	m_ShowTacInfo(inCtrlCode,inID);
	if (pToAgent[inID] != NULL)
	{
		
		//zbw pre ball
		//pToAgent[inID]->Predictball(inCtrlCode,m_trueball.x,m_trueball.y,tpballjiao,tpballchang);
	    if(goalx>0&&goaly>0){
		pToAgent[inID]->FollowPosition(inCtrlCode,goalx,goaly,inAngle,m_nDistOfMultiDef);
        
		//AMoveTo(inID,goalx,goaly,0);
		m_tacagent[inID].allocated = true;
		}
	}	
	
	//Display
	

}
bool CUPFullField::m_bGoalKeeperHoldBall()
{
	//m_asc.Lock();
	for (int i=1;i<7;i++)
	{
		if (m_agents[i].online == true)
		{
			if (m_agents[i].status == CTRL_GOALKEEP)
			{
				if (m_agents[i].dist_ball < 50)
				{
					//m_asc.Unlock();
					return true;
				}
			}
		}
	}
	//m_asc.Unlock();

	return false;
}

bool CUPFullField::AChangeTo(int inID, int inToindex,uagent* inList)
{
	if (inList == NULL)
	{
		return false;
	}
	if (inID<1 || inID>6)
	{
		return false;
	}
	if (inToindex<1 || inToindex>6)
	{
		return false;
	}
	//寻找源index
	int srcindex = 0;
	for (int i=1;i<7;i++)
	{
		if (inList[i].ID == inID)
		{
			srcindex = i;
		}
	}
	//寻找目标index
	int tarindex = 0;
	int count = 0;
	for (i=1;i<7;i++)
	{
		if (inList[i].online == true)
		{
			count++;
			if (count == inToindex)
			{
				tarindex = i;
			}
		}
	}
	uagent tobak = inList[tarindex];
	inList[tarindex] = inList[srcindex];
	inList[srcindex] = tobak;
	return true;
	/*链表版本/////////////////////////////////////////////////////////////////////////
	//先找到目标位置
	uagent* target = inList;
	for (int i=0;i<inToindex-1;i++)
	{
		if (target != NULL)
		{
			target = target->next;
		}
		else
			break;
	}
	if (target == NULL)
	{
		return false;
	}

	//找源位置
	uagent* src = inList;
	for (i=0;i<inID-1;i++)
	{
		if (src != NULL)
		{
			src = src->next;
		}
		else
			break;
	}
	if (src == NULL)
	{
		return false;
	}

	//开始进行交换
	uagent tempa = *target;		//target -> tempa

	*target = *src;				//src -> target
	target->next = tempa.next;	//恢复链表联系
	tempa.next = src->next;
	*src = tempa;
	
	return true;
	/////////////////////////////////////////////////////////////////////////*/
}

int CUPFullField::TheBestAtk(uagent *inList)
{
	if (inList == NULL)
	{
		return NULL;
	}

	int best = 0;
	int atkperform = 10000;
	for (int i=2;i<7;i++)
	{
		if (inList[i].online == true && inList[i].allocated == false )
		{
			//守门员剔除
			if (inList[i].status == CTRL_GOALKEEP)
			{
				continue;
			}
			
				if (m_isMidPasserCannotTakeBall==true && m_nMidPassPlayer==i)
				{
					continue;
				}
			
			if (m_Perform_ATK(&inList[i]) < atkperform)
			{
				best = i;
				atkperform = m_Perform_ATK(&inList[i]);
			}
		}
	}
	return best;
}

int CUPFullField::m_Perform_ATK(uagent *inagent)
{
	//出现故障的未带球机器效用最低，让其与球的距离最大
	if (inagent->status == ROBST_ERR && inagent->dist_ball != 0)
	{
		return 10000;
	}

	//以机器人与球的距离作为绩效考核基础分
	int ret = inagent->dist_ball;

	//之前被分配进攻角色的机器人有优待，这样可保持进攻角色的连贯性
	if (inagent->status == CTRL_ATTACK /*&& m_nSWAtkDly[inagent->ID] > ATK_SW_DLY-1*/)
	{
		ret -= 200;
	}

	if (inagent->status == CTRL_CATCH)
	{
		ret -= 1000;
	}

	//0角度防守堵枪眼的队员尽量不转换
	if (inagent->status == CTRL_DEFENCE && inagent->logDefAngle == 0)
	{
		ret += 300;
	}

	//对具有站位优势的队员优待(夹角)(主动带球的机器不需要考虑此项)
	int ang = (int)m_CalAngle(inagent->x,inagent->y,m_trueball.x,m_trueball.y,1500,600);
	ret -= (int)(ang*0.5);

	//看不见球的队员进攻效用下降
// 	if (inagent->foundball == false)
// 	{
// 		ret += 500;
// 	}

	//进攻效用补偿，根据机器人位置变化的情况判断其是否故障
// 	if (m_nAtkDly > 0)
// 	{
// 		if (inagent->ID == m_bakAtkAgent.ID)
// 		{
// 			ret += AG_ATKADD;
// 		}
// 	}

	return ret;
}

CString CUPFullField::GetCTRLString(int inCtrlCode)
{
	CString strC;
	switch(inCtrlCode)
	{
		case CTRL_MID_PASS:
			strC.Format("mid传球");
			break;
		case CTRL_MID_CATCH:
			strC.Format("mid接球");
			break;
		case CTRL_ATTACK:
			strC.Format("进攻");
			break;
		case CTRL_STOP:
			strC.Format("停止");
			break;
		case CTRL_MOVETO:
			strC.Format("移动");
			break;

		case CTRL_GOALKEEP:
			strC.Format("守门员");
			break;

		case CTRL_PRODEF:
			strC.Format("高级防守");
			break;

		case CTRL_ATTENTIONBALL:
			strC.Format("球观察员");
			break;

		case CTRL_SHIFTATK:
			strC.Format("进攻跑位");
			break;

		case CTRL_DEFENCE:
			strC.Format("防守");
			break;

		case CTRL_PASS:
			strC.Format("传球");
			break;

		case CTRL_3M_PASS:
			strC.Format("常规传球");
			break;

		case CTRL_3M_CATCH:
			strC.Format("常规接球");
			break;

		case CTRL_CATCH:
			strC.Format("接球");
			break;

		case CTRL_IDLE:
			strC.Format("解锁");
			break;

		//跑位
		case CTRL_KICKOFF_PRIM_READY:
			strC.Format("主导开球跑位");
			break;

		case CTRL_KICKOFF_SLAVE_READY:
			strC.Format("辅助开球跑位");
			break;
		
		case CTRL_KICKOFF_PRIM:
			strC.Format("主导开球");
			break;

		case CTRL_KICKOFF_SLAVE:
			strC.Format("辅助开球");
			break;

		case CTRL_SEARCHBALL:
			strC.Format("搜球");
			break;

		case ROBST_ERR:
			strC.Format("待机状态");
			break;

			case CTRL_FOLLOWPOSITION:
			strC.Format("贴身防守状态");
			break;
			
		default:
			strC.Format("未知");
			break;
	}
	return strC;
}

CString CUPFullField::GetTime()
{
	m_critiTime.Lock();
	CString time = m_strTime;
	m_critiTime.Unlock();
	return time;
}

void CUPFullField::Tac_Wait(int inSec)
{
	SetTimer(TAC_WAIT_TIMER,inSec*1000*1.2 ,NULL);
}

void CUPFullField::Ready_KickOff()
{
	m_eStage = sKickOffReady;
	if (m_bFoundball == true)
	{	
		m_strCurSt.Format("我方预备开球...");
		//2010年使用moveto策略
		m_tacctrl[1].ctrl = CTRL_GOALKEEP;
		//其他队员跑动到指定点
		for (int i=2;i<6;i++)
		{
			m_tacctrl[i].ctrl = CTRL_MOVETO;
			m_tacctrl[i].target_angle = 0;	
		}

		CPoint tmpball = m_trueball;
		
		CPoint posCatch;
		CPoint posPass;
		
		{
			//传球队员-球-对方球门 = -120度
			CPoint tmp = m_Loca_Ray(tmpball.x,tmpball.y,1600,600,(int)(100*m_fKField));	//最后一个参数是距离
			tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,-92);
			m_tacctrl[2].target_x = tmp.x;
			m_tacctrl[2].target_y = tmp.y;
			m_tacctrl[2].target_angle = -60;//m_Loca_CalAngle(tmp.x,tmp.y,tmpball.x,tmpball.y);		
			posPass.x = tmp.x;
			posPass.y = tmp.y;
			
			//接球进攻队员-球-对方球门 = 120度
			tmp = m_Loca_Ray(tmpball.x,tmpball.y,1600,600,(int)(250*m_fKField));			//最后一个参数是距离
			tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,88);
			m_tacctrl[3].target_x = tmp.x;
			m_tacctrl[3].target_y = tmp.y;
			m_tacctrl[3].target_angle = m_Loca_CalAngle(tmp.x,tmp.y,tmpball.x,tmpball.y);
			posCatch.x = tmp.x;
			posCatch.y = tmp.y;
		}
		
		//修正跑动位置，避免不巧与球的距离太近
		if (m_CalDist(m_trueball.x,m_trueball.y,m_Pos_Back.x,m_Pos_Back.y) > 200)
		{
			m_tacctrl[4].target_x = m_Pos_Back.x;
			m_tacctrl[4].target_y = m_Pos_Back.y;
		} 
		else
		{	
			m_tacctrl[4].target_x = 300;
			m_tacctrl[4].target_y = 900;
		}
		
		if (m_CalDist(m_trueball.x,m_trueball.y,300,300) > 200)
		{
			m_tacctrl[5].target_x = 300;
			m_tacctrl[5].target_y = 300;
		} 
		else
		{
			m_tacctrl[5].target_x = m_Pos_Back.x;
			m_tacctrl[5].target_y = m_Pos_Back.y;
		}	
		//指定接球者ID
		//m_nCatchPlayer = m_Ag_GetNearBy(m_tacctrl[3].target_x,m_tacctrl[3].target_y,m_tacagent);	
		m_nCatchPlayer = m_Ag_GetNearBy(posCatch.x,posCatch.y,m_tacagent);
		m_nPassPlayer = m_Ag_GetNearBy(posPass.x,posPass.y,m_tacagent);
		//DefaultReady();
		//debug
		CString strTmp;
		strTmp.Format(
			"传球%d(%d,%d) 接球%d(%d,%d)",
			m_nPassPlayer,posPass.x,posPass.y,
			m_nCatchPlayer,posCatch.x,posCatch.y
			);
		printf(strTmp);

		m_TacAction(m_tacagent);		
	} 
	else
	{
		m_strCurSt.Format("我方预备开球，球失踪");
		//开球跑位
		m_tacctrl[1].ctrl = CTRL_GOALKEEP;

		//其他队员跑动到指定点
		for (int i=2;i<7;i++)
		{
			m_tacctrl[i].ctrl = CTRL_MOVETO;
			m_tacctrl[i].target_angle = 0;	
		}

		//计算跑动位置
		m_tacctrl[2].target_x = 600;
		m_tacctrl[2].target_y = 600;

		m_tacctrl[3].target_x = 600;
		m_tacctrl[3].target_y = 900;

		m_tacctrl[4].target_x = 300;
		m_tacctrl[4].target_y = 300;

		m_tacctrl[5].target_x = 300;
		m_tacctrl[5].target_y = 900;

		m_tacctrl[6].target_x = 300;
		m_tacctrl[6].target_y = 600;

		m_TacAction(m_tacagent);
	}
}

void CUPFullField::Ready_Freekick()
{
	if (m_bFoundball == true)
	{
		m_strCurSt.Format("我方预备任意球...");
		DefaultReady();
	} 
	else
	{
		m_strCurSt.Format("我方预备任意球时，看不到球...");
		//我方开球时看不到球
		m_tacctrl[1].ctrl = CTRL_GOALKEEP;

		//其他队员跑动到指定点
		for (int i=2;i<7;i++)
		{
			m_tacctrl[i].ctrl = CTRL_MOVETO;
			m_tacctrl[i].target_angle = 0;	
		}
		//关键角色	
		m_tacctrl[2].ctrl = CTRL_SEARCHBALL;

		m_tacctrl[3].target_x = m_trueball.x - 100;
		m_tacctrl[3].target_y = m_trueball.y;
		m_tacctrl[3].target_angle = 0;

		//计算跑动位置
		m_tacctrl[4].target_x = 400;
		m_tacctrl[4].target_y = 300;

		m_tacctrl[5].target_x = 400;
		m_tacctrl[5].target_y = 900;

		m_tacctrl[6].target_x = 400;
		m_tacctrl[6].target_y = 700;

		m_TacAction(m_tacagent);
	}
}

void CUPFullField::Ready_Goalkick()
{
	if (m_bFoundball == true)
	{
		m_strCurSt.Format("我方预备球门球...");
		DefaultReady();
	} 
	else
	{	
		m_strCurSt.Format("我方预备球门球，球失踪");
		//我方球门球时看不到球
		m_tacctrl[1].ctrl = CTRL_GOALKEEP;

		//其他队员跑动到指定点
		for (int i=2;i<7;i++)
		{
			m_tacctrl[i].ctrl = CTRL_MOVETO;
			m_tacctrl[i].target_angle = 0;	
		}
		//关键角色
		m_tacctrl[2].target_x = 200;
		m_tacctrl[2].target_y = 300;
		m_tacctrl[2].target_angle = 0;

		m_tacctrl[3].target_x = 200;
		m_tacctrl[3].target_y = 900;
		m_tacctrl[3].target_angle = 0;
		
		m_tacctrl[4].ctrl = CTRL_SEARCHBALL;

		//次要角色
		m_tacctrl[5].target_x = m_trueball.x-100;
		m_tacctrl[5].target_y = m_trueball.y;
		m_tacctrl[5].target_angle = 0;

		m_tacctrl[6].target_x = 200;
		m_tacctrl[6].target_y = 700;
		m_tacctrl[6].target_angle = 0;

		m_TacAction(m_tacagent);
	}
}

void CUPFullField::Ready_Throwin()
{
	if (m_bFoundball == true)
	{
		m_strCurSt.Format("我方预备界外球...");
		DefaultReady();
	} 
	else
	{
		m_strCurSt.Format("我方预备界外球，球失踪");
		//我方球门球时看不到球
		m_tacctrl[1].ctrl = CTRL_GOALKEEP;

		//其他队员跑动到指定点
		for (int i=2;i<7;i++)
		{
			m_tacctrl[i].ctrl = CTRL_MOVETO;
			m_tacctrl[i].target_angle = 0;	
		}
		//关键角色	
		m_tacctrl[2].ctrl = CTRL_SEARCHBALL;

		m_tacctrl[3].target_x = m_trueball.x-100;
		m_tacctrl[3].target_y = m_trueball.y;
		m_tacctrl[3].target_angle = 0;

		//计算跑动位置
		m_tacctrl[4].target_x = 400;
		m_tacctrl[4].target_y = 300;

		m_tacctrl[5].target_x = 400;
		m_tacctrl[5].target_y = 900;

		m_tacctrl[6].target_x = 400;
		m_tacctrl[6].target_y = 700;

		m_TacAction(m_tacagent);
	}
}

void CUPFullField::Ready_CornerKick()
{
	if (m_bFoundball == true)
	{
		m_strCurSt.Format("我方预备角球...");
		DefaultReady();
	} 
	else
	{
		printf("角球跑位时球失踪！");

		m_tacctrl[1].ctrl = CTRL_GOALKEEP;
		for (int i=2;i<7;i++)
		{
			m_tacctrl[i].ctrl = CTRL_MOVETO;
			m_tacctrl[i].target_angle = 0;
		}
		
		m_tacctrl[2].ctrl = CTRL_SEARCHBALL;

		m_tacctrl[3].target_x = 1600;
		m_tacctrl[3].target_y = 1100;
		m_tacctrl[3].target_angle = -60;

		m_tacctrl[4].target_x = 1600;
		m_tacctrl[4].target_y = 100;
		m_tacctrl[4].target_angle = 120;
		
		m_tacctrl[5].target_x = 500;
		m_tacctrl[5].target_y = 600;
		m_tacctrl[5].target_angle = 0;
		
		m_tacctrl[6].target_x = 900;
		m_tacctrl[6].target_y = 600;
		m_tacctrl[6].target_angle = 0;

		m_TacAction(m_tacagent);
	}
}

void CUPFullField::Ready_Penalty()
{
	m_strCurSt.Format("我方预备点球...");
	//m_tacctrl[1].ctrl = CTRL_GOALKEEP;
	m_tacctrl[1].ctrl = CTRL_MOVETO;
	m_tacctrl[1].target_x = 10;
	m_tacctrl[1].target_y = 600;
	m_tacctrl[1].target_angle = 0;
	
	m_tacctrl[2].ctrl = CTRL_MOVETO;
	m_tacctrl[2].target_x = 900;
	m_tacctrl[2].target_y = 650;
	m_tacctrl[2].target_angle = 0;

	//其他队员跑动到指定点
	for (int i=3;i<7;i++)
	{
		m_tacctrl[i].ctrl = CTRL_STOP;
		m_tacctrl[i].target_angle = 90;	
	}
	//其他

	m_tacctrl[3].target_x = 300;
	m_tacctrl[3].target_y = 100;

	m_tacctrl[4].target_x = 500;
	m_tacctrl[4].target_y = 100;

	m_tacctrl[5].target_x = 800;
	m_tacctrl[5].target_y = 100;

	m_tacctrl[6].target_x = 100;
	m_tacctrl[6].target_y = 100;

	m_TacAction(m_tacagent);
}

void CUPFullField::Ready_DroppedBall()
{
	/*/////////////////////////////////////////////////////////////////////////
	Tac_DefTest(300);
	return;
	/////////////////////////////////////////////////////////////////////////*/
	if (m_bFoundball == true)
	{
		m_strCurSt.Format("预备争球...");
		//////////////////////////////////////////////////////////////////////////
		//09年策略
		DropBall_Ready(TAC_DIST_DROPBALL);

		/*/////////////////////////////////////////////////////////////////////////
		// 08年策略
		//对方开球跑位
		m_tacctrl[1].ctrl = CTRL_GOALKEEP;

		m_tacctrl[2].ctrl = CTRL_DEFENCE;
		m_tacctrl[2].def_dist = TAC_DIST_DROPBALL;
		m_tacctrl[2].def_angle = 0;

		if (m_trueball.x < 1000)
		{
			if (m_trueball.y > 600)
			{
				//球靠上
				//其他队员在上边线围堵态势
				for (int i=3;i<7;i++)
				{
					m_tacctrl[i].ctrl = CTRL_DEFENCE;
				}

				m_tacctrl[3].def_dist = TAC_DIST_ANTI;
				m_tacctrl[3].def_angle = 0;

				m_tacctrl[4].def_dist = TAC_DIST_ANTI;
				m_tacctrl[4].def_angle = 30;

				m_tacctrl[5].def_dist = TAC_DIST_ANTI;
				m_tacctrl[5].def_angle = 60;

				m_tacctrl[6].def_dist = TAC_DIST_ANTI;
				m_tacctrl[6].def_angle = 90;
			}
			else
			{
				//球靠下
				//其他队员在下边线围堵态势
				for (int i=3;i<7;i++)
				{
					m_tacctrl[i].ctrl = CTRL_DEFENCE;
				}

				m_tacctrl[3].def_dist = TAC_DIST_ANTI;
				m_tacctrl[3].def_angle = 0;

				m_tacctrl[4].def_dist = TAC_DIST_ANTI;
				m_tacctrl[4].def_angle = -30;

				m_tacctrl[5].def_dist = TAC_DIST_ANTI;
				m_tacctrl[5].def_angle = -60;

				m_tacctrl[6].def_dist = TAC_DIST_ANTI;
				m_tacctrl[6].def_angle = -90;
			}
		} 
		else
		{		
			m_tacctrl[3].ctrl = CTRL_DEFENCE;
			m_tacctrl[3].def_dist = TAC_DIST_ANTI+200;
			if (m_trueball.y > 600)
			{
				m_tacctrl[3].def_angle = -30;
			} 
			else
			{
				m_tacctrl[3].def_angle = 30;
			}

			//其他队员跑动到指定点
			for (int i=4;i<7;i++)
			{
				m_tacctrl[i].ctrl = CTRL_MOVETO;
				m_tacctrl[i].target_angle = 0;	
			}

			//计算跑动位置
			m_tacctrl[6].target_x = 400;
			m_tacctrl[6].target_y = 700;

			m_tacctrl[4].target_x = 400;
			m_tacctrl[4].target_y = 300;

			m_tacctrl[5].target_x = 400;
			m_tacctrl[5].target_y = 900;
		}

		m_TacAction(m_tacagent);
		/////////////////////////////////////////////////////////////////////////*/
	} 
	else
	{		
		m_strCurSt.Format("预备争球，球失踪...");
		m_tacctrl[1].ctrl = CTRL_GOALKEEP;

		//其他队员跑动到指定点
		for (int i=2;i<7;i++)
		{
			m_tacctrl[i].ctrl = CTRL_MOVETO;
			m_tacctrl[i].target_angle = 0;	
		}
		//关键角色
		m_tacctrl[2].ctrl = CTRL_SEARCHBALL;

		m_tacctrl[3].target_x = m_trueball.x-100;
		m_tacctrl[3].target_y = m_trueball.y;	

		//计算跑动位置
		m_tacctrl[4].target_x = 600;
		m_tacctrl[4].target_y = 300;

		m_tacctrl[5].target_x = 600;
		m_tacctrl[5].target_y = 900;

		m_tacctrl[6].target_x = 600;
		m_tacctrl[6].target_y = 700;

		m_TacAction(m_tacagent);
	}
}

void CUPFullField::Ready_Anti_Kickoff()
{
	if (m_bFoundball == true)
	{
		m_strCurSt.Format("对方预备开球...");
		//////////////////////////////////////////////////////////////////////////
		//2010年
		m_tacctrl[1].ctrl = CTRL_GOALKEEP;
		
		for (int i=2;i<7;i++)
		{
			m_tacctrl[i].ctrl = CTRL_MOVETO;
		}
		
		//复制球位置
		CPoint tmpball = m_trueball;
		
		CPoint tmp;	

		//在球到球门之间堵上一个
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)(TAC_DIST_ANTI*m_fKField));
		m_tacctrl[2].target_x = tmp.x;
		m_tacctrl[2].target_y = tmp.y;
		if (m_tacctrl[2].target_x < 200)
		{
			m_tacctrl[2].target_x = 200;
		}
		//////////////////////////////////////////////////////////////////////////
		m_tacctrl[2].ctrl = CTRL_AROUNDBALL;
		m_tacctrl[2].target_x = 0;
		m_tacctrl[2].target_y = 600;
		m_tacctrl[2].def_dist = TAC_DIST_ANTI*m_fKField;
		m_tacctrl[2].def_angle = 0;
		//////////////////////////////////////////////////////////////////////////
		
		//边翼1
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)(TAC_DIST_ANTI*m_fKField));
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,-70);
		m_tacctrl[3].target_x = tmp.x;
		m_tacctrl[3].target_y = tmp.y;
		//////////////////////////////////////////////////////////////////////////
		m_tacctrl[3].ctrl = CTRL_AROUNDBALL;
		m_tacctrl[3].target_x = 0;
		m_tacctrl[3].target_y = 600;
		m_tacctrl[3].def_dist = TAC_DIST_ANTI*m_fKField;
		m_tacctrl[3].def_angle = -70;
		//////////////////////////////////////////////////////////////////////////
		
		//后卫
		m_tacctrl[4].target_x = m_Pos_Back.x-150;
		m_tacctrl[4].target_y = m_Pos_Back.y;
		
		//边翼2
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)(TAC_DIST_ANTI*m_fKField));
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,70);
		m_tacctrl[5].target_x = tmp.x;
		m_tacctrl[5].target_y = tmp.y;
		//////////////////////////////////////////////////////////////////////////
		m_tacctrl[5].ctrl = CTRL_AROUNDBALL;
		m_tacctrl[5].target_x = 0;
		m_tacctrl[5].target_y = 600;
		m_tacctrl[5].def_dist = TAC_DIST_ANTI*m_fKField;
		m_tacctrl[5].def_angle = 70;
		//////////////////////////////////////////////////////////////////////////
		
		//////////////////////////////////////////////////////////////////////////
		/*/修正，盯人修正，为防御开球添加防守对方队员的修正	
		MSG_Control defctrl[2];	//离球最近的两个对方球员 
		defctrl[0].target_x = defctrl[1].target_x =900;	
		defctrl[0].target_y = defctrl[1].target_y =600;

		memcpy(m_tmpObst,m_tacObst,(EACH_AG_OBST_NUM*(AG_NUM+1))*sizeof(obstacle));
		int nearobst = -1;		
		//（让两个边翼防守可能接球的对方队员）
		int flag = 0;//0：没有找到过障碍物，1：上一个障碍物位置偏上，2：上一个障碍物位置偏下
		for (i=0;i<2;i++)
		{
			int nodead = 0;
			while (1)
			{
				nodead ++;
				if (nodead > 100)
				{
					break;
				}
				nearobst = m_Obst_NearBy(m_trueball.x,m_trueball.y,m_tmpObst);
				if (nearobst != -1)
				{
					//获取到一个障碍物，查看它与球的距离
					if (m_CalDist(m_tmpObst[nearobst].x,m_tmpObst[nearobst].y,m_trueball.x,m_trueball.y) < 150 || m_tmpObst[nearobst].x > 1200)
					{
						//它与球距离小于2米，应该是准备传球的队员，忽略
						m_tmpObst[nearobst].valid = false;
						nearobst = -1;		//让循环得以继续
					}
					else
					{
						if (m_CalDist(m_tmpObst[nearobst].x,m_tmpObst[nearobst].y,1800,600) < 200 )
						{
							//距离对方球门很近，有可能是对方守门员，不符合要求
							m_tmpObst[nearobst].valid = false;
							nearobst = -1;		//让循环得以继续
							continue;
						}

						//查看上一个障碍物是否和这次搜索的处于同一侧
						if (flag == 1 && m_tmpObst[nearobst].y > 600)
						{
							m_tmpObst[nearobst].valid = false;
							nearobst = -1;		//让循环得以继续
							continue;
						}
						if (flag == 2 && m_tmpObst[nearobst].y < 600)
						{
							m_tmpObst[nearobst].valid = false;
							nearobst = -1;		//让循环得以继续
							continue;
						}
						//符合要求
						break;
					}
				}
				else
				{
					//结果返回-1，说明没有障碍物
					break;
				}
			}
			
			CPoint tmpPnt;
			if (nearobst != -1)
			{
				//从障碍物向球延伸1米
				tmpPnt = m_Loca_Ray(m_tmpObst[nearobst].x,m_tmpObst[nearobst].y,m_trueball.x,m_trueball.y,100);

				//我方半场对应位置
				tmpPnt.x = 800;
				
				//如果距离球太近，则向场外延伸
				int nodead = 0;
				while(m_CalDist(tmpPnt.x,tmpPnt.y,m_trueball.x,m_trueball.y) < TAC_DIST_ANTI+50)
				{
					nodead ++;
					if (nodead > 100)
					{
						break;
					}
					if (tmpPnt.y > 600)
					{
						tmpPnt.y += 50;
					}
					else
					{
						tmpPnt.y -= 50;
					}
				}

				if (tmpPnt.y < 600)
				{
					m_tacctrl[3].target_x = tmpPnt.x;
					m_tacctrl[3].target_y = tmpPnt.y;
					flag = 2;
				}
				else
				{
					m_tacctrl[5].target_x = tmpPnt.x;
					m_tacctrl[5].target_y = tmpPnt.y;
					flag = 1;
				}

				//该障碍物已经被防守，关闭其使能
				m_tmpObst[nearobst].valid = false;
			}		
		}

		//角度
		for (i=2;i<6;i++)
		{
			m_tacctrl[i].target_angle = m_Loca_CalAngle(m_tacctrl[i].target_x,m_tacctrl[i].target_y,tmpball.x,tmpball.y);
		}*/

		//////////////////////////////////////////////////////////////////////////
		//09年合并到一个行为里了
		//Default_Anti_Ready(TAC_DIST_ANTI+20);
		//////////////////////////////////////////////////////////////////////////

	} 
	else
	{
		m_strCurSt.Format("对方预备开球，球失踪");
		//对方开球，我方却看不到球（太背了。。）
		m_tacctrl[1].ctrl = CTRL_GOALKEEP;

		//其他队员跑动到指定点
		for (int i=2;i<7;i++)
		{
			m_tacctrl[i].ctrl = CTRL_MOVETO;
			m_tacctrl[i].target_angle = 0;	
		}
		//关键角色
		m_tacctrl[2].target_x = 700;
		m_tacctrl[2].target_y = 800;
		m_tacctrl[2].target_angle = 0;
		
		m_tacctrl[3].target_x = 700;
		m_tacctrl[3].target_y = 400;
		m_tacctrl[3].target_angle = 0;

		//计算跑动位置
		m_tacctrl[4].target_x = 400;
		m_tacctrl[4].target_y = 300;

		m_tacctrl[5].target_x = 400;
		m_tacctrl[5].target_y = 900;

		m_tacctrl[6].target_x = 400;
		m_tacctrl[6].target_y = 700;	
	}
	m_TacAction(m_tacagent);
}

void CUPFullField::Ready_Anti_Freekick()
{
	if (m_bFoundball == true)
	{
		m_strCurSt.Format("对方预备任意球...");
		//////////////////////////////////////////////////////////////////////////
		//09年合并到一个行为里了
		Default_Anti_Ready(TAC_DIST_ANTI);

		/*/////////////////////////////////////////////////////////////////////////
		//08年
		//优先判断球的位置
		if (m_trueball.x < 1000)
		{
			//球在我方半场
			m_tacctrl[1].ctrl = CTRL_GOALKEEP;

			if (m_CalDist(m_trueball.x,m_trueball.y,0,600) > TAC_DIST_ANTI+250)
			{
				//其他队员跑位切换到围堵态势
				for (int i=2;i<7;i++)
				{
					m_tacctrl[i].ctrl = CTRL_DEFENCE;
				}

				m_tacctrl[2].def_dist = TAC_DIST_ANTI;
				m_tacctrl[2].def_angle = 0;

				m_tacctrl[3].def_dist = TAC_DIST_ANTI;
				m_tacctrl[3].def_angle = 60;

				m_tacctrl[4].def_dist = TAC_DIST_ANTI;
				m_tacctrl[4].def_angle = -30;

				m_tacctrl[5].def_dist = TAC_DIST_ANTI;
				m_tacctrl[5].def_angle = 30;

				m_tacctrl[6].def_dist = TAC_DIST_ANTI;
				m_tacctrl[6].def_angle = -60;
			} 
			else
			{
				//其他队员定点跑位
				for (int i=2;i<7;i++)
				{
					m_tacctrl[i].ctrl = CTRL_MOVETO;
				}

				m_tacctrl[2].target_x = 250;
				m_tacctrl[2].target_y = m_trueball.y + 200;

				m_tacctrl[3].target_x = 250;
				m_tacctrl[3].target_y = m_trueball.y - 200;

				m_tacctrl[4].target_x = m_trueball.x + 300;
				m_tacctrl[4].target_y = m_trueball.y;

				m_tacctrl[5].target_x = 500;
				m_tacctrl[5].target_y = 600;

				m_tacctrl[6].target_x = 500;
				m_tacctrl[6].target_y = 100;
			}
			
		}
		else
		{
			//球在对方半场
			m_tacctrl[1].ctrl = CTRL_GOALKEEP;

			m_tacctrl[2].ctrl = CTRL_DEFENCE;
			m_tacctrl[2].def_dist = TAC_DIST_ANTI;
			m_tacctrl[2].def_angle = 0;

			//其他队员跑动到指定点
			for (int i=3;i<7;i++)
			{
				m_tacctrl[i].ctrl = CTRL_MOVETO;
				m_tacctrl[i].target_angle = 0;
			}

			//计算跑动位置
			m_tacctrl[3].target_x = 500;
			m_tacctrl[3].target_y = 700;

			m_tacctrl[4].target_x = 600;
			m_tacctrl[4].target_y = 300;

			m_tacctrl[5].target_x = 600;
			m_tacctrl[5].target_y = 900;
			
			m_tacctrl[6].target_x = 800;
			m_tacctrl[6].target_y = 800;
		}
		/////////////////////////////////////////////////////////////////////////*/
	} 
	else
	{
		m_strCurSt.Format("对方预备任意球，球失踪");
		//对方预备任意球时看不到球
		m_tacctrl[1].ctrl = CTRL_GOALKEEP;

		//其他队员跑动到指定点
		for (int i=2;i<7;i++)
		{
			m_tacctrl[i].ctrl = CTRL_MOVETO;
			m_tacctrl[i].target_angle = 0;	
		}
		//关键角色	
		m_tacctrl[2].ctrl = CTRL_SEARCHBALL;

		m_tacctrl[3].target_x = m_trueball.x-100;
		m_tacctrl[3].target_y = m_trueball.y;
		m_tacctrl[3].target_angle = 0;

		//计算跑动位置
		m_tacctrl[4].target_x = 400;
		m_tacctrl[4].target_y = 300;

		m_tacctrl[5].target_x = 400;
		m_tacctrl[5].target_y = 900;

		m_tacctrl[6].target_x = 400;
		m_tacctrl[6].target_y = 700;

		m_TacAction(m_tacagent);
	}
}

void CUPFullField::Ready_Anti_Goalkick()
{
	
	if (m_bFoundball == true)
	{
		m_strCurSt.Format("对方预备球门球...");
		Default_Anti_Ready(TAC_DIST_ANTI);
	} 
	else
	{
		m_strCurSt.Format("对方预备球门球，未发现球");
		m_tacctrl[1].ctrl = CTRL_GOALKEEP;

		//其他队员跑动到指定点
		for (int i=2;i<7;i++)
		{
			m_tacctrl[i].ctrl = CTRL_MOVETO;
			m_tacctrl[i].target_angle = 0;	
		}
		//关键角色，跑到对方球门附近
		m_tacctrl[2].target_x = 1400;
		m_tacctrl[2].target_y = 400;
		
		m_tacctrl[3].target_x = 1400;
		m_tacctrl[3].target_y = 800;

		//计算跑动位置
		m_tacctrl[4].target_x = 400;
		m_tacctrl[4].target_y = 300;

		m_tacctrl[5].target_x = 400;
		m_tacctrl[5].target_y = 900;

		m_tacctrl[6].target_x = 400;
		m_tacctrl[6].target_y = 700;

		m_TacAction(m_tacagent);
	}

}

void CUPFullField::Ready_Anti_Throwin()
{
	if (m_bFoundball == true)
	{
		m_strCurSt.Format("对方预备边线球...");//对方开球跑位
		//////////////////////////////////////////////////////////////////////////
		//09年合并到一个行为里了
		Default_Anti_Ready(TAC_DIST_ANTI);

		/*/////////////////////////////////////////////////////////////////////////
		//08年
		m_tacctrl[1].ctrl = CTRL_GOALKEEP;

		m_tacctrl[2].ctrl = CTRL_DEFENCE;
		m_tacctrl[2].def_dist = TAC_DIST_ANTI+100;
		m_tacctrl[2].def_angle = 0;

		if (m_trueball.x > 1000)
		{
			//球在对方半场
			
			m_tacctrl[3].ctrl = CTRL_DEFENCE;
			m_tacctrl[3].def_dist = TAC_DIST_ANTI;
			if (m_trueball.y > 600)
			{
				m_tacctrl[3].def_angle = -30;
			} 
			else
			{
				m_tacctrl[3].def_angle = 30;
			}

			//其他队员跑动到指定点
			for (int i=4;i<7;i++)
			{
				m_tacctrl[i].ctrl = CTRL_MOVETO;
				m_tacctrl[i].target_angle = 0;	
			}

			//计算跑动位置
			m_tacctrl[6].target_x = 600;
			m_tacctrl[6].target_y = 700;

			m_tacctrl[4].target_x = 600;
			m_tacctrl[4].target_y = 300;

			m_tacctrl[5].target_x = 600;
			m_tacctrl[5].target_y = 900;
		} 
		else
		{
			//球在己方半场
			if (m_trueball.y > 600)
			{
				//球在上边界
				//其他队员在上边线围堵态势
				for (int i=3;i<7;i++)
				{
					m_tacctrl[i].ctrl = CTRL_DEFENCE;
				}

				m_tacctrl[3].def_dist = TAC_DIST_ANTI;
				m_tacctrl[3].def_angle = 0;

				m_tacctrl[4].def_dist = TAC_DIST_ANTI;
				m_tacctrl[4].def_angle = 30;

				m_tacctrl[5].def_dist = TAC_DIST_ANTI;
				m_tacctrl[5].def_angle = 60;

				m_tacctrl[6].def_dist = TAC_DIST_ANTI;
				m_tacctrl[6].def_angle = 90;
			}
			else
			{
				//球在下边界
				//其他队员在下边线围堵态势
				for (int i=3;i<7;i++)
				{
					m_tacctrl[i].ctrl = CTRL_DEFENCE;
				}

				m_tacctrl[3].def_dist = TAC_DIST_ANTI;
				m_tacctrl[3].def_angle = 0;

				m_tacctrl[4].def_dist = TAC_DIST_ANTI;
				m_tacctrl[4].def_angle = -30;

				m_tacctrl[5].def_dist = TAC_DIST_ANTI;
				m_tacctrl[5].def_angle = -60;

				m_tacctrl[6].def_dist = TAC_DIST_ANTI;
				m_tacctrl[6].def_angle = -90;
			}
		}
		/////////////////////////////////////////////////////////////////////////*/
	} 
	else
	{		
		m_strCurSt.Format("对方预备边线球，未发现球");
		m_tacctrl[1].ctrl = CTRL_GOALKEEP;

		//其他队员跑动到指定点
		for (int i=2;i<7;i++)
		{
			m_tacctrl[i].ctrl = CTRL_MOVETO;
			m_tacctrl[i].target_angle = 0;	
		}
		//关键角色		
		m_tacctrl[2].ctrl = CTRL_SEARCHBALL;

		m_tacctrl[3].target_x = m_trueball.x-100;
		m_tacctrl[3].target_y = m_trueball.y;

		//计算跑动位置
		m_tacctrl[4].target_x = 400;
		m_tacctrl[4].target_y = 300;

		m_tacctrl[5].target_x = 400;
		m_tacctrl[5].target_y = 900;

		m_tacctrl[6].target_x = 400;
		m_tacctrl[6].target_y = 700;
		
		m_TacAction(m_tacagent);
	}
}

void CUPFullField::Ready_Anti_CornerKick()
{
	if (m_bFoundball == true)
	{
		m_strCurSt.Format("对方预备角球...");
		//////////////////////////////////////////////////////////////////////////
		//09年合并到一个行为里了
		Default_Anti_Ready(TAC_DIST_ANTI);

		/*/////////////////////////////////////////////////////////////////////////
		//08年
		m_tacctrl[1].ctrl = CTRL_GOALKEEP;

		//其他队员跑位切换到围堵态势
		for (int i=2;i<7;i++)
		{
			m_tacctrl[i].ctrl = CTRL_DEFENCE;
		}

		if (m_trueball.y > 600)
		{
			//球在上边角
			m_tacctrl[2].def_dist = TAC_DIST_ANTI;
			m_tacctrl[2].def_angle = 0;
			

			m_tacctrl[3].def_dist = TAC_DIST_ANTI;
			m_tacctrl[3].def_angle = 45;

			m_tacctrl[4].def_dist = TAC_DIST_ANTI;
			m_tacctrl[4].def_angle = 60;

			m_tacctrl[5].def_dist = TAC_DIST_ANTI + 300;
			m_tacctrl[5].def_angle = 60;
		} 
		else
		{
			//球在下边角
			m_tacctrl[2].target_x = TAC_DIST_ANTI;
			m_tacctrl[2].target_angle = 0;
			

			m_tacctrl[3].target_x = TAC_DIST_ANTI;
			m_tacctrl[3].target_angle = -45;

			m_tacctrl[4].target_x = TAC_DIST_ANTI;
			m_tacctrl[4].target_angle = -60;

			m_tacctrl[5].target_x = TAC_DIST_ANTI + 300;
			m_tacctrl[5].target_angle = -60;
		}
		m_tacctrl[6].ctrl = CTRL_MOVETO;
		m_tacctrl[6].target_x = 500;
		m_tacctrl[6].target_y = 600;
		m_tacctrl[6].target_angle = 0;
		/////////////////////////////////////////////////////////////////////////*/
	}
	else
	{
		m_strCurSt.Format("对方预备角球，球失踪");
		//看不到球的时候
		m_tacctrl[1].ctrl = CTRL_GOALKEEP;

		//其他队员跑动到指定点
		for (int i=2;i<7;i++)
		{
			m_tacctrl[i].ctrl = CTRL_MOVETO;
			m_tacctrl[i].target_angle = 0;	
		}
		//两名关键角色跑向己方角球区
		m_tacctrl[2].target_x = 200;
		m_tacctrl[2].target_y = 100;
	
		m_tacctrl[3].target_x = 200;
		m_tacctrl[3].target_y = 1100;

		//计算其他跑动位置
		m_tacctrl[4].ctrl = CTRL_SEARCHBALL;

		m_tacctrl[5].target_x = 400;
		m_tacctrl[5].target_y = 300;

		m_tacctrl[6].target_x = 400;
		m_tacctrl[6].target_y = 700;
		m_TacAction(m_tacagent);
	}
}

void CUPFullField::Ready_Anti_Penalty()
{
	m_strCurSt.Format("对方预备点球...");
	KillTimer(TAC_WAIT_TIMER);
	//其他队员跑动到指定点
	for (int i=1;i<7;i++)
	{
		m_tacctrl[i].ctrl = CTRL_MOVETO;
		m_tacctrl[i].target_angle = 90;	
	}
	m_tacctrl[1].target_x = 10;
	m_tacctrl[1].target_y = 600;
	m_tacctrl[1].target_angle = 0;	

	//跑到中间
	m_tacctrl[2].target_x = 500;
	m_tacctrl[2].target_y = 100;

	//其他

	m_tacctrl[3].target_x = 400;
	m_tacctrl[3].target_y = 100;

	m_tacctrl[4].target_x = 300;
	m_tacctrl[4].target_y = 100;

	m_tacctrl[5].target_x = 200;
	m_tacctrl[5].target_y = 100;

	m_tacctrl[6].target_x = 100;
	m_tacctrl[6].target_y = 100;

	m_TacAction(m_tacagent);
}

void CUPFullField::KickAgent(int inID)
{
	m_asc.Lock();
	m_agents[inID].online = false;
	m_asc.Unlock();

	if (SelID == inID)
	{
		SelID = 0;
	}
	Invalidate();
}

void CUPFullField::Test_Multidef(int inDist)
{
	m_strCurSt.Format("联防测试");

	m_nDistOfMultiDef = inDist;
	//m_eStage = sTestMultiDef;
	m_eStage = sMani;
	Tac_DefTest(m_nDistOfMultiDef);
}

void CUPFullField::Test_PressDef()
{
	m_strCurSt.Format("紧逼防守测试");

	m_eStage = sTestPressDef;
	Tac_PressDef(350);
}

void CUPFullField::m_CheckAgentReply()
{
	for (int i=1;i<7;i++)
	{
		if (m_recvcnt[i] > 0)
		{
			//数据包持续返回，则清零
			m_recvcnt[i] = 0;
		} 
		else
		{
			//数据包已经不返回了，将其踢出决策考虑范围
			KickAgent(i);
		}
	}
}

void CUPFullField::m_ShowAgentsInfo()
{
	//避免操作空指针
	if (pAgInfo == NULL)
	{
		return;
	}

	//pAgInfo->DeleteAllItems();

	CString str;
	for (int i=1;i<7;i++)
	{
		if (m_agents[i].online == true)
		{
			//str.Format("%d",i);
			//pAgInfo->InsertItem(i,str);
			//show id
			//str.Format("%d",m_agents[i].ID);
			//pAgInfo->SetItemText(index,0,str);

			//状态
			str = GetCTRLString(m_agents[i].status);
			pAgInfo->SetItemText(i-1,1,str);
			
			//自身位置
			str.Format("%d",m_agents[i].x);
			pAgInfo->SetItemText(i-1,2,str);

			str.Format("%d",m_agents[i].y);
			pAgInfo->SetItemText(i-1,3,str);

			if (m_agents[i].foundball == true)
			{
				str.Format("(%d,%d)",m_balls[i].x,m_balls[i].y);
			} 
			else
			{
				str.Format("视野里无球");
			}
			pAgInfo->SetItemText(i-1,4,str);

			//数据包计数
			str.Format("%d",m_recvcnt[i]);
			pAgInfo->SetItemText(i-1,5,str);

			switch(m_prRightOfPlayer[i])
			{
			case PLAYER_RIGHT_INFIELD:
				str.Format("InField");
				break;

			case PLAYER_RIGHT_WAITFOEPOINT:
				str.Format("WaitForPoint");
				break;

			case PLAYER_RIGHT_READY:
				str.Format("Ready");
				break;

			case PLAYER_RIGHT_OUT:
				str.Format("Out");
				break;
			}
			pAgInfo->SetItemText(i-1,6,str);
		}
		else
		{
			//离线agent的状态显示
			pAgInfo->SetItemText(i-1,1,"OFFLINE");
			
			pAgInfo->SetItemText(i-1,2,"");

			pAgInfo->SetItemText(i-1,3,"");

			pAgInfo->SetItemText(i-1,4,"");

			pAgInfo->SetItemText(i-1,5,"");

			pAgInfo->SetItemText(i-1,6,"");
		}
	}
}

void CUPFullField::Test_checkeveryone()
{
	m_eStage = sMani;
	for (int i=1;i<7;i++)
	{
		m_tacctrl[i].ctrl = CTRL_ATTACK;	
	}
	m_TacAction(m_tacagent);

}

void CUPFullField::Enter()
{
	m_eStage = sEnter;
	m_strCurSt.Format("机器人入场...");
	//控制机器人跑位
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;

	for (int i=2;i<7;i++)
	{
		m_tacctrl[i].ctrl = CTRL_MOVETO;
	}

	m_tacctrl[2].target_x = 800;
	m_tacctrl[2].target_y = 700;

	m_tacctrl[3].target_x = 800;
	m_tacctrl[3].target_y = 500;

	m_tacctrl[4].target_x = 300;
	m_tacctrl[4].target_y = 900;

	m_tacctrl[5].target_x = 300;
	m_tacctrl[5].target_y = 300;

	//角度,都朝向球
	for (i=2;i<6;i++)
	{
		m_tacctrl[i].target_angle = 0;
		//m_tacctrl[i].target_angle = m_Loca_CalAngle(m_tacctrl[i].target_x,m_tacctrl[i].target_y,m_trueball.x,m_trueball.y);
	}

	m_TacAction(m_tacagent);
}

void CUPFullField::Leave()
{
	m_eStage = sOut;
	m_strCurSt.Format("机器人离场...");
	//控制机器人跑位	
	for (int i=1;i<7;i++)
	{
		m_tacctrl[i].ctrl = CTRL_MOVETO;
	}
	
	m_tacctrl[1].target_x = 200;
	m_tacctrl[1].target_y = 100;
	
	m_tacctrl[2].target_x = 350;
	m_tacctrl[2].target_y = 100;
	
	m_tacctrl[3].target_x = 500;
	m_tacctrl[3].target_y = 100;
	
	m_tacctrl[4].target_x = 650;
	m_tacctrl[4].target_y = 100;
	
	m_tacctrl[5].target_x = 800;
	m_tacctrl[5].target_y = 100;

	if (m_fpPlayerIn.x > 900)
	{
		for (i=1;i<AG_NUM;i++)
		{
			m_tacctrl[i].target_x = 1800 - m_tacctrl[i].target_x;
		}
	}

	if (m_fpPlayerIn.y > 600)
	{
		for (i=1;i<AG_NUM;i++)
		{
			m_tacctrl[i].target_y = 1200 - m_tacctrl[i].target_y;
		}
	}
	
	//角度,都朝向球
	for (i=1;i<6;i++)
	{
		if (m_fpPlayerIn.y < 600)
		{
			m_tacctrl[i].target_angle = 90;
		}
		else
		{
			m_tacctrl[i].target_angle = -90;
		}
	}
	
	m_TacAction(m_tacagent);
}

void CUPFullField::TestMode()
{
	m_eStage = sMani;
}

void CUPFullField::ManiMode()
{
	m_eStage = sMani;
}

//查找距离x，y最近的ag
int CUPFullField::m_Ag_GetNearBy(int x, int y, uagent* inalist)
{
	int ret = 0;
	int mindist = 0x7fffffff;
	int tmp_dist = 0;

	bool scanerr = true;	//标记是否还有必要对err的ag进行扫描
	//先扫描健康的ag
	for (int i=2;i<7;i++)
	{
		if (inalist[i].online == true && inalist[i].allocated == false && inalist[i].status != ROBST_ERR)
		{
			//健康的ag里有人，不需要对err的ag进行扫描了
			scanerr = false;

			//判断该健康的ag距离
			tmp_dist = m_CalDist(inalist[i].x,inalist[i].y,x,y);
			if (tmp_dist < mindist)
			{
				ret = i;
				mindist = tmp_dist;
			}
		}
	}

	//如果健康ag已经无人选，则从剩余的ag里挑选
	if (scanerr == true)
	{
		for (int i=1;i<7;i++)
		{
			if (inalist[i].online == true && inalist[i].allocated == false)
			{
				//判断该err的ag距离
				tmp_dist = m_CalDist(inalist[i].x,inalist[i].y,x,y);
				if (tmp_dist < mindist)
				{
					ret = i;
					mindist = tmp_dist;
				}
			}
		}
	}

	return ret;
}

int CUPFullField::m_Ag_GetStatus(UINT inSt, uagent* inalist)
{
	int ret = 0;
	for (int i=1;i<7;i++)
	{
		if (inalist[i].online == true)
		{
			if (inalist[i].status == inSt)
			{
				ret = i;
				break;
			}
		}
	}
	return ret;
}

CPoint CUPFullField::m_Loca_Rotate(int inx, int iny, int inCx, int inCy,int inAng)
{
	CPoint newpoint;
	if (inAng%360 == 0)
	{
		newpoint.x = inx;
		newpoint.y = iny;
		return newpoint;
	}

	//将角度换算成数值
	double vAng = (double)inAng*3.14159/180;

	newpoint.x = (int)((inx - inCx)*cos(vAng)+(iny - inCy)*sin(vAng)+inCx);
	newpoint.y = (int)(-(inx - inCx)*sin(vAng) +(iny - inCy)*cos(vAng) + inCy);

	return newpoint;
}

CPoint CUPFullField::m_Loca_Ray(int inXbase, int inYbase, int inXtarg, int inYtarg, int inDist)
{
	CPoint newpoint;
	int srcDist = m_CalDist(inXbase,inYbase,inXtarg,inYtarg);
	if (srcDist == 0)
	{
		newpoint.x = inXbase;
		newpoint.y = inYbase;
		return newpoint;
	}
	int dx = (int)((double)(inXtarg - inXbase)*inDist/srcDist);
	int dy = (int)((double)(inYtarg - inYbase)*inDist/srcDist);
	newpoint.x = inXbase + dx;
	newpoint.y = inYbase + dy;

	return newpoint;
}

int CUPFullField::m_Loca_CalAngle(int inXbase, int inYbase, int inXtarg, int inYtarg)
{
	double retAngle;
	if (inXbase != inXtarg)
	{
		//分母不为零
		int dx = inXtarg - inXbase;
		double dy = inYtarg - inYbase;
		retAngle = atan(dy/dx);
		
		//换算成角度
		retAngle = retAngle*180/3.14158;

		//象限的修正
		if (retAngle < 0 )
		{
			if (dy > 0)
			{
				retAngle += 180;
			}
		} 
		else
		{
			if (dy < 0)
			{
				retAngle += 180;
			}
		}

		//修正
		while (retAngle < 0)
		{
			retAngle += 360;
		}
		while (retAngle > 360)
		{
			retAngle -= 360;
		}

		//修正
		if (dy == 0)
		{
			if (dx > 0)
			{
				retAngle = 0;
			} 
			else
			{
				retAngle = 180;
			}
		}
	} 
	else
	{
		//inXbase == inXtarg
		if (inYtarg > inYbase)
		{
			retAngle = 90;
		} 
		else
		{
			retAngle = 270;
		}
	}

	return (int)retAngle;
}

//////////////////////////////////////////////////////////////////////////
/*-----------------------------普通进攻模式-----------------------------*/
void CUPFullField::DefaultActive(int inDefDist)
{
	//printf("DefaultActive");

	//初始化控制结构体，按角色关键程度排列
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;
	
	//进攻角色
	m_tacctrl[2].ctrl = CTRL_ATTACK;
	
	//3~5号队员跑位切换到围堵态势
	for (int i=3;i<6;i++)
	{
		m_tacctrl[i].ctrl = CTRL_MOVETO;
	}

	//先假设球位于中线上方，计算位置
	CPoint tmpball = m_trueball;
	if (m_trueball.y < 600)
	{
		tmpball.y = 1200 - tmpball.y;
	}
	
 	CPoint tmp;

	//下面将场地分为上中下三个区域，根据球所处区域形成防守阵型
	if (tmpball.y > 1000)
	{
		//上侧
		//先在球到球门之间堵上一个
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)(inDefDist*m_fKField));
		m_tacctrl[3].target_x = tmp.x;
		m_tacctrl[3].target_y = tmp.y;

		//防守半径比堵球门的大一些，避免换位的时候相撞
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)((inDefDist+150)*m_fKField));
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,-30);
		m_tacctrl[4].target_x = tmp.x;
		m_tacctrl[4].target_y = tmp.y;

		//后卫
		m_tacctrl[5].target_x = m_Pos_Back.x;
 		m_tacctrl[5].target_y = m_Pos_Back.y+300;
		
		//test
// 		tmp = m_Loca_CalBackPos(tmpball.x,tmpball.y,m_Pos_Back.x);
// 		m_tacctrl[4].target_x = tmp.x;
// 		m_tacctrl[4].target_y = tmp.y;
	}
	else
	{
		// tmpball.y < 1000 中间区域
		//先在球到球门之间堵上一个
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)(inDefDist*m_fKField));
		m_tacctrl[3].target_x = tmp.x;
		m_tacctrl[3].target_y = tmp.y;
	
		
		//两翼的长度
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)((inDefDist+150)*m_fKField));
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,30);
		m_tacctrl[4].target_x = tmp.x;
		m_tacctrl[4].target_y = tmp.y;

		//后卫
		m_tacctrl[5].target_x = m_Pos_Back.x;
		m_tacctrl[5].target_y = m_Pos_Back.y;
		
		
		//test
// 		tmp = m_Loca_CalBackPos(tmpball.x,tmpball.y,m_Pos_Back.x);
// 		m_tacctrl[4].target_x = tmp.x;
// 		m_tacctrl[4].target_y = tmp.y;

	}

	//printf("针对自己球员进入禁区做修正");
	//CString tStr;
	//针对自己球员进入禁区做修正
	for (i=3;i<6;i++)
	{
		if (m_Check_MovetoOurPenaltyArea(m_tacctrl[i]) == true)
		{
			//tStr.Format("%d号角色进入禁区，需要修正",i);
			//printf(tStr);
			m_Fix_MovetoOurPenaltyArea(&m_tacctrl[i]);
			//m_tacctrl[i].target_x = 900;
		}
	}

	//针对自己球员出边界进行修正	
	for (i=3;i<6;i++)
	{
		if (m_Check_MoveOutOffFrame(m_tacctrl[i]) == true)
		{
			m_Fix_MoveOutOffFrame(&m_tacctrl[i]);
		}
	}

	//防止撞击
	m_Fix_CollisionAvoid(m_tacctrl, 50);

	//如果球在中线下方，则将坐标上下翻转
	if (m_trueball.y < 600)
	{
		m_tacctrl[2].target_y = 1200 - m_tacctrl[2].target_y;
		m_tacctrl[3].target_y = 1200 - m_tacctrl[3].target_y;		
		m_tacctrl[4].target_y = 1200 - m_tacctrl[4].target_y;
		m_tacctrl[5].target_y = 1200 - m_tacctrl[5].target_y;
	}

	//角度,都朝向球
	for (i=3;i<6;i++)
	{
		m_tacctrl[i].target_angle = 0;		//角度不变化，平移的时候稳定一些
		//m_tacctrl[i].target_angle = m_Loca_CalAngle(m_tacctrl[i].target_x,m_tacctrl[i].target_y,m_trueball.x,m_trueball.y);
	}

	m_TacAction(m_tacagent);
}
//////////////////////////////////////////////////////////////////////////
//------------------------------常规进攻---------------------------------
void CUPFullField:: Tac_NormalAttack()
{
	m_tacctrl[1].ctrl = CTRL_GOALKEEP ;
	m_tacctrl[2].ctrl = CTRL_ATTACK ; 
	m_tacctrl[3].ctrl = CTRL_MOVETO ; // attack cover [wzc]
	
	/*m_tacctrl[3].target_x = m_trueball.x + 150;
	if (m_trueball.y <= 1200 && m_trueball.y > 700)
	{
		m_tacctrl[3].target_y = m_trueball.y -300 ;
	}
	else if (m_trueball.y<= 500 && m_trueball.y >=0 )
	{
		m_tacctrl[3].target_y = m_trueball.y +300 ;
	}
	else
	{
		m_tacctrl[3].target_x = m_trueball.x + 300 ;
		m_tacctrl[3].target_y = m_trueball.y ;
	}*/
	//kong();
	m_tacctrl[3].target_x = kongx;
	m_tacctrl[3].target_y = kongy;
	m_tacctrl[3].target_angle = m_Loca_CalAngle(m_tacctrl[3].target_x,m_tacctrl[3].target_y,m_trueball.x,m_trueball.y);	
	

	for (int i=4;i<6;i++)
	{
		m_tacctrl[i].ctrl = CTRL_MOVETO;
	}
	
	
	CPoint tmpball = m_trueball;
	CPoint tmp;
	
	tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)(80*m_fKField));
	m_tacctrl[4].target_x = tmp.x;
	m_tacctrl[4].target_y = tmp.y;
		
	m_tacctrl[5].target_x = m_Pos_Back.x;
	m_tacctrl[5].target_y = m_Pos_Back.y+200;
	if ((m_tacctrl[4].target_x-m_tacctrl[5].target_x)*(m_tacctrl[4].target_x-m_tacctrl[5].target_x)+(m_tacctrl[4].target_y-m_tacctrl[5].target_y)*(m_tacctrl[4].target_y-m_tacctrl[5].target_y)<275)
	{
		m_tacctrl[5].target_x = m_Pos_Back.x;
		m_tacctrl[5].target_y = m_Pos_Back.y+100;
	}

	
	

	//针对自己球员进入禁区做修正
	for (i=4;i<6;i++)
	{
		if (m_Check_MovetoOurPenaltyArea(m_tacctrl[i]) == true)
		{
			m_Fix_MovetoOurPenaltyArea(&m_tacctrl[i]);
		}
	}

	//针对自己球员出边界进行修正	
	for (i=4;i<6;i++)
	{
		if (m_Check_MoveOutOffFrame(m_tacctrl[i]) == true)
		{
			m_Fix_MoveOutOffFrame(&m_tacctrl[i]);
		}
	}
	
	for (i=4;i<6;i++)
	{
		//m_tacctrl[i].target_angle = m_Loca_CalAngle(m_tacctrl[i].target_x,m_tacctrl[i].target_y,m_trueball.x,m_trueball.y);
		m_tacctrl[i].target_angle = 0;
	}
	 m_TacAction(m_tacagent);
}
//////////////////////////////////////////////////////////////////////////
/*-----------------------------紧逼防守---------------------------------*/
void CUPFullField::Tac_PressDef(int inDefDist)
{
	//printf("Tac_PressDef");
	//初始化控制结构体，按角色关键程度排列
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;
	
	//判断一下是否需要紧逼
// 	if (m_bSomeoneHoldBall() == true)
// 	{
// 		//已经有人抢到球，切换到普通进攻模式
// 		DefaultActive(150);
// 		return;
// 	}

	//紧逼模式，两个抢球队员
	m_tacctrl[2].ctrl = CTRL_ATTACK;
	m_tacctrl[3].ctrl = CTRL_ATTACK;
	
	//其他队员跑位切换到围堵态势
	for (int i=3;i<6;i++)
	{
		m_tacctrl[i].ctrl = CTRL_MOVETO;
	}
	
	//先假设球位于中线上方，计算位置
	CPoint tmpball = m_trueball;
	/*if (m_trueball.y < 600)
	{
		tmpball.y = 1200 - tmpball.y;
	}*/
	
	CPoint tmp;
	
	//下面将场地分为上中下三个区域，根据球所处区域形成防守阵型
	//if (tmpball.y > 900)
	{
		//上侧
		//先在球到球门之间堵上一个
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)(80*m_fKField));
		m_tacctrl[3].target_x = tmp.x;
		m_tacctrl[3].target_y = tmp.y;
		m_tacctrl[3].target_angle = m_Loca_CalAngle(m_tacctrl[3].target_x,m_tacctrl[3].target_y,m_trueball.x,m_trueball.y);

		//后卫
		m_tacctrl[4].target_x = m_Pos_Back.x;
		m_tacctrl[4].target_y = m_Pos_Back.y+200;

		if ((m_tacctrl[3].target_x-m_tacctrl[4].target_x)*(m_tacctrl[3].target_x-m_tacctrl[4].target_x)+(m_tacctrl[3].target_y-m_tacctrl[4].target_y)*(m_tacctrl[3].target_y-m_tacctrl[4].target_y)<275)
		{
			m_tacctrl[4].target_x = m_Pos_Back.x;
			m_tacctrl[4].target_y = m_Pos_Back.y+100;
		}

		m_tacctrl[5].target_x = m_Pos_Back.x;
		m_tacctrl[5].target_y = m_Pos_Back.y-200;

		if ((m_tacctrl[3].target_x-m_tacctrl[5].target_x)*(m_tacctrl[3].target_x-m_tacctrl[5].target_x)+(m_tacctrl[3].target_y-m_tacctrl[5].target_y)*(m_tacctrl[3].target_y-m_tacctrl[5].target_y)<275)
		{
			m_tacctrl[5].target_x = m_Pos_Back.x;
			m_tacctrl[5].target_y = m_Pos_Back.y-100;
		}
		//test
// 		tmp = m_Loca_CalBackPos(tmpball.x,tmpball.y,m_Pos_Back.x);
// 		m_tacctrl[5].target_x = tmp.x;
// 		m_tacctrl[5].target_y = tmp.y;


		//两翼的长度
// 		CPoint longtmp = m_Loca_Ray(tmpball.x,tmpball.y,m_tacctrl[4].target_x,m_tacctrl[4].target_y,(inDefDist+100)*m_fKField);
// 		tmp = m_Loca_Rotate(longtmp.x,longtmp.y,tmpball.x,tmpball.y,-30);
// 		m_tacctrl[5].target_x = tmp.x;
//		m_tacctrl[5].target_y = tmp.y;
		
// 		tmp = m_Loca_Rotate(m_tacctrl[4].target_x,m_tacctrl[4].target_y,tmpball.x,tmpball.y,30);
// 		m_tacctrl[6].target_x = tmp.x;
// 		m_tacctrl[6].target_y = tmp.y;
	}
	

	//针对自己球员进入禁区做修正
	for (i=3;i<6;i++)
	{
		if (m_Check_MovetoOurPenaltyArea(m_tacctrl[i]) == true)
		{
			m_Fix_MovetoOurPenaltyArea(&m_tacctrl[i]);
		}
	}

	//针对自己球员出边界进行修正	
	for (i=3;i<6;i++)
	{
		if (m_Check_MoveOutOffFrame(m_tacctrl[i]) == true)
		{
			m_Fix_MoveOutOffFrame(&m_tacctrl[i]);
		}
	}
	
	//如果球在中线下方，则将坐标上下翻转
	/*if (m_trueball.y < 600)
	{
		m_tacctrl[2].target_y = 1200 - m_tacctrl[2].target_y;
		m_tacctrl[3].target_y = 1200 - m_tacctrl[3].target_y;		
		m_tacctrl[4].target_y = 1200 - m_tacctrl[4].target_y;
		m_tacctrl[5].target_y = 1200 - m_tacctrl[5].target_y;
	}*/
	
	//角度,都朝向球
	for (i=3;i<6;i++)
	{
		//m_tacctrl[i].target_angle = m_Loca_CalAngle(m_tacctrl[i].target_x,m_tacctrl[i].target_y,m_trueball.x,m_trueball.y);
		m_tacctrl[i].target_angle = 0;
	}
	
	m_TacAction(m_tacagent);
}

//////////////////////////////////////////////////////////////////////////
/*-----------------------------围堵防守---------------------------------*/
void CUPFullField::Tac_ContainDef(int inDefDist)
{
	//printf("Tac_ContainDef");
	//初始化控制结构体，按角色关键程度排列
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;

	//分配一个近身抢球的队员
	m_tacctrl[2].ctrl = CTRL_ATTACK;
	
	//其他队员跑位切换到围堵态势
	for (int i=3;i<6;i++)
	{
		m_tacctrl[i].ctrl = CTRL_MOVETO;
	}
	
	//先假设球位于中线上方，计算位置
	CPoint tmpball = m_trueball;
	CPoint tmpForeBall = m_Foreball;
	if (m_trueball.y < 600)
	{
		tmpball.y = 1200 - tmpball.y;
		tmpForeBall.y = 1200 - tmpForeBall.y;
	}
	
	CPoint tmp;
	tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)(300*m_fKField));	
	tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,-10); 
	m_tacctrl[3].target_x = tmp.x;
	m_tacctrl[3].target_y = tmp.y;
	//////////////////////////////////////////////////////////////////////////

	tmp = m_Loca_Ray(tmpForeBall.x,tmpForeBall.y,1900,600,(int)(300*m_fKField));
	tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpForeBall.x,tmpForeBall.y,180); 
	m_tacctrl[4].target_x = tmp.x;
	m_tacctrl[4].target_y = tmp.y +100;
	//////////////////////////////////////////////////////////////////////////

	//第五名球员在另一侧防御，防止对方大范围转移时移防不及时
	m_tacctrl[5].target_x = 300;
	m_tacctrl[5].target_y = 300;

//	CString strtmp;
	//针对自己球员进入禁区做修正
	for (i=3;i<6;i++)
	{
		if (m_Check_MovetoOurPenaltyArea(m_tacctrl[i]) == true)
		{
			m_Fix_MovetoOurPenaltyArea(&m_tacctrl[i]);
		}
	}

	//针对自己球员出边界进行修正	
	for (i=3;i<6;i++)
	{
		if (m_Check_MoveOutOffFrame(m_tacctrl[i]) == true)
		{
// 			strtmp.Format("%d号策略出界",i);
// 			printf(strtmp);
// 			m_tacctrl[i].target_y = 1200;
			m_Fix_MoveOutOffFrame(&m_tacctrl[i]);
		}
	}
	
	//如果球在中线下方，则将坐标上下翻转
	if (m_trueball.y < 600)
	{
		m_tacctrl[2].target_y = 1200 - m_tacctrl[2].target_y;
		m_tacctrl[3].target_y = 1200 - m_tacctrl[3].target_y;		
		m_tacctrl[4].target_y = 1200 - m_tacctrl[4].target_y;
		m_tacctrl[5].target_y = 1200 - m_tacctrl[5].target_y;
	}
	
	//角度,都朝向球
	for (i=4;i<6;i++)
	{
		//m_tacctrl[i].target_angle = m_Loca_CalAngle(m_tacctrl[i].target_x,m_tacctrl[i].target_y,m_trueball.x,m_trueball.y);
		m_tacctrl[i].target_angle = 0;
	}
	
	m_TacAction(m_tacagent);
}
bool CUPFullField::m_bSomeoneHoldBall_fromAG()
{
	for (int i=1;i<7;i++)
	{
		//剔除离线球员和守门员
		if (m_agents[i].online == true && m_agents[i].status != CTRL_GOALKEEP && m_agents[i].status != ROBST_ERR)
		{		
			if (m_agents[i].isAgHoldBall==true)
			{
				return true;
			}
		}
	}	
	return false;			
}
bool CUPFullField::m_bSomeoneHoldBall()
{
	for (int i=1;i<7;i++)
	{
		//剔除离线球员和守门员
		if (m_agents[i].online == true && m_agents[i].status != CTRL_GOALKEEP && m_agents[i].status != ROBST_ERR)
		{		
			if (m_agents[i].dist_ball < 100)
			{
				return true;
			}
		}
	}	
	return false;			
}

bool CUPFullField::m_bCatchHoldBall()
{
	for (int i=1;i<7;i++)
	{
		//剔除离线球员和守门员
		if (m_agents[i].online == true && m_agents[i].status == CTRL_MID_CATCH && m_agents[i].status != ROBST_ERR)
		{		
			if (m_agents[i].dist_ball < 100)
			{
				return true;
			}
		}
	}	
	return false;			
}

void CUPFullField::Tac_ReleaseKickoff()
{
	for (int i=1;i<7;i++)
	{
		//释放开球队员角色
		if (m_agents[i].status == CTRL_KICKOFF_SLAVE)
		{
			AChangeStatus(m_agents[i].ID,CTRL_STOP,0,0);
			m_agents[i].status = CTRL_STOP;
		}
	}
	m_curKOPrim = 0;
	m_curKOSlave = 0;
}

bool CUPFullField::m_Check_inOurPenaltyArea(uagent inagent)
{
	if (/*inagent.x >0 && */inagent.x <225 &&
		inagent.y > 275 && inagent.y < 925)
	{
		return true;
	} 
	else
	{
		return false;
	}
}

bool CUPFullField::m_Check_outOffFrame(uagent inagent,int inReachOut)
{
	if (inagent.x < -inReachOut || inagent.x >1800+inReachOut ||
		inagent.y < -inReachOut || inagent.y > 1200+inReachOut)
	{
		return true;
	} 
	else
	{
		return false;
	}
}

void CUPFullField::m_Fix_inOurPenaltyArea(uagent inagent)
{
	//将大禁区分成俩部分,判断处于那边
	if (inagent.y > 600)
	{
		//处于上半部分，看和哪个边比较靠近
		if (abs(255 - inagent.x) > abs(925 - inagent.y))
		{
			//比较靠近上边界，则将其排斥出去
			AMoveTo(inagent.ID,inagent.x,925,(int)inagent.angle);
		} 
		else
		{
			//比较靠近右边界，则将其排斥出去
			AMoveTo(inagent.ID,255,inagent.y,(int)inagent.angle);
		}
	} 
	else
	{
		//处于下半部分，看和哪个边比较靠近
		if (abs(255 - inagent.x) > abs(inagent.y - 275))
		{
			//比较靠近下边界，则将其排斥出去
			AMoveTo(inagent.ID,inagent.x,275,(int)inagent.angle);
		} 
		else
		{
			//比较靠近右边界，则将其排斥出去
			AMoveTo(inagent.ID,255,inagent.y,(int)inagent.angle);
		}
	}
}

void CUPFullField::m_Fix_outOffFrame(uagent inagent)
{
	if (inagent.x < -100)
	{
		AMoveTo(inagent.ID,-100,inagent.y,(int)inagent.angle);
	}

	if (inagent.x >1900)
	{
		AMoveTo(inagent.ID,1900,inagent.y,(int)inagent.angle);
	}
	
	if (inagent.y < -100)
	{
		AMoveTo(inagent.ID,inagent.x,-100,(int)inagent.angle);
	}

	if (inagent.y > 1300)
	{
		AMoveTo(inagent.ID,inagent.x,1300,(int)inagent.angle);
	}
}

/************************************************************************/
/* Move out                                                                     */
/************************************************************************/

bool CUPFullField::m_Check_MovetoOurPenaltyArea(MSG_Control inTac)
{
	if (inTac.ctrl != CTRL_MOVETO && inTac.ctrl != CTRL_PRODEF && inTac.ctrl != CTRL_BLOCK )
	{
		return false;
	}

	CString tStr;
	//////////////////////////////////////////////////////////////////////////
	//正常的
	if (/*inTac.target_x >0 && */inTac.target_x <225 &&
		inTac.target_y > 275 && inTac.target_y < 925) 
	//////////////////////////////////////////////////////////////////////////
	/*/稍微小一些
	if (inTac.target_x >0 && inTac.target_x <100 &&
	inTac.target_y > 200 && inTac.target_y < 850)
	/////////////////////////////////////////////////////////////////////////*/
	{
		return true;
	} 
	else
	{
		//判断是否会撞到球门或跑到球门后面去
		if (inTac.target_x <100 &&
		inTac.target_y > 400 && inTac.target_y < 800)
		{
			return true;
		}

		return false;
	}
}

bool CUPFullField::m_Check_MoveOutOffFrame(MSG_Control inTac)
{
	if (inTac.ctrl != CTRL_MOVETO  && inTac.ctrl != CTRL_PRODEF  && inTac.ctrl != CTRL_BLOCK )
	{
		return false;
	}

	if (inTac.target_x < -100 || inTac.target_x >1900 ||
		inTac.target_y < -100 || inTac.target_y > 1300)
	{
		return true;
	} 
	else
	{
		return false;
	}
}

#define PENTOP 950
#define PRNRIGHT 255
#define PENBTM 275
void CUPFullField::m_Fix_MovetoOurPenaltyArea(MSG_Control* inTac)
{	
	if (inTac->ctrl != CTRL_MOVETO && inTac->ctrl != CTRL_PRODEF && inTac->ctrl != CTRL_BLOCK )
	{
		return ;
	}

	//将大禁区分成俩部分,判断处于那边
	if (inTac->target_y > 600)
	{
		//处于上半部分，看和哪个边比较靠近
		if (abs(PRNRIGHT - inTac->target_x) > abs(PENTOP - inTac->target_y))
		{
			//比较靠近上边界，则将其排斥出去
			inTac->target_y = PENTOP;
		} 
		else
		{
			//比较靠近右边界，则将其排斥出去
			inTac->target_x = PRNRIGHT;
		}
	} 
	else
	{
		//处于下半部分，看和哪个边比较靠近
		if (abs(PRNRIGHT - inTac->target_x) > abs(inTac->target_y - PENBTM))
		{
			//比较靠近下边界，则将其排斥出去
			inTac->target_y = PENBTM;
		} 
		else
		{
			//比较靠近右边界，则将其排斥出去
			inTac->target_x = PRNRIGHT;
		}
	}
}

void CUPFullField::m_Fix_MoveOutOffFrame(MSG_Control* inTac)
{	
	if (inTac->ctrl != CTRL_MOVETO && inTac->ctrl != CTRL_PRODEF && inTac->ctrl != CTRL_BLOCK )
	{
		return ;
	}

// 	if (inTac->target_x < -100)
// 	{
// 		inTac->target_x = -100;
// 	}

	if (inTac->target_x < -50)
	{
		inTac->target_x = -50;
	}

	
	if (inTac->target_x >1850)
	{
		inTac->target_x = 1850;
	}
	
	if (inTac->target_y < -50)
	{
		inTac->target_y = -50;
	}
	
	if (inTac->target_y > 1250)
	{
		inTac->target_y = 1250;
	}
}

void CUPFullField::m_Ball_Forecast()
{
	if (m_bFoundball == false)
	{
		m_Foreball = m_trueball;
		return;
	}

	if (m_trueball == m_lastball)
	{
		m_Foreball = m_trueball;
		return;
	}

	m_Foreball = m_Loca_Ray(m_trueball.x,m_trueball.y,m_lastball.x,m_lastball.y,m_CalDist(m_trueball.x,m_trueball.y,m_lastball.x,m_lastball.y));
	m_Foreball = m_Loca_Rotate(m_Foreball.x,m_Foreball.y,m_trueball.x,m_trueball.y,180);

	//慎重
	m_trueball = m_Foreball;
}

void CUPFullField::Tac_DefTest(int inDefDist)
{
	//初始化控制结构体，按角色关键程度排列
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;

	//2~5号队员跑位切换到围堵态势
	for (int i=2;i<6;i++)
	{
		m_tacctrl[i].ctrl = CTRL_DEFENCE;
		m_tacctrl[i].def_dist = inDefDist;
	}

	m_tacctrl[2].def_angle = 0;
	m_tacctrl[3].def_angle = 45;
	m_tacctrl[4].def_angle = -45;
	m_tacctrl[5].def_angle = 90;


	/*/////////////////////////////////////////////////////////////////////////
	//2~5号队员跑位切换到围堵态势
	for (int i=2;i<6;i++)
	{
		m_tacctrl[i].ctrl = CTRL_MOVETO;
	}

	//先假设球位于中线上方，计算位置
	CPoint tmpball = m_trueball;
	if (m_trueball.y < 600)
	{
		tmpball.y = 1200 - tmpball.y;
	}
	
 	CPoint tmp;

	//下面将场地分为上中下三个区域，根据球所处区域形成防守阵型
	if (tmpball.y > 800)
	{
		//上侧
		//先在球到球门之间堵上一个
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)(inDefDist*m_fKField));
		m_tacctrl[2].target_x = tmp.x;
		m_tacctrl[2].target_y = tmp.y;

		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)((inDefDist+100)*m_fKField));
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,-30);
		m_tacctrl[3].target_x = tmp.x;
		m_tacctrl[3].target_y = tmp.y;

		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)((inDefDist+100)*m_fKField));
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,-45);
		m_tacctrl[4].target_x = tmp.x;
		m_tacctrl[4].target_y = tmp.y;

		//后卫
		m_tacctrl[5].target_x = m_Pos_Back.x;
 		m_tacctrl[5].target_y = m_Pos_Back.y;
	}

	if (tmpball.y < 800)
	{
		//中间区域
		//先在球到球门之间堵上一个
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)(inDefDist*m_fKField));
		m_tacctrl[2].target_x = tmp.x;
		m_tacctrl[2].target_y = tmp.y;
		//m_tacctrl[2].target_angle = m_Loca_CalAngle(m_tacctrl[2].target_x,m_tacctrl[2].target_y,tmpball.x,tmpball.y);

		//两翼的长度
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)((inDefDist+100)*m_fKField));
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,-45);
		m_tacctrl[3].target_x = tmp.x;
		m_tacctrl[3].target_y = tmp.y;

		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)((inDefDist+100)*m_fKField));
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,45);
		m_tacctrl[4].target_x = tmp.x;
		m_tacctrl[4].target_y = tmp.y;

		//后卫
		m_tacctrl[5].target_x = m_Pos_Back.x;
 		m_tacctrl[5].target_y = m_Pos_Back.y;

	}

	//针对自己球员进入禁区做修正
	for (i=2;i<6;i++)
	{
		if (m_Check_MovetoOurPenaltyArea(m_tacctrl[i]) == true)
		{
			m_Fix_MovetoOurPenaltyArea(&m_tacctrl[i]);
		}
	}

	//针对自己球员出边界进行修正	
	for (i=2;i<6;i++)
	{
		if (m_Check_MoveOutOffFrame(m_tacctrl[i]) == true)
		{
			m_Fix_MoveOutOffFrame(&m_tacctrl[i]);
		}
	}

	//如果球在中线下方，则将坐标上下翻转
	if (m_trueball.y < 600)
	{
		m_tacctrl[2].target_y = 1200 - m_tacctrl[2].target_y;
		m_tacctrl[3].target_y = 1200 - m_tacctrl[3].target_y;		
		m_tacctrl[4].target_y = 1200 - m_tacctrl[4].target_y;
		m_tacctrl[5].target_y = 1200 - m_tacctrl[5].target_y;
	}

	//角度,都朝向球
	for (i=3;i<6;i++)
	{
		//m_tacctrl[i].target_angle = 0;
		m_tacctrl[i].target_angle = m_Loca_CalAngle(m_tacctrl[i].target_x,m_tacctrl[i].target_y,m_trueball.x,m_trueball.y);
	}
	/////////////////////////////////////////////////////////////////////////*/

	m_TacAction(m_tacagent);
}

void CUPFullField::Field_Change()
{
	if (m_bField == true)
	{
		m_bField = false;
	} 
	else
	{
		m_bField = true;
	}
}

CPoint CUPFullField::m_Point_Img2Field(int inx, int iny)
{
	CPoint Fieldpoint;
	Fieldpoint.x = (inx-48)*18/4;
	Fieldpoint.y = (305-iny)*1200/266;

	if (m_bHFlip == true)
	{
		//横向翻转
		Fieldpoint.x = 1800 - Fieldpoint.x;
	}

	if (m_bVFlip == true)
	{
		//纵向翻转
		Fieldpoint.y = 1200 - Fieldpoint.y;
	}

	return Fieldpoint;
}

CPoint CUPFullField::m_Point_Field2Img(int inx, int iny)
{
	CPoint ImagePoint;
	ImagePoint.x = inx*400/1800 + 48;
	ImagePoint.y = 305 - iny*266/1200;

	if (m_bHFlip == true)
	{
		ImagePoint.x = 400 +48*2 - ImagePoint.x;
	}

	if (m_bVFlip == true)
	{
		ImagePoint.y = 266 + 39*2 - ImagePoint.y;
	}

	//限一下值
	if (ImagePoint.x > 490)
	{
		ImagePoint.x = 490;
	}
	if (ImagePoint.x <0)
	{
		ImagePoint.x =0;
	}
	if (ImagePoint.y < 0)
	{
		ImagePoint.y = 0;
	}
	if (ImagePoint.y > 340)
	{
		ImagePoint.y = 340;
	}
	return ImagePoint;
}

void CUPFullField::Display_HFlip()
{
	if (m_bHFlip == true)
	{
		m_bHFlip = false;
	} 
	else
	{
		m_bHFlip = true;
	}
	Invalidate();
}

void CUPFullField::Display_VFlip()
{
	if (m_bVFlip == true)
	{
		m_bVFlip = false;
	} 
	else
	{
		m_bVFlip = true;
	}
	Invalidate();
}

double CUPFullField::m_CalAngle(int inx1, int iny1, int inVx, int inVy, int inx2, int iny2)
{
	int a2 = (inx1 - inx2)*(inx1 - inx2) + (iny1 - iny2)*(iny1 - iny2);
	int b2 = (inx1 - inVx)*(inx1 - inVx) + (iny1 - inVy)*(iny1 - inVy);
	int c2 = (inx2 - inVx)*(inx2 - inVx) + (iny2 - inVy)*(iny2 - inVy);

	int b = (int)sqrt(static_cast<float>(b2));
	int c = (int)sqrt(static_cast<float>(c2));

	if (b == 0 || c == 0)
	{
		return 360;
	}

	double Angle = acos((double)(c2+b2-a2)/(2*b*c));

	if (Angle == 0)
	{
		if (a2 > c2)
		{
			return 180;
		} 
		else
		{
			return 0;
		}
	}

	//换算成角度值
	Angle = Angle*180/3.14159;

	return Angle;
}

void CUPFullField::m_Fix_MoveTargetDist(MSG_Control *inList, int inAvoidDist, int inFixDist)
{
	for (int i=1;i<6;i++)
	{
		if (inList[i].ctrl== CTRL_MOVETO)
		{
			//在后面的策略里寻找moveto的目标，如果离较近，则调整自己位置
			for (int j=i;j<6;j++)
			{
				if (inList[j].ctrl == CTRL_MOVETO)
				{
					if (m_CalDist(inList[i].target_x,inList[i].target_y,inList[j].target_x,inList[j].target_y) < inAvoidDist)
					{
						//距离太近，需要修正，避免相撞
					}
				}
			}
		}
	}
}

void CUPFullField::m_Fix_KeepDistance(uagent* inList, int inAvoidDist, int inFixDist)
{
	for (int i=2;i<6;i++)
	{	
		//在后面的策略里寻找moveto的目标，如果离较近，则调整自己位置
		for (int j=i;j<6;j++)
		{
			//只对moveto进行修正
			if (inList[j].status == CTRL_MOVETO)
			{
				if (m_CalDist(inList[i].x,inList[i].y,inList[j].x,inList[j].y) < inAvoidDist)
				{
					//距离太近，需要修正，避免相撞。inList[i]不动，仅对inList[j]修正
					CPoint fixp = m_Loca_Ray(inList[i].x,inList[i].y,inList[j].x,inList[j].y,(int)(200*m_fKField));
					AMoveTo(j,fixp.x,fixp.y,(int)inList[i].angle);
				}
			}
		}
	}
}

CPoint CUPFullField::m_Loca_CalBackPos(int inballx, int inbally, int inBackx)
{
	CPoint res;
	res = m_Pos_Back;
	if (inballx > 100)
	{
		res.y = inbally*(inballx - res.x)/inballx;
	}
	return res;
}

void CUPFullField::m_Fix_CollisionAvoid(MSG_Control *inTac, int inAvoidDist)
{	
	for (int i=2;i<6;i++)
	{	
		//在后面的策略里寻找moveto的目标，如果离较近，则调整自己位置
		for (int j=i;j<6;j++)
		{
			//只对moveto进行修正
			if (inTac[j].ctrl == CTRL_MOVETO)
			{
				if (m_CalDist(inTac[i].target_x,inTac[i].target_y,inTac[j].target_x,inTac[j].target_y) < inAvoidDist)
				{
					//距离太近，需要修正，避免相撞。inList[i]不动，仅对inList[j]修正
					CPoint fixp = m_Loca_Ray(inTac[i].target_x,inTac[i].target_y,inTac[j].target_x,inTac[j].target_y,(int)(100*m_fKField));
					inTac[j].target_x = fixp.x;
					inTac[j].target_y = fixp.y;
				//	printf("Fix_CollisionAvoid");
				}
			}
		}
	}
}

int CUPFullField::m_Ag_GetStatus(UINT inSt, uagent *inalist, bool inAllocated)
{
	int ret = 0;
	for (int i=1;i<7;i++)
	{
		if (inalist[i].online == true && inalist[i].allocated == inAllocated)
		{
			if (inalist[i].status == inSt)
			{
				ret = i;
				break;
			}
		}
	}
	return ret;
}

void CUPFullField::m_CheckAgentStatus()
{
	if (m_nAtkNotMove > 0)
	{
		m_nAtkNotMove --;
	}
	
	if (m_nAtkNotMove == 0)
	{
		for (int i=0;i<7;i++)
		{
			if (m_tacagent[i].status == CTRL_ATTACK && m_tacagent[i].online == true)	//查找进攻队员
			{
				if (m_tacagent[i].ID == m_bakAtkAgent.ID)				//查看是否前一时刻他也是进攻
				{
// 					if (m_tacagent[i].dist_ball != 0)	//是否带球
// 					{
						if (m_CalDist(m_tacagent[i].x,m_tacagent[i].y,m_bakAtkAgent.x,m_bakAtkAgent.y) < 30)
						{
							//进攻队员未带球，且没明显移动，开始倒计时
							//m_nAtkDly = 30;
							m_nNumOfAtk = 2;
						}
						else
						{
							m_nNumOfAtk = 1;
						}
				/*	}*/
				}
				//对进攻队员进行一下备份
				m_bakAtkAgent = m_tacagent[i];
				break;
			}
		}
		m_nAtkNotMove = 20;
	}

	if (m_nAtkDly > 0)
	{
		m_nAtkDly --;
		if (m_nAtkDly == 0)
		{	
			for (int i=0;i<7;i++)
			{
				if (m_tacagent[i].status == CTRL_ATTACK && m_tacagent[i].online == true)
				{
					//对进攻队员进行一下备份
					m_bakAtkAgent = m_tacagent[i];
				}
			}
		}
	}
}

int CUPFullField::m_Ag_GetMoveTo(int x, int y, uagent *inalist)
{
	int ret = 0;
	
	bool scanerr = true;	//标记是否还有必要对err的ag进行扫描
	//先扫描健康的ag
	for (int i=2;i<7;i++)
	{
		if (inalist[i].online == true && inalist[i].allocated == false && inalist[i].status != ROBST_ERR)
		{
			//健康的ag里有人，不需要对err的ag进行扫描了
			scanerr = false;
			
			//判断该健康的ag
			if (inalist[i].moveto_x==x && inalist[i].moveto_y==y)
			{
				ret = i;
			}
		}
	}
	
	//如果健康ag已经无人选，则从剩余的ag里挑选
	if (scanerr == true)
	{
		for (int i=1;i<7;i++)
		{
			if (inalist[i].online == true && inalist[i].allocated == false)
			{
				//判断该err的ag
				if (inalist[i].moveto_x == x && inalist[i].moveto_y == y)
				{
					ret = i;
				}
			}
		}
	}
	
	return ret;
}

void CUPFullField::m_ResetAtkDly(int inID)
{
	//m_nSWAtkDly[inID] = ATK_SW_DLY;
	m_nSWAtkDly[inID] = 0;
}

void CUPFullField::Tac_3Atk(int inDefDist)
{
	//初始化控制结构体，按角色关键程度排列
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;
	
	//判断一下是否需要紧逼
// 	if (m_bSomeoneHoldBall() == true)
// 	{
// 		//已经有人抢到球，切换到普通进攻模式
// 		DefaultActive(150);
// 		return;
// 	}

	//紧逼模式，两个抢球队员
	m_tacctrl[2].ctrl = CTRL_ATTACK;
	m_tacctrl[3].ctrl = CTRL_ATTACK;
	m_tacctrl[4].ctrl = CTRL_ATTACK;
	
	//其他队员跑位切换到围堵态势
	for (int i=3;i<6;i++)
	{
		m_tacctrl[i].ctrl = CTRL_MOVETO;
	}
	
	//先假设球位于中线上方，计算位置
	CPoint tmpball = m_trueball;
	if (m_trueball.y < 600)
	{
		tmpball.y = 1200 - tmpball.y;
	}
	
	CPoint tmp;
	
	//下面将场地分为上中下三个区域，根据球所处区域形成防守阵型
	if (tmpball.y > 900)
	{
		//上侧
		//先在球到球门之间堵上一个

		//后卫
		m_tacctrl[5].target_x = m_Pos_Back.x;
		m_tacctrl[5].target_y = m_Pos_Back.y+300;
	}
	else
	{
		//tmpball.y < 900 中间区域
		//进攻角色
		m_tacctrl[2].ctrl = CTRL_ATTACK;
		m_tacctrl[3].ctrl = CTRL_ATTACK;
		m_tacctrl[4].ctrl = CTRL_ATTACK;
		
		
		//后卫
		m_tacctrl[5].target_x = m_Pos_Back.x;
		m_tacctrl[5].target_y = m_Pos_Back.y;

		
	}

	//针对自己球员进入禁区做修正
	for (i=3;i<6;i++)
	{
		if (m_Check_MovetoOurPenaltyArea(m_tacctrl[i]) == true)
		{
			m_Fix_MovetoOurPenaltyArea(&m_tacctrl[i]);
		}
	}

	//针对自己球员出边界进行修正	
	for (i=3;i<6;i++)
	{
		if (m_Check_MoveOutOffFrame(m_tacctrl[i]) == true)
		{
			m_Fix_MoveOutOffFrame(&m_tacctrl[i]);
		}
	}
	
	//如果球在中线下方，则将坐标上下翻转
	if (m_trueball.y < 600)
	{
		
		m_tacctrl[5].target_y = 1200 - m_tacctrl[5].target_y;
	}
	
	//角度,都朝向球
	for (i=4;i<6;i++)
	{
		//m_tacctrl[i].target_angle = m_Loca_CalAngle(m_tacctrl[i].target_x,m_tacctrl[i].target_y,m_trueball.x,m_trueball.y);
		m_tacctrl[i].target_angle = 0;
	}
	
	m_TacAction(m_tacagent);
}

/************************围捕防守***********************/
void CUPFullField::Tac_RoundedDef(int inDefDist)
{
	//初始化控制结构体，按角色关键程度排列
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;
	
	//进攻角色
	m_tacctrl[2].ctrl = CTRL_ATTACK;
	
	//3~5号队员跑位切换到围堵态势
	for (int i=3;i<6;i++)
	{
		m_tacctrl[i].ctrl = CTRL_MOVETO;
	}

	//先假设球位于中线上方，计算位置
	CPoint tmpball = m_trueball;
	if (m_trueball.y < 600)
	{
		tmpball.y = 1200 - tmpball.y;
	}
	
 	CPoint tmp;

	//下面将场地分为上中下三个区域，根据球所处区域形成防守阵型
	if (tmpball.y > 1000)
	{
		//上侧
		//先在球到球门之间堵上一个
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)(inDefDist*m_fKField));
		m_tacctrl[3].target_x = tmp.x;
		m_tacctrl[3].target_y = tmp.y;

		//防守半径比堵球门的大一些，避免换位的时候相撞
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)((inDefDist+50)*m_fKField));
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,-45);
		m_tacctrl[4].target_x = tmp.x;
		m_tacctrl[4].target_y = tmp.y;

		//后卫
		m_tacctrl[5].target_x = m_Pos_Back.x;
 		m_tacctrl[5].target_y = m_Pos_Back.y+300;
		
		//test
// 		tmp = m_Loca_CalBackPos(tmpball.x,tmpball.y,m_Pos_Back.x);
// 		m_tacctrl[4].target_x = tmp.x;
// 		m_tacctrl[4].target_y = tmp.y;
	}
	else
	{
		// tmpball.y < 1000 中间区域
		//先在球到球门之间堵上一个
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)(inDefDist*m_fKField));
		m_tacctrl[3].target_x = tmp.x;
		m_tacctrl[3].target_y = tmp.y;
	
		
		//两翼的长度
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)((inDefDist+50)*m_fKField));
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,45);
		m_tacctrl[4].target_x = tmp.x;
		m_tacctrl[4].target_y = tmp.y;

		//后卫
		m_tacctrl[5].target_x = m_Pos_Back.x;
		m_tacctrl[5].target_y = m_Pos_Back.y;
		
		
		//test
// 		tmp = m_Loca_CalBackPos(tmpball.x,tmpball.y,m_Pos_Back.x);
// 		m_tacctrl[4].target_x = tmp.x;
// 		m_tacctrl[4].target_y = tmp.y;

	}

	//printf("针对自己球员进入禁区做修正");
	//CString tStr;
	//针对自己球员进入禁区做修正
	for (i=3;i<6;i++)
	{
		if (m_Check_MovetoOurPenaltyArea(m_tacctrl[i]) == true)
		{
			//tStr.Format("%d号角色进入禁区，需要修正",i);
			//printf(tStr);
			m_Fix_MovetoOurPenaltyArea(&m_tacctrl[i]);
			//m_tacctrl[i].target_x = 900;
		}
	}

	//针对自己球员出边界进行修正	
	for (i=3;i<6;i++)
	{
		if (m_Check_MoveOutOffFrame(m_tacctrl[i]) == true)
		{
			m_Fix_MoveOutOffFrame(&m_tacctrl[i]);
		}
	}

	//防止撞击
	m_Fix_CollisionAvoid(m_tacctrl, 50);

	//如果球在中线下方，则将坐标上下翻转
	if (m_trueball.y < 600)
	{
		m_tacctrl[2].target_y = 1200 - m_tacctrl[2].target_y;
		m_tacctrl[3].target_y = 1200 - m_tacctrl[3].target_y;		
		m_tacctrl[4].target_y = 1200 - m_tacctrl[4].target_y;
		m_tacctrl[5].target_y = 1200 - m_tacctrl[5].target_y;
	}

	//角度,都朝向球
	for (i=3;i<6;i++)
	{
		m_tacctrl[i].target_angle = 0;		//角度不变化，平移的时候稳定一些
		//m_tacctrl[i].target_angle = m_Loca_CalAngle(m_tacctrl[i].target_x,m_tacctrl[i].target_y,m_trueball.x,m_trueball.y);
	}

	m_TacAction(m_tacagent);
}

void CUPFullField::AClearObst(int inID)
{
	if (inID >= AG_NUM)
	{
		return;
	}
	for (int i=0;i<EACH_AG_OBST_NUM;i++)
	{
		m_tacObst[inID*EACH_AG_OBST_NUM+i].valid = false;
	}
}

void CUPFullField::m_ObstFusion()
{

	//////////////////////////////////////////////////////////////////////////
 	//将筛选后的障碍物复制到临时数组里,供后面挑选用
	memcpy(m_tacObst,m_cObst,(EACH_AG_OBST_NUM*(AG_NUM+1))*sizeof(obstacle));

	//将这一时间段内未返回障碍物信息的队员的障碍物信息清除，避免过期信息干扰决策
	for(int i=1;i<AG_NUM;i++)
	{
		if (m_nObstMsgCnt[i] == 0 )
		{
			AClearObst(i);
		}
		m_nObstMsgCnt[i] = 0;
	}

	//将处于场外的障碍物去除
	m_Obst_RemoveOutOfField(m_tacObst);

	//计算每个障碍物到它的提供者的距离
	for (i=0;i<EACH_AG_OBST_NUM*(AG_NUM+1);i++)
	{
		if (m_tacObst[i].valid == true)
		{
			m_tacObst[i].DistToOwner = m_CalDist(m_tacObst[i].x,m_tacObst[i].y,m_agents[m_tacObst[i].ID].x,m_agents[m_tacObst[i].ID].y);
			//////////////////////////////////////////////////////////////////////////
			//将距离观察者很远的障碍物剔除
			if (m_tacObst[i].DistToOwner > 500)
			{
				m_tacObst[i].valid = false;
			}
			//////////////////////////////////////////////////////////////////////////
		}
	}

	//相距较近的队员，可以先剔除其本身
	int nNumOfObst = EACH_AG_OBST_NUM*(AG_NUM+1);
	int j,DistToNearAg;
	for (i=0;i<nNumOfObst;i++)
	{
		if (m_tacObst[i].valid == true)
		{
			//检查跟非观察者是否太近，如果太近，可以认为是队友，移除
			for (j=0;j<AG_NUM;j++)
			{
				if (m_agents[j].online == true && m_tacObst[i].ID != j)
				{
					//j为距离障碍物较近的agent id（非所有者）
					DistToNearAg = m_CalDist(m_tacObst[i].x,m_tacObst[i].y,m_agents[j].x,m_agents[j].y);
					if (DistToNearAg < 100 )
					{
						//近身，去除
						m_tacObst[i].valid = false;	
						break;
					}

					//查看是否应该被当作远距离定位不准的障碍物剔除
					if (DistToNearAg < OBST_DIST_RANGE)
					{
						if (m_tacObst[i].DistToOwner > 500)
						{
							m_tacObst[i].valid = false;	
							break;
						}
					}

				}
			}
		}
	}

	
	/************************************************************************/
	/*                    大团障碍物的聚合									*/
	/************************************************************************/
	//obstacle tmpObst;
	for (i=0;i<nNumOfObst;i++)
	{
		if (m_tacObst[i].valid == true)
		{
			//为障碍物i寻找距离接近者
			for (j=0;j<nNumOfObst;j++)
			{
				if (m_tacObst[j].valid == true)
				{
					if (i == j)
					{
						continue;
					}
					if (m_CalDist(m_tacObst[i].x,m_tacObst[i].y,m_tacObst[j].x,m_tacObst[j].y) < OBST_CLOSE_RANGE)
					{
						//距离很近，判断谁的距离更近一些
						if (m_tacObst[i].DistToOwner < m_tacObst[j].DistToOwner)
						{
							//i障碍物更可信
							m_tacObst[j].valid = false;
						} 
						else
						{
							//j障碍物更可信,i被剔除，没有继续循环j的必要了
							m_tacObst[i].valid = false;
							break;
						}
					}
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	/*/深度图滤波
	m_obstFilter.ClearMap();
	for (int i=1;i<AG_NUM;i++)
	{
		m_BufObst[i].GetData(m_tmpBufObst,OBSTBUF_LEN);
		for (int j=0;j<OBSTBUF_LEN;j++)
		{
			if(m_tmpBufObst[j].size > 0)
			{	
				m_obstFilter.AddMark(
					m_tmpBufObst[j].x/10,
					m_tmpBufObst[j].y/10,
					m_tmpBufObst[j].size
				);
			}
		}
	}
	m_obstFilter.doFilter();
	m_obstFilter.Display();
	/////////////////////////////////////////////////////////////////////////*/
}


void CUPFullField::SetObstacle(int inID,obstacle *inpObstAr)
{
	while (inID >= AG_NUM)
	{
		inID--;
	}
	int tmpDist;

	m_nObstMsgCnt[inID] ++;

	
   
	memcpy((char*)&(m_cObst[inID*EACH_AG_OBST_NUM]),(char*)inpObstAr,EACH_AG_OBST_NUM*sizeof(obstacle));

	//缓冲2
	MSL_Position tmpPos;
	for (int i=0;i<EACH_AG_OBST_NUM;i++)
	{
		if (inpObstAr[i].valid == true && m_agents[inID].online == true)
		{
			tmpDist = m_CalDist(m_agents[inID].x,m_agents[inID].y,inpObstAr[i].x,inpObstAr[i].y);
			tmpPos.x = inpObstAr[i].x;
			tmpPos.y = inpObstAr[i].y;
			tmpPos.size = tmpDist/30;
			
			if (tmpDist < 50)
			{
				if (inpObstAr[i].size < 20)
				{
					tmpPos.size = inpObstAr[i].size;
				} 
				else
				{
					tmpPos.size = 20;
				}
			}

		   
			m_BufObst[inID].AddPos(&tmpPos);
		} 
		else
		{
			tmpPos.x = inpObstAr[i].x;
			tmpPos.y = inpObstAr[i].y;
			tmpPos.size = 0;
			
			
			m_BufObst[inID].AddPos(&tmpPos);
		}
	}

	


}


void CUPFullField::JudgeObstacle(int inID,obstacle *inpObstAr){
	int i,bestpos=-1,j;
	
	//先重设为不可信
	m_follow[inID].believe = false;
	
	if(m_agents[inID].online == true){
		for ( i=0;i<EACH_AG_OBST_NUM;i++)
	{
		if(inpObstAr[i].valid == true&&(inpObstAr[i].x>0&&inpObstAr[i].x<900)&&(inpObstAr[i].y>0&&inpObstAr[i].y<1200)){
			m_follow[inID].believe = true;
			
			//去掉我方球员
			for( j=0;j<7;j++){
				if(m_agents[j].online ==true&&inID!=i){
					//障碍物里队友的坐标与数组中的会有一定的差距！（大概在70—85）
				if(abs(m_agents[j].x-inpObstAr[i].x)<100&&abs(m_agents[j].y-inpObstAr[i].y<100)){
		           m_follow[inID].believe = false;
				   break;
                 }
				
				}
				
			   
			}
			//
			
			//
			if(m_follow[inID].believe = true&&(i==0||inpObstAr[i].size>inpObstAr[bestpos].size))
				bestpos = i;
			//

		}
		// 输出障碍物
		
			
			
			 
			
	}
		
		//////
		if(bestpos!=-1){
		
				m_follow[inID].x = inpObstAr[bestpos].x;
				m_follow[inID].y = inpObstAr[bestpos].y;
			
			    m_follow[inID].size = inpObstAr[bestpos].size;
				
		}
		

}
	//输出到文件

}

//
void CUPFullField::m_TeammateShare()
{	
	//将队员信息从m_tacagent数值复制到m_tm数值
	for (int i=0;i<AG_NUM;i++)
	{
		if (m_tacagent[i].online == true )
		{
			m_tm[i].valid = true;
			m_tm[i].ID = m_tacagent[i].ID;
			m_tm[i].x = m_tacagent[i].x;
			m_tm[i].y = m_tacagent[i].y;
			m_tm[i].V = m_tacagent[i].V;
			m_tm[i].V_angle = m_tacagent[i].V_Angle;
		}
		else
		{
			m_tm[i].valid = false;
		}
	}
	//给在线的队员共享其他队友的信息
	for (i=0;i<AG_NUM;i++)
	{
		if (m_tacagent[i].online == true && pToAgent[i] != NULL)
		{
			pToAgent[i]->Send_InfoOfTM(m_tm,AG_NUM);
		}
	}
}

void CUPFullField::Test_PassCatch()
{
	m_strCurSt.Format("传接球测试");
	
	m_eStage = sTestPassCatch;
	m_bTaskPassCatch = true;	
	
	bool bsomeoneonline = false;
	//复制球员信息
	for (int i=0;i<7;i++)
	{
		m_tacagent[i] = m_agents[i];
		if (m_tacagent[i].online == true)
		{
			bsomeoneonline = true;
		}
	}
	
	//没有球员在线，没必要进行决策
	if (bsomeoneonline == false)
	{
		return;
	}

	//如果接球队员拿到球，则终结传接球任务
	if (m_agents[m_nCatchPlayer].status == CTRL_ATTACK)
	{
		m_taskPassCatchEnd();
		m_eStage = sMani;
		return;
	}
	
	if (m_bFoundball == true && m_bTaskPassCatch == true && m_nCatchPlayer > 0)
	{
		if (m_bTaskPassCatch_kick == false)
		{
			//角色分配
			m_tacctrl[1].ctrl = CTRL_GOALKEEP;
			m_tacctrl[2].ctrl = CTRL_PASS;
			
			//3~5号队员停止
			for (i=3;i<AG_NUM;i++)
			{
				m_tacctrl[i].ctrl = CTRL_STOP;
			}
		} 
		else
		{
			//角色分配
			m_tacctrl[1].ctrl = CTRL_GOALKEEP;	
			m_tacctrl[2].ctrl = CTRL_CATCH;
			
			//3~5号队员停止
			for (i=3;i<AG_NUM;i++)
			{
				m_tacctrl[i].ctrl = CTRL_STOP;
			}
		}
		
		
	}
	else
	{
		//找不到球情况的决策
		for (i=0;i<AG_NUM;i++)
		{
			m_tacctrl[i].ctrl = CTRL_STOP;
		}
	}
	m_TacAction(m_tacagent);
}

void CUPFullField::APass(int inID, int inTargX, int inTargY)
{
	if (inID<0 || inID>6)
	{
		return;
	}

// 	if (m_agents[inID].status == CTRL_PASS)
// 	{
// 		m_tacagent[inID].allocated = true;
// 		return;
// 	}
	
	if (pToAgent[inID] != NULL)
	{
		pToAgent[inID]->PassCmd(inTargX,inTargY);
		//标记该agent已经分配角色
		m_tacagent[inID].allocated = true;
	}

	//Display
	m_ShowTacInfo(CTRL_PASS,inID);
}
void CUPFullField::APass_Normal(int inID, int inTargX, int inTargY)
{
	if (inID<0 || inID>6)
	{
		return;
	}

// 	if (m_agents[inID].status == CTRL_PASS)
// 	{
// 		m_tacagent[inID].allocated = true;
// 		return;
// 	}
	
	if (pToAgent[inID] != NULL)
	{
		pToAgent[inID]->PassCmd_Normal(inTargX,inTargY);
		//标记该agent已经分配角色
		m_tacagent[inID].allocated = true;
	}

	//Display
	m_ShowTacInfo(CTRL_3M_PASS,inID);
}
void CUPFullField::ACatch(int inID, int inFromX, int inFromY)
{
	if (inID<0 || inID>6)
	{
		return;
	}
	
	if (pToAgent[inID] != NULL)
	{
		pToAgent[inID]->CatchCmd(inFromX,inFromY);
		//标记该agent已经分配角色
		m_tacagent[inID].allocated = true;
	}
	
	//Display
	m_ShowTacInfo(CTRL_CATCH,inID);
}
void CUPFullField::ACatch_Normal(int inID, int inFromX, int inFromY)
{
	if (inID<0 || inID>6)
	{
		return;
	}
	
	if (pToAgent[inID] != NULL)
	{
		pToAgent[inID]->CatchCmd_Normal(inFromX,inFromY);
		//标记该agent已经分配角色
		m_tacagent[inID].allocated = true;
	}
	
	//Display
	m_ShowTacInfo(CTRL_3M_CATCH,inID);
}

void CUPFullField::m_taskPassCatchEnd()
{
	m_bTaskPassCatch = false;
	m_bTaskPassCatch_kick = false;
	m_nCatchPlayer = 0;
	if (m_agents[m_nPassPlayer].x < m_balls[m_nPassPlayer].x)
	{	
		//向对方半场传球
		m_nPassPlayer = 0;
		m_nPassEndDelay = 0;
	}
	else
	{
		//向己方半场传球，传球队员需要停一下
		m_nPassEndDelay = PASS_END_DELAY;
	}
}

void CUPFullField::Attention_SomeOneKick(int inID, int inSelfX, int inSelfY, int inTargetX, int inTargetY)
{
	CString info;
	if (m_bTaskPassCatch == true)
	{
		//正在进行传球任务,立刻选择接球队员
		if (m_nCatchPlayer == 0)
		{
			//没有选定接球队员的话，查找最合适的接球队员
			m_nCatchPlayer = m_Ag_GetNearBy(inTargetX,inTargetY,m_tacagent);

			if (m_nCatchPlayer == m_nPassPlayer)
			{
				//防止自己传球给自己
				printf("1\r\n1\r\n1\r\n1\r\n");
				Active();
				return;
			}
		}
		m_nLogTacIndex --;
		m_bTaskPassCatch_kick = true;
		ACatch(m_nCatchPlayer,inSelfX,inSelfY);
		m_posPass.x = inSelfX;
		m_posPass.y = inSelfY;
	}
	if (m_bTaskMidPassCatch ==true)
	{
		m_bTaskMidPassCatch_kick = true;
		m_posFinalMidPass.x = inTargetX;
		//m_posPass.y = inTargetY; bug!!!![mc]
		m_posFinalMidPass.y = inTargetY;
	}
	if (m_bTask3MeterPass==true)
	{
		m_bClear3meterPassInfo();
		m_bTask3MeterPass_Kick=true;
		m_bTask3MeterPass=false;
		Attack_Convert_Flag=1;
		info.Format("射门方向：(%d,%d)!!!!!!!\n",inTargetX,inTargetY);
	    printf(info);
	}
	
}

void CUPFullField::PR_Out(int inID)
{
	if (inID<0 || inID > AG_NUM)
	{
		return;
	}
	m_prRightOfPlayer[inID] = PLAYER_RIGHT_OUT; 
	m_nTimeToBeIn = PLAYER_TIME_TO_BEIN;
	SetTimer(PLAYOUT_COUNT,1000,NULL);
}

void CUPFullField::PR_Ready(int inID)
{
	if (inID<0 || inID > AG_NUM)
	{
		return;
	}
	m_prRightOfPlayer[inID] = PLAYER_RIGHT_READY; 

	if (m_eStage == sStop)
	{
		PR_AllReadyCanBeIn();
	}
}

void CUPFullField::PR_AllReadyCanBeIn()
{
	for (int i=0;i<AG_NUM;i++)
	{
		if (m_prRightOfPlayer[i] == PLAYER_RIGHT_READY)
		{
			m_prRightOfPlayer[i] = PLAYER_RIGHT_WAITFOEPOINT;
		}
	}
}

int CUPFullField::m_Obst_NearBy(int inx, int iny, obstacle *inObstAr)
{
	int ret = -1;
	int mindist = 0x7fffffff;
	int tmp_dist = 0;
    int nNumObst = EACH_AG_OBST_NUM*(AG_NUM+1);
	//搜索
	for (int i=0;i<nNumObst;i++)
	{
		if (inObstAr[i].valid == true)
		{
			//计算距离
			tmp_dist = m_CalDist(inObstAr[i].x,inObstAr[i].y,inx,iny);
			if (tmp_dist < mindist)
			{
				ret = i;
				mindist = tmp_dist;
			}
		}
	}
	return ret;
}

void CUPFullField::Default_Anti_Ready_Add(int inDist)
{
	//2010年，为防御开球添加防守对方队员的修正	
	memcpy(m_tmpObst,m_tacObst,(EACH_AG_OBST_NUM*(AG_NUM+1))*sizeof(obstacle));
	//角色3 - （原来是侧面，现在防守可能接球的对方队员）
	int nearobst = -1;	
	int nodead = 0;
	while (1)
	{
		nodead ++;
		if (nodead > 100)
		{
			break;
		}

		nearobst = m_Obst_NearBy(m_trueball.x,m_trueball.y,m_tmpObst);
		if (nearobst != -1)
		{
			//获取到一个障碍物，查看它与球的距离
			if (m_CalDist(m_tmpObst[nearobst].x,m_tmpObst[nearobst].y,m_trueball.x,m_trueball.y) < 150)
			{
				//它与球距离小于2米，应该是准备传球的队员，忽略
				m_tmpObst[nearobst].valid = false;
				nearobst = -1;		//让循环得以继续
			}
			else
			{
				if (m_CalDist(m_tmpObst[nearobst].x,m_tmpObst[nearobst].y,1800,600) < 200)
				{
					//距离对方球门很近，有可能是对方守门员，不符合要求
					m_tmpObst[nearobst].valid = false;
					nearobst = -1;		//让循环得以继续
					continue;
				}
				//符合要求
				break;
			}
		}
		else
		{
			//结果返回-1，说明没有障碍物
			break;
		}
	}
	
	CPoint tmpPnt;
	if (nearobst != -1)
	{
		//////////////////////////////////////////////////////////////////////////
		//方法一
		//从障碍物向球延伸1米
		tmpPnt = m_Loca_Ray(m_tmpObst[nearobst].x,m_tmpObst[nearobst].y,m_trueball.x,m_trueball.y,100);

		//绕障碍物旋转，直到距离球最大位置
		for (int i=0;i<=180/5;i++)
		{
			tmpPnt = m_Loca_Rotate(tmpPnt.x,tmpPnt.y,m_tmpObst[nearobst].x,m_tmpObst[nearobst].y,-5);

			if (m_CalDist(tmpPnt.x,tmpPnt.y,m_trueball.x,m_trueball.y) > inDist)
			{
				break;
			}
		}

		int nodead = 0;
		//如果距离球太近，则向己方球门延伸，直到站位与球距离大于规则要求的防守距离
		while(m_CalDist(tmpPnt.x,tmpPnt.y,m_trueball.x,m_trueball.y) < inDist)
		{
			nodead ++;
			if (nodead > 100)
			{
				break;
			}
			tmpPnt = m_Loca_Ray(tmpPnt.x,tmpPnt.y,0,600,50);
		}
		m_tacctrl[3].ctrl = CTRL_MOVETO;
		m_tacctrl[3].target_x = tmpPnt.x;
		m_tacctrl[3].target_y = tmpPnt.y;

		//该障碍物已经被防守，关闭其使能
		m_tmpObst[nearobst].valid = false;
	}
	
	nodead = 0;
	//角色4 - （原来是后防，现在为防守离我方球门最近的一个对方球员）
	while (1)
	{
		nodead ++;
		if (nodead > 100)
		{
			break;
		}
		nearobst = m_Obst_NearBy(0,600,m_tmpObst);
		if (nearobst != -1)
		{	
			if (m_CalDist(m_tmpObst[nearobst].x,m_tmpObst[nearobst].y,1800,600) < 200)
			{
				//距离对方球门很近，有可能是对方守门员，不符合要求
				m_tmpObst[nearobst].valid = false;
				nearobst = -1;		//让循环得以继续
				continue;
			}
			//符合要求
			break;
		}
		else
		{
			//结果返回-1，说明没有障碍物
			break;
		}
	}

	if (nearobst != -1)
	{
		//从该障碍物向我方球门延伸一点
		tmpPnt = m_Loca_Ray(m_tmpObst[nearobst].x,m_tmpObst[nearobst].y,0,600,80);	
		m_tacctrl[4].ctrl = CTRL_MOVETO;
		m_tacctrl[4].target_x = tmpPnt.x;
		m_tacctrl[4].target_y = tmpPnt.y;
		
		//该障碍物已经被防守，关闭其使能
		m_tmpObst[nearobst].valid = false;
	}
}

//m_trueball.x < 1000时会调用
void CUPFullField::Tac_RoundedDef2(int inDefDist)
{
	//初始化控制结构体，按角色关键程度排列
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;
	
	//进攻角色
	m_tacctrl[2].ctrl = CTRL_ATTACK;
	
	//3~5号队员跑位切换到定角度防守态势
	for (int i=3;i<6;i++)
	{
		m_tacctrl[i].ctrl = CTRL_DEFENCE;
		m_tacctrl[i].def_dist = (int)(inDefDist*m_fKField);
	}

	CPoint tmpball = m_trueball;	
 	CPoint tmp;

	//下面将场地分为上中下三个区域，根据球所处区域形成防守阵型
	if (tmpball.y > 900)
	{
		//上侧
		//先在球到球门之间堵上一个
		m_tacctrl[3].def_angle = 0;

		//防守半径比堵球门的大一些，避免换位的时候相撞
		m_tacctrl[4].def_angle = -45;
		if (m_trueball.x < 400)
		{
			m_tacctrl[4].def_dist += abs(m_trueball.x - 400);
		}

		//后卫
		//对方距离球门较近且不是我方持球时，切换成定角度防守
// 		if (m_CalDist(m_trueball.x,m_trueball.y,0,600) < 700 && m_bSomeoneHoldBall() == false)
// 		{
// 			m_tacctrl[5].ctrl = CTRL_DEFENCE;
// 			m_tacctrl[5].def_dist = 100*m_fKField;
// 			m_tacctrl[5].def_angle = 0;
// 		} 
// 		else
// 		{
// 			m_tacctrl[5].ctrl = CTRL_MOVETO;
// 			m_tacctrl[5].target_x = m_Pos_Back.x;
//  			m_tacctrl[5].target_y = m_Pos_Back.y+300;
// 		}
		m_tacctrl[5].ctrl = CTRL_MOVETO;
		m_tacctrl[5].target_x = m_Pos_Back.x;
 		m_tacctrl[5].target_y = m_Pos_Back.y+300;
		
	}
	else
	{
		if (tmpball.y > 300)
		{
			// 300<tmpball.y < 900 中间区域
			//先在球到球门之间堵上一个
			m_tacctrl[3].def_angle = 0;	
			m_Fix_DefOutOffFrame(&(m_tacctrl[3]),0);
			m_Fix_DefInOurPenaltyArea(&(m_tacctrl[3]));
			
			//
			m_tacctrl[4].def_dist += 50;
			m_tacctrl[4].def_angle = 45;
			m_Fix_DefOutOffFrame(&(m_tacctrl[4]),0);
			m_Fix_DefInOurPenaltyArea(&(m_tacctrl[4]));

			//后卫
			//对方距离球门较近且不是我方持球时，切换成定角度防守
// 			if (m_CalDist(m_trueball.x,m_trueball.y,0,600) < 700 && m_bSomeoneHoldBall() == false)
// 			{
// 				m_tacctrl[5].ctrl = CTRL_DEFENCE;
// 				m_tacctrl[5].def_dist = 100*m_fKField;
// 				m_tacctrl[5].def_angle = 0;
// 			} 
// 			else
// 			{
// 				m_tacctrl[5].ctrl = CTRL_MOVETO;
// 				m_tacctrl[5].target_x = m_Pos_Back.x;
// 				m_tacctrl[5].target_y = m_trueball.y;
// 			}
			m_tacctrl[5].ctrl = CTRL_MOVETO;
			m_tacctrl[5].target_x = m_Pos_Back.x;
			m_tacctrl[5].target_y = m_Pos_Back.y;
		}
		else
		{
			//tempball.y < 300 下半区
			//先在球到球门之间堵上一个
			m_tacctrl[3].def_angle = 0;
			
			//防守半径比堵球门的大一些，避免换位的时候相撞
			m_tacctrl[4].def_angle = 45;		
			if (m_trueball.x < 400)
			{
				m_tacctrl[4].def_dist += abs(m_trueball.x - 400);
			}
			
			//后卫
			//对方距离球门较近且不是我方持球时，切换成定角度防守
// 			if (m_CalDist(m_trueball.x,m_trueball.y,0,600) < 700 && m_bSomeoneHoldBall() == false)
// 			{
// 				m_tacctrl[5].ctrl = CTRL_DEFENCE;	
// 				m_tacctrl[5].def_dist = 100*m_fKField;
// 				m_tacctrl[5].def_angle = 0;
// 			} 
// 			else
// 			{
// 				m_tacctrl[5].ctrl = CTRL_MOVETO;
// 				m_tacctrl[5].target_x = m_Pos_Back.x;
// 				m_tacctrl[5].target_y = m_Pos_Back.y-300;
// 			}
			m_tacctrl[5].ctrl = CTRL_MOVETO;
			m_tacctrl[5].target_x = m_Pos_Back.x;
			m_tacctrl[5].target_y = m_Pos_Back.y-300;
		}
	}

	//(测试)原来斜向防守的队员，球靠前时拉回到中场进行防守
	//（未测试）进攻掩护
	//m_tacctrl[4].ctrl = CTRL_ATKCOVER;
	//m_tacctrl[4].def_dist = 100;		//最小掩护半径

	//当球到对方禁区内时，无论如何都要将防守队员拉出来
	if (m_trueball.x > 1600 && m_trueball.y < 925 && m_trueball.y > 275)
	{
		for (i=3;i<AG_NUM;i++)
		{
			m_tacctrl[i].def_dist = 200;
		}
	}

	m_TacAction(m_tacagent);
}

void CUPFullField::Tac_PressDef2(int inDefDist)
{
	//printf("Tac_PressDef");
	//初始化控制结构体，按角色关键程度排列
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;

	//判断一下是否需要紧逼
	// 	if (m_bSomeoneHoldBall() == true)
	// 	{
	// 		//已经有人抢到球，切换到普通进攻模式
	// 		DefaultActive(150);
	// 		return;
	// 	}

	//紧逼模式，两个抢球队员
	m_tacctrl[2].ctrl = CTRL_ATTACK;
	m_tacctrl[3].ctrl = CTRL_ATTACK;

	//其他队员跑位切换到围堵态势
	for (int i=4;i<6;i++)
	{
		m_tacctrl[i].ctrl = CTRL_MOVETO;
	}

	//先假设球位于中线上方，计算位置
	CPoint tmpball = m_trueball;
	if (m_trueball.y < 600)
	{
		tmpball.y = 1200 - tmpball.y;
	}

	CPoint tmp;

	//下面将场地分为上中下三个区域，根据球所处区域形成防守阵型
	if (tmpball.y > 900)
	{
		//上侧
		//先在球到球门之间堵上一个
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)(inDefDist*m_fKField));
		m_tacctrl[4].target_x = tmp.x;
		m_tacctrl[4].target_y = tmp.y;

		//后卫
		m_tacctrl[5].target_x = m_Pos_Back.x;
		m_tacctrl[5].target_y = m_Pos_Back.y+300;

		//test
		// 		tmp = m_Loca_CalBackPos(tmpball.x,tmpball.y,m_Pos_Back.x);
		// 		m_tacctrl[5].target_x = tmp.x;
		// 		m_tacctrl[5].target_y = tmp.y;


		//两翼的长度
		// 		CPoint longtmp = m_Loca_Ray(tmpball.x,tmpball.y,m_tacctrl[4].target_x,m_tacctrl[4].target_y,(inDefDist+100)*m_fKField);
		// 		tmp = m_Loca_Rotate(longtmp.x,longtmp.y,tmpball.x,tmpball.y,-30);
		// 		m_tacctrl[5].target_x = tmp.x;
		//		m_tacctrl[5].target_y = tmp.y;

		// 		tmp = m_Loca_Rotate(m_tacctrl[4].target_x,m_tacctrl[4].target_y,tmpball.x,tmpball.y,30);
		// 		m_tacctrl[6].target_x = tmp.x;
		// 		m_tacctrl[6].target_y = tmp.y;
	}
	else
	{
		//tmpball.y < 900 中间区域
		//进攻角色
		m_tacctrl[2].ctrl = CTRL_ATTACK;
		m_tacctrl[3].ctrl = CTRL_ATTACK;

		//先在球到球门之间堵上一个
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)(inDefDist*m_fKField));
		m_tacctrl[4].target_x = tmp.x;
		m_tacctrl[4].target_y = tmp.y;


		//后卫
		m_tacctrl[5].target_x = m_Pos_Back.x;
		m_tacctrl[5].target_y = m_Pos_Back.y;

		//两翼的长度
		// 		CPoint longtmp = m_Loca_Ray(tmpball.x,tmpball.y,m_tacctrl[4].target_x,m_tacctrl[4].target_y,(inDefDist+100)*m_fKField);
		// 		tmp = m_Loca_Rotate(longtmp.x,longtmp.y,tmpball.x,tmpball.y,-40);
		// 		m_tacctrl[5].target_x = tmp.x;
		// 		m_tacctrl[5].target_y = tmp.y;


		//		tmp = m_Loca_Rotate(m_tacctrl[4].target_x,m_tacctrl[4].target_y,tmpball.x,tmpball.y,45);
		// 		tmp = m_Loca_Rotate(longtmp.x,longtmp.y,tmpball.x,tmpball.y,45);
		// 		m_tacctrl[6].target_x = tmp.x;
		// 		m_tacctrl[6].target_y = tmp.y;
	}

	//针对自己球员进入禁区做修正
	for (i=3;i<6;i++)
	{
		if (m_Check_MovetoOurPenaltyArea(m_tacctrl[i]) == true)
		{
			m_Fix_MovetoOurPenaltyArea(&m_tacctrl[i]);
		}
	}

	//针对自己球员出边界进行修正	
	for (i=3;i<6;i++)
	{
		if (m_Check_MoveOutOffFrame(m_tacctrl[i]) == true)
		{
			m_Fix_MoveOutOffFrame(&m_tacctrl[i]);
		}
	}

	//如果球在中线下方，则将坐标上下翻转
	if (m_trueball.y < 600)
	{
		m_tacctrl[2].target_y = 1200 - m_tacctrl[2].target_y;
		m_tacctrl[3].target_y = 1200 - m_tacctrl[3].target_y;		
		m_tacctrl[4].target_y = 1200 - m_tacctrl[4].target_y;
		m_tacctrl[5].target_y = 1200 - m_tacctrl[5].target_y;
	}

	//角度,都朝向球
	for (i=4;i<6;i++)
	{
		//m_tacctrl[i].target_angle = m_Loca_CalAngle(m_tacctrl[i].target_x,m_tacctrl[i].target_y,m_trueball.x,m_trueball.y);
		m_tacctrl[i].target_angle = 0;
	}

	m_TacAction(m_tacagent);
}

void CUPFullField::m_Fix_DefOutOffFrame(MSG_Control* inCtrl, int inReachOut)
{
	CPoint tmp;
	
	tmp = m_Loca_Ray(m_trueball.x,m_trueball.y,0,600,inCtrl->def_dist);
	if (inCtrl->def_angle != 0)
	{
		tmp = m_Loca_Rotate(tmp.x,tmp.y,m_trueball.x,m_trueball.y,inCtrl->def_angle);
	}
	//////////////////////////////////////////////////////////////////////////
// 		inCtrl->ctrl = CTRL_MOVETO;
// 		inCtrl->target_x = tmp.x;
// 		inCtrl->target_y = tmp.y;
	//////////////////////////////////////////////////////////////////////////
	if (tmp.x < 0)
	{
		//超出我方底线，Moveto退回底线
		inCtrl->ctrl = CTRL_MOVETO;
		inCtrl->target_x = 0;
		inCtrl->target_y = tmp.y;
		inCtrl->target_angle = m_Loca_CalAngle(inCtrl->target_x,inCtrl->target_y,m_trueball.x,m_trueball.y);
	}

	if (tmp.x > 1800)
	{
		//超出对方底线，Moveto退回底线
		inCtrl->ctrl = CTRL_MOVETO;
		inCtrl->target_x = 1800;
		inCtrl->target_y = tmp.y;
		inCtrl->target_angle = m_Loca_CalAngle(inCtrl->target_x,inCtrl->target_y,m_trueball.x,m_trueball.y);
	}

	if (tmp.y < 0)
	{
		//超出我方底线，Moveto退回底线
		inCtrl->ctrl = CTRL_MOVETO;
		inCtrl->target_x = tmp.x;
		inCtrl->target_y = 0;
		inCtrl->target_angle = m_Loca_CalAngle(inCtrl->target_x,inCtrl->target_y,m_trueball.x,m_trueball.y);
	}

	if (tmp.y > 1200)
	{
		//超出我方底线，Moveto退回底线
		inCtrl->ctrl = CTRL_MOVETO;
		inCtrl->target_x = tmp.x;
		inCtrl->target_y = 1200;
		inCtrl->target_angle = m_Loca_CalAngle(inCtrl->target_x,inCtrl->target_y,m_trueball.x,m_trueball.y);
	}
}

void CUPFullField::m_Fix_DefInOurPenaltyArea(MSG_Control *inTac)
{
	CPoint tmp;
	tmp = m_Loca_Ray(m_trueball.x,m_trueball.y,0,600,inTac->def_dist);
	if (inTac->def_angle != 0)
	{
		tmp = m_Loca_Rotate(tmp.x,tmp.y,m_trueball.x,m_trueball.y,inTac->def_angle);
	}

	if (
		tmp.y < (1200-(1200-PENALTY_LONG)/2) && 
		tmp.y > ((1200-PENALTY_LONG)/2) && 
		tmp.x < PENALTY_WIDTH
		)
	{
		//防守位置在我方禁区内，Moveto退出禁区
		tmp.x = PENALTY_WIDTH+20;
		inTac->ctrl = CTRL_MOVETO;
		inTac->target_x = tmp.x;
		inTac->target_y = tmp.y;
		inTac->target_angle = m_Loca_CalAngle(inTac->target_x,inTac->target_y,m_trueball.x,m_trueball.y);
	}
}

bool CUPFullField::m_Obst_RemoveOutOfField(obstacle *inObstAr)
{
	int nNumOfObst = EACH_AG_OBST_NUM*(AG_NUM+1);
	for (int i=0;i<nNumOfObst;i++)
	{
		if (inObstAr[i].valid == true)
		{
			//场外的障碍物
			if (
				inObstAr[i].x < 0 ||
				inObstAr[i].x > 1800 ||
				inObstAr[i].y < 0 ||
				inObstAr[i].y > 1200
				)
			{
				inObstAr[i].valid = false;
			}

			/*/////////////////////////////////////////////////////////////////////////
			//对方禁区内的障碍物
			if(
				inObstAr[i].y < (1200-(1200-PENALTY_LONG)/2) && 
				inObstAr[i].y > ((1200-PENALTY_LONG)/2) && 
				inObstAr[i].x > (1200 - PENALTY_WIDTH)
				)
			{
				inObstAr[i].valid = false;
			}		/////////////////////////////////////////////////////////////////////////*/
			
		}
	}
	return true;
}

bool CUPFullField::GetAgentOnline(int inID)
{
	return m_agents[inID].online;
}

//修正moveto行为，让他在保持与球距离的基础上，移出禁区
void CUPFullField::m_Fix_MoveOutOfOurPenaltyArea(MSG_Control *inTac, int inDist)
{
	if (inTac->ctrl != CTRL_MOVETO && inTac->ctrl != CTRL_PRODEF && inTac->ctrl != CTRL_BLOCK )
	{
		return ;
	}
	
}

//后卫优先
void CUPFullField::DefaultReady2()
{
	KillTimer(TAC_WAIT_TIMER);

	//2010年使用moveto策略
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;
	//其他队员跑动到指定点
	for (int i=2;i<6;i++)
	{
		m_tacctrl[i].ctrl = CTRL_MOVETO;
		m_tacctrl[i].target_angle = 0;	
	}
	//先假设球靠上，计算位置
	CPoint tmpball = m_trueball;
	if (tmpball.y < 500)
	{
		tmpball.y = 1200 - tmpball.y;
	}
	
	CPoint tmp;
	//根据球所在的半场来确定开球策略
	if (tmpball.x < 700)
	{
		//1、球处于我方半场，处理的时候尽量大脚转移
		
		//接球进攻队员-球-对方球门 = 180度
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,1600,600,(int)(700*m_fKField));			//最后一个参数是距离
		//tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,180);
		m_tacctrl[3].target_x = tmp.x;
		m_tacctrl[3].target_y = tmp.y;
		m_tacctrl[3].target_angle = m_Loca_CalAngle(tmp.x,tmp.y,tmpball.x,tmpball.y);
	
		//传球队员-球-对方球门 = 180度
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,1800,600,(int)(100*m_fKField));	//最后一个参数是距离
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,180);
		m_tacctrl[2].target_x = tmp.x;
		m_tacctrl[2].target_y = tmp.y;
		m_tacctrl[2].target_angle = m_Loca_CalAngle(tmp.x,tmp.y,tmpball.x,tmpball.y);
	} 
	else
	{
		//2、球处于对方半场，争取直接射门	
		//接球进攻队员-球-对方球门 = 180度
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,1600,600,(int)(200*m_fKField));			//最后一个参数是距离
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,180);
		m_tacctrl[3].target_x = tmp.x;
		m_tacctrl[3].target_y = tmp.y;
		m_tacctrl[3].target_angle = m_Loca_CalAngle(tmp.x,tmp.y,tmpball.x,tmpball.y);

		//接球队员位置修正,避免出界
		int nodead = 0;
		while (m_tacctrl[3].target_y > 1100)
		{
			nodead ++;
			if (nodead > 100)
			{
				break;
			}
			//角球修正
			tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,-5);
			m_tacctrl[3].target_x = tmp.x;
			m_tacctrl[3].target_y = tmp.y;
		}
		m_tacctrl[3].target_angle = m_Loca_CalAngle(tmp.x,tmp.y,tmpball.x,tmpball.y);

		//传球队员-球-接球队员 = 0度
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,m_tacctrl[3].target_x,m_tacctrl[3].target_y,(int)(100*m_fKField));	//最后一个参数是距离
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,180);
		m_tacctrl[2].target_x = tmp.x;
		m_tacctrl[2].target_y = tmp.y;
		m_tacctrl[2].target_angle = m_Loca_CalAngle(tmp.x,tmp.y,tmpball.x,tmpball.y);
	}
	
	//修正跑动位置，避免不巧与球的距离太近
	if (m_CalDist(tmpball.x,tmpball.y,m_Pos_Back.x,m_Pos_Back.y) > 300)
	{		
		m_tacctrl[4].target_x = m_Pos_Back.x;
		m_tacctrl[4].target_y = m_Pos_Back.y;
	} 
	else
	{	
		m_tacctrl[4].target_x = 300;
		m_tacctrl[4].target_y = 300;
	}
	
	
	tmp = m_Loca_Ray(m_tacctrl[3].target_x,m_tacctrl[3].target_y,0,600,(int)(400*m_fKField));
	tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,-35);
	m_tacctrl[5].target_x = tmp.x;
	m_tacctrl[5].target_y = tmp.y;
	m_tacctrl[5].target_angle = m_Loca_CalAngle(tmp.x,tmp.y,tmpball.x,tmpball.y);
	
	//若球在下方，则坐标上下翻转
	if (m_trueball.y < 500)
	{
		m_tacctrl[2].target_y = 1200 - m_tacctrl[2].target_y;
		m_tacctrl[2].target_angle = -m_tacctrl[2].target_angle;
		m_tacctrl[3].target_y = 1200 - m_tacctrl[3].target_y;
		m_tacctrl[3].target_angle = -m_tacctrl[3].target_angle;
		m_tacctrl[4].target_y = 1200 - m_tacctrl[4].target_y;
		m_tacctrl[4].target_angle = -m_tacctrl[4].target_angle;
		m_tacctrl[5].target_y = 1200 - m_tacctrl[5].target_y;
		m_tacctrl[5].target_angle = -m_tacctrl[5].target_angle;
	}

	//指定接球者ID
	m_nCatchPlayer = m_Ag_GetNearBy(m_tacctrl[3].target_x,m_tacctrl[3].target_y,m_tacagent);

	//防止撞击
	m_Fix_CollisionAvoid(m_tacctrl, 50);

	m_TacAction(m_tacagent);
}

void CUPFullField::BallShotRes(char inRes,int inx,int iny)
{
	CString info;
	switch(inRes)
	{
	case 0:
		break;

	case 1:
		info.Format("(%d,%d)起飞点",inx,iny);
	//	printf(info);
		break;

	case 2:
		info.Format("(%d,%d)起飞点",inx,iny);
	//	printf(info);
		break;

	default:

		break;
	}

}

int CUPFullField::m_Obst_NearBy(int inx, int iny, obstacle *inObstAr, int inNumOfObst)
{
	int ret = -1;
	int mindist = 0x7fffffff;
	int tmp_dist = 0;
    int nNumObst = inNumOfObst;
	//搜索
	for (int i=0;i<nNumObst;i++)
	{
		if (inObstAr[i].valid == true)
		{
			//计算距离
			tmp_dist = m_CalDist(inObstAr[i].x,inObstAr[i].y,inx,iny);
			if (tmp_dist < mindist)
			{
				ret = i;
				mindist = tmp_dist;
			}
		}
	}
	return ret;
}

void CUPFullField::TC_Demo1()
{
	m_eStage = sTechChallenge;
	m_bTCArrived = false;
}
void CUPFullField::TC_Demo2012()
{
	m_eStage = sTechChallenge2012;
	techStep2012 = s_start;
	isStart = false;
	//m_bTCArrived = false;
}

void CUPFullField::DropBall_Ready(int inDistFB)
{
	KillTimer(TAC_WAIT_TIMER);
	//对方开球，我方跑位应对
	//////////////////////////////////////////////////////////////////////////
	//10年策略
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;

	for (int i=2;i<7;i++)
	{
		m_tacctrl[i].ctrl = CTRL_MOVETO;
	}

	//先假设球位于中线上方，计算位置
	CPoint tmpball = m_trueball;
	if (m_trueball.y < 600)
	{
		tmpball.y = 1200 - tmpball.y;
	}
	
 	CPoint tmp;	
	////////////////////////////////////////////////////////////////////////////
	if (tmpball.x > 200)
	{
		//球在我方半场中等距离时
		//在球到球门之间堵上一个
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)(150*m_fKField));
		m_tacctrl[2].target_x = tmp.x;
		m_tacctrl[2].target_y = tmp.y;

		//边翼1
		int defAngle = -50;
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)(250*m_fKField));
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,defAngle);
		m_tacctrl[3].target_x = tmp.x;
		m_tacctrl[3].target_y = tmp.y;

		//!替换方案：用进攻掩护来防守对方接球队员
// 		m_tacctrl[3].ctrl = CTRL_ATKCOVER;
// 		m_tacctrl[3].def_dist = (inDistFB+50)*m_fKField; 

		//后卫
		m_tacctrl[4].target_x = m_Pos_Back.x;
 		m_tacctrl[4].target_y = m_Pos_Back.y;

		//边翼2，容易出界，拉回来
		defAngle = 50;
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)((inDistFB+50)*m_fKField));
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,defAngle);
		while(tmp.y > 1150)
		{
			defAngle -= 5;
			tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)((inDistFB+50)*m_fKField));
			tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,defAngle);
		}
		m_tacctrl[5].target_x = tmp.x;
		m_tacctrl[5].target_y = tmp.y;
		
		if (m_tacctrl[2].target_x < 200)
		{
			//堵门队员退到禁区里了，应该是球门任意球，位置调整
			m_tacctrl[2].target_x = 200;	//禁区内队员前移

			//两翼稍微张大一些
			tmp = m_Loca_Rotate(m_tacctrl[3].target_x,m_tacctrl[3].target_y,tmpball.x,tmpball.y,-50);
			m_tacctrl[3].target_x = tmp.x;
			m_tacctrl[3].target_y = tmp.y;

			tmp = m_Loca_Rotate(m_tacctrl[5].target_x,m_tacctrl[5].target_y,tmpball.x,tmpball.y,20);
			m_tacctrl[5].target_x = tmp.x;
			m_tacctrl[5].target_y = tmp.y;
		}
		
	}
	else//(tmpball.x < 200),应该是角球一类的
	{
		//球在己方近球门
		//先在球到球门之间堵上一个
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)(110*m_fKField));
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,-15);
		m_tacctrl[2].target_x = tmp.x;
		m_tacctrl[2].target_y = tmp.y;
		//m_tacctrl[2].target_angle = m_Loca_CalAngle(m_tacctrl[2].target_x,m_tacctrl[2].target_y,tmpball.x,tmpball.y);

		//在第一个防卫上旋转一个角度
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)(160*m_fKField));
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,-60);
		m_tacctrl[3].target_x = tmp.x;
		m_tacctrl[3].target_y = tmp.y;

		//!替换方案：用进攻掩护来防守对方接球队员
// 		m_tacctrl[3].ctrl = CTRL_ATKCOVER;
// 		m_tacctrl[3].def_dist = (inDistFB+50)*m_fKField; 
		
		//后防，避免对方向另一侧转移
		m_tacctrl[4].target_x = m_Pos_Back.x;
		m_tacctrl[4].target_y = 900;

		//后卫
		m_tacctrl[5].target_x = m_Pos_Back.x;
		m_tacctrl[5].target_y = m_Pos_Back.y;
	}
		
	//如果球在中线下方，则将坐标上下翻转
	if (m_trueball.y < 600)
	{
		m_tacctrl[2].target_y = 1200 - m_tacctrl[2].target_y;
		m_tacctrl[3].target_y = 1200 - m_tacctrl[3].target_y;		
		m_tacctrl[4].target_y = 1200 - m_tacctrl[4].target_y;
		m_tacctrl[5].target_y = 1200 - m_tacctrl[5].target_y;
	}

	//2010添加防守队员
	//Default_Anti_Ready_Add(inDistFB);

	//防止移动到场地外
	for (i=2;i<6;i++)
	{
		m_Fix_MoveOutOffFrame(&(m_tacctrl[i]));
	}

	//防止撞击
	m_Fix_CollisionAvoid(m_tacctrl, 50);

	//角度,都朝向球
	for (i=2;i<6;i++)
	{
		m_tacctrl[i].target_angle = m_Loca_CalAngle(m_tacctrl[i].target_x,m_tacctrl[i].target_y,m_trueball.x,m_trueball.y);
	}

	m_TacAction(m_tacagent);
}

void CUPFullField::m_Fix_AroundOutOffFrame(MSG_Control *inTac)
{

}

void CUPFullField::m_ShowTacInfo(int inCtrl,int inAgentId)
{
	//避免出错
	if (inAgentId < 0 || inAgentId >= AG_NUM)
	{
		return;
	}
	if (pTacInfo == NULL)
	{
		return;
	}
	if (m_agents[inAgentId].online == false)
	{
		return;
	}

	if (m_eStage == sMani)
	{
		return;
	}
	
	CString str;
	//////////////////////////////////////////////////////////////////////////
// 	str.Format(" %d",inAgentId);
// 	str = GetCTRLString(inCtrl) + str;
// 	printf(str);
	//////////////////////////////////////////////////////////////////////////
	//tac id
	str.Format("%d",m_nLogTacIndex);
	pTacInfo->SetItemText(m_nLogTacIndex,0,str);

	//ctrl
	str = GetCTRLString(inCtrl);
	pTacInfo->SetItemText(m_nLogTacIndex,1,str);

	//agentid
	str.Format("%d",inAgentId);
	pTacInfo->SetItemText(m_nLogTacIndex,2,str);

	m_nLogTacIndex ++;
}

//m_trueball.x < 1000时会调用，与Tac_RoundedDef2的区别是优先考虑防守
void CUPFullField::Tac_RoundedDef3(int inDefDist)
{
	//初始化控制结构体，按角色关键程度排列
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;	
	
	//3~5号角色跑位切换到定角度防守态势（3号角色后面会被覆盖成进攻）
	for (int i=2;i<6;i++)
	{
		m_tacctrl[i].ctrl = CTRL_DEFENCE;
		m_tacctrl[i].def_dist = (int)(inDefDist*m_fKField);
	}

	//进攻角色
	m_tacctrl[3].ctrl = CTRL_ATTACK;

	CPoint tmpball = m_trueball;	
 	CPoint tmp;

	//下面将场地分为上中下三个区域，根据球所处区域形成防守阵型
	if (tmpball.y > 900)
	{
		//上侧
		//先在球到球门之间堵上一个
		m_tacctrl[2].def_angle = 0;

		//防守半径比堵球门的大一些，避免换位的时候相撞
		m_tacctrl[4].def_angle = -45;
		if (m_trueball.x < 400)
		{
			m_tacctrl[4].def_dist += abs(m_trueball.x - 400);
		}

		//后卫
		//对方距离球门较近且不是我方持球时，切换成定角度防守
// 		if (m_CalDist(m_trueball.x,m_trueball.y,0,600) < 700 && m_bSomeoneHoldBall() == false)
// 		{
// 			m_tacctrl[5].ctrl = CTRL_DEFENCE;
// 			m_tacctrl[5].def_dist = 100*m_fKField;
// 			m_tacctrl[5].def_angle = 0;
// 		} 
// 		else
// 		{
// 			m_tacctrl[5].ctrl = CTRL_MOVETO;
// 			m_tacctrl[5].target_x = m_Pos_Back.x;
//  			m_tacctrl[5].target_y = m_Pos_Back.y+300;
// 		}
		m_tacctrl[5].ctrl = CTRL_MOVETO;
		m_tacctrl[5].target_x = m_Pos_Back.x;
 		m_tacctrl[5].target_y = m_Pos_Back.y+300;
		
	}
	else
	{
		if (tmpball.y > 300)
		{
			// 300<tmpball.y < 900 中间区域
			//先在球到球门之间堵上一个
			m_tacctrl[2].def_angle = 0;	
			m_Fix_DefOutOffFrame(&(m_tacctrl[2]),0);
			m_Fix_DefInOurPenaltyArea(&(m_tacctrl[2]));
			
			//
			m_tacctrl[4].def_dist += 50;
			m_tacctrl[4].def_angle = 45;
			m_Fix_DefOutOffFrame(&(m_tacctrl[4]),0);
			m_Fix_DefInOurPenaltyArea(&(m_tacctrl[4]));

			//后卫
			//对方距离球门较近且不是我方持球时，切换成定角度防守
// 			if (m_CalDist(m_trueball.x,m_trueball.y,0,600) < 700 && m_bSomeoneHoldBall() == false)
// 			{
// 				m_tacctrl[5].ctrl = CTRL_DEFENCE;
// 				m_tacctrl[5].def_dist = 100*m_fKField;
// 				m_tacctrl[5].def_angle = 0;
// 			} 
// 			else
// 			{
// 				m_tacctrl[5].ctrl = CTRL_MOVETO;
// 				m_tacctrl[5].target_x = m_Pos_Back.x;
// 				m_tacctrl[5].target_y = m_trueball.y;
// 			}
			m_tacctrl[5].ctrl = CTRL_MOVETO;
			m_tacctrl[5].target_x = m_Pos_Back.x;
			m_tacctrl[5].target_y = m_Pos_Back.y;
		}
		else
		{
			//tempball.y < 300 下半区
			//先在球到球门之间堵上一个
			m_tacctrl[2].def_angle = 0;
			
			//防守半径比堵球门的大一些，避免换位的时候相撞
			m_tacctrl[4].def_angle = 45;		
			if (m_trueball.x < 400)
			{
				m_tacctrl[4].def_dist += abs(m_trueball.x - 400);
			}
			
			//后卫
			//对方距离球门较近且不是我方持球时，切换成定角度防守
// 			if (m_CalDist(m_trueball.x,m_trueball.y,0,600) < 700 && m_bSomeoneHoldBall() == false)
// 			{
// 				m_tacctrl[5].ctrl = CTRL_DEFENCE;	
// 				m_tacctrl[5].def_dist = 100*m_fKField;
// 				m_tacctrl[5].def_angle = 0;
// 			} 
// 			else
// 			{
// 				m_tacctrl[5].ctrl = CTRL_MOVETO;
// 				m_tacctrl[5].target_x = m_Pos_Back.x;
// 				m_tacctrl[5].target_y = m_Pos_Back.y-300;
// 			}
			m_tacctrl[5].ctrl = CTRL_MOVETO;
			m_tacctrl[5].target_x = m_Pos_Back.x;
			m_tacctrl[5].target_y = m_Pos_Back.y-300;
		}
	}

	//(测试)原来斜向防守的队员，球靠前时拉回到中场进行防守
	//（未测试）进攻掩护
	//m_tacctrl[4].ctrl = CTRL_ATKCOVER;
	//m_tacctrl[4].def_dist = 100;		//最小掩护半径

	m_TacAction(m_tacagent);
}

void CUPFullField::m_Fix_GetOutOfTheWayOfATK(MSG_Control *inCtrl)
{

}

void CUPFullField::m_Fix_GetOutOfTheWayOfATK()
{
	//寻找进攻队员
	int nAtkID = 0;
	for (int i=2;i<AG_NUM;i++)
	{
		if (m_tacagent[i].online == true && (m_tacagent[i].status == CTRL_ATTACK ||m_tacagent[i].status == CTRL_MID_PASS))
		{
			nAtkID = i;
			break;
		}
	}

	if (nAtkID<2)
	{
		//未找到进攻队员，退出
		return;
	}

	//进攻队员存在，查看其是否很靠近球,和球超过一定距离时，他应该自动绕过去，不必避让他
	if (m_CalDist(m_tacagent[nAtkID].x , m_tacagent[nAtkID].y , m_trueball.x,m_trueball.y) > 100)
	{
		return;
	}

	CPoint fixTarget;
	int fixAngle;
	//查找stop、moveto和defence，这两个角色最可能阻挡进攻
	for (i=2;i<AG_NUM;i++)
	{
		if (m_tacagent[i].online == true && m_tacagent[i].status == CTRL_MOVETO)
		{
			if (m_CalDist(m_tacagent[nAtkID].x , m_tacagent[nAtkID].y , m_tacagent[i].moveto_x , m_tacagent[i].moveto_y) < 100)
			{
				//moveto的目标点和进攻距离太近，移开
				fixTarget = m_Loca_Ray(m_tacagent[nAtkID].x , m_tacagent[nAtkID].y,m_tacagent[i].moveto_x , m_tacagent[i].moveto_y,100);
				fixAngle = m_Loca_CalAngle(fixTarget.x , fixTarget.y , m_trueball.x , m_trueball.y);
				AMoveTo(i,fixTarget.x,fixTarget.y,fixAngle);
			}
		}

		if (m_tacagent[i].online == true && (m_tacagent[i].status == CTRL_DEFENCE||m_tacagent[i].status == CTRL_FOLLOWPOSITION))
		{
			if (m_CalDist(m_tacagent[nAtkID].x , m_tacagent[nAtkID].y , m_tacagent[i].x , m_tacagent[i].y) < 100)
			{
				//defenc卡住的位置和进攻距离太近，移开
				fixTarget = m_Loca_Ray(m_tacagent[nAtkID].x , m_tacagent[nAtkID].y,m_tacagent[i].x , m_tacagent[i].y,100);
				fixAngle = m_Loca_CalAngle(fixTarget.x , fixTarget.y , m_trueball.x , m_trueball.y);
				AMoveTo(i,fixTarget.x,fixTarget.y,fixAngle);
			}
		}

		if (m_tacagent[i].online == true && m_tacagent[i].status == CTRL_STOP)
		{
			if (m_CalDist(m_tacagent[nAtkID].x , m_tacagent[nAtkID].y , m_tacagent[i].x , m_tacagent[i].y) < 100)
			{
				//defenc卡住的位置和进攻距离太近，移开
				fixTarget = m_Loca_Ray(m_tacagent[nAtkID].x , m_tacagent[nAtkID].y,m_tacagent[i].x , m_tacagent[i].y,100);
				fixAngle = m_Loca_CalAngle(fixTarget.x , fixTarget.y , m_trueball.x , m_trueball.y);
				AMoveTo(i,fixTarget.x,fixTarget.y,fixAngle);
			}
		}
	}
}

bool CUPFullField::m_bAg_InParkingArea()
{
	bool bIn = true;
	for (int i=1;i<AG_NUM;i++)
	{
		if (m_tacagent[i].online == true && m_tacagent[i].status != ROBST_ERR)
		{

		}
	}
	return bIn;
}

void CUPFullField::Tac_3meterPass(int holdballId)//[wzc]
{
	KillTimer(TAC_WAIT_TIMER);
	int k=holdballId;
	m_tacctrl[1].ctrl=CTRL_GOALKEEP;

	m_n3mPassPlayer = k;
	if (m_bTask3MeterPass_Kick==false)
	{
	    m_tacctrl[2].ctrl=CTRL_3M_PASS;
		m_tacctrl[3].ctrl=CTRL_PASS_MOVE;
		//kong();
		m_tacctrl[3].target_x = kongx;
		m_tacctrl[3].target_y = kongy;
		m_tacctrl[3].target_angle = m_Loca_CalAngle(m_tacctrl[3].target_x,m_tacctrl[3].target_y,m_trueball.x,m_trueball.y);
	}
	else
	{
		m_tacctrl[2].ctrl=CTRL_3M_CATCH;
		m_tacctrl[3].ctrl=CTRL_STOP;
	}

	CPoint des;
	des.x = kongx;
	des.y = kongy;
	m_n3mCatchPlayer=0;
	int one = SelectBestCatcher(des);
	m_n3mCatchPlayer = one;
	
	
		for (int i=4;i<6;i++)
		{
			m_tacctrl[i].ctrl = CTRL_DEFENCE;
			m_tacctrl[i].def_dist = (int)(TAC_DIST_DEFCLOSE*m_fKField);
		}

		CPoint tmpball = m_trueball;

		//下面将场地分为上中下三个区域，根据球所处区域形成防守阵型
		if (tmpball.y > 900)
		{
			//上侧
			//先在球到球门之间堵上一个
			m_tacctrl[4].def_angle = 0;

			//后卫
			m_tacctrl[5].ctrl = CTRL_MOVETO;
			m_tacctrl[5].target_x = m_Pos_Back.x;
			m_tacctrl[5].target_y = m_Pos_Back.y+200;

		}
		else
		{
			if (tmpball.y > 300)
			{
				// 300<tmpball.y < 900 中间区域
				//先在球到球门之间堵上一个
				m_tacctrl[4].def_angle = 0;
				m_Fix_DefOutOffFrame(&(m_tacctrl[4]),0);
				m_Fix_DefInOurPenaltyArea(&(m_tacctrl[4]));

				//后卫
				m_tacctrl[5].ctrl = CTRL_MOVETO;
				m_tacctrl[5].target_x = m_Pos_Back.x;
				m_tacctrl[5].target_y = m_Pos_Back.y;
			}
			else
			{
				//tempball.y < 300 下半区
				//先在球到球门之间堵上一个
				m_tacctrl[4].def_angle = 0;

				//后卫
				m_tacctrl[5].ctrl = CTRL_MOVETO;
				m_tacctrl[5].target_x = m_Pos_Back.x;
				m_tacctrl[5].target_y = m_Pos_Back.y-200;
			}
		}
	CString info;
	info.Format("3 meter pass");
	printf(info);
	
	m_TacAction(m_tacagent);	

	//********************************************************************
	//******************************************************
}

int CUPFullField::m_nAgentWhoHoldBall_fromAg()
{
	for (int i=1;i<7;i++)
	{
	
		if (m_agents[i].online == true && m_agents[i].status != CTRL_GOALKEEP && m_agents[i].status != ROBST_ERR)
		{		
			if (m_agents[i].isAgHoldBall==true)
			{
				
				return i;
			}
			else
			{
				continue;
			}
		}
	}	
	return -1;
}
int CUPFullField::m_nAgentWhoHoldBall()
{
	for (int i=1;i<7;i++)
	{
	
		if (m_agents[i].online == true && m_agents[i].status != CTRL_GOALKEEP && m_agents[i].status != ROBST_ERR)
		{		
			if (m_agents[i].dist_ball < 50)
			{
				
				return i;
			}
			else
			{
				continue;
			}
		}
	}	
	return -1;
}




void CUPFullField::m_bClear3meterPassInfo()
{
	HoldBallFlag =0;
	MemHoldBallFlag=0;
	InitialHoldBall.x=0;
	InitialHoldBall.y=0;
	DistToInitialHoldBallPoint=0;
	HoldBallBelieve=0;
	m_3meterPassBelieve=0;
	for (int i=1;i<AG_NUM;i++)
	{
		m_memstatus[i]=NULL;
	}
}
void CUPFullField ::Tac_3meterPass_Kicked()
{
	//if (believe>=100)
	//{
	//	m_tacctrl[2].ctrl = CTRL_ATTACK;
	//	m_tacctrl[3].ctrl = CTRL_STOP;
	//} 
	//else
	//{
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;
		//继续传球者角色
		m_tacctrl[2].ctrl = CTRL_3M_CATCH;
		m_n3mPassPlayer=Last3meterPasser;
		m_n3mCatchPlayer=Last3meterCatcher;
		//3~5号队员跑位切换到定角度防守态势
		for (int i=3;i<6;i++)
		{
			//m_tacctrl[i].ctrl = CTRL_DEFENCE;
			m_tacctrl[i].ctrl = CTRL_STOP;
			m_tacctrl[i].def_dist = (int)(300*m_fKField);
		}
		m_tacctrl[3].ctrl = CTRL_MOVETO;
		m_tacctrl[3].target_x = m_trueball.x;
		m_tacctrl[3].target_y =1200 - m_trueball.y;

		CPoint tmpball = m_trueball;

		//下面将场地分为上中下三个区域，根据球所处区域形成防守阵型
		if (tmpball.y > 900)
		{
			//上侧
			//先在球到球门之间堵上一个
			m_tacctrl[4].def_angle = 0;

			//后卫
			m_tacctrl[5].ctrl = CTRL_MOVETO;
			m_tacctrl[5].target_x = m_Pos_Back.x;
			m_tacctrl[5].target_y = m_Pos_Back.y+300;

		}
		else
		{
			if (tmpball.y > 300)
			{
				// 300<tmpball.y < 900 中间区域
				//先在球到球门之间堵上一个
				m_tacctrl[4].def_angle = 0;
				m_Fix_DefOutOffFrame(&(m_tacctrl[4]),0);
				m_Fix_DefInOurPenaltyArea(&(m_tacctrl[4]));

				//后卫
				m_tacctrl[5].ctrl = CTRL_MOVETO;
				m_tacctrl[5].target_x = m_Pos_Back.x;
				m_tacctrl[5].target_y = m_Pos_Back.y;
			}
			else
			{
				//tempball.y < 300 下半区
				//先在球到球门之间堵上一个
				m_tacctrl[4].def_angle = 0;

				//后卫
				m_tacctrl[5].ctrl = CTRL_MOVETO;
				m_tacctrl[5].target_x = m_Pos_Back.x;
				m_tacctrl[5].target_y = m_Pos_Back.y-300;
			}
		}
		m_TacAction(m_tacagent);
	//}
}
void CUPFullField::Tac_MidPass( int defdis,int px,int py,int believe )//[yzz]
{
	//throw std::exception("The method or operation is not implemented.");
	//初始化控制结构体，按角色关键程度排列
	m_bTaskMidPassCatch = true;
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;

	if(believe>=100 && believe<200)
	{
		//进攻角色
		m_tacctrl[2].ctrl = CTRL_MID_PASS;	
		m_tacctrl[3].ctrl = CTRL_MID_CATCH;

		final_mid_pass_x = px;//真正发送给单体的传球点
		final_mid_pass_y = py;
		final_mid_pass_believe = believe;
		for (int i=3;i<AG_NUM;i++)
		{
			m_tacctrl[i].ctrl = CTRL_DEFENCE;
			if (m_bSomeoneHoldBall() == true)
			{
				m_tacctrl[i].def_dist = (int)(TAC_DIST_DEFFAR*m_fKField);
			} 
			else
			{	
				m_tacctrl[i].def_dist = (int)(TAC_DIST_DEFCLOSE*m_fKField);
			}
		}
		m_tacctrl[3].def_angle = 0;
		
	}
	else
	{
		//进攻角色
		m_tacctrl[2].ctrl = CTRL_MID_PASS;	
		m_tacctrl[3].ctrl = CTRL_MID_CATCH;

		final_mid_pass_x = px;//真正发送给单体的传球点
		final_mid_pass_y = py;
		final_mid_pass_believe = believe;

		//3~5号队员跑位切换到定角度防守态势

		for (int i=4;i<6;i++)
		{
			//m_tacctrl[i].ctrl = CTRL_DEFENCE;
			m_tacctrl[i].ctrl = CTRL_DEFENCE;
			m_tacctrl[i].def_dist = (int)(defdis*m_fKField);
		}

		CPoint tmpball = m_trueball;

		//下面将场地分为上中下三个区域，根据球所处区域形成防守阵型
		if (tmpball.y > 900)
		{
			//上侧
			//先在球到球门之间堵上一个
			m_tacctrl[4].def_angle = 0;

			//后卫
			m_tacctrl[5].ctrl = CTRL_MOVETO;
			m_tacctrl[5].target_x = m_Pos_Back.x;
			m_tacctrl[5].target_y = m_Pos_Back.y+200;

		}
		else
		{
			if (tmpball.y > 300)
			{
				// 300<tmpball.y < 900 中间区域
				//先在球到球门之间堵上一个
				m_tacctrl[4].def_angle = 0;
				m_Fix_DefOutOffFrame(&(m_tacctrl[4]),0);
				m_Fix_DefInOurPenaltyArea(&(m_tacctrl[4]));

				//后卫
				m_tacctrl[5].ctrl = CTRL_MOVETO;
				m_tacctrl[5].target_x = m_Pos_Back.x;
				m_tacctrl[5].target_y = m_Pos_Back.y;
			}
			else
			{
				//tempball.y < 300 下半区
				//先在球到球门之间堵上一个
				m_tacctrl[4].def_angle = 0;

				//后卫
				m_tacctrl[5].ctrl = CTRL_MOVETO;
				m_tacctrl[5].target_x = m_Pos_Back.x;
				m_tacctrl[5].target_y = m_Pos_Back.y-200;
			}
		}
	}
	
	CString info;
	info.Format("进入到角色分配 \n");
	printf(info);
	m_TacAction(m_tacagent);
}

void CUPFullField::kong(){
	int nNumOfObst = EACH_AG_OBST_NUM*(AG_NUM+1);
	double map[36][24],min;
	int zhangai[80][2];
	int i,j,k,n;
	for(i=0;i<36;i++)
	{
		for(j=0;j<24;j++)
		{
			map[i][j]=0.0;//初始化
		}
	}
	//int count=0;
	for(i=0,n=0;i<nNumOfObst;i++){
		if(m_tacObst[i].valid == true)
		{
			/*count++;
			ofstream file;
			if(!file.is_open())
			{
				file.open("heizhang.txt",ios::app);
			}
			file<<count<<" : ("<<m_tacObst[i].x<<","<<m_tacObst[i].y<<")"<<endl;
			file.close();*/
			j=(m_tacObst[i].x)/50;
			k=m_tacObst[i].y/50;
			if(j>=18&&j<36&&k>=0&&k<24){//筛选障碍物
				map[j][k]=1.0;
			}
		}
	}
	for(i=18,n=0;i<36;i++){
		for(j=0;j<24;j++){
			if(map[i][j]==1.0){
				zhangai[n][0]=i;//i标记障碍物
				zhangai[n][1]=j;
				n++;
			}
		}
	}

	for(i=18;i<36;i++)
	{
		for(j=0;j<24;j++)
		{
			for(k=0;k<n;k++)
			{
				int length_dian_zai=((double)i-(double)zhangai[k][0])*((double)i-(double)zhangai[k][0])+((double)j-(double)zhangai[k][1])*((double)j-(double)zhangai[k][1]);
				if((length_dian_zai!=0)&&(zhangai[k][0]>=18)){
					map[i][j]+=1.0/length_dian_zai;
				}
			}//地图点赋值
		}
	}
	for(i=18,min=99999999.0;i<36;i++){//18!!!!!9!!!!
		for(j=12;j>=0;j--){
			if((i<27)&&(i>22)&&(j-3>=0)&&(j+3<24)){
				double a=map[i][j-2]+map[i][j-1]+map[i][j]+map[i][j+1]+map[i][j+2]
					+map[i-1][j-2]+map[i-1][j-1]+map[i-1][j]+map[i-1][j+1]+map[i-1][j+2]
					+map[i+1][j-2]+map[i+1][j-1]+map[i+1][j]+map[i+1][j+1]+map[i+1][j+2];
				if(a<min){
					kongx=i*50;
					kongy=j*50;
					min=a;
				}
			}
		}
		for(j=13;j<24;j++){
			if((i<27)&&(i>22)&&(j-3>=0)&&(j+3<24)){
				double a=map[i][j-2]+map[i][j-1]+map[i][j]+map[i][j+1]+map[i][j+2]
					+map[i-1][j-2]+map[i-1][j-1]+map[i-1][j]+map[i-1][j+1]+map[i-1][j+2]
					+map[i+1][j-2]+map[i+1][j-1]+map[i+1][j]+map[i+1][j+1]+map[i+1][j+2];
				if(a<min){
					kongx=i*50;
					kongy=j*50;
					min=a;
				}
			}
		}
	}
}
void CUPFullField::BestPassPoint(int *EvaluteX, int *EvaluteY, int *CanPassBall)
{
	int ExistFlag = 0, PassFlag = 1;
		int mbf = 0;
	ExistFlag = 0;
	PassFlag = 1;
	int nNumOfObst = EACH_AG_OBST_NUM*(AG_NUM+1);
	evalutepoint EvalutePoint[33];
	if(*CanPassBall==-100)
	{ 
		mbf=1;
	}
	//初始化接球点
	/*for (int i = 0; i < 11; i++)
		for (int j = 0; j < 3; j++)
		{
			if(mbf==1)
			{
				EvalutePoint[j * 11 + i].x = j * 100 + 1170;
				EvalutePoint[j * 11 + i].y = (i + 1) * 100;
				EvalutePoint[j * 11 + i].valid = true;
			}
			else
			{
				EvalutePoint[j * 11 + i].x = j * 100 + 1100;
				EvalutePoint[j * 11 + i].y = (i + 10) * 40;
				EvalutePoint[j * 11 + i].valid = true;
			}
			
		}*/

		//随机选取坐标点
		if (m_iEvaluteFrame == 0)
		{
			
			/*int temp = rand() % 33;
			*EvaluteX = EvalutePoint[temp].x;
			*EvaluteY = EvalutePoint[temp].y;
			m_iLastX = *EvaluteX;
			m_iLastY = *EvaluteY;*/
			kong();
			*EvaluteX = kongx;
			*EvaluteY = kongy;
			m_iLastX = *EvaluteX;
			m_iLastY = *EvaluteY;

		}
		else
		{
			*EvaluteX = m_iLastX;
			*EvaluteY = m_iLastY;
		}

		//统计帧数增加
		m_iEvaluteFrame++;
		
		
		//每300帧换一个点
		if (m_iEvaluteFrame == 300 && mbf==0)
			m_iEvaluteFrame = 0;
		else if (m_iEvaluteFrame == 50 && mbf==1)
		{
			m_iEvaluteFrame=0;
		}

		if (m_eStage==sTechChallenge2012)
		{
			*EvaluteX = 800;
			
			*EvaluteY = 600;
		}

		//计算黑色障碍到两点连线的距离
		for (int i = 0; i < nNumOfObst; i++)
		{
			if (m_tacObst[i].valid == true)
			{
				if ((m_trueball.x-m_tacObst[i].x)*(m_trueball.x-m_tacObst[i].x)+(m_trueball.y-m_tacObst[i].y)*(m_trueball.y-m_tacObst[i].y)<100*100)
				{
					//PassFlag = 0;
					
					continue;   //2013.9.13_xj
				}
				if(m_tacObst[i].x>900)
				{
					continue;
				}
				double DisMolecular = (*EvaluteY - m_trueball.y) * m_tacObst[i].x + (m_trueball.x - *EvaluteX) * m_tacObst[i].y
					+ (*EvaluteX * m_trueball.y - *EvaluteY * m_trueball.x);
				double DisDenominator = sqrt((double)((*EvaluteY - m_trueball.y) * (*EvaluteY - m_trueball.y) + (m_trueball.x - *EvaluteX) *
					(m_trueball.x - *EvaluteX)));
				double TempDis = DisMolecular / DisDenominator;
				if (TempDis < 45)
				{ 
					PassFlag = 0;
					break;
					//continue;   2013.9.13_xj
				}
			}
		}
		//PassFlag =1;
		if (PassFlag == 1 )
			*CanPassBall = *CanPassBall + 5;
		int tbb=8;
		while (PassFlag!=1 && tbb-->0 && mbf==0)
		{
			/*int temp = rand() % 33;
			*EvaluteX = EvalutePoint[temp].x;
			*EvaluteY = EvalutePoint[temp].y;
			m_iLastX = *EvaluteX;
			m_iLastY = *EvaluteY;*/
			kong();
			*EvaluteX = kongx;
			*EvaluteY = kongy;
			m_iLastX = *EvaluteX;
			m_iLastY = *EvaluteY;
		}
		//*CanPassBall = 10;
		
		////计算接球者是否到接球点
		for (int i=1;i<7;i++)
		{
		if (m_tacagent[i].online == true 
				&& m_tacagent[i].status == CTRL_MID_CATCH && m_tacagent[i].status != CTRL_MID_PASS 
					&&  m_CalDist(*EvaluteX, *EvaluteY, m_tacagent[i].x, m_tacagent[i].y) < 50.0)
			{
					CString info;
					info.Format("fuckfuck点%d,%d,%d,%d,%d\n",*EvaluteX, *EvaluteY ,m_tacagent[i].x,m_tacagent[i].y,PassFlag );
					printf(info);
					ExistFlag = 1;
					break;
			}
		}

		if (ExistFlag == 1)
		{
			*CanPassBall = *CanPassBall + 5;
		}
	//	CString info;
	//	info.Format("接球点%d,%d\n",*EvaluteX, *EvaluteY);
	//	printf(info);
		//printf();

}

void CUPFullField::AChangeMidPassStatus( int inID, int ctrl, int mid_pass_x, int mid_pass_y, int mid_pass_believe,int mid_pass_jiao )
{ 
	//throw std::exception("The method or operation is not implemented.");
	if (inID<0 || inID>6)
	{
		return;
	}

	if (pToAgent[inID] != NULL)
	{
		pToAgent[inID]->CtrlMidPassCmd(ctrl,mid_pass_x,mid_pass_y,mid_pass_believe,mid_pass_jiao);
		m_tacagent[inID].allocated = true;
	}	

	//Display
	m_ShowTacInfo(ctrl,inID);
}

int CUPFullField::TheBestMidCatch( uagent *inList ,int x,int y )
{

	if (m_eStage==sTechChallenge2012)
	{
		return m_nMidCatchPlayer;
	}
	//throw std::exception("The method or operation is not implemented.");
	if (inList == NULL)
	{
		return NULL;
	}
	

	int best = 0;
	best=m_Ag_GetNearBy(x,y,m_tacagent);
	CString ip;
	ip.Format("选择接球队员%d",best);
	printf(ip);
	return best;
}

void CUPFullField::Tac_MidPass_kicked( int midCatchPlayer, int x, int y ,int believe )
{
	
	//初始化控制结构体，按角色关键程度排列
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;

	if (believe>=100)
	{
		m_tacctrl[2].ctrl = CTRL_ATTACK;
		m_tacctrl[3].ctrl = CTRL_STOP;
	} 
	else
	{
		//继续传球者角色
		m_tacctrl[2].ctrl = CTRL_MID_CATCH;
		//m_tacctrl[2].ctrl = CTRL_ATTACK;
		final_mid_pass_x = x;
		final_mid_pass_y = y;
		final_mid_pass_believe = believe;

		//3~5号队员跑位切换到定角度防守态势
		for (int i=3;i<6;i++)
		{
			//m_tacctrl[i].ctrl = CTRL_DEFENCE;
			m_tacctrl[i].ctrl = CTRL_STOP;
			m_tacctrl[i].def_dist = (int)(300*m_fKField);
		}
		m_tacctrl[3].ctrl = CTRL_MOVETO;
		m_tacctrl[3].target_x = m_trueball.x;
		m_tacctrl[3].target_y =1200 - m_trueball.y;

		CPoint tmpball = m_trueball;

		//下面将场地分为上中下三个区域，根据球所处区域形成防守阵型
		if (tmpball.y > 900)
		{
			//上侧
			//先在球到球门之间堵上一个
			m_tacctrl[4].def_angle = 0;

			//后卫
			m_tacctrl[5].ctrl = CTRL_MOVETO;
			m_tacctrl[5].target_x = m_Pos_Back.x;
			m_tacctrl[5].target_y = m_Pos_Back.y+300;

		}
		else
		{
			if (tmpball.y > 300)
			{
				// 300<tmpball.y < 900 中间区域
				//先在球到球门之间堵上一个
				m_tacctrl[4].def_angle = 0;
				m_Fix_DefOutOffFrame(&(m_tacctrl[4]),0);
				m_Fix_DefInOurPenaltyArea(&(m_tacctrl[4]));

				//后卫
				m_tacctrl[5].ctrl = CTRL_MOVETO;
				m_tacctrl[5].target_x = m_Pos_Back.x;
				m_tacctrl[5].target_y = m_Pos_Back.y;
			}
			else
			{
				//tempball.y < 300 下半区
				//先在球到球门之间堵上一个
				m_tacctrl[4].def_angle = 0;

				//后卫
				m_tacctrl[5].ctrl = CTRL_MOVETO;
				m_tacctrl[5].target_x = m_Pos_Back.x;
				m_tacctrl[5].target_y = m_Pos_Back.y-300;
			}
		}
	}

	

	m_TacAction(m_tacagent);
}

int CUPFullField::Cal_Angle_Mid_Catch()
{
	//throw std::exception("The method or operation is not implemented.");
	//m_find.jizuobiao(net.rec_midpassx,net.rec_midpassy,&dingweijiao,&dingweichang,y.benjix,y.benjiy);
	//dingweijiao=-jiajiao(-dingweijiao,y.benjijiao-90);
	//jizuobiao(double x,double y,double *jiao,double *chang,int zhongxinx,int zhongxiny)
	
	double chang,jiao;
	double x = m_tacagent[m_nMidPassPlayer].x;
	double y=m_tacagent[m_nMidPassPlayer].y;
	int zhongxinx =m_tacagent[m_nMidCatchPlayer].x ;
	int zhongxiny=m_tacagent[m_nMidCatchPlayer].y;
	double x1;
	double y1;
	if(( x!=zhongxinx) || (y!=zhongxiny))
	{
		x1=double(x-zhongxinx);
		y1=double(y-zhongxiny);
		chang=sqrt(x1*x1+y1*y1);
		if(x1>0)
			jiao=(90-asin(y1/ chang)*57.29578);
		else
			jiao=-(90-asin(y1/ chang)*57.29578);
	}
	else
	{
		chang=0;
		jiao=0;
	}
	return -jiajiao(-jiao,m_tacagent[m_nMidCatchPlayer].angle);
}

int CUPFullField::jiajiao( int jizhun, int jiao )
{
	//throw std::exception("The method or operation is not implemented.");
	double jiajiao;
	jiajiao=jiao-jizhun;
	if (jiajiao>180)
		jiajiao=-360+jiajiao;
	if(jiajiao<-180)
		jiajiao=360+jiajiao;
	return jiajiao;
}

bool CUPFullField::GetStart()
{
	for(int i=2;i<=4;i++)
	{
		if(!this->m_agents[i].online)
		{
			return false;
		}
	}
	m_tacctrl[2].ctrl = CTRL_MOVETO;
	m_tacctrl[2].def_angle = 0;

		
		//m_tacctrl[5].ctrl = CTRL_MOVETO;
	m_tacctrl[2].target_x = 1450;
	m_tacctrl[2].target_y = 300;

	m_tacctrl[3].ctrl = CTRL_MOVETO;
	m_tacctrl[3].def_angle = 0;

		
		//m_tacctrl[5].ctrl = CTRL_MOVETO;
	m_tacctrl[3].target_x = 1450;
	m_tacctrl[3].target_y = 900;

	m_tacctrl[4].ctrl = CTRL_MOVETO;
	m_tacctrl[4].def_angle = 0;

		
		//m_tacctrl[5].ctrl = CTRL_MOVETO;
	m_tacctrl[4].target_x = 270;
	m_tacctrl[4].target_y = 600;

	if (m_CalDist(m_agents[2].x,m_agents[2].y,1450,300)<50&&
		m_CalDist(m_agents[3].x,m_agents[3].y,1450,900)<50&&
		 m_CalDist(m_agents[4].x,m_agents[4].y,270,600)<50)
	{
		return true;
	}
	return false;
}

void CUPFullField::GoPass( int passNum, int catchNum, int direction)
{
	CString info;
	m_nMidPassPlayer = passNum;
	m_nMidCatchPlayer = catchNum;
	
	if ((m_bTaskMidPassCatch==true) &&( m_bTaskMidPassCatch_kick==true) && m_bSomeoneHoldBall()==true)//[yzz]对于接球者 应该回去接球
	{
		m_bTaskMidPassCatch=false;
		m_bTaskMidPassCatch_kick=false;
		CString info;
		info.Format("中场传球结束\n");
		printf(info);
	}
	else if ((m_bTaskMidPassCatch==true) &&( m_bTaskMidPassCatch_kick==true))
	{
		//如果传球动作成功，己方半场的球员不要来抢球,接球者继续 等待接球，不设进攻者,其余球员跑位， 当believe大于10 的时候，继续等待接球，>20（应计算平均接球时间，20为估计值）时，自主决策
		if (m_nMidPassCatchBelieve_last >= 10 )//(m_nMidPassCatchBelieve_last<20) mc
		{
			info.Format("中场传球已经起脚!!!!%d\n",m_nMidPassCatchBelieve_last);
			printf(info);
			//Tac_MidPass_kicked(m_nMidCatchPlayer,m_posFinalMidPass.x,m_posFinalMidPass.y,++m_nMidPassCatchBelieve_last);
			final_mid_pass_x = m_posBestMidPass_last.x;
			final_mid_pass_y = m_posBestMidPass_last.y;
			final_mid_pass_believe = m_nMidPassCatchBelieve_last++;
			//m_tacctrl[passNum].ctrl = CTRL_MID_PASS;	
			m_tacctrl[catchNum].ctrl = CTRL_MID_CATCH;
			return;
		}
		else
		{
			
		}
	}
	else if ((m_bTaskMidPassCatch==true) &&( m_bTaskMidPassCatch_kick==false))
	{
		//mc 2012.8.5
		//if (m_bSomeoneHoldBall()==false)
		//{
		//	info.Format("m_bSomeoneHoldBall()=false\n");
		//	printf(info);
		//	//传球过程中球被断了
		//	m_bTaskMidPassCatch = false;
		//}
		//else
		{

		//CString info;
		info.Format("中场传球启动\n");
		printf(info);

		int believe = 0,px=0,py=0;
		BestPassPointTechChallenge2012(&px,&py,&believe, direction);//believe默认为0，>=10 开始准备起脚传球
		//px = 800;						//zhao chuanqiu dian 
		//py = 600;

		if (direction == 0)
		//{
		//	px = 1100;
		//	py = 600;
		//}
		//else if (direction == 1)
		//{
		//	px = 700;
		//	py = 600;
		//}
		info.Format("believe = %d\n",believe);
		printf(info);
		m_posBestMidPass_last.x = px;
		m_posBestMidPass_last.y = py;
		m_nMidPassCatchBelieve_last = believe;

		//m_bTaskMidPassCatch = true;

		
		//进攻角色
		m_tacctrl[passNum].ctrl = CTRL_MID_PASS;	
		m_tacctrl[catchNum].ctrl = CTRL_MID_CATCH;

		final_mid_pass_x = px;//真正发送给单体的传球点
		final_mid_pass_y = py;
		final_mid_pass_believe = believe;

		info.Format("px = %d py = %d\n",px, py);
		printf(info);

		return;
		}
	} 
}

void CUPFullField::BestPassPointTechChallenge2012(int *EvaluteX, int *EvaluteY, int *CanPassBall, int direction)
{
	int ExistFlag = 0, PassFlag = 1;
	int nNumOfObst = EACH_AG_OBST_NUM*(AG_NUM+1);
	evalutepoint FrontFieldPoint[33];
	evalutepoint BackFieldPoint[33];

	if (direction == 1)
	{
		//初始化接球点
		for (int i = 0; i < 11; i++)
			for (int j = 0; j < 3; j++)
			{
				FrontFieldPoint[j * 11 + i].x = j * 100 + 1100;
				FrontFieldPoint[j * 11 + i].y = (i + 1) * 100;
				FrontFieldPoint[j * 11 + i].valid = true;
			}

			//随机选取坐标点
			if (m_iEvaluteFrame == 0)
			{
			srand((unsigned)time(NULL));
			int temp = rand() % 33;
			*EvaluteX = FrontFieldPoint[temp].x;
			*EvaluteY = FrontFieldPoint[temp].y;
			m_iLastX = *EvaluteX;
			m_iLastY = *EvaluteY;
			}
		
	}
	else if (direction == 0)
	{
		for (int i = 0; i < 11; i++)
			for (int j = 0; j < 3; j++)
			{
				BackFieldPoint[j * 11 + i].x = 800 - j * 100;
				BackFieldPoint[j * 11 + i].y = (i + 1) * 100;
				BackFieldPoint[j * 11 + i].valid = true;
			}
			
			//随机选取坐标点
			if (m_iEvaluteFrame == 0)
			{
			srand((unsigned)time(NULL));
			int temp = rand() % 33;
			*EvaluteX = BackFieldPoint[temp].x;
			*EvaluteY = BackFieldPoint[temp].y;
			m_iLastX = *EvaluteX;
			m_iLastY = *EvaluteY;
			}
	}
		
		if (m_iEvaluteFrame != 0)
		{
			*EvaluteX = m_iLastX;
			*EvaluteY = m_iLastY;
		}

		//统计帧数增加
		m_iEvaluteFrame++;

		//每300帧换一个点
		if (m_iEvaluteFrame == 300)
			m_iEvaluteFrame = 0;

		/*if (m_eStage==sTechChallenge2012)
		{
			*EvaluteX = 800;

			*EvaluteY = 600;
		}*/

		if (direction == 0)
		{
			*EvaluteX = 1400;
			*EvaluteY = 800;
			
				while(m_CalDist(*EvaluteX, *EvaluteY , m_tacagent[2].x, m_tacagent[2].y) <= 150 ){
							if(*EvaluteY<600)
								*EvaluteY+=300;
							else
								*EvaluteY-=300;
						}
				for (int i=0;i<EACH_AG_OBST_NUM*(AG_NUM+1);i++)
			{
					if (m_tacObst[i].valid == true)
					{
						while(m_CalDist(*EvaluteX, *EvaluteY, m_tacObst[i].x, m_tacObst[i].y) <= 150 ){
							*EvaluteY += 100;
						}
					}
			}
		}
		else if (direction == 1)
		{
			*EvaluteX = 500;
			*EvaluteY = 600;
			for (int i=0;i<EACH_AG_OBST_NUM*(AG_NUM+1);i++)
			{
					if (m_tacObst[i].valid == true)
					{
						while(m_CalDist(*EvaluteX, *EvaluteY, m_tacObst[i].x, m_tacObst[i].y) <= 150 ){
							*EvaluteY += 100;
						}
					}
			}

		}

		//计算黑色障碍到两点连线的距离
		/*for (int i = 0; i < nNumOfObst; i++)
		{
			if (m_tacObst[i].valid == true)
			{
				double DisMolecular = (*EvaluteY - m_trueball.y) * m_tacObst[i].x + (m_trueball.x - *EvaluteX) * m_tacObst[i].y
					+ (*EvaluteX * m_trueball.y - *EvaluteY * m_trueball.x);
				double DisDenominator = sqrt((double)((*EvaluteY - m_trueball.y) * (*EvaluteY - m_trueball.y) + (m_trueball.x - *EvaluteX) *
					(m_trueball.x - *EvaluteX)));
				double TempDis = DisMolecular / DisDenominator;
				if (TempDis < 150)
				{
					PassFlag = 0;
					break;
				}
			}
		}*/

		if (PassFlag == 1)
			*CanPassBall = *CanPassBall + 10;


		//计算接球者是否到接球点
		for (int i=1;i<7;i++)
		{
			//printf("%d\n", m_tacagent[i].allocated);
			if (
				 m_tacagent[i].status == CTRL_MID_CATCH
				&&  m_CalDist(*EvaluteX, *EvaluteY, m_tacagent[i].x, m_tacagent[i].y) <= 200.0)
			{
				ExistFlag = 1;
				break;
			}
		}

		if (ExistFlag == 1)
		{
			*CanPassBall = *CanPassBall + 5;
		}
		CString info;
		info.Format("%d     %d\n", PassFlag,ExistFlag);
		printf(info);
		//printf();
}

void CUPFullField::midcatchmode()
{
	m_eStage = midcatch;
}

void CUPFullField::Attention_DEBUG( unsigned char inBuf, int PlayerX, int PlayerY, int TargetX, int TargetY )
{
	//throw std::exception("The method or operation is not implemented.");
	int nn = rand()%10000;
	if (nn>50)
	{
		return;
	}
	/*CString info;
	info.Format("是否持到球%d;电量为%d\n", PlayerX,PlayerY);
	printf(info);*/
}
CPoint CUPFullField::SelectCatchPoint(CPoint Pass_Point)
{
	CPoint BestCatchPoint;




	return BestCatchPoint;
}
int CUPFullField::SelectBestCatcher(CPoint Destination)
{
	CString info;
	
	int ID=0;
	CPoint PosCatchp;
	int TempDistance=9999999;
	int Top=999999;
	for (int i=2;i<6;i++)
	{
		TempDistance = (m_agents[i].x - Destination.x)*(m_agents[i].x - Destination.x) + (m_agents[i].y - Destination.y)*(m_agents[i].y - Destination.y);
		if (TempDistance < Top && i!= m_n3mPassPlayer)
		{
			Top=TempDistance;
			ID=i;
		}
	}
	info.Format(" destination: %d,%d\n and id: %d",Destination.x,Destination.y,ID);
	printf(info);
	return ID;
}
bool CUPFullField::IsBallRollingToOwnGoal(CPoint memBallPos)
{
	if (memBallPos.x > m_trueball.x)
	{
		return true;

	}
	else
	{
		return false;
	}
}


