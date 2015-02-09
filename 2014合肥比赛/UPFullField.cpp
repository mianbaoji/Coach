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
	m_bFoundball = false;	//��ʼ��Ϊδ������

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
	m_strCurSt.Format("������δ��ʼ");

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

	//��ת
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

	//������
	m_bTaskPassCatch = false;
	m_bTaskPassCatch_kick = false;
	m_nCatchPlayer = 0;
	m_nPassPlayer = 0;

	//mid������
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

	//���Ƴ��߲ɼ���
// 	if (m_px != NULL && m_py != NULL)
// 	{
// 		for (int i=0;i<m_pcnt;i++)
// 		{	
// 			dcMemory.SetPixel(m_Point_Field2Img(m_px[i],m_py[i]),RGB(255,255,0));
// 		}
// 	}
	//����PlayerIn�ϳ���λ��	
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
		dcMemory.TextOut(380,320,"�����볡��");
	}
	dcMemory.SelectObject(oldfont);

	//�ϰ���
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

	//����Agnet
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
		
		//����·��
		if ((m_agents[i].x != m_agents[i].moveto_x || m_agents[i].y != m_agents[i].moveto_y) && m_agents[i].status == CTRL_MOVETO )
		{
			oldpen = dcMemory.SelectObject(&path);
			CPoint moveto = m_Point_Field2Img(m_agents[i].moveto_x,m_agents[i].moveto_y);
			dcMemory.MoveTo(coodinate);
			dcMemory.LineTo(moveto);
			dcMemory.SelectObject(oldpen);
		}

		//����ǵ�ѡĿ�꣬����Χ����Ϊ��ɫ
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
		
		//���ݷ�ת������㳯��Ƕ�
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

		//���ݷ�ת��������ٶȷ���
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

		//��ʾ״̬
		aNum = GetCTRLString(m_agents[i].status);
		oldfont = dcMemory.SelectObject(&txtfont);
		dcMemory.SetTextColor(RGB(255,255,255));
// 		if (aNum == "����")
// 		{
// 			dcMemory.SetTextColor(RGB(200,0,0));
// 		}
		dcMemory.TextOut(coodinate.x-10,coodinate.y-20,aNum);
		dcMemory.SelectObject(oldfont);

		//test ��ʾ����Ч��
