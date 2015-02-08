#if !defined(MSLPROTOCOL_H)
#define MSLPROTOCOL_H

//��Ϣ����	
#define MSL_INFO		1
#define MSL_CMD			2
#define MSL_OBST		3
#define MSL_TEAMMATE	4
#define MSL_KICK		5
#define MSL_INITPOS		6
#define MSL_HB_INFO     99

#define MSL_DEBUG		7
#define MSL_PREDICTBALL 8
//ctrl�궨�� 11

#define CTRL_ATK_CAT_CONVERT  103 // wzc for pass ball action
#define CTRL_STOP		0
#define CTRL_ATTACK		1

#define CTRL_GOALKEEP	2
#define CTRL_DEFENCE	3
#define CTRL_PASS		4
#define CTRL_CATCH		5
#define CTRL_TAC_WAIT   333

#define CTRL_MID_PASS		6
#define CTRL_MID_CATCH		7

#define CTRL_3M_PASS      8
#define CTRL_3M_CATCH     9

#define CTRL_MOVETO			11
#define CTRL_PASS_MOVE      133   // wzc for pass ball action
#define CTRL_BLOCK			12
#define CTRL_ATTENTIONBALL	13
#define CTRL_PRODEF			14
#define CTRL_SEARCHBALL		15
#define CTRL_SHIFTATK		16
#define CTRL_REMOTECTRL		17
#define CTRL_AROUNDBALL		18
#define CTRL_ATKCOVER		19
//dig in
#define CTRL_FOLLOWPOSITION 20
//pre ball
#define CTRL_PREDICTBALL     21
//
#define CTRL_TEST		40
#define CTRL_IDLE		41

//KickOff
#define CTRL_KICKOFF_PRIM_READY		50
#define CTRL_KICKOFF_SLAVE_READY	51
#define CTRL_KICKOFF_PRIM			52
#define CTRL_KICKOFF_SLAVE			53

//FreeKick
#define CTRL_FREEKICK_PRIM_READY	60
#define CTRL_FREEKICK_SLAVE_READY	61
#define CTRL_FREEKICK_PRIM			62
#define CTRL_FREEKICK_SLAVE			63

//Goalkick
#define CTRL_GOALKICK_PRIM_READY	70
#define CTRL_GOALKICK_SLAVE_READY	71
#define CTRL_GOALKICK_PRIM			72
#define CTRL_GOALKICK_SLAVE			73

//Throw in
#define CTRL_THROWIN_PRIM_READY		80
#define CTRL_THROWIN_SLAVE_READY	81
#define CTRL_THROWIN_PRIM			82
#define CTRL_THROWIN_SLAVE			83

//Corner Kick
#define CTRL_CORNERKICK_PRIM_READY	90
#define CTRL_CORNERKICK_SLAVE_READY	91
#define CTRL_CORNERKICK_PRIM		92
#define CTRL_CORNERKICK_SLAVE		93

//Anti_KickOff

//PENALTY
#define CTRL_PENALTY_READY			100
#define CTRL_PENALTY				101

//status
#define ROBST_NEW					110
#define ROBST_ERR					111
#define ROBST_PROBE					112

//������ս��
#define TC_FINDBALL					200

//�ϰ��� 
/*
struct obstacle
{
	UINT ID;	//�ϰ����Ŀ����ID
	int x;
	int y;
	int size;
	int DistToOwner;
	bool valid;
};
*/
//������
struct obstacle
{
	UINT ID;	//�ϰ����Ŀ����ID
	int x;
	int y;
	int size;
	int DistToOwner;
	bool valid;

};

struct teammate
{
	UINT ID;
	int x;
	int y;
	int V_angle;
	int V;
	bool valid;
};

#endif