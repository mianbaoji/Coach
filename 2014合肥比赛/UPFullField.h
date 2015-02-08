/*************************************************
  http://robot.up-tech.com
  Author: �����      Version: 2.4       Date: 2010/5/26
  Email:  zwj_uptech@126.com
  Description:    // ͼƬ��ʾ��
  Others:         // 1.1ΪMSLCoach���˸Ķ�����Ϊ����ģ��
				  // 1.4�Ľ������ӶϿ�ʱ�����Ĵ���
				  // 1.5������ʱ�����
				  // 1.6�������״̬��ʾ
				  // 1.7�����ɸѡ���������˸Ľ�
				  // 1.8m_pb�Ĺ���ʽ����m_pa������Ƶ�������������ɲ��ȶ�
				  // 1.9Ϊ���ױ������������ɵĲ��ȶ������ԣ��������������
				  // 2.0����������ȶ��ԣ�����Ա����Ҳ�������鷽ʽ
				  // 2.1�����˶��ת���������Ժ��bug
				  // 2.2�����ֶ�����ģʽ
				  // 2.3����޸ģ���Ӧ09�����
				  //2.4����޸ģ���Ӧ2010�����
*************************************************/

#if !defined(AFX_UPFULLFIELD_H__6D15FBB6_5DE4_408C_AE94_83C240DA6ED4__INCLUDED_)
#define AFX_UPFULLFIELD_H__6D15FBB6_5DE4_408C_AE94_83C240DA6ED4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UPFullField.h : header file
//
#include <math.h>
#include "NumDisplay.h"
#include <afxmt.h>
#include "ToAgent.h"
#include "MSG_Control.h"
#include "MSL_Protocol.h"
#include "PositionCycBuf.h"
// #include "2DFilter.h"

#include"NodeInput.h"



#include "ballshot.h"
//�ϰ����ж�
struct Dig_In{
	int x;
	int y;
	int size;
	bool believe;
};
//������ṹ
struct preball{
	UINT ID;
	float jiao;
	float chang;
	preball * next;
};
//
struct uagent 
{
	int ID;
	int x;
	int y;
	double angle;

	int V;
	int V_Angle;

	//move target log
	int moveto_x;
	int moveto_y;

	//status
	UINT status;	//״̬
	bool foundball;	//��ag�Ƿ񿴵�����
	int dist_ball;	//��ag����ľ���

	//log
	int logDefAngle;

	bool online;	//��ag�Ƿ�����
	bool allocated; //������ʱ��������ʾag�Ƿ��Ѿ������ɫ
	bool isAgHoldBall; // �ӵ��崫�����Ĳ�������ʾag�Ƿ����
};

//�� ��¼��������Ϣ
struct uball
{
	UINT ID;
	int x;
	int y;
	bool updated;
	uball* next;
};

//����ɸѡ������ʹ�õ���ʱ�ṹ��
struct suspball 
{
	int x;
	int y;
	int num;		//�������������
	int distance;	//�����ĵ�ľ���
};
//�����������ṹ��
typedef struct evalutepoint
{
	int x;
	int y;
	bool valid;
}evalutepoint;

//Field
#define PENALTY_LONG	650
#define PENALTY_WIDTH	225
#define GOAL_LONG		350
#define GOAL_WIDTH		75

#define BLUEGOAL 0
#define YELLOWGOAL 1

#define TAC_WAIT_TIMER 10
#define PENALTY_PRE	   11
#define TAC_HALFREADY  12
#define PLAYOUT_COUNT	13
//�������Ա���ӳ�
#define TIMER_CATCH_DELAY	14
#define PASS_END_DELAY	30

//distanc
#define TAC_DIST_ANTI	450		//�Է�����ʱ���ҷ�����������ľ���
#define TAC_DIST_DEF	100		//���ع����У��ҷ�����������ľ���
#define TAC_DIST_DROPBALL 200	//DropBallʱ���ҷ�����������ľ���

#define TAC_DIST_PASS	100		//�ҷ�����ʱ�������Ա����ľ���
#define TAC_DIST_CATCH	250		//�ҷ�����ʱ�������Ա����ľ���

#define MOVETO_DEADZONE	50				//moveto��Ϊ�ĵ�������
#define MOVETO_ANGLE_DEADZONE	5		//moveto��Ϊ�ĵ�������

#define TAC_DIST_DEFCLOSE	250			//��������ʱ��������صľ���
#define TAC_DIST_DEFFAR		350			//��������ʱ��Զ�̷��صľ���
#define TAC_DYNAMIC_DEFDIST 300

#define TAC_DIST_3MeterPassJudge  350  // �ж����״����ʵ�ʾ���

//atk
#define AG_ATKADD	500
#define ATK_SW_DLY	20

//�ϰ�����Ϣ
#define EACH_AG_OBST_NUM	10

#define AG_NUM				7
#define OBST_RANGE			200
#define OBST_CLOSE_RANGE	100
#define OBST_DIST_RANGE		200

#define OBSTBUF_LEN		100

//�򻺳���Ϣ
#define BALLBUF_LEN		30

//���и����Ա��Ȩ��
#define PLAYER_RIGHT_INFIELD		0
#define PLAYER_RIGHT_READY			1
#define PLAYER_RIGHT_WAITFOEPOINT	2
#define PLAYER_RIGHT_OUT			3

#define PLAYER_RIGHT_RANGETOPOINT	100

#define PLAYER_TIME_TO_BEIN			150					

/////////////////////////////////////////////////////////////////////////////
// CUPFullField window

class CUPFullField : public CStatic
{
// Construction
public:
	CUPFullField();

// Attributes
public:
	//zbw dig_in
	Dig_In m_follow[7];
	int goalx,goaly,goalangle;
	//zbw predict ball
	float tpballjiao,tpballchang;
	float lpballjiao,lpballchang;
	//
	UINT OwnGoalColor;

	double midcatch_n,midcatch_x,midcatch_y,midcatch_b;
	//select
	int SelID;
	CPoint m_self;
	CPoint m_sTp;
	CNumDisplay* pNum;
	double m_SelAng;
	float m_fKField;

	int m_memstatus[AG_NUM]; //��¼��һ֡��������״̬ 

	//Cmd Send to Agents
	CToAgent* pToAgent[7];

	//output
	CListBox* pSndList;
	bool m_bDebug;
	CListCtrl * pAgInfo;
	CListCtrl * pTacInfo;
	int m_nLogTacIndex;

	//�Ƕ�movoto����
	bool bMovetoTest;
	int nMovetoAng;

	//��������

	bool m_bTaskPassCatch;
	bool m_bTaskPassCatch_kick;
	int m_nPassPlayer;
	int m_nCatchPlayer;
	CPoint m_posPass;
	bool m_bCatchDelay;
	int m_nPassEndDelay;
	//3 meter pass task
	CPoint InitialHoldBall;// ��¼��������ʱ�ĳ�ʼ����wzc
	int DistToInitialHoldBallPoint; // ��¼����Ŀǰλ�úͳ�ʼ������λ�� wzc
	int MemHoldBallFlag; // ��¼��һ֡������Ƿ����� wzc
	int HoldBallFlag;// ��¼��ǰ֡������Ƿ�����wzc
	int HoldBallBelieve; // ��������ֵ [wzc] // �������� wzc
	int m_3meterPassBelieve;// 3�״�������ֵ [wzc]
	int Last3meterCatcher;//��һ�������Ա	[wzc]
	int Last3meterPasser;// ��һ�������Ա  [wzc]
	bool m_bTask3MeterPass;// 3�״���������־  [wzc]
	bool m_bTask3MeterPass_Kick;//3�״��򴫳���־  [wzc]
	int Default_start_delay;// ������ʱ�������ã���ֹһ��������н���3�״����ɫ[wzc]
	int Attack_Convert_Flag;//3�״���ʧ��ת���������ű��� [wzc]
	int Attack_Convert_Counter;//ת���ۼ�ֵ
	int m_n3mPassPlayer;//3�״�����
	int m_n3mCatchPlayer;//3�׽�����
	int Flag333;// ��ӽ��ֵ�����־
	CPoint memballpos;// ��һ֡�����λ��
	//�г���������

	bool m_bTaskMidPassCatch;
	bool m_bTaskMidPassCatch_kick;
	bool m_isMidPasserCannotTakeBall;
	int m_MidPasserCannotTakeBallCount;
	int m_nMidPassPlayer;
	int m_nMidCatchPlayer;
	CPoint m_posFinalMidPass;
	CPoint m_posBestMidPass_last;
	int m_nMidPassCatchBelieve_last;
	bool m_bMidCatchDelay;
	int m_nMidPassEndDelay;

	//ref
	bool bCntRefbox;
	//������ս��2012
	enum E_TechStep2012
	{
		s_start,
		s_search1,
		s_run1,
		s_pass1,
		s_run2,
		s_pass2,
		s_kick
	}techStep2012;
	enum {
		sStop,
		sAUTO,
		sStart,
		sEnter,
		sOut,
		sMani,
		midcatch,

		sKickOffReady,
		sFreekickReady,
		sGoalkickReady,
		sThrowinReady,
		sCornerKickReady,
		sCornerKickStart,
		sPenaltyReady,

		sDroppedBallReady,

		sAntiKickOffReady,
		sAntiFreekickReady,
		sAntiGoalkickReady,
		sAntiThrowinReady,
		sAntiCornerKickReady,
		sAntiCornerKickStart,
		sAntiPenaltyReady,

		sTestMultiDef,
		sTestPressDef,
		sTestPassCatch,
		sTechChallenge,
		sTechChallenge2012,
		sTestATKCover,
	}m_eStage;				//�������н׶�(������״̬���)
protected:
	preball m_preballs[7];  //zbw ���·��Ԥ��
	CBitmap m_BkBmp;
	bool m_bField;		//��������

	uagent m_agents[7];	//��Ա����
	CCriticalSection m_asc;
	uagent m_tmpAgents[AG_NUM];
	uball m_balls[7];	//��Ĺ���
	CCriticalSection m_bsc;
	uball m_tacballs[7];
	int* m_px;			//ƥ����x����
	int* m_py;			//ƥ����y����
	int m_pcnt;			//����

	int m_nAgentNum;	//agent�ĸ���

	//info of ball
	CPoint m_trueball;	//�ںϺ��������
	CPoint m_lastball;	//��һ������ֵ�λ��
	bool m_bFoundball;	//ȷ���Ƿ��ж�Ա������
	CPoint m_Foreball;	//Ԥ���������
	BallShot m_ballshot[AG_NUM];	//�������ж�

	//tactic
	MSG_Control m_tacctrl[AG_NUM];	//������ʱ�������������ɵĽ�ɫ����
	uagent m_tacagent[AG_NUM];		//������ʱ����������ag��״̬
	CPoint m_Pos_Back;				//�����������
	bool m_bHalfready;				//�Է��ٳ�δ�����ҷ�׼���������
	int m_nDistOfMultiDef;			//�������Եľ���ֵ
	bool m_b10SecEnd;				//����Ƿ�Ϊ�Է�δ��ʱ�����Դ˾�����������λ
	//int m_AG_ATKToMove;				//����Ӧ���ǽ�����Ա�ģ�����ƶ��ˣ����ǻ���Ҫ�չ�һ��
	CPoint m_BallLogAt10SecEnd;		//��¼

	uagent m_bakAtkAgent;			//״̬���ݣ��������쳣״̬���ж�
	int m_nAtkNotMove;				//������δ�ƶ���ʱ�䵹��ʱ
	int m_nAtkDly;					//����������ɫ��ʱ��
	int m_nNumOfAtk;				//��������ʱ������Ա������
	int m_nSWAtkDly[AG_NUM];		//�����л���ʱ

	//��¼�������븨�������id����ά����������ɫ�ȶ���
	int m_curKOPrim;
	int m_curKOSlave;

	//״̬��ʾ
	CFont font;  
	CFont txtfont;
	CString m_strCurSt;
	CString m_strTime;
	CString m_strTmp;
	CCriticalSection m_critiTime;
	int m_recvcnt[AG_NUM];				//�������ݰ�����
	int m_chkagnt;

	int m_eLastStage;