// 		dcMemory.SetTextColor(RGB(255,255,0));
// 		aNum.Format("%d",m_Perform_ATK(&m_agents[i]));
// 		dcMemory.TextOut(coodinate.x+10,coodinate.y,aNum);
	}

	//���ƴ���·��
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
	//�г�����·��[yzz]
	if (m_bTaskMidPassCatch == true)
	{
		oldpen = dcMemory.SelectObject(&passPath);
		coodinate = m_Point_Field2Img(m_agents[m_nMidPassPlayer].x,m_agents[m_nMidPassPlayer].y);
		direct = m_Point_Field2Img(m_agents[m_nMidCatchPlayer].x,m_agents[m_nMidCatchPlayer].y);

		dcMemory.MoveTo(coodinate);
		dcMemory.LineTo(direct);

		dcMemory.SelectObject(oldpen);
	}


	//����Ball	
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
	if (m_strCurSt.FindOneOf("��") == 0)
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
// 		aNum.Format("%d��%d��δ�ƶ�",m_bakAtkAgent.ID,m_nAtkNotMove);
// 		dcMemory.SetTextColor(RGB(200,0,0));
// 		dcMemory.TextOut(200,320,aNum);
// 	}

	//refbox
	if (bCntRefbox == true)
	{
		dcMemory.SetTextColor(RGB(0,0,200));
		dcMemory.TextOut(200,320,"���к�������");
	}
	else
	{
		dcMemory.SetTextColor(RGB(250,0,0));
		dcMemory.TextOut(170,320,"ע�⣺δ���Ӳ��кУ�");
	}

	//������
	if (m_bTaskPassCatch == true)
	{	
		dcMemory.SetTextColor(RGB(255,255,0));
		CString strTask;
		if (m_bTaskPassCatch_kick == false)
		{
			if (m_nPassPlayer == 0 || m_nCatchPlayer == 0)
			{
				strTask.Format("��������������");
			}
			else
			{
				strTask.Format("%d����%d�Ŵ���",m_nPassPlayer,m_nCatchPlayer);
			}
		} 
		else
		{
			strTask.Format("���Ѵ�����");
		}
		dcMemory.TextOut(320,10,strTask);
	} 
	//mid����
	if (m_bTaskMidPassCatch == true)//[yzz]
	{	
		dcMemory.SetTextColor(RGB(255,255,0));
		CString strTask;
		if (m_bTaskMidPassCatch_kick == false)
		{
			if (m_nMidPassPlayer == 0 || m_nMidCatchPlayer == 0)
			{
				strTask.Format("mid��������������");
			}
			else
			{
				strTask.Format("%d����%d�Ŵ���",m_nMidPassPlayer,m_nMidCatchPlayer);
			}
		} 
		else
		{
			strTask.Format("���Ѵ�����");
		}
		dcMemory.TextOut(320,10,strTask);
	} 

	//�볡ʱ��ͳ��
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
				aNum.Format("%d�� ",i);
				strTimeInfo += aNum;
				bSomeInfoToDisp = true;
			}
		}

		aNum.Format("����%d���볡��",m_nTimeToBeIn);
		strTimeInfo += aNum;
	}

	for (i=0;i<AG_NUM;i++)
	{
		if (m_prRightOfPlayer[i] == PLAYER_RIGHT_WAITFOEPOINT)
		{
			aNum.Format("%d�� �ƶ���ƥ��㣻",i);
			strTimeInfo += aNum;
			bSomeInfoToDisp = true;
		}

		if (m_prRightOfPlayer[i] == PLAYER_RIGHT_READY)
		{
			aNum.Format("%d�� �ȴ��볡ʱ����",i);
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
		//�������ϵĶ�Ա����ƥ���λ��
		//pToAgent[inID]->SendInitPos(m_fpPlayerIn.x,m_fpPlayerIn.y,0);
		pToAgent[inID]->SetInitPos(m_fpPlayerIn.x,m_fpPlayerIn.y,0);
		//�����ߵĻ�����һ��ָֹͣ��
		AChangeStatus(inID,CTRL_STOP,0);
		m_agents[inID].online = true;
	}
	m_recvcnt[inID] ++;
	m_asc.Unlock();
	
	//�ж��Ƿ�WaitForPoint
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
//DEL 	//��һ��ֵ
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
		//����ƥ����ʼ��
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
		//�жϵ�ѡ��Χ
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

		//�����ƶ�Ŀ���
		if (SelID != 0)
		{
			//�ƶ�
			if (m_eStage == sStop)
			{
				ManiMode();
			}
			/*/////////////////////////////////////////////////////////////////////////
			int tx,ty;
			tx = (point.x-48)*18/4;
			ty = (305-point.y)*1200/266;
			AMoveTo(SelID,tx,ty,(int)m_SelAng);
			//ֱ�Ӹ�����ʾ���ƶ�Ŀ���
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
			//ֱ�Ӹ�����ʾ���ƶ�Ŀ���
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

	//����ָ������
	if (pToAgent[ID] != NULL)
	{
		//����������ƶ���С��������Χ������stopָ��
// 		if (
// 			m_CalDist(m_agents[ID].x,m_agents[ID].y,inx,iny)<MOVETO_DEADZONE && 
// 			(int)abs((int)(m_agents[ID].angle) - inAngle) < MOVETO_ANGLE_DEADZONE
// 			)
// 		{
// 			//���ڵ�����������STOP
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
		//����ö�Ա֮ǰ���ڽ���״̬�������л���״̬
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
// 			//����������ƶ���С��������Χ������stopָ��
// 			if (
// 				m_CalDist(m_agents[ID].x,m_agents[ID].y,inx,iny)<MOVETO_DEADZONE && 
// 				(int)abs((int)(m_agents[ID].angle) - inAngle) < MOVETO_ANGLE_DEADZONE
// 				)
// 			{
// 				//���ڵ�����������STOP
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
		//��Ǹ�agent�Ѿ������ɫ
		m_tacagent[ID].allocated = true;
	}

	//������ʾ
	m_agents[ID].moveto_x = inx;
	m_agents[ID].moveto_y = iny;
}

void CUPFullField::Exec()
{
	
	m_EliminateFalseBall();	//����Ϣ�ں�
	
	//������Ĳο�λ����Ϣ
	int i;
	for(i=0;i<7;i++)
	{
		pToAgent[i]->SetRefBall(m_trueball.x,m_trueball.y);
	}

	GetCurTime();			//��ȡϵͳʱ��

	m_chkagnt ++;			//����������ֵ����ֵ����agents���ص����ݰ�
	if (m_chkagnt > 10)		//ÿִ��10��exec����һ��agents�������ݰ�ͳ��
	{
		m_CheckAgentReply();
		m_chkagnt = 0;
	}

	
	//m_agents�����ȶ��ˣ������Ǹ��Ƶ�m_tacagent,����ֱ�Ӷ�m_tacagent���в���
	m_asc.Lock();
	for (i=1;i<7;i++)
	{
		if (m_agents[i].online == true && m_prRightOfPlayer[i] == PLAYER_RIGHT_INFIELD && m_agents[i].status != ROBST_ERR)
		{
			memcpy(&m_tacagent[i],&m_agents[i],sizeof(uagent));
		}
		else
		{
			//���ߡ����������⣬��agent������Ϊ������,���������Ǹ���һ��
			memcpy(&m_tacagent[i],&m_agents[i],sizeof(uagent));
			m_tacagent[i].online = false;

			if (m_prRightOfPlayer[i] == PLAYER_RIGHT_OUT)
			{
				//�����볡�ģ�����������
				AChangeStatus(i,CTRL_IDLE,0);
			}
		}
	}
	m_asc.Unlock();

	//�ϰ�����Ϣ
	m_ObstFusion();
	m_EliminateFalseObst(); //�ϰ�����Ϣ�ں�,ѡ������ϰ���[zbw]
	//////////////////////////////////////////////////////////////////////////
	//Ϊ����Ա�ַ���������Ϣ
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
// 			// 			str.Format("dbg ��%d�Ż�����%d���ϰ�����Ϣ",i,tmpNum);
// 			// 			printf(str);
// 			//////////////////////////////////////////////////////////////////////////
// 		}
// 	}
	//////////////////////////////////////////////////////////////////////////
	
	//�ڶ�Ա�������Ϣ
	//m_TeammateShare();
	//////////////////////////////////////////////////////////////////////////

	NodeInput* behaviorTreeInput;
	behaviorTreeInput = new NodeInput(pToAgent, m_agents, m_tacagent);
	if (m_eStage == sMani)
	{
		//�ֶ�����ģʽ
	}
	if (m_eStage==midcatch)
	{
		AChangeMidPassStatus(midcatch_n,CTRL_MID_CATCH,midcatch_x,midcatch_y,midcatch_b,0);
	}
	if (m_eStage == sEnter)
	{
		//�볡
		Enter();
	}

	if (m_eStage == sOut)
	{
		Leave();
	}

	if (m_eStage == sAUTO)
	{
		//�������״̬
		Tac_Default();
	}

	if (m_eStage == sPenaltyReady)
	{
		//Ԥ������
		Ready_Penalty();
	}
	
	if (m_eStage == sStart)
	{
		//��ʱ��һ���ж�Ա�л�������״̬�����̿�ʼ��������
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
		//	printf("δʵ�֣�");
			break;

		default:
			break;
		}

		//��ִ�е�����������ҷ�����Ӧ���ǶԷ���������Ĳ����Ǳ�֤������һ���۲���Ķ�Ա
		//�Է�����Start�������ҷ���Ա��δ��λ��������һЩ���ֲ��У�
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
	/*                     2010���ڼ�����ս��                               */
	/************************************************************************/
	if (m_eStage == sTechChallenge)
	{
		//Ѱ���ʺϵĽ�����Ա
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
	/*                     2012���ڼ�����ս��                               */
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
			if (this->GetStart()==true )//���û�п�����
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
					m_tacctrl[2].ctrl=CTRL_ATTACK;//4�Ż�Ӧ�ñ�����
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
		//�������
		
	}
	
	m_ShowAgentsInfo();

	//debug
// 	CString st;����
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
		//��Ǹ�agent�Ѿ������ɫ
		m_tacagent[inID].allocated = true;
	}

// 	if (inCtrlCode == CTRL_SEARCHBALL)
// 	{
// 		CString str;
// 		str.Format("ID: %d SERCHBALL",inID);
// 		printf(str);
// 	}
// 	CString str;
// 	str.Format("%d�Ż�  ��Ϊ%d",inID,inCtrlCode);
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
	//���������Ա���ص�����Ϣ
	m_bFoundball = false;
	m_asc.Lock();
	m_bsc.Lock();
	for (int i=1;i<7;i++)
	{
		if (m_agents[i].online == true && m_balls[i].updated == true)
		{	
			//��̫���Եĳ��������ȥ��
			if (m_balls[i].x < -150 || m_balls[i].x > 1950 || m_balls[i].y < -150 || m_balls[i].y > 1350)
			{
				m_agents[i].foundball = false;
				m_balls[i].updated = false;
				continue;
			}

			//��������߶�Ա����ľ���
			m_agents[i].dist_ball = (m_agents[i].x - m_balls[i].x)*(m_agents[i].x - m_balls[i].x) + (m_agents[i].y - m_balls[i].y)*(m_agents[i].y - m_balls[i].y);
			//m_agents[i].dist_ball = sqrt(m_agents[i].dist_ball);//yz 10.19
			//�����ߵĶ�Ա�����˸����������꣬�������б�
			

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
		return;	//��û������û��˵�ˡ�����
	}
	
	//Ѱ����۲����������������Ϊ��������
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
			//�жϸö�Ա��������Ƿ���С
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

	//����ÿ������������ľ���
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

//����ִ�в��֣���̬��ɫ�������������ʵ��
void CUPFullField::m_TacAction(uagent* inlist)
{
	//////////////////////////////////////////////////////////////////////////
	//09�� ����Ч�ÿ��˵Ľ�ɫ���䣺�ɸߵ������ȼ�������ɫ���У�����Ϊÿ����ɫ��ɫ��ѵĶ�Ա��ֱ�����ж�Ա�������
	bool bNoOneToAlloc = false;
	int tempdistance;
	//�Ƚ�������Ա�Ľ�ɫ���������
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
	//��ʼ�����ɫ
	int bestID;
	bestID = 0;

	//������Ϊ�õ���
	CPoint tmp;
	CPoint tmpball;
	int tmpangle;
	bool bflip = false;
	int tmpDist;
	int nNoLoop;

	//������
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
	//if (m_bTaskMidPassCatch ==true && m_bTaskMidPassCatch_kick==true)//[yzz]test ����ͣ�����ߵ��ж���֮���Ϊ�������أ����ӳٴ���ͽ����ߵ���Ϊ
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
			
		/*����Ա����ѡ*/
		case CTRL_GOALKEEP:
			//1����Ա����
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
				//1����Ա���ڣ�������������Ķ�Ա��������Ա(�ݲ�ʵ�֣��ÿ���1����ԱͨѶ��ʱ�жϵ�����)
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
				//�Ѿ�û����Ա�ɷ�����
				bNoOneToAlloc = true;
				break;
			}
			m_nMidPassPlayer = bestID;
			//CString info;
			info.Format("�����г������ɫid=%d \n",bestID);
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
				//�Ѿ�û����Ա�ɷ�����
				bNoOneToAlloc = true;
				info.Format("û�ж�Ա���Է���\n",bestID);
				printf(info);
				break;
			}
			m_nMidCatchPlayer = bestID;
			
			info.Format("�����г������ɫid=%d \n",bestID);
			printf(info);
			m_memstatus[bestID] = CTRL_MID_CATCH;
			AChangeMidPassStatus(bestID,CTRL_MID_CATCH,final_mid_pass_x,final_mid_pass_y,final_mid_pass_believe,0/*Cal_Angle_Mid_Catch()*/);
			break;
		case CTRL_3M_PASS:
		
			bestID = 0;
			info.Format("3�״����Pass��ɫid=%d \n",m_n3mPassPlayer);
			printf(info);
			info.Format("������)%d,%d)\n",kongx,kongy);
			printf(info);
			bestID = m_n3mPassPlayer;
			m_tacagent[bestID].allocated=true;
			Last3meterPasser=bestID;
		
			//////////////////////////////////////////////////////////////////////////
			//��λ����,������봫���ľ��룬̫Զ�Ļ����ƶ���ȥ
			//printf("//��λ����,������봫���ľ��룬̫Զ�Ļ����ƶ���ȥ");
			//posPass = m_Loca_Ray(m_balls[m_nPassPlayer].x,m_balls[m_nPassPlayer].y,m_agents[m_nCatchPlayer].x,m_agents[m_nCatchPlayer].y,100*m_fKField);
			//posPass = m_Loca_Rotate(posPass.x,posPass.y,m_balls[m_nPassPlayer].x,m_balls[m_nPassPlayer].y,180);
			//int angPass;
			//angPass = m_Loca_CalAngle(posPass.x,posPass.y,m_balls[m_nPassPlayer].x,m_balls[m_nPassPlayer].y);
			//if (m_CalDist(m_agents[m_nPassPlayer].x,m_agents[m_nPassPlayer].y,posPass.x,posPass.y) > 300)	//���ƶ�Ŀ������
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
			//��ѡʣ�µ����ƶ�������Ķ�Ա
			bestID = m_n3mCatchPlayer;
			if (m_n3mCatchPlayer >0 && m_n3mCatchPlayer <6)
			{
				Last3meterCatcher=m_n3mCatchPlayer;
			}
			m_tacagent[bestID].allocated=true;
			m_memstatus[m_n3mCatchPlayer] = CTRL_PASS_MOVE;
			if (bestID == 0)
			{
				//�Ѿ�û����Ա�ɷ�����
				bNoOneToAlloc = true;
				break;
			}
			info.Format("׼������Ļ����ǣ�%d",bestID);
			printf(info);
			AMoveTo(bestID,m_tacctrl[ti].target_x,m_tacctrl[ti].target_y,m_tacctrl[ti].target_angle);
			break;
		case CTRL_3M_CATCH:
			//����ָ�������Ա
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
				info.Format("��ӽ��ˣ�ת���������� \n");
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
				info.Format("��ӽ��ֵ����ˣ�ת���������� \n");
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
				info.Format("�����˶��ˣ�ת���������� \n");
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
			//	info.Format("��Խ���˽����Ա��ת���������� \n");
			//	printf(info);
			//	//Active();
			//	break;
			//}


			
			if (Attack_Convert_Counter>60)//5�뻹δ�ӵ���
			{
				m_bClear3meterPassInfo();
				Attack_Convert_Flag=0;
				Last3meterCatcher=0;
				Last3meterPasser=0;
				m_bTask3MeterPass=false;
				m_bTask3MeterPass_Kick=false;
				m_n3mCatchPlayer=0;
				m_n3mPassPlayer=0;
				info.Format("3�׽���ʱ��ת���� \n");
				printf(info);
				//Active(); 
				break ;
			}
			
			
			
			info.Format("����Catch��ɫid=%d \n",m_n3mCatchPlayer);
			printf(info);
			info.Format("����λ��(%d,%d)\n",m_tacagent[m_n3mCatchPlayer].x,m_tacagent[m_n3mCatchPlayer].y );
			printf(info);
			ACatch_Normal(m_n3mCatchPlayer,m_agents[m_n3mPassPlayer].x,m_agents[m_n3mPassPlayer].y);
			break;
		/*������Ա����ѡ*/
		case CTRL_ATTACK:
			//�Է�10��δ����Ļ������⴦�����ܵ����˳��ߵ�λ��
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
					//�Ѿ�û����Ա�ɷ�����
					bNoOneToAlloc = true;
					break;
				}
				//CString info;
				info.Format("���������ɫid=%d \n",bestID);
				printf(info);
				AChangeStatus(bestID,CTRL_ATTACK,0,0);
			} 
			else
			{	
				//m_b10SecEnd == true �Է�10��δ��
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
// 				printf("�Է�10��δ��");
				//////////////////////////////////////////////////////////////////////////
				bestID = 0;
				bestID = TheBestAtk(m_tacagent);
				if (bestID == 0)
				{
					//�Ѿ�û����Ա�ɷ�����
					bNoOneToAlloc = true;
					break;
				}
				AChangeStatus(bestID,CTRL_ATTACK,0,0);
				//////////////////////////////////////////////////////////////////////////
			}
			//////////////////////////////////////////////////////////////////////////
			//������ʱ
			if (m_nPassEndDelay > 0)
			{
				if (bestID == m_nPassPlayer)
				{
					//������ʱ���л��ɽ���������ʱ��¼����
					m_nPassPlayer = 0;
					m_nPassEndDelay = 0;
				}
			}
			//////////////////////////////////////////////////////////////////////////
			break;

		/*�����ƶ�*/
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
			//��ѡʣ�µ����ƶ�������Ķ�Ա
			bestID = m_Ag_GetNearBy(m_tacctrl[ti].target_x,m_tacctrl[ti].target_y,m_tacagent);
			if (bestID == 0)
			{
				//�Ѿ�û����Ա�ɷ�����
				bNoOneToAlloc = true;
				break;
			}
			//////////////////////////////////////////////////////////////////////////
			//������ʱ
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
					//��ʱ��������ռ�¼
					//m_nPassPlayer = 0;
					m_nPassEndDelay=0;//[yzz]�����޸ģ���Ϊ���ڷ��򡢵���ʱ�����Ǵ���ͽ����ˣ������ΪӦ��Ҳ���������
				}
			}
			
			
			//////////////////////////////////////////////////////////////////////////
			AMoveTo(bestID,m_tacctrl[ti].target_x,m_tacctrl[ti].target_y,m_tacctrl[ti].target_angle);
			break;

		/*���������*/
		case CTRL_DEFENCE:
			//��ά�����ض�Ա�ȶ���
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
				//���֮ǰû��������Ƕȷ��صĶ�Ա������ѡ����ص�����Ķ�Ա
				tmp = m_Loca_Ray(m_trueball.x,m_trueball.y,0,600,m_tacctrl[ti].def_dist);
				if (m_tacctrl[ti].def_angle != 0)
				{
					tmp = m_Loca_Rotate(tmp.x,tmp.y,m_trueball.x,m_trueball.y,m_tacctrl[ti].def_angle);
				}
				bestID = m_Ag_GetNearBy(tmp.x,tmp.y,m_tacagent);
			}
			
			if (bestID == 0)
			{
				//�Ѿ�û����Ա�ɷ�����
				bNoOneToAlloc = true;
				break;
			}
			//////////////////////////////////////////////////////////////////////////
			//������ʱ
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
					//��ʱ��������ռ�¼
					m_nPassPlayer = 0;
				}
			}
			//////////////////////////////////////////////////////////////////////////
			AChangeStatus(bestID,m_tacctrl[ti].ctrl,m_tacctrl[ti].def_angle,m_tacctrl[ti].def_dist);
			break;

		/*�߼�����*/
		case CTRL_PRODEF:
			//��ѡʣ�µ���������Ķ�Ա
			bestID = m_Ag_GetNearBy(m_trueball.x,m_trueball.y,m_tacagent);
			if (bestID == 0)
			{
				//�Ѿ�û����Ա�ɷ�����
				bNoOneToAlloc = true;
				break;
			}

			//������صľ���
			tmpDist = m_CalDist(m_trueball.x,m_trueball.y,m_tacagent[bestID].x,m_tacagent[bestID].y);
			if (tmpDist > (m_tacctrl[ti].def_dist+100))
			{
				tmpDist = 300;
			} 
			else
			{
				tmpDist = m_tacctrl[ti].def_dist;
			}

			//�����ƶ�������
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

		/*����*/
		case CTRL_PASS:
			//////////////////////////////////////////////////////////////////////////
			//�����ı�������
			if (m_bTaskPassCatch == true && m_bTaskPassCatch_kick == true)
			{
				//printf("if (m_bTaskPassCatch == true && m_bTaskPassCatch_kick == true)");
				if (m_nPassPlayer > 0 && m_nCatchPlayer >0 && m_nPassPlayer != m_nCatchPlayer)
				{
					//printf("if (m_nPassPlayer > 0 && m_nCatchPlayer >0 && m_nPassPlayer != m_nCatchPlayer)");
					//�����߾�������ס������������Ұ
					tmp.x = m_agents[m_nPassPlayer].x;
					tmp.y = m_agents[m_nPassPlayer].y;
					//////////////////////////////////////////////////////////////////////////
					/*/�˱ܷ�ʽһ
					if (m_agents[m_nPassPlayer].y > 600)
					{
						tmp.y = 0;
					} 
					else
					{
						tmp.y = 1200;
					}
					/////////////////////////////////////////////////////////////////////////*/
					//�˱ܷ�ʽ��
// 					if (m_agents[m_nPassPlayer].x < m_balls[m_nPassPlayer].x)
// 					{	
// 						//��Է��볡����
// 						tmp.x = m_trueball.x;
// 						
// 					}
// 					else
// 					{
// 						//�򼺷��볡����
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
						//��Է��볡����
						AChangeStatus(m_nPassPlayer,CTRL_STOP,0,0);
						break;
					}
					else
					{
						//�򼺷��볡���򣬳���Է�����λ��
						tmp = m_Loca_Ray(m_trueball.x,m_trueball.y,1700,600,500);
						//����������
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
			//���֮ǰָ�������Ա����������ɫ�ȶ���
			//printf("//���֮ǰָ�������Ա����������ɫ�ȶ���");
			bestID = 0;
			info.Format("����ʱ ����Pass��ɫid=%d \n",m_nPassPlayer);
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
					//�Ѿ�û����Ա�ɷ�����
					bNoOneToAlloc = true;
					break;
				}
				m_nPassPlayer = bestID;
			}
			//////////////////////////////////////////////////////////////////////////
			//��λ����,������봫���ľ��룬̫Զ�Ļ����ƶ���ȥ
			//printf("//��λ����,������봫���ľ��룬̫Զ�Ļ����ƶ���ȥ");
			posPass = m_Loca_Ray(m_balls[m_nPassPlayer].x,m_balls[m_nPassPlayer].y,m_agents[m_nCatchPlayer].x,m_agents[m_nCatchPlayer].y,100*m_fKField);
			posPass = m_Loca_Rotate(posPass.x,posPass.y,m_balls[m_nPassPlayer].x,m_balls[m_nPassPlayer].y,180);
			int angPass;
			angPass = m_Loca_CalAngle(posPass.x,posPass.y,m_balls[m_nPassPlayer].x,m_balls[m_nPassPlayer].y);
			if (m_CalDist(m_agents[m_nPassPlayer].x,m_agents[m_nPassPlayer].y,posPass.x,posPass.y) > 300)	//���ƶ�Ŀ������
			{
				AMoveTo(m_nPassPlayer,posPass.x,posPass.y,angPass);
				break;
			}
			//////////////////////////////////////////////////////////////////////////
		//	printf("	APass(bestID,m_tacagent[m_nCatchPlayer].x,m_tacagent[m_nCatchPlayer].y);");
			info.Format("����Pass��ɫid=%d \n",bestID);
			printf(info);
			APass(bestID,m_tacagent[m_nCatchPlayer].x,m_tacagent[m_nCatchPlayer].y);
		//	printf("break");
			break;

		/*����*/
		case CTRL_CATCH:
			//����ָ�������Ա
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
			//ʱ��δ����ʱ���һֱbreak;
			if (m_bCatchDelay == true)
			{
				printf("������ʱ��");
				AChangeStatus(m_nCatchPlayer,CTRL_STOP,0,0);
				break;
			}
			//ʵ���Ҳ����ܽ�����˻��������߾��Լ��е�����
			if (m_nCatchPlayer == 0)
			{
				
				info.Format("ʵ���Ҳ����ܽ�����˻��������߾��Լ��е�����!");
				printf(info);
				AChangeStatus(m_nCatchPlayer,CTRL_ATTACK,0,0);
				
			}
			else
			{		
			info.Format("����Catch��ɫid=%d \n",m_nCatchPlayer);
				printf(info);
				ACatch(m_nCatchPlayer,m_tacagent[m_nPassPlayer].x,m_tacagent[m_nPassPlayer].y);
			}
			break;
		/*****������*****/
		case CTRL_AROUNDBALL:
			//����trueball�����ŵ��ƶ�Ŀ���
			tmp = m_Loca_Ray(m_trueball.x,m_trueball.y,m_tacctrl[ti].target_x,m_tacctrl[ti].target_y,m_tacctrl[ti].def_dist);
			tmp = m_Loca_Rotate(tmp.x,tmp.y,m_trueball.x,m_trueball.y,m_tacctrl[ti].def_angle);			
			//��Գ��������������
			
			//�ϱ߽�
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
			
			//�±߽�
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
			
			//��߽�(����)
			nNoLoop = 100;
			while (tmp.x < -50 || (tmp.x < 250 && tmp.y < 925 && tmp.y > 275))
			{
				//����ǽ��������ǽ��������Ŵ�������
				if (
					(tmp.x < 250 && tmp.y < 925 && tmp.y > 275) &&	//�ܽ������˽�����Χ
					(m_nAgCanBeInPenaltyArea > 0)					//�����Ŵ����û����
					)
				{
					if (m_trueball.x > 225 && m_trueball.y < 900 && m_trueball.y > 300)//��λ�ý����ұ�
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
					break;	//�Ŵ������������ת��
				}
				
				//������ת����λ�����ж�������ת
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
			
			//�ұ߽�(�Է�)
			nNoLoop = 100;
			while (tmp.x > 1900 || (tmp.x > 1575 && tmp.y > 275 && tmp.y < 925))
			{
				//������ת����λ�����ж�������ת
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
			//!!�ܿ��������ѣ���δ����
// 			nNoLoop = 100;
// 			int nDistToPeer;
// 			nDistToPeer = 0;
// 			while(nDistToPeer < 100)
// 			{
// 				//����agent��������룬ֻҪ���κ�һ����Ա�����������Ҫ��취ƫ��
// 				for (int i=1;i<AG_NUM;i++)
// 				{
// 					if (m_agents[i].online == true)
// 					{
// 						nDistToPeer = m_CalDist(tmp.x,tmp.y,m_agents[i].x,m_agents[i].y);
// 						if (nDistToPeer < 100)
// 						{
// 							//������ת����λ�����ж�������ת
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
			/*            �������������ʼ���㵥����Ա��λ��                        */
			/************************************************************************/
			//Ѱ�Һ��ʵĶ�Ա
			bestID = m_Ag_GetNearBy(tmp.x,tmp.y,m_tacagent);
			if (bestID <= 0 || bestID >AG_NUM)
			{
				//�Ҳ������ʵĶ�Ա������
				break;
			}
			//����ö�Ա������������ȫ��������Ϊ׼
			if (m_agents[bestID].foundball == false)
			{
				m_balls[bestID].x = m_trueball.x;
				m_balls[bestID].y = m_trueball.y;
			}
			bestID = m_Ag_GetNearBy(tmp.x,tmp.y,m_tacagent);
			if (bestID == 0)
			{
				//�Ѿ�û����Ա�ɷ�����
				bNoOneToAlloc = true;
				break;
			}
			tmp = m_Loca_Ray(m_balls[bestID].x,m_balls[bestID].y,m_tacctrl[ti].target_x,m_tacctrl[ti].target_y,m_tacctrl[ti].def_dist);
			tmp = m_Loca_Rotate(tmp.x,tmp.y,m_balls[bestID].x,m_balls[bestID].y,m_tacctrl[ti].def_angle);
			//�ϱ߽�
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
			
			//�±߽�
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
			
			//��߽�(����)
			nNoLoop = 100;
			while (tmp.x < -50 || (tmp.x < 250 && tmp.y < 925 && tmp.y > 275))
			{
				//����ǽ��������ǽ��������Ŵ�������
				if (
					(tmp.x < 250 && tmp.y < 925 && tmp.y > 275) &&	//�ܽ������˽�����Χ
					(m_nAgCanBeInPenaltyArea > 0)					//�����Ŵ����û����
					)
				{
					if (m_balls[bestID].x > 225 && m_balls[bestID].y < 900 && m_balls[bestID].y > 300)//��λ�ý����ұ�
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
					break;	//�Ŵ������������ת��
				}
				
				//������ת����λ�����ж�������ת
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
			
			//�ұ߽�
			nNoLoop = 100;
			while (tmp.x > 1900|| (tmp.x > 1575 && tmp.y > 275 && tmp.y < 925))
			{
				//������ת����λ�����ж�������ת
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
			//!!�ܿ��������ѣ���δ����
 		//	nNoLoop = 100;
 		//	nDistToPeer = 0;
			//int ddd=10;
 		//	while((nDistToPeer < 100) && (ddd-->0))
 		//	{
 		//		//����agent��������룬ֻҪ���κ�һ����Ա�����������Ҫ��취ƫ��
 		//		for (int i=1;i<AG_NUM;i++)
 		//		{
 		//			if (m_agents[i].online == true)
 		//			{
 		//				nDistToPeer = m_CalDist(tmp.x,tmp.y,m_agents[i].x,m_agents[i].y);
 		//				if (nDistToPeer < 100)
 		//				{
 		//					//������ת����λ�����ж�������ת
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

		/*****�½���*****/
		case CTRL_BLOCK:
			bestID = 0;
			//��ѡʣ�µ���������Ķ�Ա
			bestID = m_Ag_GetNearBy(m_trueball.x,m_trueball.y,m_tacagent);
			if (bestID == 0)
			{
				//�Ѿ�û����Ա�ɷ�����
				bNoOneToAlloc = true;
				break;
			}
			
			//����½صľ���
			tmpDist = m_CalDist(m_trueball.x,m_trueball.y,m_tacagent[bestID].x,m_tacagent[bestID].y);
			if (tmpDist > (m_tacctrl[ti].def_dist+100))
			{
				tmpDist = 300;
			} 
			else
			{
				tmpDist = m_tacctrl[ti].def_dist;
			}
			
			//�����ƶ�������
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

		/****������λ****/
		case CTRL_SHIFTATK:
			//��ѡʣ�µ���������Ķ�Ա
			bestID = m_Ag_GetNearBy(m_trueball.x,m_trueball.y,m_tacagent);
			if (bestID == 0)
			{
				//�Ѿ�û����Ա�ɷ�����
				bNoOneToAlloc = true;
				break;
			}
			
			//������صľ���
			tmpDist = m_CalDist(m_Foreball.x,m_Foreball.y,m_tacagent[bestID].x,m_tacagent[bestID].y);
			if (tmpDist > (m_tacctrl[ti].def_dist+100))
			{
				tmpDist = 400;
			} 
			else
			{
				tmpDist = m_tacctrl[ti].def_dist;
			}
			
			//�����ƶ�������
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
         //�������ؽ�ɫ����Ҫ��Ŀ����˽��Ľ�ɫ
		case CTRL_FOLLOWPOSITION:
			bestID = m_Ag_GetStatus(CTRL_FOLLOWPOSITION,m_tacagent,false);	//����ά�ֽ�ɫ�ȶ���,Ѱ��δ����ĸý�ɫ
			if (bestID > 0)
			{
				//ֱ���л�
				AFollowPosition(bestID,CTRL_FOLLOWPOSITION,m_tacctrl[ti].target_x,m_tacctrl[ti].target_y);
			}
			else
			{
				bestID = m_Ag_GetNearBy(goalx,goaly,m_tacagent);  //����Ŀ����������
				AFollowPosition(bestID,CTRL_FOLLOWPOSITION,0,0);
			}
		/*��۲��ɫ����Ҫ������Ľ�ɫ*/
		case CTRL_ATTENTIONBALL:
			bestID = m_Ag_GetStatus(CTRL_ATTENTIONBALL,m_tacagent,false);	//����ά�ֽ�ɫ�ȶ���,Ѱ��δ����ĸý�ɫ
			if (bestID > 0)
			{
				//ֱ���л�
				AChangeStatus(bestID,CTRL_ATTENTIONBALL,m_tacctrl[ti].target_x,m_tacctrl[ti].target_y);
			}
			else
			{
				bestID = m_Ag_GetNearBy(m_trueball.x,m_trueball.y,m_tacagent);  //����Ŀ����������
				AChangeStatus(bestID,CTRL_ATTENTIONBALL,0,0);
			}

			//////////////////////////////////////////////////////////////////////////
// 			if (bestID != 0 && bestID == m_Ag_GetMoveTo(m_tacctrl[ti].target_x,m_tacctrl[ti].target_y,m_tacagent))
// 			{
// 				//ֱ���л�
// 				AChangeStatus(bestID,CTRL_ATTENTIONBALL,m_tacctrl[ti].target_x,m_tacctrl[ti].target_y);			
// 			}
// 			else
// 			{
// 				//û����֮ǰ���ݹ��˽�ɫ����ʼ��ѡ�ϸ���ѡ
// 				bestID = m_Ag_GetNearBy(m_tacctrl[ti].target_x,m_tacctrl[ti].target_y,m_tacagent);  //����Ŀ����������
// 				if (m_CalDist(m_tacagent[bestID].x,m_tacagent[bestID].y,m_tacctrl[ti].target_x,m_tacctrl[ti].target_y) > 50*m_fKField)
// 				{
// 					//����ϴ�ʱ���ƶ���ȥ
// 					AMoveTo(bestID,m_tacctrl[ti].target_x,m_tacctrl[ti].target_y,m_tacctrl[ti].target_angle);
// 					// 					m_strTmp.Format("%d�ž���۲���Զ���ƶ���ȥ",bestID);
// 					// 					printf(m_strTmp);
// 					break;
// 				}
// 				else
// 				{
// 					//���빻����,�л���
// 					AChangeStatus(bestID,CTRL_ATTENTIONBALL,0,0);
// 				}
// 			}
			//////////////////////////////////////////////////////////////////////////

			break;

		case CTRL_KICKOFF_PRIM_READY:
			//1��moveto ��ʽ
			//ά�ֽ�ɫ�ȶ���
			if (
				m_curKOPrim != 0 && 
				m_tacagent[m_curKOPrim].online == true
				)
			{
				bestID = m_curKOPrim;
			} 
			else
			{
				//�ҳ���������Ķ�Ա
				bestID = m_Ag_GetNearBy(m_trueball.x,m_trueball.y,m_tacagent);
				
			}
			//���������Լ�����������Ϊ�ƶ���׼�����������
			if (m_tacagent[bestID].foundball == true)
			{
				tmpball.x = m_balls[bestID].x;
				tmpball.y = m_balls[bestID].y;
			} 
			else
			{
				tmpball = m_trueball;
			}

			//�ȼ���������
			if (tmpball.y < 500)
			{
				tmpball.y = 1200 - tmpball.y;
				bflip = true;
			}

			//���������Ա-��-�Է����� = 120��
			tmp = m_Loca_Ray(tmpball.x,tmpball.y,1600,600,(int)(150*m_fKField));			//���һ�������Ǿ���
			tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,120);
			tmpangle = m_Loca_CalAngle(tmp.x,tmp.y,1800,600);

			//�������·������������·�ת
			if (bflip == true)
			{
				tmp.y = 1200 - tmp.y;
				tmpangle = -tmpangle;
			}
			AMoveTo(bestID,tmp.x,tmp.y,tmpangle);
			m_curKOPrim = bestID;
			break;

			/*/2�������ƺŵķ�ʽ
			bestID = m_Ag_GetStatus(CTRL_KICKOFF_PRIM_READY,m_tacagent);	//����ά�ֽ�ɫ�ȶ���
			if (bestID != 0)
			{
				AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
				break;
			}*/

		case CTRL_KICKOFF_SLAVE_READY:
			//1��moveto ��ʽ
			//ά�ֽ�ɫ�ȶ���
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
				//�ҳ���������Ľ�ɫ
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
			//���������Լ�����������Ϊ�ƶ���׼�����������
			if (m_tacagent[bestID].foundball == true)
			{
				tmpball.x = m_balls[bestID].x;
				tmpball.y = m_balls[bestID].y;
			} 
			else
			{
				tmpball = m_trueball;
			}

			//�ȼ���������
			if (tmpball.y < 500)
			{
				tmpball.y = 1200 - tmpball.y;
				bflip = true;
			}

			//�����Ա-��-�Է����� = -120��
			tmp = m_Loca_Ray(tmpball.x,tmpball.y,1600,600,(int)(100*m_fKField));	//���һ�������Ǿ���
			tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,-120);
			tmpangle = m_Loca_CalAngle(tmp.x,tmp.y,tmpball.x,tmpball.y)+120;

			//�������·������������·�ת
			if (bflip == true)
			{
				tmp.y = 1200 - tmp.y;
				tmpangle = -tmpangle;
			}
			AMoveTo(bestID,tmp.x,tmp.y,tmpangle);
			m_curKOSlave = bestID;
			break;

			/*/2�������ƺŵķ�ʽ
			bestID = m_Ag_GetStatus(CTRL_KICKOFF_SLAVE_READY,m_tacagent);	//����ά�ֽ�ɫ�ȶ���
			if (bestID != 0)
			{
				AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
				break;
			}*/

		case CTRL_FREEKICK_PRIM_READY:
			bestID = m_Ag_GetStatus(CTRL_FREEKICK_PRIM_READY,m_tacagent);	//����ά�ֽ�ɫ�ȶ���
			if (bestID != 0)
			{
				AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
			}
			break;

		case CTRL_FREEKICK_SLAVE_READY:
			bestID = m_Ag_GetStatus(CTRL_FREEKICK_SLAVE_READY,m_tacagent);	//����ά�ֽ�ɫ�ȶ���
			if (bestID != 0)
			{
				AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
				break;
			}
			break;

		case CTRL_GOALKICK_PRIM_READY:
			bestID = m_Ag_GetStatus(CTRL_GOALKICK_PRIM_READY,m_tacagent);	//����ά�ֽ�ɫ�ȶ���
			if (bestID != 0)
			{
				AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
				break;
			}
			break;

		case CTRL_GOALKICK_SLAVE_READY:
			bestID = m_Ag_GetStatus(CTRL_GOALKICK_SLAVE_READY,m_tacagent);	//����ά�ֽ�ɫ�ȶ���
			if (bestID != 0)
			{
				AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
				break;
			}
			break;

		case CTRL_THROWIN_PRIM_READY:
			bestID = m_Ag_GetStatus(CTRL_THROWIN_PRIM_READY,m_tacagent);	//����ά�ֽ�ɫ�ȶ���
			if (bestID != 0)
			{
				AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
				break;
			}
			break;

		case CTRL_THROWIN_SLAVE_READY:
			bestID = m_Ag_GetStatus(CTRL_THROWIN_SLAVE_READY,m_tacagent);	//����ά�ֽ�ɫ�ȶ���
			if (bestID != 0)
			{
				AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
				break;
			}
			break;

		case CTRL_CORNERKICK_PRIM_READY:
			bestID = m_Ag_GetStatus(CTRL_CORNERKICK_PRIM_READY,m_tacagent);	//����ά�ֽ�ɫ�ȶ���
			if (bestID != 0)
			{
				AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
				break;
			}
			break;

		case CTRL_CORNERKICK_SLAVE_READY:
			bestID = m_Ag_GetStatus(CTRL_CORNERKICK_SLAVE_READY,m_tacagent);	//����ά�ֽ�ɫ�ȶ���
			if (bestID != 0)
			{
				AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
				break;
			}

			//��ѡʣ�µ���������Ķ�Ա
			bestID = m_Ag_GetNearBy(m_trueball.x,m_trueball.y,m_tacagent);
			if (bestID == 0)
			{
				//�Ѿ�û����Ա�ɷ�����
				bNoOneToAlloc = true;
				break;
			}
			AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
			break;

		//////////////////////////////////////////////////////////////////////////
		//���м̳����ʵĽ�ɫ
		//////////////////////////////////////////////////////////////////////////
		/*�������Ա*/
		case CTRL_KICKOFF_PRIM:
			//����Ϊready״̬������
			bestID = m_Ag_GetStatus(CTRL_KICKOFF_PRIM_READY,m_tacagent);
			if (bestID == 0)
			{
				//���ڸ�״̬�����
				bestID = m_Ag_GetStatus(CTRL_KICKOFF_PRIM,m_tacagent);
				if (bestID == 0)
				{
					if (m_curKOPrim != 0)
					{
						bestID = m_curKOPrim;
					}
					else
					{
						//��û�о���ѡ��������Ľ�ɫ
						bestID = m_Ag_GetNearBy(m_trueball.x,m_trueball.y,m_tacagent);
					}
					if (bestID == 0)
					{
						//�Ѿ�û����Ա�ɷ�����
						bNoOneToAlloc = true;
						break;
					}
				}
			}	
			AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
			break;

		/*���������Ա*/
		case CTRL_KICKOFF_SLAVE:
			//����Ϊready״̬������
			bestID = m_Ag_GetStatus(CTRL_KICKOFF_SLAVE_READY,m_tacagent);
			if (bestID == 0)
			{
				//���ڸ�״̬�����
				bestID = m_Ag_GetStatus(CTRL_KICKOFF_SLAVE,m_tacagent);
				if (bestID == 0)
				{
					if (m_curKOSlave != 0)
					{
						bestID = m_curKOSlave;
					}
					else
					{
						//��û�о���ѡ��������Ľ�ɫ
						bestID = m_Ag_GetNearBy(m_trueball.x,m_trueball.y,m_tacagent);
					}
					if (bestID == 0)
					{
						//�Ѿ�û����Ա�ɷ�����
						bNoOneToAlloc = true;
						break;
					}
				}
			}	
			AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
			break;


		/*�����������Ա*/
		case CTRL_FREEKICK_PRIM:
			//����Ϊready״̬������
			bestID = m_Ag_GetStatus(CTRL_FREEKICK_PRIM_READY,m_tacagent);
			if (bestID == 0)
			{
				//���ڸ�״̬�����
				bestID = m_Ag_GetStatus(CTRL_FREEKICK_PRIM,m_tacagent);
				if (bestID == 0)
				{
					//��û�о���ѡ��������Ľ�ɫ
					bestID = m_Ag_GetNearBy(m_trueball.x,m_trueball.y,m_tacagent);
					if (bestID == 0)
					{
						//�Ѿ�û����Ա�ɷ�����
						bNoOneToAlloc = true;
						break;
					}
				}
			}	
			AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
			break;


		/*�����������Ա*/
		case CTRL_FREEKICK_SLAVE:
			//����Ϊready״̬������
			bestID = m_Ag_GetStatus(CTRL_FREEKICK_SLAVE_READY,m_tacagent);
			if (bestID == 0)
			{
				//���ڸ�״̬�����
				bestID = m_Ag_GetStatus(CTRL_FREEKICK_SLAVE,m_tacagent);
				if (bestID == 0)
				{
					//��û�о���ѡ��������Ľ�ɫ
					bestID = m_Ag_GetNearBy(m_trueball.x,m_trueball.y,m_tacagent);
					if (bestID == 0)
					{
						//�Ѿ�û����Ա�ɷ�����
						bNoOneToAlloc = true;
						break;
					}
				}
			}	
			AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
			break;


		/*�����������Ա*/
		case CTRL_GOALKICK_PRIM:
			//����Ϊready״̬������
			bestID = m_Ag_GetStatus(CTRL_GOALKICK_PRIM_READY,m_tacagent);
			if (bestID == 0)
			{
				//���ڸ�״̬�����
				bestID = m_Ag_GetStatus(CTRL_GOALKICK_PRIM,m_tacagent);
				if (bestID == 0)
				{
					//��û�о���ѡ��������Ľ�ɫ
					bestID = m_Ag_GetNearBy(m_trueball.x,m_trueball.y,m_tacagent);
					if (bestID == 0)
					{
						//�Ѿ�û����Ա�ɷ�����
						bNoOneToAlloc = true;
						break;
					}
				}
			}	
			AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
			break;


		/*�����������Ա*/
		case CTRL_GOALKICK_SLAVE:
			//����Ϊready״̬������
			bestID = m_Ag_GetStatus(CTRL_GOALKICK_SLAVE_READY,m_tacagent);
			if (bestID == 0)
			{
				//���ڸ�״̬�����
				bestID = m_Ag_GetStatus(CTRL_GOALKICK_SLAVE,m_tacagent);
				if (bestID == 0)
				{
					//��û�о���ѡ��������Ľ�ɫ
					bestID = m_Ag_GetNearBy(m_trueball.x,m_trueball.y,m_tacagent);
					if (bestID == 0)
					{
						//�Ѿ�û����Ա�ɷ�����
						bNoOneToAlloc = true;
						break;
					}
				}
			}	
			AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
			break;


		/*�����������Ա*/
		case CTRL_THROWIN_PRIM:
			//����Ϊready״̬������
			bestID = m_Ag_GetStatus(CTRL_THROWIN_PRIM_READY,m_tacagent);
			if (bestID == 0)
			{
				//���ڸ�״̬�����
				bestID = m_Ag_GetStatus(CTRL_THROWIN_PRIM,m_tacagent);
				if (bestID == 0)
				{
					//��û�о���ѡ��������Ľ�ɫ
					bestID = m_Ag_GetNearBy(m_trueball.x,m_trueball.y,m_tacagent);
					if (bestID == 0)
					{
						//�Ѿ�û����Ա�ɷ�����
						bNoOneToAlloc = true;
						break;
					}
				}
			}	
			AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
			break;


		/*�����������Ա*/
		case CTRL_THROWIN_SLAVE:
			//����Ϊready״̬������
			bestID = m_Ag_GetStatus(CTRL_THROWIN_SLAVE_READY,m_tacagent);
			if (bestID == 0)
			{
				//���ڸ�״̬�����
				bestID = m_Ag_GetStatus(CTRL_THROWIN_SLAVE,m_tacagent);
				if (bestID == 0)
				{
					//��û�о���ѡ��������Ľ�ɫ
					bestID = m_Ag_GetNearBy(m_trueball.x,m_trueball.y,m_tacagent);
					if (bestID == 0)
					{
						//�Ѿ�û����Ա�ɷ�����
						bNoOneToAlloc = true;
						break;
					}
				}
			}	
			AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
			break;


		/*���������Ա*/
		case CTRL_CORNERKICK_PRIM:
			//����Ϊready״̬������
			bestID = m_Ag_GetStatus(CTRL_CORNERKICK_PRIM_READY,m_tacagent);
			if (bestID == 0)
			{
				//���ڸ�״̬�����
				bestID = m_Ag_GetStatus(CTRL_CORNERKICK_PRIM,m_tacagent);
				if (bestID == 0)
				{
					//��û�о���ѡ��������Ľ�ɫ
					bestID = m_Ag_GetNearBy(m_trueball.x,m_trueball.y,m_tacagent);
					if (bestID == 0)
					{
						//�Ѿ�û����Ա�ɷ�����
						bNoOneToAlloc = true;
						break;
					}
				}
			}	
			AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
			break;


		/*���������Ա*/
		case CTRL_CORNERKICK_SLAVE:
			//����Ϊready״̬������
			bestID = m_Ag_GetStatus(CTRL_CORNERKICK_SLAVE_READY,m_tacagent);
			if (bestID == 0)
			{
				//���ڸ�״̬�����
				bestID = m_Ag_GetStatus(CTRL_CORNERKICK_SLAVE,m_tacagent);
				if (bestID == 0)
				{
					//��û�о���ѡ��������Ľ�ɫ
					bestID = m_Ag_GetNearBy(m_trueball.x,m_trueball.y,m_tacagent);
					if (bestID == 0)
					{
						//�Ѿ�û����Ա�ɷ�����
						bNoOneToAlloc = true;
						break;
					}
				}
			}	
			AChangeStatus(bestID,m_tacctrl[ti].ctrl,0,0);
			break;

		//////////////////////////////////////////////////////////////////////////	
		/*�����ڻ�*/
		case CTRL_ATKCOVER:
			//1��Ѱ����������ĵĶ�Ա���ڻ���Ա��������Ա�Ѿ������ߣ����ص��ģ�
			bestID = m_Ag_GetNearBy(m_trueball.x,m_trueball.y,m_tacagent);
			if (bestID <= 0)
			{
				break;
			}
			//2�������ڻ���Ա���ϰ�����Ϣ���Թ��жϵ���
			memcpy(m_tmpAgObst,(char*)&(m_cObst[bestID*EACH_AG_OBST_NUM]),EACH_AG_OBST_NUM*sizeof(obstacle));
			//3��Ѱ���ϰ����в��Ǽ�����Ա���Ҿ���Ͻ����ϰ���
			int nTmpOb;
			nTmpOb = 0;
			while(nTmpOb >= 0)
			{
				nTmpOb = m_Obst_NearBy(m_trueball.x,m_trueball.y,m_tmpAgObst,EACH_AG_OBST_NUM);
				if (nTmpOb >=0 )
				{
					//��agents��Ķ�Ա���ȡ���ϰ�����бȶԣ��ų�������Ա
					for(int i=0;i<AG_NUM;i++)
					{
						if (i != m_tmpAgObst[nTmpOb].ID)
						{
							if (m_CalDist(m_tmpAgObst[nTmpOb].x,m_tmpAgObst[nTmpOb].y,m_agents[i].x,m_agents[i].y) < 100)
							{
								//nTmpOb�ϰ�����ĳ����Ա�������1�ף�������Ա�����ɱȽϴ�
								m_tmpAgObst[nTmpOb].valid = false;
								break;
							}
						}
					}
					//���ʹ�ܣ�˵�����ϰ���Ϊ��ЧĿ��
					if (m_tmpAgObst[nTmpOb].valid == true)
					{
						break;
					}
				}
			}

			//��Ч�ĵз�Ŀ�꣬������
			if (nTmpOb >= 0)
			{
				tmp = m_Loca_Ray(m_tmpAgObst[nTmpOb].x,m_tmpAgObst[nTmpOb].y,m_trueball.x,m_trueball.y,50);
				//����ʵ�ʵķ��ؾ��룬�����С
				tmpDist = m_CalDist(tmp.x,tmp.y,m_trueball.x,m_trueball.y);
				if (tmpDist < m_tacctrl[ti].def_dist)
				{
					//���ؾ���̫С����Ҫ�ӳ�
					tmp = m_Loca_Ray(m_trueball.x,m_trueball.y,tmp.x,tmp.y,m_tacctrl[ti].def_dist);
				}
				tmpangle = m_Loca_CalAngle(tmp.x,tmp.y,m_trueball.x,m_trueball.y);
				AMoveTo(bestID,tmp.x,tmp.y,tmpangle);
			}
			else
			{
				//�Ҳ����ϰ���򶨵���λ
				if (m_trueball.y > 600)
				{
					tmp.y = m_trueball.y - 200;
					//�ж��Ƿ��Ѿ��ж�Ա���ǣ�����ѵ�һ����
					for (int i=1;i<AG_NUM;i++)
					{
						if (m_agents[i].online == true)
						{
							int x = 100;
							//Ҫȥ�ĵ��Ѿ������ˣ�Χ����תѰ����ŵ�
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
					//�ж��Ƿ��Ѿ��ж�Ա���ǣ�����ѵ�һ����
					for (int i=1;i<AG_NUM;i++)
					{
						if (m_agents[i].online == true)
						{
							int x = 100;
							//Ҫȥ�ĵ��Ѿ������ˣ�Χ����תѰ����ŵ�
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

		/**������ս������**/
		case TC_FINDBALL:
			//�����볡�ķ�����Ա��Ա
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

		/*�����еĽ�ɫ*/
		case CTRL_SEARCHBALL:
			strTmp.Format("���� tac%d ctrl%d",ti,m_tacctrl[ti].ctrl);
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
// 			str.Format("������Ϊ tac%d ctrl%d",ti,m_tacctrl[ti].ctrl);
		//	printf(str);
			//ʣ�µĶ�Ա���η���
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
					//������ʱ
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
							//��ʱ��������ռ�¼
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

	//����moveto
	//m_Fix_KeepDistance(m_agents,100,300);
		//info.Format("Pass��ɫid=%d  catch id=%d \n",m_nPassPlayer,m_nCatchPlayer);
		//printf(info);
		//����Ѿ��޶�Ա���Է����ˣ�����ѭ��
		if (bNoOneToAlloc == true)
		{
			break;
		}
	}
	}
	

	//////////////////////////////////////////////////////////////////////////


/************************************************************************/
/*                              ����                                    */
/************************************************************************/
void CUPFullField::REF_KickOff()
{
	m_eStage = sKickOffReady;
	Ready_KickOff();
}

void CUPFullField::START_Kickoff()
{
	m_strCurSt.Format("�ҷ�����ʼ��");
	m_eLastStage = m_eStage;
	DefaultSTART();
	
	Tac_Wait(8);	//8���ǿ�ƿ�ʼ��������
}

/************************************************************************/
/*                             ������                                   */
/************************************************************************/
void CUPFullField::REF_Freekick()
{
	m_eStage = sFreekickReady;
	Ready_Freekick();
}

void CUPFullField::START_Freekick()
{
	m_strCurSt.Format("�ҷ�������ʼ��");
	m_eLastStage = m_eStage;
	DefaultSTART();
	
	Tac_Wait(8);	//8���ǿ�ƿ�ʼ��������
}

/************************************************************************/
/*                             ������                                   */
/************************************************************************/
void CUPFullField::REF_Goalkick()
{
	m_eStage = sGoalkickReady;
	Ready_Goalkick();
}

void CUPFullField::START_Goalkick()
{
	m_strCurSt.Format("�ҷ�������ʼ��");
	m_eLastStage = m_eStage;
	DefaultSTART();
	
	Tac_Wait(8);	//8���ǿ�ƿ�ʼ��������
}

/************************************************************************/
/*                              ������                                  */
/************************************************************************/
void CUPFullField::REF_Throwin()
{
	m_eStage = sThrowinReady;
	Ready_Throwin();
}

void CUPFullField::START_Throwin()
{
	m_strCurSt.Format("�ҷ�������ʼ��");
	m_eLastStage = m_eStage;
	DefaultSTART();
	
	Tac_Wait(8);	//8���ǿ�ƿ�ʼ��������
}

/************************************************************************/
/*                              ����                                    */
/************************************************************************/
void CUPFullField::REF_CornerKick()
{
	m_eStage = sCornerKickReady;
	Ready_CornerKick();
}

void CUPFullField::START_CornerKick()
{
	m_strCurSt.Format("�ҷ�����ʼ��");
	m_eLastStage = m_eStage;
	if (m_bFoundball == true)
	{
		DefaultSTART();
	} 
	else
	{
		printf("����ʼʱ��ʧ�٣�");
		DefaultSTART();
	}
	m_eStage = sStart;
	Tac_Wait(8);	//8���ǿ�ƿ�ʼ��������

}

/************************************************************************/
/*                               ����/����                              */
/************************************************************************/
void CUPFullField::REF_Penalty()
{
	m_eStage = sPenaltyReady;
	Ready_Penalty();
}

void CUPFullField::START_Penalty()
{
	m_strCurSt.Format("�ҷ�����ʼ��");
	m_eStage = sMani;

	m_tacctrl[1].ctrl = CTRL_MOVETO;
	m_tacctrl[1].target_x = 10;
	m_tacctrl[1].target_y = 600;
	m_tacctrl[1].target_angle = 0;
	
	m_tacctrl[2].ctrl = CTRL_ATTACK;
	
	//������Ա�ܶ���ָ����
	for (int i=3;i<7;i++)
	{
		m_tacctrl[i].ctrl = CTRL_STOP;
		m_tacctrl[i].target_angle = 90;	
	}
	//����
	
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
/*                                ����                                  */
/************************************************************************/
void CUPFullField::REF_DroppedBall()
{
	m_eStage = sDroppedBallReady;
	Ready_DroppedBall();
}

void CUPFullField::START_DroppedBall()
{
	m_strCurSt.Format("����ʼ��");
	Active();
}

//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/*                               �Է�����                               */
/************************************************************************/
void CUPFullField::REF_Anti_Kickoff()
{
	m_eStage = sAntiKickOffReady;
	//Default_Anti_Ready(TAC_DIST_ANTI+10);
	Ready_Anti_Kickoff();
}

void CUPFullField::START_Anti_Kickoff()
{
	m_strCurSt.Format("�Է�����ʼ��");
	m_eLastStage = m_eStage;
	Default_Anti_START();
}

/************************************************************************/
/*                               �Է�������                               */
/************************************************************************/
void CUPFullField::REF_Anti_Freekick()
{
	m_eStage = sAntiFreekickReady;
	Default_Anti_Ready(TAC_DIST_ANTI);
	//Ready_Anti_Freekick();
}

void CUPFullField::START_Anti_Freekick()
{
	m_strCurSt.Format("�Է�������ʼ��");
	m_eLastStage = m_eStage;
	Default_Anti_START();
}

/************************************************************************/
/*                               �Է�������                               */
/************************************************************************/
void CUPFullField::REF_Anti_Goalkick()
{
	m_eStage = sAntiGoalkickReady;
	Default_Anti_Ready(TAC_DIST_ANTI);
	//Ready_Anti_Goalkick();
}

void CUPFullField::START_Anti_Goalkick()
{
	m_strCurSt.Format("�Է�������ʼ��");	
	m_eLastStage = m_eStage;
	Default_Anti_START();
}

/************************************************************************/
/*                               �Է�������                               */
/************************************************************************/
void CUPFullField::REF_Anti_Throwin()
{
	m_eStage = sAntiThrowinReady;
	Default_Anti_Ready(TAC_DIST_ANTI);
	//Ready_Anti_Throwin();
}

void CUPFullField::START_Anti_Throwin()
{
	m_strCurSt.Format("�Է�������ʼ��");	
	m_eLastStage = m_eStage;
	Default_Anti_START();
}

/************************************************************************/
/*                               �Է�����                               */
/************************************************************************/
void CUPFullField::REF_Anti_CornerKick()
{
	m_eStage = sAntiCornerKickReady;
	Default_Anti_Ready(TAC_DIST_ANTI);
	//Ready_Anti_CornerKick();
}

void CUPFullField::START_Anti_CornerKick()
{
	m_strCurSt.Format("�Է�����ʼ��");	
	m_eLastStage = m_eStage;
	Default_Anti_START();
}

/************************************************************************/
/*                               �Է�����                               */
/************************************************************************/
void CUPFullField::REF_Anti_Penalty()
{
	m_eStage = sAntiPenaltyReady;
	Ready_Anti_Penalty();
}

void CUPFullField::START_Anti_Penalty()
{
	m_strCurSt.Format("�Է�����ʼ��");
	m_eStage = sMani;
	//������Ա�ܶ���ָ����
	for (int i=1;i<7;i++)
	{
		m_tacctrl[i].ctrl = CTRL_MOVETO;
		m_tacctrl[i].target_angle = 90;	
	}
	m_tacctrl[1].target_x = 10;
	m_tacctrl[1].target_y = 600;
	m_tacctrl[1].target_angle = 0;	
	
	//�ܵ����
	m_tacctrl[2].target_x = 500;
	m_tacctrl[2].target_y = 100;
	
	//����
	
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
	m_strCurSt.Format("������ͣ...");
	KillTimer(TAC_WAIT_TIMER);
	KillTimer(999);
	Tac_ReleaseKickoff();
	Disactive();
	PR_AllReadyCanBeIn();
	m_taskPassCatchEnd();
	m_b10SecEnd = false;

	m_bTaskMidPassCatch = false;//[yzz]�˴�Ӧ��Ϊ �г����� �����Ľ�������
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
//��������
void CUPFullField::DefaultReady()
{
	m_nAgCanBeInPenaltyArea = 5;
	KillTimer(TAC_WAIT_TIMER);

	m_bTaskPassCatch = false;
	m_bTaskPassCatch_kick = false;
	m_nCatchPlayer = 0;
	m_nPassPlayer = 0;
	m_nPassEndDelay = 0;

	//2010��ʹ��moveto����
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;
	//������Ա�ܶ���ָ����
	for (int i=2;i<6;i++)
	{
		m_tacctrl[i].ctrl = CTRL_MOVETO;
		m_tacctrl[i].target_angle = 0;	
	}
	//�ȼ������ϣ�����λ��
	CPoint tmpball = m_trueball;
	if (tmpball.y < 600)
	{
		tmpball.y = 1200 - tmpball.y;
	}
	
	CPoint tmp;
	CPoint posCatch;
	CPoint posPass;
	int tmpAng;

	//������ĺ����꣨Ϊ���������ã�
	int halfField = 500;
	if (abs(tmpball.x - halfField) < 20)
	{
		//������
		tmpball.x = halfField;
	} 
	//���������ڵİ볡��ȷ���������
	if (tmpball.x < halfField)
	{
	 	
		//1�������ҷ��볡�������ʱ�������ת��
		//[yzz]���ڽ����߶��� ΪʲôҪ�� 1600,600����������У����
		//���������Ա-��-�Է����� = 180��
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
		//�����Ա-��-�Է����� = 180��
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,1800,600,(int)(TAC_DIST_PASS*m_fKField));			//���һ�������Ǿ���
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,180);//�Ƶ������
		m_tacctrl[2].target_x = tmp.x;
		m_tacctrl[2].target_y = tmp.y;
		m_tacctrl[2].target_angle = m_Loca_CalAngle(tmp.x,tmp.y,tmpball.x,tmpball.y);
		posPass.x = tmp.x;
		posPass.y = tmp.y;

		//��ʱ������̫��
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
		
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,1600,600,(int)(TAC_DIST_CATCH*m_fKField));			//���һ�������Ǿ���
		m_tacctrl[3].target_x = tmp.x;
		m_tacctrl[3].target_y = tmp.y;
		m_tacctrl[3].target_angle = m_Loca_CalAngle(tmp.x,tmp.y,tmpball.x,tmpball.y);
		posCatch.x = tmp.x;
		posCatch.y = tmp.y;
		int n = 0;
		while((tmp.x<925) && (n++<5))
		{
			tmp = m_Loca_Ray(tmpball.x,tmpball.y,1600,600,(int)(TAC_DIST_CATCH*m_fKField+n*20));			//���һ�������Ǿ���
			m_tacctrl[3].target_x = tmp.x;
			m_tacctrl[3].target_y = tmp.y;
			m_tacctrl[3].target_angle = m_Loca_CalAngle(tmp.x,tmp.y,tmpball.x,tmpball.y);
			posCatch.x = tmp.x;
			posCatch.y = tmp.y;
		}

		//�����Ա-��-�Է����� = 180��
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,1800,600,(int)(TAC_DIST_PASS*m_fKField));			//���һ�������Ǿ���
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,180);//�Ƶ������
		m_tacctrl[2].target_x = tmp.x;
		m_tacctrl[2].target_y = tmp.y;
		m_tacctrl[2].target_angle = m_Loca_CalAngle(tmp.x,tmp.y,tmpball.x,tmpball.y);
		posPass.x = tmp.x;
		posPass.y = tmp.y;

		//��ʱ������̫��
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
		
		//2�����ڶԷ��볡����ȡֱ������	(tmpball.x > 500)
		//���������Ա-��-�Է����� = 180�� (����tmp�����������ж�ʵ���ƶ�Ŀ���Ĵ��λ��)
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,1800,600,(int)(TAC_DIST_CATCH*m_fKField));			//���һ�������Ǿ���
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
		//�����Աλ������,�������
		while (posCatch.y > 1000)
		{
			//��������
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

		//�����Ա-��-�����Ա = 0��
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,posCatch.x,posCatch.y,(int)(TAC_DIST_PASS*m_fKField));	//���һ�������Ǿ���
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
			//��������
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
	
	//�����ܶ�λ�ã����ⲻ������ľ���̫��
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
	
	//�������·������������·�ת
	if (m_trueball.y < 550)		//���ⲻ����Ϊ600�����������ĸ�������ʱ������Ķ�λ�ζ���ɵĻ���λ�����ػζ�
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

		//posCatch �ķ�ת
		posCatch.y = 1200 - posCatch.y;
		posPass.y = 1200 - posPass.y;
	}

	//ָ��������ID
	//	m_nCatchPlayer = m_Ag_GetNearBy(m_tacctrl[3].target_x,m_tacctrl[3].target_y,m_agents);
	m_nCatchPlayer = m_Ag_GetNearBy(posCatch.x,posCatch.y,m_tacagent);
	m_nPassPlayer = m_Ag_GetNearBy(posPass.x,posPass.y,m_tacagent);

	//debug
	CString strTmp;
	strTmp.Format(
		"����%d(%d,%d) ����%d(%d,%d)",
		m_nPassPlayer,posPass.x,posPass.y,
		m_nCatchPlayer,posCatch.x,posCatch.y
		);
	printf(strTmp);

	//////////////////////////////////////////////////////////////////////////
	/*/09�� �þ��ߺſ���
	//�ؼ���ɫ����
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;
	m_tacctrl[2].ctrl = CTRL_KICKOFF_SLAVE_READY;
	m_tacctrl[3].ctrl = CTRL_KICKOFF_PRIM_READY;

	for (int i=4;i<6;i++)
	{
		m_tacctrl[i].ctrl = CTRL_MOVETO;
		m_tacctrl[i].target_angle = 0;	
	}
	//�����ܶ�λ�ã����ⲻ������ľ���̫��
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
	//09��ʹ��moveto����
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;
	//������Ա�ܶ���ָ����
	for (int i=2;i<6;i++)
	{
		m_tacctrl[i].ctrl = CTRL_MOVETO;
		m_tacctrl[i].target_angle = 0;	
	}
	//�ȼ������ϣ�����λ��
	CPoint tmpball = m_trueball;
	if (tmpball.y < 500)
	{
		tmpball.y = 1200 - tmpball.y;
	}

	{
		//�����Ա-��-�Է����� = -120��
		CPoint tmp = m_Loca_Ray(tmpball.x,tmpball.y,1600,600,80*m_fKField);	//���һ�������Ǿ���
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,-120);
		m_tacctrl[2].target_x = tmp.x;
 		m_tacctrl[2].target_y = tmp.y;
		m_tacctrl[2].target_angle = m_Loca_CalAngle(tmp.x,tmp.y,tmpball.x,tmpball.y)+120;

		//���������Ա-��-�Է����� = 120��
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,1600,600,100*m_fKField);			//���һ�������Ǿ���
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,120);
		m_tacctrl[3].target_x = tmp.x;
 		m_tacctrl[3].target_y = tmp.y;
		m_tacctrl[3].target_angle = m_Loca_CalAngle(tmp.x,tmp.y,1800,600);
	}

	//�������·������������·�ת
	if (m_trueball.y < 500)
	{
		m_tacctrl[2].target_y = 1200 - m_tacctrl[2].target_y;
		m_tacctrl[2].target_angle = -m_tacctrl[2].target_angle;
		m_tacctrl[3].target_y = 1200 - m_tacctrl[3].target_y;
		m_tacctrl[3].target_angle = -m_tacctrl[3].target_angle;
	}

	//�����ܶ�λ�ã����ⲻ������ľ���̫��
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

	//��ֹײ��
	//m_Fix_CollisionAvoid(m_tacctrl, 50);

	m_TacAction(m_tacagent);
}

void CUPFullField::DefaultSTART()
{
	//2010��,һ�����˽������״̬���򼤻���������
	m_bClear3meterPassInfo();
	Default_start_delay=0;
	SetTimer(999,1000,NULL);
	m_bTaskPassCatch = true;
	m_bTask3MeterPass=false;// ��ֹ�������3�״������ wzc
	int i;
	for (i=0;i<AG_NUM;i++)
	{
		if (m_tacagent[i].online == true)
		{
			if (m_tacagent[i].status == CTRL_ATTACK)
			{
				CString strTmp;
				printf("2\r\n2\r\n2\r\n2\r\n");
				strTmp.Format("���������ת�������ߡ�����������������");
				printf(strTmp);
				Active();
				return;
			}
		}
	}
	CString strTmp;
	strTmp.Format("DefaultSTART: ����%d  ����%d",m_nPassPlayer,m_nCatchPlayer);
	printf(strTmp);

	//���������£�һ����ӽ������Ա���򼤻���������
	if (m_bTaskPassCatch == true)
	{
		
		//һ��������Ա������£���Ҫ�����������Աid
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
			//ֻ��һ��������Ա����Ա,���⴦��
			printf("//ֻ��һ��������Ա����Ա,���⴦��");
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
			//���������£�һ����ӽ������Ա���򼤻���������
			if (m_nCatchPlayer > 0)
			{
				//�����򵽽�����Ա���룬������л�����������
				if (m_CalDist(m_balls[m_nCatchPlayer].x,m_balls[m_nCatchPlayer].y,m_agents[m_nCatchPlayer].x,m_agents[m_nCatchPlayer].y) < 100)
				{
					CString strTmp;
					strTmp.Format("��ӽ��ˡ�������������������");
					printf(strTmp);
					printf("12\r\n12\r\n12\r\n12\r\n");
					Active();
					return;
				}
			}
			else
			{
				//�����Ա������ģ������ֱ�ӽ�����������
				if (m_bTaskPassCatch_kick == true)
				{
					printf("//�����Ա������ģ������ֱ�ӽ�����������");
					printf("14\r\n14\r\n14\r\n31\r\n");
					Active();
					return;
				}
			}
		}		
	}
	
			
	//�����Լ�������Լ������
	if (m_nCatchPlayer == m_nPassPlayer)
	{
		//Ѱ����������ķǴ����Ա�������Ա
		printf("m_nCatchPlayer == m_nPassPlayer,Ѱ����������ķǴ����Ա�������Ա");

		memcpy(m_tmpAgents,m_agents,AG_NUM*sizeof(uagent));
		m_tmpAgents[m_nPassPlayer].online = false;
		m_tmpAgents[m_nPassPlayer].allocated = true;
		int tmpcatch = TheBestAtk(m_tmpAgents);
		while(tmpcatch > 0)	//tmpcatch > 0 ˵�����ж�Ա���Է����ɫ
		{
			if (tmpcatch != m_nPassPlayer)
			{
				CString strInfo;
				strInfo.Format("�ҵ������Ա%d,�����ԱΪ%d",tmpcatch,m_nPassPlayer);
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
		//�����Ҳ������Խ���Ķ�Ա������һ��0����Ա
		if (m_nCatchPlayer == m_nPassPlayer)
		{
			printf("�����Ҳ������Խ���Ķ�Ա������һ��0����Ա");
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
			//��ɫ����
			m_tacctrl[1].ctrl = CTRL_GOALKEEP;
			m_tacctrl[2].ctrl = CTRL_PASS;
			
			//3~5�Ŷ�Աֹͣ
			for (i=3;i<AG_NUM;i++)         //4��λ��ɽ���
			{
				m_tacctrl[i].ctrl = CTRL_STOP;
			}
		} 
		else
		{
			//(m_bTaskPassCatch_kick == true)	���Ѿ��߳������
			//��ɫ����
			m_tacctrl[1].ctrl = CTRL_GOALKEEP;	
			m_tacctrl[2].ctrl = CTRL_CATCH;
			m_tacctrl[3].ctrl = CTRL_PASS;
			
			//3~5�Ŷ�Աֹͣ
			for (i=3;i<AG_NUM;i++)
			{
				m_tacctrl[i].ctrl = CTRL_STOP;
			}

			//�����Ա�����ڵ������ѡ��һ���Ǵ����Ա��Ϊ������Ա(һ�����ֽ�����Ա���ͻἤ����������)
			if (m_agents[m_nCatchPlayer].online == false)
			{
				printf("�����Ա�����ڵ������ѡ��һ���Ǵ����Ա��Ϊ������Ա");
				
				m_tacctrl[2].ctrl = CTRL_PASS;	//���CTRL_PASS��Ϊ�˱��⴫���Ա�Լ�ȥ����
				m_tacctrl[3].ctrl = CTRL_ATTACK;
			}
		}
	}
		
	//printf("m_TacAction(m_tacagent)ǰ");
	m_TacAction(m_tacagent);	
	//printf("m_TacAction(m_tacagent)��");

	//m_eLastStage = m_eStage;
	m_eStage = sStart;

	/*/////////////////////////////////////////////////////////////////////////
	//09��
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;

	//�����Ŀ�����
	m_tacctrl[2].ctrl = CTRL_KICKOFF_SLAVE;
	m_tacctrl[4].ctrl = CTRL_KICKOFF_PRIM;
	
	m_TacAction(m_tacagent);
	
	m_eLastStage = m_eStage;
	m_eStage = sStart;

	//���ݳ��϶�Ա����������������е�������ʱ�䣬������ԱҪ���Լ��е�����������ʱ���һЩ
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
		//�г���������Ա���л��ٶ�����
		Tac_Wait(3);
	}
	else
	{
		//������Ա�������
		Tac_Wait(2);
	}
	/////////////////////////////////////////////////////////////////////////*/
}

//�Է�����ʼ��Ĭ�ϴ���ʽ
void CUPFullField::Default_Anti_START()
{
	//�����Ŷ�Ա�Ķ��پ�����۲�Ա��λ��
	int nAgOnline = 0;
	for (int i=2;i<AG_NUM;i++)
	{
		if (m_tacagent[i].online == true && m_tacagent[i].status != ROBST_ERR)
		{
			nAgOnline ++;
		}
	}

	//��ʱm_tacctrl[2]Ϊ0�Ƕȷ��� 3Ϊ����A 4Ϊ���� 5Ϊ����B
	//��۲�Ա
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
	m_nAgCanBeInPenaltyArea = 1;	//��������վһ����

	KillTimer(TAC_WAIT_TIMER);
	//�Է������ҷ���λӦ��
	//////////////////////////////////////////////////////////////////////////
	//10�����
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;

	for (int i=2;i<7;i++)
	{
		m_tacctrl[i].ctrl = CTRL_MOVETO;
	}

	//�ȼ�����λ�������Ϸ�������λ��
	CPoint tmpball = m_trueball;
	if (m_trueball.y < 600)
	{
		tmpball.y = 1200 - tmpball.y;
	}
	
 	CPoint tmp;	
	////////////////////////////////////////////////////////////////////////////
	//������ĺ����꣨Ϊ���������ã�
	static int lastballx = 200;
	int halfField = 200;
	if (abs(tmpball.x - halfField) < 50)
	{
		//������
		tmpball.x = lastballx;
	} 
	else
	{
		//�����⣬����
		lastballx = tmpball.x;
	}
	lastballx = tmpball.x;
	if (tmpball.x > halfField && tmpball.x < 650)
	{
		//�����ҷ��볡�еȾ���ʱ
		//��������֮�����һ��
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)(325*m_fKField));
		m_tacctrl[2].target_x = tmp.x;
		m_tacctrl[2].target_y = tmp.y;

		

		int defAngle = -52;
		//����1
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

		//!�滻�������ý����ڻ������ضԷ������Ա
		// 		m_tacctrl[3].ctrl = CTRL_ATKCOVER;
		// 		m_tacctrl[3].def_dist = (inDistFB+50)*m_fKField; 

		//����
		m_tacctrl[4].target_x =425;
		m_tacctrl[4].target_y = 635;

		if (m_CalDist(m_tacctrl[4].target_x,m_tacctrl[4].target_y,tmpball.x,tmpball.y) < TAC_DIST_ANTI+300)
		{
			//!!!��ʱ�޸ģ���������̫��
			m_tacctrl[4].target_y = tmpball.y - TAC_DIST_ANTI+100;
		}

		//����2�������ߵ�һ�࣬���׳���
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
			//���Ŷ�Ա�˵��������ˣ�Ӧ��������������λ�õ���
			m_tacctrl[2].target_x = 200;	//�����ڶ�Աǰ��

			//������΢�Ŵ�һЩ
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
		//�����ҷ��볡�еȾ���ʱ
		//��������֮�����һ��
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
		//����1
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

		//!�滻�������ý����ڻ������ضԷ������Ա
// 		m_tacctrl[3].ctrl = CTRL_ATKCOVER;
// 		m_tacctrl[3].def_dist = (inDistFB+50)*m_fKField; 

		//����
		m_tacctrl[4].target_x = m_Pos_Back.x;
 		m_tacctrl[4].target_y = m_Pos_Back.y;

		if (m_CalDist(m_tacctrl[4].target_x,m_tacctrl[4].target_y,tmpball.x,tmpball.y) < TAC_DIST_ANTI+300)
		{
			//!!!��ʱ�޸ģ���������̫��
			m_tacctrl[4].target_y = tmpball.y - TAC_DIST_ANTI+100;
		}

		//����2�������ߵ�һ�࣬���׳���
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
			//���Ŷ�Ա�˵��������ˣ�Ӧ��������������λ�õ���
			m_tacctrl[2].target_x = 200;	//�����ڶ�Աǰ��

			//������΢�Ŵ�һЩ
			tmp = m_Loca_Rotate(m_tacctrl[3].target_x,m_tacctrl[3].target_y,tmpball.x,tmpball.y,-50);
			m_tacctrl[3].target_x = tmp.x;
			m_tacctrl[3].target_y = tmp.y;

			tmp = m_Loca_Rotate(m_tacctrl[5].target_x,m_tacctrl[5].target_y,tmpball.x,tmpball.y,20);
			m_tacctrl[5].target_x = tmp.x;
			m_tacctrl[5].target_y = tmp.y;
		}
		
	}
	else//(tmpball.x < 200),Ӧ���ǽ���һ���
	{
		//���ڼ���������
		//����������֮�����һ��
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

		//�ڵ�һ����������תһ���Ƕ�
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

		//!�滻�������ý����ڻ������ضԷ������Ա
// 		m_tacctrl[3].ctrl = CTRL_ATKCOVER;
// 		m_tacctrl[3].def_dist = (inDistFB+50)*m_fKField; 
		
		//���������Է�����һ��ת��
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

		//����
		m_tacctrl[5].target_x = m_Pos_Back.x;
		m_tacctrl[5].target_y = m_Pos_Back.y;

		

		if (m_CalDist(m_tacctrl[5].target_x,m_tacctrl[5].target_y,tmpball.x,tmpball.y) < TAC_DIST_ANTI)
		{
			//!!!��ʱ�޸ģ���������̫��
			m_tacctrl[5].target_y = tmpball.y - TAC_DIST_ANTI;
		}
	}
		
	//������������·������������·�ת
	if (m_trueball.y < 550)		//���ⲻ����Ϊ600�����������ĸ�������ʱ������Ķ�λ�ζ���ɵĻ���λ�����ػζ�
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

	//2010��ӷ��ض�Ա
	//Default_Anti_Ready_Add(inDistFB);

	//��ֹ�ƶ���������
	for (i=2;i<6;i++)
	{
		m_Fix_MoveOutOffFrame(&(m_tacctrl[i]));
	}

	//��ֹײ��
	m_Fix_CollisionAvoid(m_tacctrl, 50);

	//�Ƕ�,��������
	for (i=2;i<6;i++)
	{
		m_tacctrl[i].target_angle = m_Loca_CalAngle(m_tacctrl[i].target_x,m_tacctrl[i].target_y,m_trueball.x,m_trueball.y);
	}

	m_TacAction(m_tacagent);
}
//////////////////////////////////////////////////////////////////////////

