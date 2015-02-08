/*************************************************
  http://robot.up-tech.com
  Author: 张万杰      Version: 2.4       Date: 2010/5/26
  Email:  zwj_uptech@126.com
  Description:    // 图片显示类
  Others:         // 1.1为MSLCoach做了改动，作为环境模型
				  // 1.4改进了连接断开时引发的错误
				  // 1.5加入了时间计量
				  // 1.6加入比赛状态显示
				  // 1.7对球的筛选方法进行了改进
				  // 1.8m_pb的管理方式参照m_pa，避免频繁的链表操作造成不稳定
				  // 1.9为彻底避免链表操作造成的不稳定可能性，改用数组管理球
				  // 2.0鉴于数组的稳定性，将球员管理也改用数组方式
				  // 2.1修正了多个转换到数组以后的bug
				  // 2.2加入手动控制模式
				  // 2.3大幅修改，适应09年规则
				  //2.4大幅修改，适应2010年规则
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
//障碍物判断
struct Dig_In{
	int x;
	int y;
	int size;
	bool believe;
};
//智能体结构
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
	UINT status;	//状态
	bool foundball;	//该ag是否看到了球
	int dist_ball;	//该ag与球的距离

	//log
	int logDefAngle;

	bool online;	//该ag是否在线
	bool allocated; //决策临时变量，表示ag是否已经分配角色
	bool isAgHoldBall; // 从单体传上来的参数，表示ag是否持球
};

//球 记录单个球信息
struct uball
{
	UINT ID;
	int x;
	int y;
	bool updated;
	uball* next;
};

//对球筛选过程中使用的临时结构体
struct suspball 
{
	int x;
	int y;
	int num;		//附近嫌疑球个数
	int distance;	//与中心点的距离
};
//传球的评估点结构体
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
//传接球队员的延迟
#define TIMER_CATCH_DELAY	14
#define PASS_END_DELAY	30

//distanc
#define TAC_DIST_ANTI	450		//对方开球时，我方机器人与球的距离
#define TAC_DIST_DEF	100		//防守过程中，我方机器人与球的距离
#define TAC_DIST_DROPBALL 200	//DropBall时，我方机器人与球的距离

#define TAC_DIST_PASS	100		//我方开球时，传球队员与球的距离
#define TAC_DIST_CATCH	250		//我方开球时，接球队员与球的距离

#define MOVETO_DEADZONE	50				//moveto行为的调节死区
#define MOVETO_ANGLE_DEADZONE	5		//moveto行为的调节死区

#define TAC_DIST_DEFCLOSE	250			//自主决策时，近身防守的距离
#define TAC_DIST_DEFFAR		350			//自主决策时，远程防守的距离
#define TAC_DYNAMIC_DEFDIST 300

#define TAC_DIST_3MeterPassJudge  350  // 判定三米传球的实际距离

//atk
#define AG_ATKADD	500
#define ATK_SW_DLY	20

//障碍物信息
#define EACH_AG_OBST_NUM	10

#define AG_NUM				7
#define OBST_RANGE			200
#define OBST_CLOSE_RANGE	100
#define OBST_DIST_RANGE		200

#define OBSTBUF_LEN		100

//球缓冲信息
#define BALLBUF_LEN		30

//裁判赋予队员的权限
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

	int m_memstatus[AG_NUM]; //记录上一帧各机器的状态 

	//Cmd Send to Agents
	CToAgent* pToAgent[7];

	//output
	CListBox* pSndList;
	bool m_bDebug;
	CListCtrl * pAgInfo;
	CListCtrl * pTacInfo;
	int m_nLogTacIndex;

	//角度movoto测试
	bool bMovetoTest;
	int nMovetoAng;

	//特殊任务

	bool m_bTaskPassCatch;
	bool m_bTaskPassCatch_kick;
	int m_nPassPlayer;
	int m_nCatchPlayer;
	CPoint m_posPass;
	bool m_bCatchDelay;
	int m_nPassEndDelay;
	//3 meter pass task
	CPoint InitialHoldBall;// 记录机器接球时的初始坐标wzc
	int DistToInitialHoldBallPoint; // 记录机器目前位置和初始坐标间的位移 wzc
	int MemHoldBallFlag; // 记录上一帧里机器是否拿球 wzc
	int HoldBallFlag;// 记录当前帧里机器是否拿球wzc
	int HoldBallBelieve; // 持球置信值 [wzc] // 舍弃不用 wzc
	int m_3meterPassBelieve;// 3米传球置信值 [wzc]
	int Last3meterCatcher;//上一个接球队员	[wzc]
	int Last3meterPasser;// 上一个传球队员  [wzc]
	bool m_bTask3MeterPass;// 3米传球启动标志  [wzc]
	bool m_bTask3MeterPass_Kick;//3米传球传出标志  [wzc]
	int Default_start_delay;// 开球延时，测试用，防止一开球就误判进入3米传球角色[wzc]
	int Attack_Convert_Flag;//3米传球失败转进攻的置信变量 [wzc]
	int Attack_Convert_Counter;//转换累加值
	int m_n3mPassPlayer;//3米传球者
	int m_n3mCatchPlayer;//3米接球者
	int Flag333;// 球接近又弹开标志
	CPoint memballpos;// 上一帧中球的位置
	//中场传球任务

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
	//技术挑战赛2012
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
	}m_eStage;				//比赛进行阶段(比赛的状态标记)
protected:
	preball m_preballs[7];  //zbw 球的路径预测
	CBitmap m_BkBmp;
	bool m_bField;		//进攻方向

	uagent m_agents[7];	//球员管理
	CCriticalSection m_asc;
	uagent m_tmpAgents[AG_NUM];
	uball m_balls[7];	//球的管理
	CCriticalSection m_bsc;
	uball m_tacballs[7];
	int* m_px;			//匹配点的x坐标
	int* m_py;			//匹配点的y坐标
	int m_pcnt;			//点数

	int m_nAgentNum;	//agent的个数

	//info of ball
	CPoint m_trueball;	//融合后的球坐标
	CPoint m_lastball;	//上一次球出现的位置
	bool m_bFoundball;	//确定是否有队员发现球
	CPoint m_Foreball;	//预测球的坐标
	BallShot m_ballshot[AG_NUM];	//球的起飞判定

	//tactic
	MSG_Control m_tacctrl[AG_NUM];	//决策临时变量，决策生成的角色排序
	uagent m_tacagent[AG_NUM];		//决策临时变量，储存ag的状态
	CPoint m_Pos_Back;				//后卫的坐标点
	bool m_bHalfready;				//对方迟迟未开球，我方准备进攻标记
	int m_nDistOfMultiDef;			//联防测试的距离值
	bool m_b10SecEnd;				//标记是否为对方未及时开球，以此决定进攻的跑位
	//int m_AG_ATKToMove;				//本来应该是进攻队员的，变成移动了，但是还是要照顾一下
	CPoint m_BallLogAt10SecEnd;		//记录

	uagent m_bakAtkAgent;			//状态备份，以用于异常状态的判断
	int m_nAtkNotMove;				//机器人未移动的时间倒计时
	int m_nAtkDly;					//削弱进攻角色的时长
	int m_nNumOfAtk;				//自主决策时进攻队员的数量
	int m_nSWAtkDly[AG_NUM];		//进攻切换延时

	//记录主开球与辅助开球的id，以维持这两个角色稳定性
	int m_curKOPrim;
	int m_curKOSlave;

	//状态显示
	CFont font;  
	CFont txtfont;
	CString m_strCurSt;
	CString m_strTime;
	CString m_strTmp;
	CCriticalSection m_critiTime;
	int m_recvcnt[AG_NUM];				//返回数据包计数
	int m_chkagnt;

	int m_eLastStage;

	//障碍物
	obstacle m_cObst[EACH_AG_OBST_NUM*(AG_NUM+1)];
	int m_nObstMsgCnt[AG_NUM];
	obstacle m_tacObst[EACH_AG_OBST_NUM*(AG_NUM+1)];
	obstacle m_tmpObst[EACH_AG_OBST_NUM*(AG_NUM+1)];
	obstacle m_tmpAgObst[EACH_AG_OBST_NUM];

	CPositionCycBuf m_BufObst[AG_NUM];
	MSL_Position m_tmpBufObst[OBSTBUF_LEN];

	//各队员发回球位置的缓冲
	CPositionCycBuf m_BufBall[AG_NUM];

	//队员相互分项信息
	teammate m_tm[AG_NUM];

	//裁判盒赋予队员的权利
	int m_prRightOfPlayer[AG_NUM];
	int m_nTimeToBeIn;

	//记录待入场队员在入场点附近匹配成功帧次数
	int m_prBeInPoint[AG_NUM];

	//技术挑战赛
	int m_nTCFindBall;
	bool m_bTCArrived;

	

// Operations
public:
	//技术挑战赛2012
	bool isStart;
	bool GetStart();
	//入场位置
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
	void APass_Normal(int inID,int inTargX, int inTargY);// 常规传球 [wzc]
	bool IsBallRollingToOwnGoal(CPoint memBallPos );// 检查球是否在向己方球门移动
	void Test_PassCatch();
	void m_TeammateShare();
	void SetObstacle(int inID,obstacle* inpObstAr);
	//寻找权重最大敌人目标
	void JudgeObstacle(int inID,obstacle* inpObstAr);
	void AClearObst(int inID);
	void Tac_RoundedDef(int inDefDist);
	void Tac_3Atk(int inDefDist);
	void Tac_ContainDef(int inDefDist);		//围堵防守
	void Display_VFlip();
	void Display_HFlip();
	void Field_Change();
	void Tac_ReleaseKickoff();
	void Tac_PressDef(int inDefDist);		//贴近防御
	void Tac_NormalAttack();
	void DefaultActive(int inDefDist);		//带后卫的默认进攻
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
	void Disactive();		//暂停自主决策
	void Active();			//激活自主决策
	void Tac_Default();		//默认的自主决策
	void Exec();			//被定时调用的决策函数
	void Tac_Wait10Sec();	
	void DefaultSTART();	//默认的开球行为
	void DefaultReady();	//默认的开球准备行为
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
	void START_Kickoff();	//开球
	void REF_KickOff();		//开球跑位

	void GetCurTime();
	void m_TacAction(uagent* inlist);		//按照策略进行角色分配
	void printf(CString instr);
	void m_EliminateFalseBall();			//根据各球员提供的球信息确定最可能的球坐标
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
	bool m_bHFlip;		//横向翻转
	bool m_bVFlip;		//纵向翻转
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
	bool m_bSomeoneHoldBall();       // wzc 通过和球的距离判定是否有人拿球是
	bool m_bSomeoneHoldBall_fromAG();// wzc 直接通过单体传上来的isAgHoldBall参数来判定拿球, for test use
	int m_nAgentWhoHoldBall();       // wzc 返回拿球队员的ID
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