	//�ϰ���
	obstacle m_cObst[EACH_AG_OBST_NUM*(AG_NUM+1)];
	int m_nObstMsgCnt[AG_NUM];
	obstacle m_tacObst[EACH_AG_OBST_NUM*(AG_NUM+1)];
	obstacle m_tmpObst[EACH_AG_OBST_NUM*(AG_NUM+1)];
	obstacle m_tmpAgObst[EACH_AG_OBST_NUM];

	CPositionCycBuf m_BufObst[AG_NUM];
	MSL_Position m_tmpBufObst[OBSTBUF_LEN];

	//����Ա������λ�õĻ���
	CPositionCycBuf m_BufBall[AG_NUM];

	//��Ա�໥������Ϣ
	teammate m_tm[AG_NUM];

	//���ки����Ա��Ȩ��
	int m_prRightOfPlayer[AG_NUM];
	int m_nTimeToBeIn;

	//��¼���볡��Ա���볡�㸽��ƥ��ɹ�֡����
	int m_prBeInPoint[AG_NUM];

	//������ս��
	int m_nTCFindBall;
	bool m_bTCArrived;

	

// Operations
public:
	//������ս��2012
	bool isStart;
	bool GetStart();
	//�볡λ��
	CPoint m_fpPlayerIn;
	bool m_bSetPlayerIn;

	bool m_bObst_Org;
// 	C2DFilter m_obstFilter;

	int m_nAgCanBeInPenaltyArea;

	int final_mid_pass_x;
	int final_mid_pass_y;
	int final_mid_pass_believe;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUPFullField)
	//}}AFX_VIRTUAL