void CUPFullField::Tac_Wait10Sec()
{
	printf("10���ʱ��ʼ...");
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
		Tac_ReleaseKickoff();	//���Կ���ʱ����
		printf("1\r\n1\r\n1213213213213213\r\n1\r\n");
		Active();	//10������������߿�ʼ
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
			//�Զ�ʹ��Playin
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
	//�µ�һ֡
	kong();
	CString info;
 	//info.Format("����%d ����%d D%d",m_nPassPlayer,m_nCatchPlayer,m_nPassEndDelay);
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
	
	//������Ա��Ϣ
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

	//û����Ա���ߣ�û��Ҫ���о���
	if (bsomeoneonline == false)
	{
		return;
	}
	if (m_isMidPasserCannotTakeBall==true)//û��Ҫͣס������ȥ����
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
				info.Format("����ת�����ˣ�������");
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
				info.Format("���Ѿ����߳���������");
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
				info.Format("�򱻶���,������������������");
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
	
	//1���Ҳ���������ľ���
	if (m_bFoundball == false && m_bTaskMidPassCatch==false)//TEST 713
	{
		if(m_bTaskMidPassCatch==true)
		{
	//	m_bTaskMidPassCatch==false;
		//m_bTaskMidPassCatch_kick==false;
		CString info;
		info.Format("�Ҳ�����\n");
		printf(info);
		}
		//ͳ�Ƴ�����Ա��Ŀ
		int nAgOnline = 0;
		for (int i=2;i<AG_NUM;i++)
		{
			if (m_tacagent[i].online == true)
			{
				nAgOnline ++;
			}
		}
		
		//�ؼ���ɫ����
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
	if ((DistToInitialHoldBallPoint>(TAC_DIST_3MeterPassJudge)) ) // ��ֹ���⣺ ����5���ڲ������3�״���
	{
			m_bTask3MeterPass=true;
	}
	else
	{
		m_bTask3MeterPass=false;
	}
	info.Format("�µ�һ֡%d,,%d,,%d\n",m_bTask3MeterPass,m_bTask3MeterPass_Kick,DistToInitialHoldBallPoint);
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
		info.Format("3�״���ɹ�\n");
		m_bClear3meterPassInfo();
		printf(info);
		return;
	}
	else if (m_bTask3MeterPass == true && m_bTask3MeterPass_Kick == true)
	{
		CString info;
		info.Format("���״�������˴��������������������������������������\n");
		printf(info);
		m_bTask3MeterPass == false;
		return;
	}
	else
	{
	
	//3 meter pass code ends here [wzc]*******************************************************
	//****************************************************************************************



	//if ((m_bTaskMidPassCatch==true) &&( m_bTaskMidPassCatch_kick==true) && m_bSomeoneHoldBall()==true )//[yzz]���ڽ����� Ӧ�û�ȥ����
	//{
	//	m_bTaskMidPassCatch==false;
	//	m_bTaskMidPassCatch_kick==false;
	//	m_nMidPassCatchBelieve_last = 0;
	//	CString info;
	//	info.Format("�г��������\n");
	//	printf(info);
	//}
	//else if ((m_bTaskMidPassCatch==true) &&( m_bTaskMidPassCatch_kick==true))
	//{
	//if (m_MidPasserCannotTakeBallCount<=0)
	//	{
	//		m_isMidPasserCannotTakeBall = true;
	//		info.Format("�����߲�׷��!!!!\n");
	//		printf(info);
	//	}
	//	
	//	//����������ɹ��������볡����Ա��Ҫ������,�����߼��� �ȴ����򣬲��������,������Ա��λ�� ��believe����10 ��ʱ�򣬼����ȴ�����>20��Ӧ����ƽ������ʱ�䣬20Ϊ����ֵ��ʱ����������
	//	//if (m_nMidPassCatchBelieve_last >= 10 )//(m_nMidPassCatchBelieve_last<20) mc
	//	{
	//		info.Format("�г������Ѿ����!!!!\n");
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
	//		info.Format("�г�����ʱ\n");
	//		printf(info);*/
	//		//���⴫����ȥ׷��??
	//	}
	//}
	//else if ((m_bTaskMidPassCatch==true) &&( m_bTaskMidPassCatch_kick==false))
	//{
	//	
	//	if (m_bSomeoneHoldBall()==false)
	//	{
	//		info.Format("m_bSomeoneHoldBall()=false\n");
	//		printf(info);
	//		//����������򱻶���
	//		m_bTaskMidPassCatch = false;
	//	}
	//	else
	//	{

	//	//CString info;
	//	info.Format("�г���������\n");
	//	printf(info);
	//	//�ȹ̶� ��Ѵ���㣬�������� �� m_posBestMidPass
	//	//���Դ��룬���������λ�ú��ʣ�believe+5,ģ�� ��������,���ڴ����߶��ԣ���ʱ ���� ��ʼ���ż���
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
	//    BestPassPoint(&px,&py,&believe);//believeĬ��Ϊ0��>=10 ��ʼ׼����Ŵ���
	//
	//	m_posBestMidPass_last.x = px;
	//	m_posBestMidPass_last.y = py;
	//	m_nMidPassCatchBelieve_last = believe;
	//	Tac_MidPass(TAC_DIST_DEFFAR,px,py,believe);
	//	//�����ҷ��볡ʱ��������ֽ�����Ա���ҷ����ѵ�״��
	//	m_Fix_GetOutOfTheWayOfATK();		

	//	info.Format("x=%d,y=%d;Believe = %d\n",px,py,m_nMidPassCatchBelieve_last);
	//	printf(info);

	//	return;
	//	}
	//} 
	
	//2������Ա�ܿ��������ж����λ�ã������ֵΪ���н��Ʒ��صķֽ�ֵ
	if (m_trueball.x < 900)//0��Ϊ900[yzz]
	{
		//�����ҷ��볡
		//�ж�����Ա�Ƿ���򣬱��������Ա������Ա������ν������
		bool bgk = m_bGoalKeeperHoldBall();
		if (bgk == false)
		{
			
			//2.1����Աδ����(�����ҷ��볡)
			if (m_bSomeoneHoldBall()==true)
			{
				CString info;
				info.Format("����Աδ����");
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
//				//��һ����˵���ҷ���Ա������
			//	DefaultActive(150);		//���ó�Զ��Ķ�������һЩ����
//				//Tac_ContainDef(150);		//����Χ�·���
//			
//				Tac_RoundedDef2(TAC_DIST_DEFFAR);		//ʹ��defence�ķ���
//				
//
////						Tac_RoundedDef3(TAC_DIST_DEFCLOSE);		//ʹ��defence�ķ���
//
//			}
			
		} 
		else
		{
			//2.2����Ա����ʱ�Ĳ���(�����ҷ��볡)
			m_tacctrl[1].ctrl = CTRL_GOALKEEP;


			//������Ա�ó�һ��·
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

		//�����ҷ��볡ʱ��������ֽ�����Ա���ҷ����ѵ�״��
		m_Fix_GetOutOfTheWayOfATK();
	} 
	else
	{
		
		/**************************���ڶԷ��볡***************************/
		//printf("�Է��볡");
		if (m_bSomeoneHoldBall() == true) // ��֯����
		{
			m_tacctrl[1].ctrl = CTRL_GOALKEEP;
			m_tacctrl[2].ctrl = CTRL_ATTACK;
			m_tacctrl[3].ctrl = CTRL_MOVETO;//wzc  ��ʱ׼���Ӵ�������
			
			//һ����Ա���ͣ���ʱ׼������   wzc
			//kong();
			m_tacctrl[3].target_x = kongx;
			m_tacctrl[3].target_y = kongy;
			m_tacctrl[3].target_angle = m_Loca_CalAngle(m_tacctrl[3].target_x,m_tacctrl[3].target_y,m_trueball.x,m_trueball.y);

			for(int i = 4; i<=AG_NUM;i++)
			{
				m_tacctrl[i].ctrl = CTRL_MOVETO;
			}

			//��̬վλ���� 
			CPoint tmp;
			tmp = m_Loca_Ray(0,600,m_trueball.x,m_trueball.y,TAC_DYNAMIC_DEFDIST*m_fKField);
			m_tacctrl[4].target_x = tmp.x;
			m_tacctrl[4].target_y = tmp.y - 200*m_fKField;
			m_tacctrl[4].target_angle = m_Loca_CalAngle(m_tacctrl[4].target_x,m_tacctrl[4].target_y,m_trueball.x,m_trueball.y);

			tmp = m_Loca_Ray(0,600,m_trueball.x,m_trueball.y,TAC_DYNAMIC_DEFDIST*m_fKField);
			m_tacctrl[5].target_x = tmp.x;
			m_tacctrl[5].target_y = tmp.y + 200*m_fKField;
			m_tacctrl[5].target_angle = m_Loca_CalAngle(m_tacctrl[5].target_x,m_tacctrl[5].target_y,m_trueball.x,m_trueball.y);

			//���򵽶Է�������ʱ��������ζ�Ҫ�����ض�Ա������
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
		{  // �ҷ����˳���  �� ��һ������ƴ��  wzc
			m_tacctrl[1].ctrl = CTRL_GOALKEEP;
			m_tacctrl[2].ctrl = CTRL_ATTACK;
			m_tacctrl[3].ctrl = CTRL_ATTACK;
			

			//������Ա����   wzc
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
		
	
		


			//���򵽶Է�������ʱ��������ζ�Ҫ�����ض�Ա������
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
	printf("�������߻��Ƽ��");
	m_strCurSt.Format("��������״̬");
}

void CUPFullField::Disactive()
{
	m_eStage = sStop;
	m_bClear3meterPassInfo();
	m_bTask3MeterPass=false;
	printf("����������ͣ");
}

void CUPFullField::AChangeStatus(int inID, UINT inCtrlCode,double inAngle, int inDist)
{
	if (inID<0 || inID>6)
	{
		return;
	}
// 	if (inCtrlCode == CTRL_ATTENTIONBALL)
// 	{
// 		m_strTmp.Format("%d�ŷ���Ϊ�۲��ɫ",inID);
// 		printf(m_strTmp);
// 	}
// 	CString str;
// 		str.Format("%d�Ż�  ��Ϊ%d",inID,inCtrlCode);
// 	printf(str);

	if (pToAgent[inID] != NULL)
	{
		//���ؽǶȼ�¼����
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
	//Ѱ��Դindex
	int srcindex = 0;
	for (int i=1;i<7;i++)
	{
		if (inList[i].ID == inID)
		{
			srcindex = i;
		}
	}
	//Ѱ��Ŀ��index
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
	/*����汾/////////////////////////////////////////////////////////////////////////
	//���ҵ�Ŀ��λ��
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

	//��Դλ��
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

	//��ʼ���н���
	uagent tempa = *target;		//target -> tempa

	*target = *src;				//src -> target
	target->next = tempa.next;	//�ָ�������ϵ
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
			//����Ա�޳�
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
	//���ֹ��ϵ�δ�������Ч����ͣ���������ľ������
	if (inagent->status == ROBST_ERR && inagent->dist_ball != 0)
	{
		return 10000;
	}

	//�Ի���������ľ�����Ϊ��Ч���˻�����
	int ret = inagent->dist_ball;

	//֮ǰ�����������ɫ�Ļ��������Ŵ��������ɱ��ֽ�����ɫ��������
	if (inagent->status == CTRL_ATTACK /*&& m_nSWAtkDly[inagent->ID] > ATK_SW_DLY-1*/)
	{
		ret -= 200;
	}

	if (inagent->status == CTRL_CATCH)
	{
		ret -= 1000;
	}

	//0�Ƕȷ��ض�ǹ�۵Ķ�Ա������ת��
	if (inagent->status == CTRL_DEFENCE && inagent->logDefAngle == 0)
	{
		ret += 300;
	}

	//�Ծ���վλ���ƵĶ�Ա�Ŵ�(�н�)(��������Ļ�������Ҫ���Ǵ���)
	int ang = (int)m_CalAngle(inagent->x,inagent->y,m_trueball.x,m_trueball.y,1500,600);
	ret -= (int)(ang*0.5);

	//��������Ķ�Ա����Ч���½�
// 	if (inagent->foundball == false)
// 	{
// 		ret += 500;
// 	}

	//����Ч�ò��������ݻ�����λ�ñ仯������ж����Ƿ����
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
			strC.Format("mid����");
			break;
		case CTRL_MID_CATCH:
			strC.Format("mid����");
			break;
		case CTRL_ATTACK:
			strC.Format("����");
			break;
		case CTRL_STOP:
			strC.Format("ֹͣ");
			break;
		case CTRL_MOVETO:
			strC.Format("�ƶ�");
			break;

		case CTRL_GOALKEEP:
			strC.Format("����Ա");
			break;

		case CTRL_PRODEF:
			strC.Format("�߼�����");
			break;

		case CTRL_ATTENTIONBALL:
			strC.Format("��۲�Ա");
			break;

		case CTRL_SHIFTATK:
			strC.Format("������λ");
			break;

		case CTRL_DEFENCE:
			strC.Format("����");
			break;

		case CTRL_PASS:
			strC.Format("����");
			break;

		case CTRL_3M_PASS:
			strC.Format("���洫��");
			break;

		case CTRL_3M_CATCH:
			strC.Format("�������");
			break;

		case CTRL_CATCH:
			strC.Format("����");
			break;

		case CTRL_IDLE:
			strC.Format("����");
			break;

		//��λ
		case CTRL_KICKOFF_PRIM_READY:
			strC.Format("����������λ");
			break;

		case CTRL_KICKOFF_SLAVE_READY:
			strC.Format("����������λ");
			break;
		
		case CTRL_KICKOFF_PRIM:
			strC.Format("��������");
			break;

		case CTRL_KICKOFF_SLAVE:
			strC.Format("��������");
			break;

		case CTRL_SEARCHBALL:
			strC.Format("����");
			break;

		case ROBST_ERR:
			strC.Format("����״̬");
			break;

			case CTRL_FOLLOWPOSITION:
			strC.Format("�������״̬");
			break;
			
		default:
			strC.Format("δ֪");
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
		m_strCurSt.Format("�ҷ�Ԥ������...");
		//2010��ʹ��moveto����
		m_tacctrl[1].ctrl = CTRL_GOALKEEP;
		//������Ա�ܶ���ָ����
		for (int i=2;i<6;i++)
		{
			m_tacctrl[i].ctrl = CTRL_MOVETO;
			m_tacctrl[i].target_angle = 0;	
		}

		CPoint tmpball = m_trueball;
		
		CPoint posCatch;
		CPoint posPass;
		
		{
			//�����Ա-��-�Է����� = -120��
			CPoint tmp = m_Loca_Ray(tmpball.x,tmpball.y,1600,600,(int)(100*m_fKField));	//���һ�������Ǿ���
			tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,-92);
			m_tacctrl[2].target_x = tmp.x;
			m_tacctrl[2].target_y = tmp.y;
			m_tacctrl[2].target_angle = -60;//m_Loca_CalAngle(tmp.x,tmp.y,tmpball.x,tmpball.y);		
			posPass.x = tmp.x;
			posPass.y = tmp.y;
			
			//���������Ա-��-�Է����� = 120��
			tmp = m_Loca_Ray(tmpball.x,tmpball.y,1600,600,(int)(250*m_fKField));			//���һ�������Ǿ���
			tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,88);
			m_tacctrl[3].target_x = tmp.x;
			m_tacctrl[3].target_y = tmp.y;
			m_tacctrl[3].target_angle = m_Loca_CalAngle(tmp.x,tmp.y,tmpball.x,tmpball.y);
			posCatch.x = tmp.x;
			posCatch.y = tmp.y;
		}
		
		//�����ܶ�λ�ã����ⲻ������ľ���̫��
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
		//ָ��������ID
		//m_nCatchPlayer = m_Ag_GetNearBy(m_tacctrl[3].target_x,m_tacctrl[3].target_y,m_tacagent);	
		m_nCatchPlayer = m_Ag_GetNearBy(posCatch.x,posCatch.y,m_tacagent);
		m_nPassPlayer = m_Ag_GetNearBy(posPass.x,posPass.y,m_tacagent);
		//DefaultReady();
		//debug
		CString strTmp;
		strTmp.Format(
			"����%d(%d,%d) ����%d(%d,%d)",
			m_nPassPlayer,posPass.x,posPass.y,
			m_nCatchPlayer,posCatch.x,posCatch.y
			);
		printf(strTmp);

		m_TacAction(m_tacagent);		
	} 
	else
	{
		m_strCurSt.Format("�ҷ�Ԥ��������ʧ��");
		//������λ
		m_tacctrl[1].ctrl = CTRL_GOALKEEP;

		//������Ա�ܶ���ָ����
		for (int i=2;i<7;i++)
		{
			m_tacctrl[i].ctrl = CTRL_MOVETO;
			m_tacctrl[i].target_angle = 0;	
		}

		//�����ܶ�λ��
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
		m_strCurSt.Format("�ҷ�Ԥ��������...");
		DefaultReady();
	} 
	else
	{
		m_strCurSt.Format("�ҷ�Ԥ��������ʱ����������...");
		//�ҷ�����ʱ��������
		m_tacctrl[1].ctrl = CTRL_GOALKEEP;

		//������Ա�ܶ���ָ����
		for (int i=2;i<7;i++)
		{
			m_tacctrl[i].ctrl = CTRL_MOVETO;
			m_tacctrl[i].target_angle = 0;	
		}
		//�ؼ���ɫ	
		m_tacctrl[2].ctrl = CTRL_SEARCHBALL;

		m_tacctrl[3].target_x = m_trueball.x - 100;
		m_tacctrl[3].target_y = m_trueball.y;
		m_tacctrl[3].target_angle = 0;

		//�����ܶ�λ��
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
		m_strCurSt.Format("�ҷ�Ԥ��������...");
		DefaultReady();
	} 
	else
	{	
		m_strCurSt.Format("�ҷ�Ԥ����������ʧ��");
		//�ҷ�������ʱ��������
		m_tacctrl[1].ctrl = CTRL_GOALKEEP;

		//������Ա�ܶ���ָ����
		for (int i=2;i<7;i++)
		{
			m_tacctrl[i].ctrl = CTRL_MOVETO;
			m_tacctrl[i].target_angle = 0;	
		}
		//�ؼ���ɫ
		m_tacctrl[2].target_x = 200;
		m_tacctrl[2].target_y = 300;
		m_tacctrl[2].target_angle = 0;

		m_tacctrl[3].target_x = 200;
		m_tacctrl[3].target_y = 900;
		m_tacctrl[3].target_angle = 0;
		
		m_tacctrl[4].ctrl = CTRL_SEARCHBALL;

		//��Ҫ��ɫ
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
		m_strCurSt.Format("�ҷ�Ԥ��������...");
		DefaultReady();
	} 
	else
	{
		m_strCurSt.Format("�ҷ�Ԥ����������ʧ��");
		//�ҷ�������ʱ��������
		m_tacctrl[1].ctrl = CTRL_GOALKEEP;

		//������Ա�ܶ���ָ����
		for (int i=2;i<7;i++)
		{
			m_tacctrl[i].ctrl = CTRL_MOVETO;
			m_tacctrl[i].target_angle = 0;	
		}
		//�ؼ���ɫ	
		m_tacctrl[2].ctrl = CTRL_SEARCHBALL;

		m_tacctrl[3].target_x = m_trueball.x-100;
		m_tacctrl[3].target_y = m_trueball.y;
		m_tacctrl[3].target_angle = 0;

		//�����ܶ�λ��
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
		m_strCurSt.Format("�ҷ�Ԥ������...");
		DefaultReady();
	} 
	else
	{
		printf("������λʱ��ʧ�٣�");

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
	m_strCurSt.Format("�ҷ�Ԥ������...");
	//m_tacctrl[1].ctrl = CTRL_GOALKEEP;
	m_tacctrl[1].ctrl = CTRL_MOVETO;
	m_tacctrl[1].target_x = 10;
	m_tacctrl[1].target_y = 600;
	m_tacctrl[1].target_angle = 0;
	
	m_tacctrl[2].ctrl = CTRL_MOVETO;
	m_tacctrl[2].target_x = 900;
	m_tacctrl[2].target_y = 650;
	m_tacctrl[2].target_angle = 0;

	//������Ա�ܶ���ָ����
	for (int i=3;i<7;i++)
	{
		m_tacctrl[i].ctrl = CTRL_STOP;
		m_tacctrl[i].target_angle = 90;	
	}
	//����

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
		m_strCurSt.Format("Ԥ������...");
		//////////////////////////////////////////////////////////////////////////
		//09�����
		DropBall_Ready(TAC_DIST_DROPBALL);

		/*/////////////////////////////////////////////////////////////////////////
		// 08�����
		//�Է�������λ
		m_tacctrl[1].ctrl = CTRL_GOALKEEP;

		m_tacctrl[2].ctrl = CTRL_DEFENCE;
		m_tacctrl[2].def_dist = TAC_DIST_DROPBALL;
		m_tacctrl[2].def_angle = 0;

		if (m_trueball.x < 1000)
		{
			if (m_trueball.y > 600)
			{
				//����
				//������Ա���ϱ���Χ��̬��
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
				//����
				//������Ա���±���Χ��̬��
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

			//������Ա�ܶ���ָ����
			for (int i=4;i<7;i++)
			{
				m_tacctrl[i].ctrl = CTRL_MOVETO;
				m_tacctrl[i].target_angle = 0;	
			}

			//�����ܶ�λ��
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
		m_strCurSt.Format("Ԥ��������ʧ��...");
		m_tacctrl[1].ctrl = CTRL_GOALKEEP;

		//������Ա�ܶ���ָ����
		for (int i=2;i<7;i++)
		{
			m_tacctrl[i].ctrl = CTRL_MOVETO;
			m_tacctrl[i].target_angle = 0;	
		}
		//�ؼ���ɫ
		m_tacctrl[2].ctrl = CTRL_SEARCHBALL;

		m_tacctrl[3].target_x = m_trueball.x-100;
		m_tacctrl[3].target_y = m_trueball.y;	

		//�����ܶ�λ��
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
		m_strCurSt.Format("�Է�Ԥ������...");
		//////////////////////////////////////////////////////////////////////////
		//2010��
		m_tacctrl[1].ctrl = CTRL_GOALKEEP;
		
		for (int i=2;i<7;i++)
		{
			m_tacctrl[i].ctrl = CTRL_MOVETO;
		}
		
		//������λ��
		CPoint tmpball = m_trueball;
		
		CPoint tmp;	

		//��������֮�����һ��
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
		
		//����1
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
		
		//����
		m_tacctrl[4].target_x = m_Pos_Back.x-150;
		m_tacctrl[4].target_y = m_Pos_Back.y;
		
		//����2
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
		/*/����������������Ϊ����������ӷ��ضԷ���Ա������	
		MSG_Control defctrl[2];	//��������������Է���Ա 
		defctrl[0].target_x = defctrl[1].target_x =900;	
		defctrl[0].target_y = defctrl[1].target_y =600;

		memcpy(m_tmpObst,m_tacObst,(EACH_AG_OBST_NUM*(AG_NUM+1))*sizeof(obstacle));
		int nearobst = -1;		
		//��������������ؿ��ܽ���ĶԷ���Ա��
		int flag = 0;//0��û���ҵ����ϰ��1����һ���ϰ���λ��ƫ�ϣ�2����һ���ϰ���λ��ƫ��
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
					//��ȡ��һ���ϰ���鿴������ľ���
					if (m_CalDist(m_tmpObst[nearobst].x,m_tmpObst[nearobst].y,m_trueball.x,m_trueball.y) < 150 || m_tmpObst[nearobst].x > 1200)
					{
						//���������С��2�ף�Ӧ����׼������Ķ�Ա������
						m_tmpObst[nearobst].valid = false;
						nearobst = -1;		//��ѭ�����Լ���
					}
					else
					{
						if (m_CalDist(m_tmpObst[nearobst].x,m_tmpObst[nearobst].y,1800,600) < 200 )
						{
							//����Է����źܽ����п����ǶԷ�����Ա��������Ҫ��
							m_tmpObst[nearobst].valid = false;
							nearobst = -1;		//��ѭ�����Լ���
							continue;
						}

						//�鿴��һ���ϰ����Ƿ����������Ĵ���ͬһ��
						if (flag == 1 && m_tmpObst[nearobst].y > 600)
						{
							m_tmpObst[nearobst].valid = false;
							nearobst = -1;		//��ѭ�����Լ���
							continue;
						}
						if (flag == 2 && m_tmpObst[nearobst].y < 600)
						{
							m_tmpObst[nearobst].valid = false;
							nearobst = -1;		//��ѭ�����Լ���
							continue;
						}
						//����Ҫ��
						break;
					}
				}
				else
				{
					//�������-1��˵��û���ϰ���
					break;
				}
			}
			
			CPoint tmpPnt;
			if (nearobst != -1)
			{
				//���ϰ�����������1��
				tmpPnt = m_Loca_Ray(m_tmpObst[nearobst].x,m_tmpObst[nearobst].y,m_trueball.x,m_trueball.y,100);

				//�ҷ��볡��Ӧλ��
				tmpPnt.x = 800;
				
				//���������̫��������������
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

				//���ϰ����Ѿ������أ��ر���ʹ��
				m_tmpObst[nearobst].valid = false;
			}		
		}

		//�Ƕ�
		for (i=2;i<6;i++)
		{
			m_tacctrl[i].target_angle = m_Loca_CalAngle(m_tacctrl[i].target_x,m_tacctrl[i].target_y,tmpball.x,tmpball.y);
		}*/

		//////////////////////////////////////////////////////////////////////////
		//09��ϲ���һ����Ϊ����
		//Default_Anti_Ready(TAC_DIST_ANTI+20);
		//////////////////////////////////////////////////////////////////////////

	} 
	else
	{
		m_strCurSt.Format("�Է�Ԥ��������ʧ��");
		//�Է������ҷ�ȴ��������̫���ˡ�����
		m_tacctrl[1].ctrl = CTRL_GOALKEEP;

		//������Ա�ܶ���ָ����
		for (int i=2;i<7;i++)
		{
			m_tacctrl[i].ctrl = CTRL_MOVETO;
			m_tacctrl[i].target_angle = 0;	
		}
		//�ؼ���ɫ
		m_tacctrl[2].target_x = 700;
		m_tacctrl[2].target_y = 800;
		m_tacctrl[2].target_angle = 0;
		
		m_tacctrl[3].target_x = 700;
		m_tacctrl[3].target_y = 400;
		m_tacctrl[3].target_angle = 0;

		//�����ܶ�λ��
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
		m_strCurSt.Format("�Է�Ԥ��������...");
		//////////////////////////////////////////////////////////////////////////
		//09��ϲ���һ����Ϊ����
		Default_Anti_Ready(TAC_DIST_ANTI);

		/*/////////////////////////////////////////////////////////////////////////
		//08��
		//�����ж����λ��
		if (m_trueball.x < 1000)
		{
			//�����ҷ��볡
			m_tacctrl[1].ctrl = CTRL_GOALKEEP;

			if (m_CalDist(m_trueball.x,m_trueball.y,0,600) > TAC_DIST_ANTI+250)
			{
				//������Ա��λ�л���Χ��̬��
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
				//������Ա������λ
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
			//���ڶԷ��볡
			m_tacctrl[1].ctrl = CTRL_GOALKEEP;

			m_tacctrl[2].ctrl = CTRL_DEFENCE;
			m_tacctrl[2].def_dist = TAC_DIST_ANTI;
			m_tacctrl[2].def_angle = 0;

			//������Ա�ܶ���ָ����
			for (int i=3;i<7;i++)
			{
				m_tacctrl[i].ctrl = CTRL_MOVETO;
				m_tacctrl[i].target_angle = 0;
			}

			//�����ܶ�λ��
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
		m_strCurSt.Format("�Է�Ԥ����������ʧ��");
		//�Է�Ԥ��������ʱ��������
		m_tacctrl[1].ctrl = CTRL_GOALKEEP;

		//������Ա�ܶ���ָ����
		for (int i=2;i<7;i++)
		{
			m_tacctrl[i].ctrl = CTRL_MOVETO;
			m_tacctrl[i].target_angle = 0;	
		}
		//�ؼ���ɫ	
		m_tacctrl[2].ctrl = CTRL_SEARCHBALL;

		m_tacctrl[3].target_x = m_trueball.x-100;
		m_tacctrl[3].target_y = m_trueball.y;
		m_tacctrl[3].target_angle = 0;

		//�����ܶ�λ��
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
		m_strCurSt.Format("�Է�Ԥ��������...");
		Default_Anti_Ready(TAC_DIST_ANTI);
	} 
	else
	{
		m_strCurSt.Format("�Է�Ԥ��������δ������");
		m_tacctrl[1].ctrl = CTRL_GOALKEEP;

		//������Ա�ܶ���ָ����
		for (int i=2;i<7;i++)
		{
			m_tacctrl[i].ctrl = CTRL_MOVETO;
			m_tacctrl[i].target_angle = 0;	
		}
		//�ؼ���ɫ���ܵ��Է����Ÿ���
		m_tacctrl[2].target_x = 1400;
		m_tacctrl[2].target_y = 400;
		
		m_tacctrl[3].target_x = 1400;
		m_tacctrl[3].target_y = 800;

		//�����ܶ�λ��
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
		m_strCurSt.Format("�Է�Ԥ��������...");//�Է�������λ
		//////////////////////////////////////////////////////////////////////////
		//09��ϲ���һ����Ϊ����
		Default_Anti_Ready(TAC_DIST_ANTI);

		/*/////////////////////////////////////////////////////////////////////////
		//08��
		m_tacctrl[1].ctrl = CTRL_GOALKEEP;

		m_tacctrl[2].ctrl = CTRL_DEFENCE;
		m_tacctrl[2].def_dist = TAC_DIST_ANTI+100;
		m_tacctrl[2].def_angle = 0;

		if (m_trueball.x > 1000)
		{
			//���ڶԷ��볡
			
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

			//������Ա�ܶ���ָ����
			for (int i=4;i<7;i++)
			{
				m_tacctrl[i].ctrl = CTRL_MOVETO;
				m_tacctrl[i].target_angle = 0;	
			}

			//�����ܶ�λ��
			m_tacctrl[6].target_x = 600;
			m_tacctrl[6].target_y = 700;

			m_tacctrl[4].target_x = 600;
			m_tacctrl[4].target_y = 300;

			m_tacctrl[5].target_x = 600;
			m_tacctrl[5].target_y = 900;
		} 
		else
		{
			//���ڼ����볡
			if (m_trueball.y > 600)
			{
				//�����ϱ߽�
				//������Ա���ϱ���Χ��̬��
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
				//�����±߽�
				//������Ա���±���Χ��̬��
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
		m_strCurSt.Format("�Է�Ԥ��������δ������");
		m_tacctrl[1].ctrl = CTRL_GOALKEEP;

		//������Ա�ܶ���ָ����
		for (int i=2;i<7;i++)
		{
			m_tacctrl[i].ctrl = CTRL_MOVETO;
			m_tacctrl[i].target_angle = 0;	
		}
		//�ؼ���ɫ		
		m_tacctrl[2].ctrl = CTRL_SEARCHBALL;

		m_tacctrl[3].target_x = m_trueball.x-100;
		m_tacctrl[3].target_y = m_trueball.y;

		//�����ܶ�λ��
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
		m_strCurSt.Format("�Է�Ԥ������...");
		//////////////////////////////////////////////////////////////////////////
		//09��ϲ���һ����Ϊ����
		Default_Anti_Ready(TAC_DIST_ANTI);

		/*/////////////////////////////////////////////////////////////////////////
		//08��
		m_tacctrl[1].ctrl = CTRL_GOALKEEP;

		//������Ա��λ�л���Χ��̬��
		for (int i=2;i<7;i++)
		{
			m_tacctrl[i].ctrl = CTRL_DEFENCE;
		}

		if (m_trueball.y > 600)
		{
			//�����ϱ߽�
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
			//�����±߽�
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
		m_strCurSt.Format("�Է�Ԥ��������ʧ��");
		//���������ʱ��
		m_tacctrl[1].ctrl = CTRL_GOALKEEP;

		//������Ա�ܶ���ָ����
		for (int i=2;i<7;i++)
		{
			m_tacctrl[i].ctrl = CTRL_MOVETO;
			m_tacctrl[i].target_angle = 0;	
		}
		//�����ؼ���ɫ���򼺷�������
		m_tacctrl[2].target_x = 200;
		m_tacctrl[2].target_y = 100;
	
		m_tacctrl[3].target_x = 200;
		m_tacctrl[3].target_y = 1100;

		//���������ܶ�λ��
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
	m_strCurSt.Format("�Է�Ԥ������...");
	KillTimer(TAC_WAIT_TIMER);
	//������Ա�ܶ���ָ����
	for (int i=1;i<7;i++)
	{
		m_tacctrl[i].ctrl = CTRL_MOVETO;
		m_tacctrl[i].target_angle = 90;	
	}
	m_tacctrl[1].target_x = 10;
	m_tacctrl[1].target_y = 600;
	m_tacctrl[1].target_angle = 0;	

	//�ܵ��м�
	m_tacctrl[2].target_x = 500;
	m_tacctrl[2].target_y = 100;

	//����

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
	m_strCurSt.Format("��������");

	m_nDistOfMultiDef = inDist;
	//m_eStage = sTestMultiDef;
	m_eStage = sMani;
	Tac_DefTest(m_nDistOfMultiDef);
}

void CUPFullField::Test_PressDef()
{
	m_strCurSt.Format("���Ʒ��ز���");

	m_eStage = sTestPressDef;
	Tac_PressDef(350);
}

void CUPFullField::m_CheckAgentReply()
{
	for (int i=1;i<7;i++)
	{
		if (m_recvcnt[i] > 0)
		{
			//���ݰ��������أ�������
			m_recvcnt[i] = 0;
		} 
		else
		{
			//���ݰ��Ѿ��������ˣ������߳����߿��Ƿ�Χ
			KickAgent(i);
		}
	}
}

void CUPFullField::m_ShowAgentsInfo()
{
	//���������ָ��
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

			//״̬
			str = GetCTRLString(m_agents[i].status);
			pAgInfo->SetItemText(i-1,1,str);
			
			//����λ��
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
				str.Format("��Ұ������");
			}
			pAgInfo->SetItemText(i-1,4,str);

			//���ݰ�����
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
			//����agent��״̬��ʾ
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
	m_strCurSt.Format("�������볡...");
	//���ƻ�������λ
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

	//�Ƕ�,��������
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
	m_strCurSt.Format("�������볡...");
	//���ƻ�������λ	
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
	
	//�Ƕ�,��������
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

//���Ҿ���x��y�����ag
int CUPFullField::m_Ag_GetNearBy(int x, int y, uagent* inalist)
{
	int ret = 0;
	int mindist = 0x7fffffff;
	int tmp_dist = 0;

	bool scanerr = true;	//����Ƿ��б�Ҫ��err��ag����ɨ��
	//��ɨ�轡����ag
	for (int i=2;i<7;i++)
	{
		if (inalist[i].online == true && inalist[i].allocated == false && inalist[i].status != ROBST_ERR)
		{
			//������ag�����ˣ�����Ҫ��err��ag����ɨ����
			scanerr = false;

			//�жϸý�����ag����
			tmp_dist = m_CalDist(inalist[i].x,inalist[i].y,x,y);
			if (tmp_dist < mindist)
			{
				ret = i;
				mindist = tmp_dist;
			}
		}
	}

	//�������ag�Ѿ�����ѡ�����ʣ���ag����ѡ
	if (scanerr == true)
	{
		for (int i=1;i<7;i++)
		{
			if (inalist[i].online == true && inalist[i].allocated == false)
			{
				//�жϸ�err��ag����
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

	//���ǶȻ������ֵ
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
		//��ĸ��Ϊ��
		int dx = inXtarg - inXbase;
		double dy = inYtarg - inYbase;
		retAngle = atan(dy/dx);
		
		//����ɽǶ�
		retAngle = retAngle*180/3.14158;

		//���޵�����
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

		//����
		while (retAngle < 0)
		{
			retAngle += 360;
		}
		while (retAngle > 360)
		{
			retAngle -= 360;
		}

		//����
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
/*-----------------------------��ͨ����ģʽ-----------------------------*/
void CUPFullField::DefaultActive(int inDefDist)
{
	//printf("DefaultActive");

	//��ʼ�����ƽṹ�壬����ɫ�ؼ��̶�����
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;
	
	//������ɫ
	m_tacctrl[2].ctrl = CTRL_ATTACK;
	
	//3~5�Ŷ�Ա��λ�л���Χ��̬��
	for (int i=3;i<6;i++)
	{
		m_tacctrl[i].ctrl = CTRL_MOVETO;
	}

	//�ȼ�����λ�������Ϸ�������λ��
	CPoint tmpball = m_trueball;
	if (m_trueball.y < 600)
	{
		tmpball.y = 1200 - tmpball.y;
	}
	
 	CPoint tmp;

	//���潫���ط�Ϊ�������������򣬸��������������γɷ�������
	if (tmpball.y > 1000)
	{
		//�ϲ�
		//����������֮�����һ��
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)(inDefDist*m_fKField));
		m_tacctrl[3].target_x = tmp.x;
		m_tacctrl[3].target_y = tmp.y;

		//���ذ뾶�ȶ����ŵĴ�һЩ�����⻻λ��ʱ����ײ
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)((inDefDist+150)*m_fKField));
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,-30);
		m_tacctrl[4].target_x = tmp.x;
		m_tacctrl[4].target_y = tmp.y;

		//����
		m_tacctrl[5].target_x = m_Pos_Back.x;
 		m_tacctrl[5].target_y = m_Pos_Back.y+300;
		
		//test
// 		tmp = m_Loca_CalBackPos(tmpball.x,tmpball.y,m_Pos_Back.x);
// 		m_tacctrl[4].target_x = tmp.x;
// 		m_tacctrl[4].target_y = tmp.y;
	}
	else
	{
		// tmpball.y < 1000 �м�����
		//����������֮�����һ��
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)(inDefDist*m_fKField));
		m_tacctrl[3].target_x = tmp.x;
		m_tacctrl[3].target_y = tmp.y;
	
		
		//����ĳ���
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)((inDefDist+150)*m_fKField));
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,30);
		m_tacctrl[4].target_x = tmp.x;
		m_tacctrl[4].target_y = tmp.y;

		//����
		m_tacctrl[5].target_x = m_Pos_Back.x;
		m_tacctrl[5].target_y = m_Pos_Back.y;
		
		
		//test
