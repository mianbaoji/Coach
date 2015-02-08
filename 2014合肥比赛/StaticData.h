
static const int PLAYER_NUMBER = 7;

enum CurrentStage
{
	STAGE_STOP,
	STAGE_AUTO,
	STAGE_START,
	STAGE_ENTER,
	STAGE_OUT,
	STAGE_MANUAL,
	STAGE_MIDCATCH,

	STAGE_KICK_OFF_READY,
	STAGE_FREE_KICK_READY,
	STAGE_GOAL_KICK_READY,
	STAGE_THROW_IN_READY,
	STAGE_CORNER_KICK_READY,
	STAGE_CORNER_KICK_START,
	STAGE_PENALTY_READY,

	STAGE_DROPPED_BALL_READY,

	STAGE_ANTI_KICK_OFF_READY,
	STAGE_ANTI_FREE_KICK_READY,
	STAGE_ANTI_GOAL_KICK_READY,
	STAGE_ANTI_THROW_IN_READY,
	STAGE_ANTI_CORNER_KICK_READY,
	STAGE_ANTI_CORNER_KICK_START,
	STAGE_ANTI_PENALTY_READY,

	STAGE_TEST_MULTI_DEF,
	STAGE_TEST_PRESS_DEF,
	STAGE_TEST_PASS_CATCH,
	STAGE_TECH_CHALLENGE,
	STAGE_TECH_CHALLENGE_2012,
	STAGE_TEST_ATK_COVER,
};

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