// Implementation
public:
	void Leave();
	void m_Fix_GetOutOfTheWayOfATK(MSG_Control* inCtrl);
	void Tac_RoundedDef3(int inDefDist);
	void m_ShowTacInfo(int inCtrl,int inAgentId);
	void m_Fix_AroundOutOffFrame(MSG_Control* inTac);
	void DropBall_Ready(int inDistFB);
	void TC_Demo1();
	void TC_Demo2012();
	int m_Obst_NearBy(int inx, int iny, obstacle *inObstAr,int inNumOfObst);
	void BallShotRes(char inRes,int inx,int iny);
	void DefaultReady2();
	void m_Fix_MoveOutOfOurPenaltyArea(MSG_Control* inTac,int inDist);
	bool GetAgentOnline(int inID);
	bool m_Obst_RemoveOutOfField(obstacle *inObstAr);
	void m_Fix_DefInOurPenaltyArea(MSG_Control* inTac);
	void Tac_PressDef2(int inDefDist);
	void Tac_RoundedDef2(int inDefDist);
	void Default_Anti_Ready_Add(int inDist);
	int m_Obst_NearBy(int inx,int iny,obstacle* inObstAr);
	void PR_AllReadyCanBeIn();
	void PR_Ready(int inID);
	void PR_Out(int inID);
	void Attention_SomeOneKick(int inID,int inSelfX,int inSelfY,int inTargetX,int inTargetY);
	void m_taskPassCatchEnd();
	void ACatch(int inID, int inFromX, int inFromY);
	void ACatch_Normal(int inID, int inFromX, int inFromY);
	void APass(int inID,int inTargX,int inTargY);
	void APass_Normal(int inID,int inTargX, int inTargY);// ���洫�� [wzc]
	bool IsBallRollingToOwnGoal(CPoint memBallPos );// ������Ƿ����򼺷������ƶ�
	void Test_PassCatch();
	void m_TeammateShare();
	void SetObstacle(int inID,obstacle* inpObstAr);
	//Ѱ��Ȩ��������Ŀ��
	void JudgeObstacle(int inID,obstacle* inpObstAr);
	void AClearObst(int inID);
	void Tac_RoundedDef(int inDefDist);
	void Tac_3Atk(int inDefDist);
	void Tac_ContainDef(int inDefDist);		//Χ�·���
	void Display_VFlip();
	void Display_HFlip();
	void Field_Change();
	void Tac_ReleaseKickoff();
	void Tac_PressDef(int inDefDist);		//��������
	void Tac_NormalAttack();
	void DefaultActive(int inDefDist);		//��������Ĭ�Ͻ���
	void ManiMode();
	void TestMode();
	void midcatchmode();
	void Enter();
	void Test_checkeveryone();
	void m_ShowAgentsInfo();
	void Test_PressDef();
	void Test_Multidef(int inDist);
	void KickAgent(int inID);
	void Tac_Wait(int inSec);
	CString GetTime();
	CString GetCTRLString(int inCtrlCode);
	void Default_Anti_Ready(int inDistFB);
	void Default_Anti_START();
	int TheBestAtk(uagent* inList);
	bool AChangeTo(int inID,int inToindex,uagent* inList);
	void AChangeStatus(int inID,UINT inCtrlCode, double inAngle, int inDist);
	void AFollowPosition(int inID, UINT inCtrlCode,double inAngle, int inDist);
	void Disactive();		//��ͣ��������
	void Active();			//������������
	void Tac_Default();		//Ĭ�ϵ���������
	void Exec();			//����ʱ���õľ��ߺ���
	void Tac_Wait10Sec();	
	void DefaultSTART();	//Ĭ�ϵĿ�����Ϊ
	void DefaultReady();	//Ĭ�ϵĿ���׼����Ϊ
	void ChangeOwnGoal(UINT inOwnGoal);
	void START_Anti_Penalty();
	void START_Anti_CornerKick();
	void START_Anti_Throwin();
	void START_Anti_Goalkick();
	void START_Anti_Freekick();
	void START_Anti_Kickoff();
	void REF_Stop();
	void REF_Anti_Penalty();
	void REF_Anti_CornerKick();
	void REF_Anti_Throwin();
	void REF_Anti_Goalkick();
	void REF_Anti_Freekick();
	void REF_Anti_Kickoff();
	void START_DroppedBall();//
	void REF_DroppedBall();	//
	void START_Penalty();	//
	void REF_Penalty();		//
	void START_CornerKick();//
	void REF_CornerKick();	//
	void START_Throwin();	//
	void REF_Throwin();		//
	void START_Goalkick();	//
	void REF_Goalkick();	//
	void START_Freekick();	//
	void REF_Freekick();	//
	void START_Kickoff();	//����
	void REF_KickOff();		//������λ

	void GetCurTime();
	void m_TacAction(uagent* inlist);		//���ղ��Խ��н�ɫ����
	void printf(CString instr);
	void m_EliminateFalseBall();			//���ݸ���Ա�ṩ������Ϣȷ������ܵ�������
	void m_EliminateFalseObst();
	void AChangeStatus(int inID,UINT inCtrlCode, int inAng);
	void AMoveTo(int ID,int inx,int iny, int inAngle);
	uagent* GetAgent(UINT inID);
	void RemoveBall(UINT inID);
	void RemoveAgent(int inID);
	//preballs
	void SetPreball(UINT inID,float jiao,float chang);
	void SetBall(UINT inID,int inx,int iny);
	void RemovePoints();
	void SetPoints(int* inx,int* iny,int inNum);
	void SetAgent(UINT inID,int inx,int iny,double inangle,int inV,int inV_Angle,UINT inStatus,bool infb,int inhb);//[wzc] & [cyg] 
	void SetHoldBallInfo(UINT ID,int inhb);
	//void BestPassPoint(int *EvaluteX, int *EvaluteY, int *CanPassBall);
	void kong();
	int kongx;
	int kongy;
	int m_iEvaluteFrame;
	int m_iLastX, m_iLastY;
	virtual ~CUPFullField();
	CPoint SelectCatchPoint(CPoint Pass_Point);
	// Generated message map functions