// 		tmp = m_Loca_CalBackPos(tmpball.x,tmpball.y,m_Pos_Back.x);
// 		m_tacctrl[4].target_x = tmp.x;
// 		m_tacctrl[4].target_y = tmp.y;

	}

	//printf("����Լ���Ա�������������");
	//CString tStr;
	//����Լ���Ա�������������
	for (i=3;i<6;i++)
	{
		if (m_Check_MovetoOurPenaltyArea(m_tacctrl[i]) == true)
		{
			//tStr.Format("%d�Ž�ɫ�����������Ҫ����",i);
			//printf(tStr);
			m_Fix_MovetoOurPenaltyArea(&m_tacctrl[i]);
			//m_tacctrl[i].target_x = 900;
		}
	}

	//����Լ���Ա���߽��������	
	for (i=3;i<6;i++)
	{
		if (m_Check_MoveOutOffFrame(m_tacctrl[i]) == true)
		{
			m_Fix_MoveOutOffFrame(&m_tacctrl[i]);
		}
	}

	//��ֹײ��
	m_Fix_CollisionAvoid(m_tacctrl, 50);

	//������������·������������·�ת
	if (m_trueball.y < 600)
	{
		m_tacctrl[2].target_y = 1200 - m_tacctrl[2].target_y;
		m_tacctrl[3].target_y = 1200 - m_tacctrl[3].target_y;		
		m_tacctrl[4].target_y = 1200 - m_tacctrl[4].target_y;
		m_tacctrl[5].target_y = 1200 - m_tacctrl[5].target_y;
	}

	//�Ƕ�,��������
	for (i=3;i<6;i++)
	{
		m_tacctrl[i].target_angle = 0;		//�ǶȲ��仯��ƽ�Ƶ�ʱ���ȶ�һЩ
		//m_tacctrl[i].target_angle = m_Loca_CalAngle(m_tacctrl[i].target_x,m_tacctrl[i].target_y,m_trueball.x,m_trueball.y);
	}

	m_TacAction(m_tacagent);
}
//////////////////////////////////////////////////////////////////////////
//------------------------------�������---------------------------------
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

	
	

	//����Լ���Ա�������������
	for (i=4;i<6;i++)
	{
		if (m_Check_MovetoOurPenaltyArea(m_tacctrl[i]) == true)
		{
			m_Fix_MovetoOurPenaltyArea(&m_tacctrl[i]);
		}
	}

	//����Լ���Ա���߽��������	
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
/*-----------------------------���Ʒ���---------------------------------*/
void CUPFullField::Tac_PressDef(int inDefDist)
{
	//printf("Tac_PressDef");
	//��ʼ�����ƽṹ�壬����ɫ�ؼ��̶�����
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;
	
	//�ж�һ���Ƿ���Ҫ����
// 	if (m_bSomeoneHoldBall() == true)
// 	{
// 		//�Ѿ������������л�����ͨ����ģʽ
// 		DefaultActive(150);
// 		return;
// 	}

	//����ģʽ�����������Ա
	m_tacctrl[2].ctrl = CTRL_ATTACK;
	m_tacctrl[3].ctrl = CTRL_ATTACK;
	
	//������Ա��λ�л���Χ��̬��
	for (int i=3;i<6;i++)
	{
		m_tacctrl[i].ctrl = CTRL_MOVETO;
	}
	
	//�ȼ�����λ�������Ϸ�������λ��
	CPoint tmpball = m_trueball;
	/*if (m_trueball.y < 600)
	{
		tmpball.y = 1200 - tmpball.y;
	}*/
	
	CPoint tmp;
	
	//���潫���ط�Ϊ�������������򣬸��������������γɷ�������
	//if (tmpball.y > 900)
	{
		//�ϲ�
		//����������֮�����һ��
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)(80*m_fKField));
		m_tacctrl[3].target_x = tmp.x;
		m_tacctrl[3].target_y = tmp.y;
		m_tacctrl[3].target_angle = m_Loca_CalAngle(m_tacctrl[3].target_x,m_tacctrl[3].target_y,m_trueball.x,m_trueball.y);

		//����
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


		//����ĳ���
// 		CPoint longtmp = m_Loca_Ray(tmpball.x,tmpball.y,m_tacctrl[4].target_x,m_tacctrl[4].target_y,(inDefDist+100)*m_fKField);
// 		tmp = m_Loca_Rotate(longtmp.x,longtmp.y,tmpball.x,tmpball.y,-30);
// 		m_tacctrl[5].target_x = tmp.x;
//		m_tacctrl[5].target_y = tmp.y;
		
// 		tmp = m_Loca_Rotate(m_tacctrl[4].target_x,m_tacctrl[4].target_y,tmpball.x,tmpball.y,30);
// 		m_tacctrl[6].target_x = tmp.x;
// 		m_tacctrl[6].target_y = tmp.y;
	}
	

	//����Լ���Ա�������������
	for (i=3;i<6;i++)
	{
		if (m_Check_MovetoOurPenaltyArea(m_tacctrl[i]) == true)
		{
			m_Fix_MovetoOurPenaltyArea(&m_tacctrl[i]);
		}
	}

	//����Լ���Ա���߽��������	
	for (i=3;i<6;i++)
	{
		if (m_Check_MoveOutOffFrame(m_tacctrl[i]) == true)
		{
			m_Fix_MoveOutOffFrame(&m_tacctrl[i]);
		}
	}
	
	//������������·������������·�ת
	/*if (m_trueball.y < 600)
	{
		m_tacctrl[2].target_y = 1200 - m_tacctrl[2].target_y;
		m_tacctrl[3].target_y = 1200 - m_tacctrl[3].target_y;		
		m_tacctrl[4].target_y = 1200 - m_tacctrl[4].target_y;
		m_tacctrl[5].target_y = 1200 - m_tacctrl[5].target_y;
	}*/
	
	//�Ƕ�,��������
	for (i=3;i<6;i++)
	{
		//m_tacctrl[i].target_angle = m_Loca_CalAngle(m_tacctrl[i].target_x,m_tacctrl[i].target_y,m_trueball.x,m_trueball.y);
		m_tacctrl[i].target_angle = 0;
	}
	
	m_TacAction(m_tacagent);
}