public:
	bool m_bAg_InParkingArea();
	void m_Fix_GetOutOfTheWayOfATK();
	void m_Fix_DefOutOffFrame(MSG_Control* inCtrl,int inReachOut);
	void m_ObstFusion();
	void m_ResetAtkDly(int inID);
	int m_Ag_GetMoveTo(int x, int y, uagent* inalist);
	void m_CheckAgentStatus();
	int m_Ag_GetStatus(UINT inSt, uagent* inalist, bool inAllocated);
	void m_Fix_CollisionAvoid(MSG_Control *inTac, int inAvoidDist);
	CPoint m_Loca_CalBackPos(int inballx,int inbally,int inBackx);
	void m_Fix_KeepDistance(uagent* inList, int inAvoidDist, int inFixDist);
	void m_Fix_MoveTargetDist(MSG_Control* inList,int inAvoidDist,int inFixDist);
	double m_CalAngle(int inx1, int iny1, int inVx, int inVy, int inx2, int iny2);
	CPoint m_Point_Field2Img(int inx,int iny);
	CPoint m_Point_Img2Field(int inx, int iny);
	bool m_bHFlip;		//����ת
	bool m_bVFlip;		//����ת
	void Tac_DefTest(int inDefDist);
	void m_Ball_Forecast();
	void m_Fix_MoveOutOffFrame(MSG_Control* inTac);
	void m_Fix_MovetoOurPenaltyArea(MSG_Control* inTac);
	bool m_Check_MoveOutOffFrame(MSG_Control inTac);
	bool m_Check_MovetoOurPenaltyArea(MSG_Control inTac);
	void m_Fix_outOffFrame(uagent inagent);
	void m_Fix_inOurPenaltyArea(uagent inagent);
	bool m_Check_outOffFrame(uagent inagent,int inReachOut);
	bool m_Check_inOurPenaltyArea(uagent inagent);
	bool m_bSomeoneHoldBall();       // wzc ͨ������ľ����ж��Ƿ�����������
	bool m_bSomeoneHoldBall_fromAG();// wzc ֱ��ͨ�����崫������isAgHoldBall�������ж�����, for test use
	int m_nAgentWhoHoldBall();       // wzc ���������Ա��ID
	int m_nAgentWhoHoldBall_fromAg();// wzc return the Id of the Ag who hold ball through prameter "isAgHoldBall", for test use
    void m_bClear3meterPassInfo();
	bool m_bCatchHoldBall();
	int m_Loca_CalAngle(int inXbase,int inYbase,int inXtarg,int inYtarg);
	CPoint m_Loca_Ray(int inXbase,int inYbase,int inXtarg,int inYtarg,int inDist);
	CPoint m_Loca_Rotate(int inx,int iny,int inCx,int inCy,int inAng);
	int m_Ag_GetStatus(UINT inSt, uagent* inalist);
	int m_Ag_GetNearBy(int x,int y, uagent* inalist);
	void m_CheckAgentReply();
	void Ready_KickOff();
	void Ready_Anti_Penalty();
	void Ready_Anti_CornerKick();
	void Ready_Anti_Throwin();
	void Ready_Anti_Goalkick();
	void Ready_Anti_Freekick();
	void Ready_Anti_Kickoff();
	void Ready_DroppedBall();
	void Ready_Penalty();
	void Ready_CornerKick();
	void Ready_Throwin();
	void Ready_Goalkick();
	void Ready_Freekick();
	int m_Perform_ATK(uagent* inagent);
	bool m_bGoalKeeperHoldBall();
	int m_CalDist(int inx1,int iny1,int inx2,int iny2);
	void m_Clearpa();
	void m_Clearpb();
	//{{AFX_MSG(CUPFullField)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	int i;
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	void Tac_MidPass(int defdis,int px,int py,int believe);
	void Tac_3meterPass(int holdballID);
	void Tac_3meterPass_Kicked();
	void BestPassPoint(int *EvaluteX, int *EvaluteY, int *CanPassBall );
	public:
	void AChangeMidPassStatus( int inID, int ctrl, int mid_pass_x, int mid_pass_y, int mid_pass_believe,int mid_pass_jiao=0 );
	int TheBestMidCatch( uagent *inList ,int x,int y);
	int SelectBestCatcher(CPoint tmp);
	void Tac_MidPass_kicked( int midCatchPlayer, int x, int y ,int believe);
	int Cal_Angle_Mid_Catch();
	int jiajiao( int jizhun, int jiao );
	void GoPass( int passNum,int catchNum, int direction );
	void BestPassPointTechChallenge2012(int *EvaluteX, int *EvaluteY, int *CanPassBall, int direction);
	void Attention_DEBUG( unsigned char inBuf, int PlayerX, int PlayerY, int TargetX, int TargetY );
	//afx_msg void OnBnClickedButton1();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UPFULLFIELD_H__6D15FBB6_5DE4_408C_AE94_83C240DA6ED4__INCLUDED_)