//////////////////////////////////////////////////////////////////////////
/*-----------------------------Χ�·���---------------------------------*/
void CUPFullField::Tac_ContainDef(int inDefDist)
{
	//printf("Tac_ContainDef");
	//��ʼ�����ƽṹ�壬����ɫ�ؼ��̶�����
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;

	//����һ����������Ķ�Ա
	m_tacctrl[2].ctrl = CTRL_ATTACK;
	
	//������Ա��λ�л���Χ��̬��
	for (int i=3;i<6;i++)
	{
		m_tacctrl[i].ctrl = CTRL_MOVETO;
	}
	
	//�ȼ�����λ�������Ϸ�������λ��
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

	//��������Ա����һ���������ֹ�Է���Χת��ʱ�Ʒ�����ʱ
	m_tacctrl[5].target_x = 300;
	m_tacctrl[5].target_y = 300;

//	CString strtmp;
	//����Լ���Ա�������������
	for (i=3;i<6;i++)
	{
		if (m_Check_MovetoOurPenaltyArea(m_tacctrl[i]) == true)
		{
			m_Fix_MovetoOurPenaltyArea(&m_tacctrl[i]);
		}
	}

	//����Լ���Ա���߽��������	
	for (i=3;i<6;i++)
	{
		if (m_Check_MoveOutOffFrame(m_tacctrl[i]) == true)
		{
// 			strtmp.Format("%d�Ų��Գ���",i);
// 			printf(strtmp);
// 			m_tacctrl[i].target_y = 1200;
			m_Fix_MoveOutOffFrame(&m_tacctrl[i]);
		}
	}
	
	//������������·������������·�ת
	if (m_trueball.y < 600)
	{
		m_tacctrl[2].target_y = 1200 - m_tacctrl[2].target_y;
		m_tacctrl[3].target_y = 1200 - m_tacctrl[3].target_y;		
		m_tacctrl[4].target_y = 1200 - m_tacctrl[4].target_y;
		m_tacctrl[5].target_y = 1200 - m_tacctrl[5].target_y;
	}
	
	//�Ƕ�,��������
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
		//�޳�������Ա������Ա
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
		//�޳�������Ա������Ա
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
		//�޳�������Ա������Ա
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
		//�ͷſ����Ա��ɫ
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
	//��������ֳ�������,�жϴ����Ǳ�
	if (inagent.y > 600)
	{
		//�����ϰ벿�֣������ĸ��߱ȽϿ���
		if (abs(255 - inagent.x) > abs(925 - inagent.y))
		{
			//�ȽϿ����ϱ߽磬�����ų��ȥ
			AMoveTo(inagent.ID,inagent.x,925,(int)inagent.angle);
		} 
		else
		{
			//�ȽϿ����ұ߽磬�����ų��ȥ
			AMoveTo(inagent.ID,255,inagent.y,(int)inagent.angle);
		}
	} 
	else
	{
		//�����°벿�֣������ĸ��߱ȽϿ���
		if (abs(255 - inagent.x) > abs(inagent.y - 275))
		{
			//�ȽϿ����±߽磬�����ų��ȥ
			AMoveTo(inagent.ID,inagent.x,275,(int)inagent.angle);
		} 
		else
		{
			//�ȽϿ����ұ߽磬�����ų��ȥ
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
	//������
	if (/*inTac.target_x >0 && */inTac.target_x <225 &&
		inTac.target_y > 275 && inTac.target_y < 925) 
	//////////////////////////////////////////////////////////////////////////
	/*/��΢СһЩ
	if (inTac.target_x >0 && inTac.target_x <100 &&
	inTac.target_y > 200 && inTac.target_y < 850)
	/////////////////////////////////////////////////////////////////////////*/
	{
		return true;
	} 
	else
	{
		//�ж��Ƿ��ײ�����Ż��ܵ����ź���ȥ
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

	//��������ֳ�������,�жϴ����Ǳ�
	if (inTac->target_y > 600)
	{
		//�����ϰ벿�֣������ĸ��߱ȽϿ���
		if (abs(PRNRIGHT - inTac->target_x) > abs(PENTOP - inTac->target_y))
		{
			//�ȽϿ����ϱ߽磬�����ų��ȥ
			inTac->target_y = PENTOP;
		} 
		else
		{
			//�ȽϿ����ұ߽磬�����ų��ȥ
			inTac->target_x = PRNRIGHT;
		}
	} 
	else
	{
		//�����°벿�֣������ĸ��߱ȽϿ���
		if (abs(PRNRIGHT - inTac->target_x) > abs(inTac->target_y - PENBTM))
		{
			//�ȽϿ����±߽磬�����ų��ȥ
			inTac->target_y = PENBTM;
		} 
		else
		{
			//�ȽϿ����ұ߽磬�����ų��ȥ
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

	//����
	m_trueball = m_Foreball;
}

void CUPFullField::Tac_DefTest(int inDefDist)
{
	//��ʼ�����ƽṹ�壬����ɫ�ؼ��̶�����
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;

	//2~5�Ŷ�Ա��λ�л���Χ��̬��
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
	//2~5�Ŷ�Ա��λ�л���Χ��̬��
	for (int i=2;i<6;i++)
	{
		m_tacctrl[i].ctrl = CTRL_MOVETO;
	}

	//�ȼ�����λ�������Ϸ�������λ��
	CPoint tmpball = m_trueball;
	if (m_trueball.y < 600)
	{
		tmpball.y = 1200 - tmpball.y;
	}
	
 	CPoint tmp;

	//���潫���ط�Ϊ�������������򣬸��������������γɷ�������
	if (tmpball.y > 800)
	{
		//�ϲ�
		//����������֮�����һ��
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

		//����
		m_tacctrl[5].target_x = m_Pos_Back.x;
 		m_tacctrl[5].target_y = m_Pos_Back.y;
	}

	if (tmpball.y < 800)
	{
		//�м�����
		//����������֮�����һ��
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)(inDefDist*m_fKField));
		m_tacctrl[2].target_x = tmp.x;
		m_tacctrl[2].target_y = tmp.y;
		//m_tacctrl[2].target_angle = m_Loca_CalAngle(m_tacctrl[2].target_x,m_tacctrl[2].target_y,tmpball.x,tmpball.y);

		//����ĳ���
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)((inDefDist+100)*m_fKField));
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,-45);
		m_tacctrl[3].target_x = tmp.x;
		m_tacctrl[3].target_y = tmp.y;

		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)((inDefDist+100)*m_fKField));
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,45);
		m_tacctrl[4].target_x = tmp.x;
		m_tacctrl[4].target_y = tmp.y;

		//����
		m_tacctrl[5].target_x = m_Pos_Back.x;
 		m_tacctrl[5].target_y = m_Pos_Back.y;

	}

	//����Լ���Ա�������������
	for (i=2;i<6;i++)
	{
		if (m_Check_MovetoOurPenaltyArea(m_tacctrl[i]) == true)
		{
			m_Fix_MovetoOurPenaltyArea(&m_tacctrl[i]);
		}
	}

	//����Լ���Ա���߽��������	
	for (i=2;i<6;i++)
	{
		if (m_Check_MoveOutOffFrame(m_tacctrl[i]) == true)
		{
			m_Fix_MoveOutOffFrame(&m_tacctrl[i]);
		}
	}

	//������������·������������·�ת
	if (m_trueball.y < 600)
	{
		m_tacctrl[2].target_y = 1200 - m_tacctrl[2].target_y;
		m_tacctrl[3].target_y = 1200 - m_tacctrl[3].target_y;		
		m_tacctrl[4].target_y = 1200 - m_tacctrl[4].target_y;
		m_tacctrl[5].target_y = 1200 - m_tacctrl[5].target_y;
	}

	//�Ƕ�,��������
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
		//����ת
		Fieldpoint.x = 1800 - Fieldpoint.x;
	}

	if (m_bVFlip == true)
	{
		//����ת
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

	//��һ��ֵ
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

	//����ɽǶ�ֵ
	Angle = Angle*180/3.14159;

	return Angle;
}

void CUPFullField::m_Fix_MoveTargetDist(MSG_Control *inList, int inAvoidDist, int inFixDist)
{
	for (int i=1;i<6;i++)
	{
		if (inList[i].ctrl== CTRL_MOVETO)
		{
			//�ں���Ĳ�����Ѱ��moveto��Ŀ�꣬�����Ͻ���������Լ�λ��
			for (int j=i;j<6;j++)
			{
				if (inList[j].ctrl == CTRL_MOVETO)
				{
					if (m_CalDist(inList[i].target_x,inList[i].target_y,inList[j].target_x,inList[j].target_y) < inAvoidDist)
					{
						//����̫������Ҫ������������ײ
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
		//�ں���Ĳ�����Ѱ��moveto��Ŀ�꣬�����Ͻ���������Լ�λ��
		for (int j=i;j<6;j++)
		{
			//ֻ��moveto��������
			if (inList[j].status == CTRL_MOVETO)
			{
				if (m_CalDist(inList[i].x,inList[i].y,inList[j].x,inList[j].y) < inAvoidDist)
				{
					//����̫������Ҫ������������ײ��inList[i]����������inList[j]����
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
		//�ں���Ĳ�����Ѱ��moveto��Ŀ�꣬�����Ͻ���������Լ�λ��
		for (int j=i;j<6;j++)
		{
			//ֻ��moveto��������
			if (inTac[j].ctrl == CTRL_MOVETO)
			{
				if (m_CalDist(inTac[i].target_x,inTac[i].target_y,inTac[j].target_x,inTac[j].target_y) < inAvoidDist)
				{
					//����̫������Ҫ������������ײ��inList[i]����������inList[j]����
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
			if (m_tacagent[i].status == CTRL_ATTACK && m_tacagent[i].online == true)	//���ҽ�����Ա
			{
				if (m_tacagent[i].ID == m_bakAtkAgent.ID)				//�鿴�Ƿ�ǰһʱ����Ҳ�ǽ���
				{
// 					if (m_tacagent[i].dist_ball != 0)	//�Ƿ����
// 					{
						if (m_CalDist(m_tacagent[i].x,m_tacagent[i].y,m_bakAtkAgent.x,m_bakAtkAgent.y) < 30)
						{
							//������Աδ������û�����ƶ�����ʼ����ʱ
							//m_nAtkDly = 30;
							m_nNumOfAtk = 2;
						}
						else
						{
							m_nNumOfAtk = 1;
						}
				/*	}*/
				}
				//�Խ�����Ա����һ�±���
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
					//�Խ�����Ա����һ�±���
					m_bakAtkAgent = m_tacagent[i];
				}
			}
		}
	}
}

int CUPFullField::m_Ag_GetMoveTo(int x, int y, uagent *inalist)
{
	int ret = 0;
	
	bool scanerr = true;	//����Ƿ��б�Ҫ��err��ag����ɨ��
	//��ɨ�轡����ag
	for (int i=2;i<7;i++)
	{
		if (inalist[i].online == true && inalist[i].allocated == false && inalist[i].status != ROBST_ERR)
		{
			//������ag�����ˣ�����Ҫ��err��ag����ɨ����
			scanerr = false;
			
			//�жϸý�����ag
			if (inalist[i].moveto_x==x && inalist[i].moveto_y==y)
			{
				ret = i;
			}
		}
	}
	
	//�������ag�Ѿ�����ѡ�����ʣ���ag����ѡ
	if (scanerr == true)
	{
		for (int i=1;i<7;i++)
		{
			if (inalist[i].online == true && inalist[i].allocated == false)
			{
				//�жϸ�err��ag
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
	//��ʼ�����ƽṹ�壬����ɫ�ؼ��̶�����
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;
	
	//�ж�һ���Ƿ���Ҫ����
// 	if (m_bSomeoneHoldBall() == true)
// 	{
// 		//�Ѿ������������л�����ͨ����ģʽ
// 		DefaultActive(150);
// 		return;
// 	}

	//����ģʽ�����������Ա
	m_tacctrl[2].ctrl = CTRL_ATTACK;
	m_tacctrl[3].ctrl = CTRL_ATTACK;
	m_tacctrl[4].ctrl = CTRL_ATTACK;
	
	//������Ա��λ�л���Χ��̬��
	for (int i=3;i<6;i++)
	{
		m_tacctrl[i].ctrl = CTRL_MOVETO;
	}
	
	//�ȼ�����λ�������Ϸ�������λ��
	CPoint tmpball = m_trueball;
	if (m_trueball.y < 600)
	{
		tmpball.y = 1200 - tmpball.y;
	}
	
	CPoint tmp;
	
	//���潫���ط�Ϊ�������������򣬸��������������γɷ�������
	if (tmpball.y > 900)
	{
		//�ϲ�
		//����������֮�����һ��

		//����
		m_tacctrl[5].target_x = m_Pos_Back.x;
		m_tacctrl[5].target_y = m_Pos_Back.y+300;
	}
	else
	{
		//tmpball.y < 900 �м�����
		//������ɫ
		m_tacctrl[2].ctrl = CTRL_ATTACK;
		m_tacctrl[3].ctrl = CTRL_ATTACK;
		m_tacctrl[4].ctrl = CTRL_ATTACK;
		
		
		//����
		m_tacctrl[5].target_x = m_Pos_Back.x;
		m_tacctrl[5].target_y = m_Pos_Back.y;

		
	}

	//����Լ���Ա�������������
	for (i=3;i<6;i++)
	{
		if (m_Check_MovetoOurPenaltyArea(m_tacctrl[i]) == true)
		{
			m_Fix_MovetoOurPenaltyArea(&m_tacctrl[i]);
		}
	}

	//����Լ���Ա���߽��������	
	for (i=3;i<6;i++)
	{
		if (m_Check_MoveOutOffFrame(m_tacctrl[i]) == true)
		{
			m_Fix_MoveOutOffFrame(&m_tacctrl[i]);
		}
	}
	
	//������������·������������·�ת
	if (m_trueball.y < 600)
	{
		
		m_tacctrl[5].target_y = 1200 - m_tacctrl[5].target_y;
	}
	
	//�Ƕ�,��������
	for (i=4;i<6;i++)
	{
		//m_tacctrl[i].target_angle = m_Loca_CalAngle(m_tacctrl[i].target_x,m_tacctrl[i].target_y,m_trueball.x,m_trueball.y);
		m_tacctrl[i].target_angle = 0;
	}
	
	m_TacAction(m_tacagent);
}

/************************Χ������***********************/
void CUPFullField::Tac_RoundedDef(int inDefDist)
{
	//��ʼ�����ƽṹ�壬����ɫ�ؼ��̶�����
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;
	
	//������ɫ
	m_tacctrl[2].ctrl = CTRL_ATTACK;
	
	//3~5�Ŷ�Ա��λ�л���Χ��̬��
	for (int i=3;i<6;i++)
	{
		m_tacctrl[i].ctrl = CTRL_MOVETO;
	}

	//�ȼ�����λ�������Ϸ�������λ��
	CPoint tmpball = m_trueball;
	if (m_trueball.y < 600)
	{
		tmpball.y = 1200 - tmpball.y;
	}
	
 	CPoint tmp;

	//���潫���ط�Ϊ�������������򣬸��������������γɷ�������
	if (tmpball.y > 1000)
	{
		//�ϲ�
		//����������֮�����һ��
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)(inDefDist*m_fKField));
		m_tacctrl[3].target_x = tmp.x;
		m_tacctrl[3].target_y = tmp.y;

		//���ذ뾶�ȶ����ŵĴ�һЩ�����⻻λ��ʱ����ײ
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)((inDefDist+50)*m_fKField));
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,-45);
		m_tacctrl[4].target_x = tmp.x;
		m_tacctrl[4].target_y = tmp.y;

		//����
		m_tacctrl[5].target_x = m_Pos_Back.x;
 		m_tacctrl[5].target_y = m_Pos_Back.y+300;
		
		//test
// 		tmp = m_Loca_CalBackPos(tmpball.x,tmpball.y,m_Pos_Back.x);
// 		m_tacctrl[4].target_x = tmp.x;
// 		m_tacctrl[4].target_y = tmp.y;
	}
	else
	{
		// tmpball.y < 1000 �м�����
		//����������֮�����һ��
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)(inDefDist*m_fKField));
		m_tacctrl[3].target_x = tmp.x;
		m_tacctrl[3].target_y = tmp.y;
	
		
		//����ĳ���
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)((inDefDist+50)*m_fKField));
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,45);
		m_tacctrl[4].target_x = tmp.x;
		m_tacctrl[4].target_y = tmp.y;

		//����
		m_tacctrl[5].target_x = m_Pos_Back.x;
		m_tacctrl[5].target_y = m_Pos_Back.y;
		
		
		//test
// 		tmp = m_Loca_CalBackPos(tmpball.x,tmpball.y,m_Pos_Back.x);
// 		m_tacctrl[4].target_x = tmp.x;
// 		m_tacctrl[4].target_y = tmp.y;

	}

	//printf("����Լ���Ա�������������");
	//CString tStr;
	//����Լ���Ա�������������
	for (i=3;i<6;i++)
	{
		if (m_Check_MovetoOurPenaltyArea(m_tacctrl[i]) == true)
		{
			//tStr.Format("%d�Ž�ɫ�����������Ҫ����",i);
			//printf(tStr);
			m_Fix_MovetoOurPenaltyArea(&m_tacctrl[i]);
			//m_tacctrl[i].target_x = 900;
		}
	}

	//����Լ���Ա���߽��������	
	for (i=3;i<6;i++)
	{
		if (m_Check_MoveOutOffFrame(m_tacctrl[i]) == true)
		{
			m_Fix_MoveOutOffFrame(&m_tacctrl[i]);
		}
	}

	//��ֹײ��
	m_Fix_CollisionAvoid(m_tacctrl, 50);

	//������������·������������·�ת
	if (m_trueball.y < 600)
	{
		m_tacctrl[2].target_y = 1200 - m_tacctrl[2].target_y;
		m_tacctrl[3].target_y = 1200 - m_tacctrl[3].target_y;		
		m_tacctrl[4].target_y = 1200 - m_tacctrl[4].target_y;
		m_tacctrl[5].target_y = 1200 - m_tacctrl[5].target_y;
	}

	//�Ƕ�,��������
	for (i=3;i<6;i++)
	{
		m_tacctrl[i].target_angle = 0;		//�ǶȲ��仯��ƽ�Ƶ�ʱ���ȶ�һЩ
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
 	//��ɸѡ����ϰ��︴�Ƶ���ʱ������,��������ѡ��
	memcpy(m_tacObst,m_cObst,(EACH_AG_OBST_NUM*(AG_NUM+1))*sizeof(obstacle));

	//����һʱ�����δ�����ϰ�����Ϣ�Ķ�Ա���ϰ�����Ϣ��������������Ϣ���ž���
	for(int i=1;i<AG_NUM;i++)
	{
		if (m_nObstMsgCnt[i] == 0 )
		{
			AClearObst(i);
		}
		m_nObstMsgCnt[i] = 0;
	}

	//�����ڳ�����ϰ���ȥ��
	m_Obst_RemoveOutOfField(m_tacObst);

	//����ÿ���ϰ��ﵽ�����ṩ�ߵľ���
	for (i=0;i<EACH_AG_OBST_NUM*(AG_NUM+1);i++)
	{
		if (m_tacObst[i].valid == true)
		{
			m_tacObst[i].DistToOwner = m_CalDist(m_tacObst[i].x,m_tacObst[i].y,m_agents[m_tacObst[i].ID].x,m_agents[m_tacObst[i].ID].y);
			//////////////////////////////////////////////////////////////////////////
			//������۲��ߺ�Զ���ϰ����޳�
			if (m_tacObst[i].DistToOwner > 500)
			{
				m_tacObst[i].valid = false;
			}
			//////////////////////////////////////////////////////////////////////////
		}
	}

	//���Ͻ��Ķ�Ա���������޳��䱾��
	int nNumOfObst = EACH_AG_OBST_NUM*(AG_NUM+1);
	int j,DistToNearAg;
	for (i=0;i<nNumOfObst;i++)
	{
		if (m_tacObst[i].valid == true)
		{
			//�����ǹ۲����Ƿ�̫�������̫����������Ϊ�Ƕ��ѣ��Ƴ�
			for (j=0;j<AG_NUM;j++)
			{
				if (m_agents[j].online == true && m_tacObst[i].ID != j)
				{
					//jΪ�����ϰ���Ͻ���agent id���������ߣ�
					DistToNearAg = m_CalDist(m_tacObst[i].x,m_tacObst[i].y,m_agents[j].x,m_agents[j].y);
					if (DistToNearAg < 100 )
					{
						//����ȥ��
						m_tacObst[i].valid = false;	
						break;
					}

					//�鿴�Ƿ�Ӧ�ñ�����Զ���붨λ��׼���ϰ����޳�
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
	/*                    �����ϰ���ľۺ�									*/
	/************************************************************************/
	//obstacle tmpObst;
	for (i=0;i<nNumOfObst;i++)
	{
		if (m_tacObst[i].valid == true)
		{
			//Ϊ�ϰ���iѰ�Ҿ���ӽ���
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
						//����ܽ����ж�˭�ľ������һЩ
						if (m_tacObst[i].DistToOwner < m_tacObst[j].DistToOwner)
						{
							//i�ϰ��������
							m_tacObst[j].valid = false;
						} 
						else
						{
							//j�ϰ��������,i���޳���û�м���ѭ��j�ı�Ҫ��
							m_tacObst[i].valid = false;
							break;
						}
					}
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	/*/���ͼ�˲�
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

	//����2
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
	
	//������Ϊ������
	m_follow[inID].believe = false;
	
	if(m_agents[inID].online == true){
		for ( i=0;i<EACH_AG_OBST_NUM;i++)
	{
		if(inpObstAr[i].valid == true&&(inpObstAr[i].x>0&&inpObstAr[i].x<900)&&(inpObstAr[i].y>0&&inpObstAr[i].y<1200)){
			m_follow[inID].believe = true;
			
			//ȥ���ҷ���Ա
			for( j=0;j<7;j++){
				if(m_agents[j].online ==true&&inID!=i){
					//�ϰ�������ѵ������������еĻ���һ���Ĳ�࣡�������70��85��
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
		// ����ϰ���
		
			
			
			 
			
	}
		
		//////
		if(bestpos!=-1){
		
				m_follow[inID].x = inpObstAr[bestpos].x;
				m_follow[inID].y = inpObstAr[bestpos].y;
			
			    m_follow[inID].size = inpObstAr[bestpos].size;
				
		}
		

}
	//������ļ�

}

//
void CUPFullField::m_TeammateShare()
{	
	//����Ա��Ϣ��m_tacagent��ֵ���Ƶ�m_tm��ֵ
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
	//�����ߵĶ�Ա�����������ѵ���Ϣ
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
	m_strCurSt.Format("���������");
	
	m_eStage = sTestPassCatch;
	m_bTaskPassCatch = true;	
	
	bool bsomeoneonline = false;
	//������Ա��Ϣ
	for (int i=0;i<7;i++)
	{
		m_tacagent[i] = m_agents[i];
		if (m_tacagent[i].online == true)
		{
			bsomeoneonline = true;
		}
	}
	
	//û����Ա���ߣ�û��Ҫ���о���
	if (bsomeoneonline == false)
	{
		return;
	}

	//��������Ա�õ������սᴫ��������
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
			//��ɫ����
			m_tacctrl[1].ctrl = CTRL_GOALKEEP;
			m_tacctrl[2].ctrl = CTRL_PASS;
			
			//3~5�Ŷ�Աֹͣ
			for (i=3;i<AG_NUM;i++)
			{
				m_tacctrl[i].ctrl = CTRL_STOP;
			}
		} 
		else
		{
			//��ɫ����
			m_tacctrl[1].ctrl = CTRL_GOALKEEP;	
			m_tacctrl[2].ctrl = CTRL_CATCH;
			
			//3~5�Ŷ�Աֹͣ
			for (i=3;i<AG_NUM;i++)
			{
				m_tacctrl[i].ctrl = CTRL_STOP;
			}
		}
		
		
	}
	else
	{
		//�Ҳ���������ľ���
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
		//��Ǹ�agent�Ѿ������ɫ
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
		//��Ǹ�agent�Ѿ������ɫ
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
		//��Ǹ�agent�Ѿ������ɫ
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
		//��Ǹ�agent�Ѿ������ɫ
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
		//��Է��볡����
		m_nPassPlayer = 0;
		m_nPassEndDelay = 0;
	}
	else
	{
		//�򼺷��볡���򣬴����Ա��Ҫͣһ��
		m_nPassEndDelay = PASS_END_DELAY;
	}
}

void CUPFullField::Attention_SomeOneKick(int inID, int inSelfX, int inSelfY, int inTargetX, int inTargetY)
{
	CString info;
	if (m_bTaskPassCatch == true)
	{
		//���ڽ��д�������,����ѡ������Ա
		if (m_nCatchPlayer == 0)
		{
			//û��ѡ�������Ա�Ļ�����������ʵĽ����Ա
			m_nCatchPlayer = m_Ag_GetNearBy(inTargetX,inTargetY,m_tacagent);

			if (m_nCatchPlayer == m_nPassPlayer)
			{
				//��ֹ�Լ�������Լ�
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
		info.Format("���ŷ���(%d,%d)!!!!!!!\n",inTargetX,inTargetY);
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
	//����
	for (int i=0;i<nNumObst;i++)
	{
		if (inObstAr[i].valid == true)
		{
			//�������
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
	//2010�꣬Ϊ����������ӷ��ضԷ���Ա������	
	memcpy(m_tmpObst,m_tacObst,(EACH_AG_OBST_NUM*(AG_NUM+1))*sizeof(obstacle));
	//��ɫ3 - ��ԭ���ǲ��棬���ڷ��ؿ��ܽ���ĶԷ���Ա��
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
			//��ȡ��һ���ϰ���鿴������ľ���
			if (m_CalDist(m_tmpObst[nearobst].x,m_tmpObst[nearobst].y,m_trueball.x,m_trueball.y) < 150)
			{
				//���������С��2�ף�Ӧ����׼������Ķ�Ա������
				m_tmpObst[nearobst].valid = false;
				nearobst = -1;		//��ѭ�����Լ���
			}
			else
			{
				if (m_CalDist(m_tmpObst[nearobst].x,m_tmpObst[nearobst].y,1800,600) < 200)
				{
					//����Է����źܽ����п����ǶԷ�����Ա��������Ҫ��
					m_tmpObst[nearobst].valid = false;
					nearobst = -1;		//��ѭ�����Լ���
					continue;
				}
				//����Ҫ��
				break;
			}
		}
		else
		{
			//�������-1��˵��û���ϰ���
			break;
		}
	}
	
	CPoint tmpPnt;
	if (nearobst != -1)
	{
		//////////////////////////////////////////////////////////////////////////
		//����һ
		//���ϰ�����������1��
		tmpPnt = m_Loca_Ray(m_tmpObst[nearobst].x,m_tmpObst[nearobst].y,m_trueball.x,m_trueball.y,100);

		//���ϰ�����ת��ֱ�����������λ��
		for (int i=0;i<=180/5;i++)
		{
			tmpPnt = m_Loca_Rotate(tmpPnt.x,tmpPnt.y,m_tmpObst[nearobst].x,m_tmpObst[nearobst].y,-5);

			if (m_CalDist(tmpPnt.x,tmpPnt.y,m_trueball.x,m_trueball.y) > inDist)
			{
				break;
			}
		}

		int nodead = 0;
		//���������̫�������򼺷��������죬ֱ��վλ���������ڹ���Ҫ��ķ��ؾ���
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

		//���ϰ����Ѿ������أ��ر���ʹ��
		m_tmpObst[nearobst].valid = false;
	}
	
	nodead = 0;
	//��ɫ4 - ��ԭ���Ǻ��������Ϊ�������ҷ����������һ���Է���Ա��
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
				//����Է����źܽ����п����ǶԷ�����Ա��������Ҫ��
				m_tmpObst[nearobst].valid = false;
				nearobst = -1;		//��ѭ�����Լ���
				continue;
			}
			//����Ҫ��
			break;
		}
		else
		{
			//�������-1��˵��û���ϰ���
			break;
		}
	}

	if (nearobst != -1)
	{
		//�Ӹ��ϰ������ҷ���������һ��
		tmpPnt = m_Loca_Ray(m_tmpObst[nearobst].x,m_tmpObst[nearobst].y,0,600,80);	
		m_tacctrl[4].ctrl = CTRL_MOVETO;
		m_tacctrl[4].target_x = tmpPnt.x;
		m_tacctrl[4].target_y = tmpPnt.y;
		
		//���ϰ����Ѿ������أ��ر���ʹ��
		m_tmpObst[nearobst].valid = false;
	}
}

//m_trueball.x < 1000ʱ�����
void CUPFullField::Tac_RoundedDef2(int inDefDist)
{
	//��ʼ�����ƽṹ�壬����ɫ�ؼ��̶�����
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;
	
	//������ɫ
	m_tacctrl[2].ctrl = CTRL_ATTACK;
	
	//3~5�Ŷ�Ա��λ�л������Ƕȷ���̬��
	for (int i=3;i<6;i++)
	{
		m_tacctrl[i].ctrl = CTRL_DEFENCE;
		m_tacctrl[i].def_dist = (int)(inDefDist*m_fKField);
	}

	CPoint tmpball = m_trueball;	
 	CPoint tmp;

	//���潫���ط�Ϊ�������������򣬸��������������γɷ�������
	if (tmpball.y > 900)
	{
		//�ϲ�
		//����������֮�����һ��
		m_tacctrl[3].def_angle = 0;

		//���ذ뾶�ȶ����ŵĴ�һЩ�����⻻λ��ʱ����ײ
		m_tacctrl[4].def_angle = -45;
		if (m_trueball.x < 400)
		{
			m_tacctrl[4].def_dist += abs(m_trueball.x - 400);
		}

		//����
		//�Է��������ŽϽ��Ҳ����ҷ�����ʱ���л��ɶ��Ƕȷ���
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
			// 300<tmpball.y < 900 �м�����
			//����������֮�����һ��
			m_tacctrl[3].def_angle = 0;	
			m_Fix_DefOutOffFrame(&(m_tacctrl[3]),0);
			m_Fix_DefInOurPenaltyArea(&(m_tacctrl[3]));
			
			//
			m_tacctrl[4].def_dist += 50;
			m_tacctrl[4].def_angle = 45;
			m_Fix_DefOutOffFrame(&(m_tacctrl[4]),0);
			m_Fix_DefInOurPenaltyArea(&(m_tacctrl[4]));

			//����
			//�Է��������ŽϽ��Ҳ����ҷ�����ʱ���л��ɶ��Ƕȷ���
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
			//tempball.y < 300 �°���
			//����������֮�����һ��
			m_tacctrl[3].def_angle = 0;
			
			//���ذ뾶�ȶ����ŵĴ�һЩ�����⻻λ��ʱ����ײ
			m_tacctrl[4].def_angle = 45;		
			if (m_trueball.x < 400)
			{
				m_tacctrl[4].def_dist += abs(m_trueball.x - 400);
			}
			
			//����
			//�Է��������ŽϽ��Ҳ����ҷ�����ʱ���л��ɶ��Ƕȷ���
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

	//(����)ԭ��б����صĶ�Ա����ǰʱ���ص��г����з���
	//��δ���ԣ������ڻ�
	//m_tacctrl[4].ctrl = CTRL_ATKCOVER;
	//m_tacctrl[4].def_dist = 100;		//��С�ڻ��뾶

	//���򵽶Է�������ʱ��������ζ�Ҫ�����ض�Ա������
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
	//��ʼ�����ƽṹ�壬����ɫ�ؼ��̶�����
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;

	//�ж�һ���Ƿ���Ҫ����
	// 	if (m_bSomeoneHoldBall() == true)
	// 	{
	// 		//�Ѿ������������л�����ͨ����ģʽ
	// 		DefaultActive(150);
	// 		return;
	// 	}

	//����ģʽ�����������Ա
	m_tacctrl[2].ctrl = CTRL_ATTACK;
	m_tacctrl[3].ctrl = CTRL_ATTACK;

	//������Ա��λ�л���Χ��̬��
	for (int i=4;i<6;i++)
	{
		m_tacctrl[i].ctrl = CTRL_MOVETO;
	}

	//�ȼ�����λ�������Ϸ�������λ��
	CPoint tmpball = m_trueball;
	if (m_trueball.y < 600)
	{
		tmpball.y = 1200 - tmpball.y;
	}

	CPoint tmp;

	//���潫���ط�Ϊ�������������򣬸��������������γɷ�������
	if (tmpball.y > 900)
	{
		//�ϲ�
		//����������֮�����һ��
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)(inDefDist*m_fKField));
		m_tacctrl[4].target_x = tmp.x;
		m_tacctrl[4].target_y = tmp.y;

		//����
		m_tacctrl[5].target_x = m_Pos_Back.x;
		m_tacctrl[5].target_y = m_Pos_Back.y+300;

		//test
		// 		tmp = m_Loca_CalBackPos(tmpball.x,tmpball.y,m_Pos_Back.x);
		// 		m_tacctrl[5].target_x = tmp.x;
		// 		m_tacctrl[5].target_y = tmp.y;


		//����ĳ���
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
		//tmpball.y < 900 �м�����
		//������ɫ
		m_tacctrl[2].ctrl = CTRL_ATTACK;
		m_tacctrl[3].ctrl = CTRL_ATTACK;

		//����������֮�����һ��
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)(inDefDist*m_fKField));
		m_tacctrl[4].target_x = tmp.x;
		m_tacctrl[4].target_y = tmp.y;


		//����
		m_tacctrl[5].target_x = m_Pos_Back.x;
		m_tacctrl[5].target_y = m_Pos_Back.y;

		//����ĳ���
		// 		CPoint longtmp = m_Loca_Ray(tmpball.x,tmpball.y,m_tacctrl[4].target_x,m_tacctrl[4].target_y,(inDefDist+100)*m_fKField);
		// 		tmp = m_Loca_Rotate(longtmp.x,longtmp.y,tmpball.x,tmpball.y,-40);
		// 		m_tacctrl[5].target_x = tmp.x;
		// 		m_tacctrl[5].target_y = tmp.y;


		//		tmp = m_Loca_Rotate(m_tacctrl[4].target_x,m_tacctrl[4].target_y,tmpball.x,tmpball.y,45);
		// 		tmp = m_Loca_Rotate(longtmp.x,longtmp.y,tmpball.x,tmpball.y,45);
		// 		m_tacctrl[6].target_x = tmp.x;
		// 		m_tacctrl[6].target_y = tmp.y;
	}

	//����Լ���Ա�������������
	for (i=3;i<6;i++)
	{
		if (m_Check_MovetoOurPenaltyArea(m_tacctrl[i]) == true)
		{
			m_Fix_MovetoOurPenaltyArea(&m_tacctrl[i]);
		}
	}

	//����Լ���Ա���߽��������	
	for (i=3;i<6;i++)
	{
		if (m_Check_MoveOutOffFrame(m_tacctrl[i]) == true)
		{
			m_Fix_MoveOutOffFrame(&m_tacctrl[i]);
		}
	}

	//������������·������������·�ת
	if (m_trueball.y < 600)
	{
		m_tacctrl[2].target_y = 1200 - m_tacctrl[2].target_y;
		m_tacctrl[3].target_y = 1200 - m_tacctrl[3].target_y;		
		m_tacctrl[4].target_y = 1200 - m_tacctrl[4].target_y;
		m_tacctrl[5].target_y = 1200 - m_tacctrl[5].target_y;
	}

	//�Ƕ�,��������
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
		//�����ҷ����ߣ�Moveto�˻ص���
		inCtrl->ctrl = CTRL_MOVETO;
		inCtrl->target_x = 0;
		inCtrl->target_y = tmp.y;
		inCtrl->target_angle = m_Loca_CalAngle(inCtrl->target_x,inCtrl->target_y,m_trueball.x,m_trueball.y);
	}

	if (tmp.x > 1800)
	{
		//�����Է����ߣ�Moveto�˻ص���
		inCtrl->ctrl = CTRL_MOVETO;
		inCtrl->target_x = 1800;
		inCtrl->target_y = tmp.y;
		inCtrl->target_angle = m_Loca_CalAngle(inCtrl->target_x,inCtrl->target_y,m_trueball.x,m_trueball.y);
	}

	if (tmp.y < 0)
	{
		//�����ҷ����ߣ�Moveto�˻ص���
		inCtrl->ctrl = CTRL_MOVETO;
		inCtrl->target_x = tmp.x;
		inCtrl->target_y = 0;
		inCtrl->target_angle = m_Loca_CalAngle(inCtrl->target_x,inCtrl->target_y,m_trueball.x,m_trueball.y);
	}

	if (tmp.y > 1200)
	{
		//�����ҷ����ߣ�Moveto�˻ص���
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
		//����λ�����ҷ������ڣ�Moveto�˳�����
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
			//������ϰ���
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
			//�Է������ڵ��ϰ���
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

//����moveto��Ϊ�������ڱ����������Ļ����ϣ��Ƴ�����
void CUPFullField::m_Fix_MoveOutOfOurPenaltyArea(MSG_Control *inTac, int inDist)
{
	if (inTac->ctrl != CTRL_MOVETO && inTac->ctrl != CTRL_PRODEF && inTac->ctrl != CTRL_BLOCK )
	{
		return ;
	}
	
}

//��������
void CUPFullField::DefaultReady2()
{
	KillTimer(TAC_WAIT_TIMER);

	//2010��ʹ��moveto����
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;
	//������Ա�ܶ���ָ����
	for (int i=2;i<6;i++)
	{
		m_tacctrl[i].ctrl = CTRL_MOVETO;
		m_tacctrl[i].target_angle = 0;	
	}
	//�ȼ������ϣ�����λ��
	CPoint tmpball = m_trueball;
	if (tmpball.y < 500)
	{
		tmpball.y = 1200 - tmpball.y;
	}
	
	CPoint tmp;
	//���������ڵİ볡��ȷ���������
	if (tmpball.x < 700)
	{
		//1�������ҷ��볡�������ʱ�������ת��
		
		//���������Ա-��-�Է����� = 180��
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,1600,600,(int)(700*m_fKField));			//���һ�������Ǿ���
		//tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,180);
		m_tacctrl[3].target_x = tmp.x;
		m_tacctrl[3].target_y = tmp.y;
		m_tacctrl[3].target_angle = m_Loca_CalAngle(tmp.x,tmp.y,tmpball.x,tmpball.y);
	
		//�����Ա-��-�Է����� = 180��
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,1800,600,(int)(100*m_fKField));	//���һ�������Ǿ���
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,180);
		m_tacctrl[2].target_x = tmp.x;
		m_tacctrl[2].target_y = tmp.y;
		m_tacctrl[2].target_angle = m_Loca_CalAngle(tmp.x,tmp.y,tmpball.x,tmpball.y);
	} 
	else
	{
		//2�����ڶԷ��볡����ȡֱ������	
		//���������Ա-��-�Է����� = 180��
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,1600,600,(int)(200*m_fKField));			//���һ�������Ǿ���
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,180);
		m_tacctrl[3].target_x = tmp.x;
		m_tacctrl[3].target_y = tmp.y;
		m_tacctrl[3].target_angle = m_Loca_CalAngle(tmp.x,tmp.y,tmpball.x,tmpball.y);

		//�����Աλ������,�������
		int nodead = 0;
		while (m_tacctrl[3].target_y > 1100)
		{
			nodead ++;
			if (nodead > 100)
			{
				break;
			}
			//��������
			tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,-5);
			m_tacctrl[3].target_x = tmp.x;
			m_tacctrl[3].target_y = tmp.y;
		}
		m_tacctrl[3].target_angle = m_Loca_CalAngle(tmp.x,tmp.y,tmpball.x,tmpball.y);

		//�����Ա-��-�����Ա = 0��
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,m_tacctrl[3].target_x,m_tacctrl[3].target_y,(int)(100*m_fKField));	//���һ�������Ǿ���
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,180);
		m_tacctrl[2].target_x = tmp.x;
		m_tacctrl[2].target_y = tmp.y;
		m_tacctrl[2].target_angle = m_Loca_CalAngle(tmp.x,tmp.y,tmpball.x,tmpball.y);
	}
	
	//�����ܶ�λ�ã����ⲻ������ľ���̫��
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
	
	//�������·������������·�ת
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

	//ָ��������ID
	m_nCatchPlayer = m_Ag_GetNearBy(m_tacctrl[3].target_x,m_tacctrl[3].target_y,m_tacagent);

	//��ֹײ��
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
		info.Format("(%d,%d)��ɵ�",inx,iny);
	//	printf(info);
		break;

	case 2:
		info.Format("(%d,%d)��ɵ�",inx,iny);
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
	//����
	for (int i=0;i<nNumObst;i++)
	{
		if (inObstAr[i].valid == true)
		{
			//�������
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
	//�Է������ҷ���λӦ��
	//////////////////////////////////////////////////////////////////////////
	//10�����
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;

	for (int i=2;i<7;i++)
	{
		m_tacctrl[i].ctrl = CTRL_MOVETO;
	}

	//�ȼ�����λ�������Ϸ�������λ��
	CPoint tmpball = m_trueball;
	if (m_trueball.y < 600)
	{
		tmpball.y = 1200 - tmpball.y;
	}
	
 	CPoint tmp;	
	////////////////////////////////////////////////////////////////////////////
	if (tmpball.x > 200)
	{
		//�����ҷ��볡�еȾ���ʱ
		//��������֮�����һ��
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)(150*m_fKField));
		m_tacctrl[2].target_x = tmp.x;
		m_tacctrl[2].target_y = tmp.y;

		//����1
		int defAngle = -50;
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)(250*m_fKField));
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,defAngle);
		m_tacctrl[3].target_x = tmp.x;
		m_tacctrl[3].target_y = tmp.y;

		//!�滻�������ý����ڻ������ضԷ������Ա
// 		m_tacctrl[3].ctrl = CTRL_ATKCOVER;
// 		m_tacctrl[3].def_dist = (inDistFB+50)*m_fKField; 

		//����
		m_tacctrl[4].target_x = m_Pos_Back.x;
 		m_tacctrl[4].target_y = m_Pos_Back.y;

		//����2�����׳��磬������
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
			//���Ŷ�Ա�˵��������ˣ�Ӧ��������������λ�õ���
			m_tacctrl[2].target_x = 200;	//�����ڶ�Աǰ��

			//������΢�Ŵ�һЩ
			tmp = m_Loca_Rotate(m_tacctrl[3].target_x,m_tacctrl[3].target_y,tmpball.x,tmpball.y,-50);
			m_tacctrl[3].target_x = tmp.x;
			m_tacctrl[3].target_y = tmp.y;

			tmp = m_Loca_Rotate(m_tacctrl[5].target_x,m_tacctrl[5].target_y,tmpball.x,tmpball.y,20);
			m_tacctrl[5].target_x = tmp.x;
			m_tacctrl[5].target_y = tmp.y;
		}
		
	}
	else//(tmpball.x < 200),Ӧ���ǽ���һ���
	{
		//���ڼ���������
		//����������֮�����һ��
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)(110*m_fKField));
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,-15);
		m_tacctrl[2].target_x = tmp.x;
		m_tacctrl[2].target_y = tmp.y;
		//m_tacctrl[2].target_angle = m_Loca_CalAngle(m_tacctrl[2].target_x,m_tacctrl[2].target_y,tmpball.x,tmpball.y);

		//�ڵ�һ����������תһ���Ƕ�
		tmp = m_Loca_Ray(tmpball.x,tmpball.y,0,600,(int)(160*m_fKField));
		tmp = m_Loca_Rotate(tmp.x,tmp.y,tmpball.x,tmpball.y,-60);
		m_tacctrl[3].target_x = tmp.x;
		m_tacctrl[3].target_y = tmp.y;

		//!�滻�������ý����ڻ������ضԷ������Ա
// 		m_tacctrl[3].ctrl = CTRL_ATKCOVER;
// 		m_tacctrl[3].def_dist = (inDistFB+50)*m_fKField; 
		
		//���������Է�����һ��ת��
		m_tacctrl[4].target_x = m_Pos_Back.x;
		m_tacctrl[4].target_y = 900;

		//����
		m_tacctrl[5].target_x = m_Pos_Back.x;
		m_tacctrl[5].target_y = m_Pos_Back.y;
	}
		
	//������������·������������·�ת
	if (m_trueball.y < 600)
	{
		m_tacctrl[2].target_y = 1200 - m_tacctrl[2].target_y;
		m_tacctrl[3].target_y = 1200 - m_tacctrl[3].target_y;		
		m_tacctrl[4].target_y = 1200 - m_tacctrl[4].target_y;
		m_tacctrl[5].target_y = 1200 - m_tacctrl[5].target_y;
	}

	//2010��ӷ��ض�Ա
	//Default_Anti_Ready_Add(inDistFB);

	//��ֹ�ƶ���������
	for (i=2;i<6;i++)
	{
		m_Fix_MoveOutOffFrame(&(m_tacctrl[i]));
	}

	//��ֹײ��
	m_Fix_CollisionAvoid(m_tacctrl, 50);

	//�Ƕ�,��������
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
	//�������
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

//m_trueball.x < 1000ʱ����ã���Tac_RoundedDef2�����������ȿ��Ƿ���
void CUPFullField::Tac_RoundedDef3(int inDefDist)
{
	//��ʼ�����ƽṹ�壬����ɫ�ؼ��̶�����
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;	
	
	//3~5�Ž�ɫ��λ�л������Ƕȷ���̬�ƣ�3�Ž�ɫ����ᱻ���ǳɽ�����
	for (int i=2;i<6;i++)
	{
		m_tacctrl[i].ctrl = CTRL_DEFENCE;
		m_tacctrl[i].def_dist = (int)(inDefDist*m_fKField);
	}

	//������ɫ
	m_tacctrl[3].ctrl = CTRL_ATTACK;

	CPoint tmpball = m_trueball;	
 	CPoint tmp;

	//���潫���ط�Ϊ�������������򣬸��������������γɷ�������
	if (tmpball.y > 900)
	{
		//�ϲ�
		//����������֮�����һ��
		m_tacctrl[2].def_angle = 0;

		//���ذ뾶�ȶ����ŵĴ�һЩ�����⻻λ��ʱ����ײ
		m_tacctrl[4].def_angle = -45;
		if (m_trueball.x < 400)
		{
			m_tacctrl[4].def_dist += abs(m_trueball.x - 400);
		}

		//����
		//�Է��������ŽϽ��Ҳ����ҷ�����ʱ���л��ɶ��Ƕȷ���
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
			// 300<tmpball.y < 900 �м�����
			//����������֮�����һ��
			m_tacctrl[2].def_angle = 0;	
			m_Fix_DefOutOffFrame(&(m_tacctrl[2]),0);
			m_Fix_DefInOurPenaltyArea(&(m_tacctrl[2]));
			
			//
			m_tacctrl[4].def_dist += 50;
			m_tacctrl[4].def_angle = 45;
			m_Fix_DefOutOffFrame(&(m_tacctrl[4]),0);
			m_Fix_DefInOurPenaltyArea(&(m_tacctrl[4]));

			//����
			//�Է��������ŽϽ��Ҳ����ҷ�����ʱ���л��ɶ��Ƕȷ���
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
			//tempball.y < 300 �°���
			//����������֮�����һ��
			m_tacctrl[2].def_angle = 0;
			
			//���ذ뾶�ȶ����ŵĴ�һЩ�����⻻λ��ʱ����ײ
			m_tacctrl[4].def_angle = 45;		
			if (m_trueball.x < 400)
			{
				m_tacctrl[4].def_dist += abs(m_trueball.x - 400);
			}
			
			//����
			//�Է��������ŽϽ��Ҳ����ҷ�����ʱ���л��ɶ��Ƕȷ���
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

	//(����)ԭ��б����صĶ�Ա����ǰʱ���ص��г����з���
	//��δ���ԣ������ڻ�
	//m_tacctrl[4].ctrl = CTRL_ATKCOVER;
	//m_tacctrl[4].def_dist = 100;		//��С�ڻ��뾶

	m_TacAction(m_tacagent);
}

void CUPFullField::m_Fix_GetOutOfTheWayOfATK(MSG_Control *inCtrl)
{

}

void CUPFullField::m_Fix_GetOutOfTheWayOfATK()
{
	//Ѱ�ҽ�����Ա
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
		//δ�ҵ�������Ա���˳�
		return;
	}

	//������Ա���ڣ��鿴���Ƿ�ܿ�����,���򳬹�һ������ʱ����Ӧ���Զ��ƹ�ȥ�����ر�����
	if (m_CalDist(m_tacagent[nAtkID].x , m_tacagent[nAtkID].y , m_trueball.x,m_trueball.y) > 100)
	{
		return;
	}

	CPoint fixTarget;
	int fixAngle;
	//����stop��moveto��defence����������ɫ������赲����
	for (i=2;i<AG_NUM;i++)
	{
		if (m_tacagent[i].online == true && m_tacagent[i].status == CTRL_MOVETO)
		{
			if (m_CalDist(m_tacagent[nAtkID].x , m_tacagent[nAtkID].y , m_tacagent[i].moveto_x , m_tacagent[i].moveto_y) < 100)
			{
				//moveto��Ŀ���ͽ�������̫�����ƿ�
				fixTarget = m_Loca_Ray(m_tacagent[nAtkID].x , m_tacagent[nAtkID].y,m_tacagent[i].moveto_x , m_tacagent[i].moveto_y,100);
				fixAngle = m_Loca_CalAngle(fixTarget.x , fixTarget.y , m_trueball.x , m_trueball.y);
				AMoveTo(i,fixTarget.x,fixTarget.y,fixAngle);
			}
		}

		if (m_tacagent[i].online == true && (m_tacagent[i].status == CTRL_DEFENCE||m_tacagent[i].status == CTRL_FOLLOWPOSITION))
		{
			if (m_CalDist(m_tacagent[nAtkID].x , m_tacagent[nAtkID].y , m_tacagent[i].x , m_tacagent[i].y) < 100)
			{
				//defenc��ס��λ�úͽ�������̫�����ƿ�
				fixTarget = m_Loca_Ray(m_tacagent[nAtkID].x , m_tacagent[nAtkID].y,m_tacagent[i].x , m_tacagent[i].y,100);
				fixAngle = m_Loca_CalAngle(fixTarget.x , fixTarget.y , m_trueball.x , m_trueball.y);
				AMoveTo(i,fixTarget.x,fixTarget.y,fixAngle);
			}
		}

		if (m_tacagent[i].online == true && m_tacagent[i].status == CTRL_STOP)
		{
			if (m_CalDist(m_tacagent[nAtkID].x , m_tacagent[nAtkID].y , m_tacagent[i].x , m_tacagent[i].y) < 100)
			{
				//defenc��ס��λ�úͽ�������̫�����ƿ�
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

		//���潫���ط�Ϊ�������������򣬸��������������γɷ�������
		if (tmpball.y > 900)
		{
			//�ϲ�
			//����������֮�����һ��
			m_tacctrl[4].def_angle = 0;

			//����
			m_tacctrl[5].ctrl = CTRL_MOVETO;
			m_tacctrl[5].target_x = m_Pos_Back.x;
			m_tacctrl[5].target_y = m_Pos_Back.y+200;

		}
		else
		{
			if (tmpball.y > 300)
			{
				// 300<tmpball.y < 900 �м�����
				//����������֮�����һ��
				m_tacctrl[4].def_angle = 0;
				m_Fix_DefOutOffFrame(&(m_tacctrl[4]),0);
				m_Fix_DefInOurPenaltyArea(&(m_tacctrl[4]));

				//����
				m_tacctrl[5].ctrl = CTRL_MOVETO;
				m_tacctrl[5].target_x = m_Pos_Back.x;
				m_tacctrl[5].target_y = m_Pos_Back.y;
			}
			else
			{
				//tempball.y < 300 �°���
				//����������֮�����һ��
				m_tacctrl[4].def_angle = 0;

				//����
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
		//���������߽�ɫ
		m_tacctrl[2].ctrl = CTRL_3M_CATCH;
		m_n3mPassPlayer=Last3meterPasser;
		m_n3mCatchPlayer=Last3meterCatcher;
		//3~5�Ŷ�Ա��λ�л������Ƕȷ���̬��
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

		//���潫���ط�Ϊ�������������򣬸��������������γɷ�������
		if (tmpball.y > 900)
		{
			//�ϲ�
			//����������֮�����һ��
			m_tacctrl[4].def_angle = 0;

			//����
			m_tacctrl[5].ctrl = CTRL_MOVETO;
			m_tacctrl[5].target_x = m_Pos_Back.x;
			m_tacctrl[5].target_y = m_Pos_Back.y+300;

		}
		else
		{
			if (tmpball.y > 300)
			{
				// 300<tmpball.y < 900 �м�����
				//����������֮�����һ��
				m_tacctrl[4].def_angle = 0;
				m_Fix_DefOutOffFrame(&(m_tacctrl[4]),0);
				m_Fix_DefInOurPenaltyArea(&(m_tacctrl[4]));

				//����
				m_tacctrl[5].ctrl = CTRL_MOVETO;
				m_tacctrl[5].target_x = m_Pos_Back.x;
				m_tacctrl[5].target_y = m_Pos_Back.y;
			}
			else
			{
				//tempball.y < 300 �°���
				//����������֮�����һ��
				m_tacctrl[4].def_angle = 0;

				//����
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
	//��ʼ�����ƽṹ�壬����ɫ�ؼ��̶�����
	m_bTaskMidPassCatch = true;
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;

	if(believe>=100 && believe<200)
	{
		//������ɫ
		m_tacctrl[2].ctrl = CTRL_MID_PASS;	
		m_tacctrl[3].ctrl = CTRL_MID_CATCH;

		final_mid_pass_x = px;//�������͸�����Ĵ����
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
		//������ɫ
		m_tacctrl[2].ctrl = CTRL_MID_PASS;	
		m_tacctrl[3].ctrl = CTRL_MID_CATCH;

		final_mid_pass_x = px;//�������͸�����Ĵ����
		final_mid_pass_y = py;
		final_mid_pass_believe = believe;

		//3~5�Ŷ�Ա��λ�л������Ƕȷ���̬��

		for (int i=4;i<6;i++)
		{
			//m_tacctrl[i].ctrl = CTRL_DEFENCE;
			m_tacctrl[i].ctrl = CTRL_DEFENCE;
			m_tacctrl[i].def_dist = (int)(defdis*m_fKField);
		}

		CPoint tmpball = m_trueball;

		//���潫���ط�Ϊ�������������򣬸��������������γɷ�������
		if (tmpball.y > 900)
		{
			//�ϲ�
			//����������֮�����һ��
			m_tacctrl[4].def_angle = 0;

			//����
			m_tacctrl[5].ctrl = CTRL_MOVETO;
			m_tacctrl[5].target_x = m_Pos_Back.x;
			m_tacctrl[5].target_y = m_Pos_Back.y+200;

		}
		else
		{
			if (tmpball.y > 300)
			{
				// 300<tmpball.y < 900 �м�����
				//����������֮�����һ��
				m_tacctrl[4].def_angle = 0;
				m_Fix_DefOutOffFrame(&(m_tacctrl[4]),0);
				m_Fix_DefInOurPenaltyArea(&(m_tacctrl[4]));

				//����
				m_tacctrl[5].ctrl = CTRL_MOVETO;
				m_tacctrl[5].target_x = m_Pos_Back.x;
				m_tacctrl[5].target_y = m_Pos_Back.y;
			}
			else
			{
				//tempball.y < 300 �°���
				//����������֮�����һ��
				m_tacctrl[4].def_angle = 0;

				//����
				m_tacctrl[5].ctrl = CTRL_MOVETO;
				m_tacctrl[5].target_x = m_Pos_Back.x;
				m_tacctrl[5].target_y = m_Pos_Back.y-200;
			}
		}
	}
	
	CString info;
	info.Format("���뵽��ɫ���� \n");
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
			map[i][j]=0.0;//��ʼ��
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
			if(j>=18&&j<36&&k>=0&&k<24){//ɸѡ�ϰ���
				map[j][k]=1.0;
			}
		}
	}
	for(i=18,n=0;i<36;i++){
		for(j=0;j<24;j++){
			if(map[i][j]==1.0){
				zhangai[n][0]=i;//i����ϰ���
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
			}//��ͼ�㸳ֵ
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
	//��ʼ�������
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

		//���ѡȡ�����
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

		//ͳ��֡������
		m_iEvaluteFrame++;
		
		
		//ÿ300֡��һ����
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

		//�����ɫ�ϰ����������ߵľ���
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
		
		////����������Ƿ񵽽����
		for (int i=1;i<7;i++)
		{
		if (m_tacagent[i].online == true 
				&& m_tacagent[i].status == CTRL_MID_CATCH && m_tacagent[i].status != CTRL_MID_PASS 
					&&  m_CalDist(*EvaluteX, *EvaluteY, m_tacagent[i].x, m_tacagent[i].y) < 50.0)
			{
					CString info;
					info.Format("fuckfuck��%d,%d,%d,%d,%d\n",*EvaluteX, *EvaluteY ,m_tacagent[i].x,m_tacagent[i].y,PassFlag );
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
	//	info.Format("�����%d,%d\n",*EvaluteX, *EvaluteY);
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
	ip.Format("ѡ������Ա%d",best);
	printf(ip);
	return best;
}

void CUPFullField::Tac_MidPass_kicked( int midCatchPlayer, int x, int y ,int believe )
{
	
	//��ʼ�����ƽṹ�壬����ɫ�ؼ��̶�����
	m_tacctrl[1].ctrl = CTRL_GOALKEEP;

	if (believe>=100)
	{
		m_tacctrl[2].ctrl = CTRL_ATTACK;
		m_tacctrl[3].ctrl = CTRL_STOP;
	} 
	else
	{
		//���������߽�ɫ
		m_tacctrl[2].ctrl = CTRL_MID_CATCH;
		//m_tacctrl[2].ctrl = CTRL_ATTACK;
		final_mid_pass_x = x;
		final_mid_pass_y = y;
		final_mid_pass_believe = believe;

		//3~5�Ŷ�Ա��λ�л������Ƕȷ���̬��
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

		//���潫���ط�Ϊ�������������򣬸��������������γɷ�������
		if (tmpball.y > 900)
		{
			//�ϲ�
			//����������֮�����һ��
			m_tacctrl[4].def_angle = 0;

			//����
			m_tacctrl[5].ctrl = CTRL_MOVETO;
			m_tacctrl[5].target_x = m_Pos_Back.x;
			m_tacctrl[5].target_y = m_Pos_Back.y+300;

		}
		else
		{
			if (tmpball.y > 300)
			{
				// 300<tmpball.y < 900 �м�����
				//����������֮�����һ��
				m_tacctrl[4].def_angle = 0;
				m_Fix_DefOutOffFrame(&(m_tacctrl[4]),0);
				m_Fix_DefInOurPenaltyArea(&(m_tacctrl[4]));

				//����
				m_tacctrl[5].ctrl = CTRL_MOVETO;
				m_tacctrl[5].target_x = m_Pos_Back.x;
				m_tacctrl[5].target_y = m_Pos_Back.y;
			}
			else
			{
				//tempball.y < 300 �°���
				//����������֮�����һ��
				m_tacctrl[4].def_angle = 0;

				//����
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
	
	if ((m_bTaskMidPassCatch==true) &&( m_bTaskMidPassCatch_kick==true) && m_bSomeoneHoldBall()==true)//[yzz]���ڽ����� Ӧ�û�ȥ����
	{
		m_bTaskMidPassCatch=false;
		m_bTaskMidPassCatch_kick=false;
		CString info;
		info.Format("�г��������\n");
		printf(info);
	}
	else if ((m_bTaskMidPassCatch==true) &&( m_bTaskMidPassCatch_kick==true))
	{
		//����������ɹ��������볡����Ա��Ҫ������,�����߼��� �ȴ����򣬲��������,������Ա��λ�� ��believe����10 ��ʱ�򣬼����ȴ�����>20��Ӧ����ƽ������ʱ�䣬20Ϊ����ֵ��ʱ����������
		if (m_nMidPassCatchBelieve_last >= 10 )//(m_nMidPassCatchBelieve_last<20) mc
		{
			info.Format("�г������Ѿ����!!!!%d\n",m_nMidPassCatchBelieve_last);
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
		//	//����������򱻶���
		//	m_bTaskMidPassCatch = false;
		//}
		//else
		{

		//CString info;
		info.Format("�г���������\n");
		printf(info);

		int believe = 0,px=0,py=0;
		BestPassPointTechChallenge2012(&px,&py,&believe, direction);//believeĬ��Ϊ0��>=10 ��ʼ׼����Ŵ���
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

		
		//������ɫ
		m_tacctrl[passNum].ctrl = CTRL_MID_PASS;	
		m_tacctrl[catchNum].ctrl = CTRL_MID_CATCH;

		final_mid_pass_x = px;//�������͸�����Ĵ����
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
		//��ʼ�������
		for (int i = 0; i < 11; i++)
			for (int j = 0; j < 3; j++)
			{
				FrontFieldPoint[j * 11 + i].x = j * 100 + 1100;
				FrontFieldPoint[j * 11 + i].y = (i + 1) * 100;
				FrontFieldPoint[j * 11 + i].valid = true;
			}

			//���ѡȡ�����
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
			
			//���ѡȡ�����
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

		//ͳ��֡������
		m_iEvaluteFrame++;

		//ÿ300֡��һ����
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

		//�����ɫ�ϰ����������ߵľ���
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


		//����������Ƿ񵽽����
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
	info.Format("�Ƿ�ֵ���%d;����Ϊ%d\n", PlayerX,PlayerY);
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


