// ToAgent.h: interface for the CToAgent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOAGENT_H__A14C67D1_131F_4902_BA0A_F5DBC730260B__INCLUDED_)
#define AFX_TOAGENT_H__A14C67D1_131F_4902_BA0A_F5DBC730260B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UDPServer.h"
#include "MSL_Protocol.h"

#define TO_AG_OB_NUM	10

class CToAgent : public CUDPServer  
{
public:
	//
	void FollowPosition(int inCtrl,int goalx,int goaly,int angle,int dist);
	//zbw pre ball
	void Predictball(int inCtrl,int inReBx,int inReBy,float jiao,float chang);
	void SetInitPos(int inx,int iny,int inangle);
	void SendWithInitPos(BYTE *inBuf, int inTail);
	void SendInitPos(int inX,int inY,int inAng);
	void CatchCmd(int inFromX, int inFromY);
	void CatchCmd_Normal(int inFromX, int inFromY);
	void PassCmd(int inTargX, int inTargY);
	void PassCmd_Normal(int inTargX, int inTargY);
	void Send_InfoOfTM(teammate* inAgent,int inNum);
	void MoveTo(int inTargetx, int inTargety, int inTargetAng, int inDist, bool inbField,int inSpd);
	void Send_Obst();
	void Clear_Obst();
	void AddObst(int inX, int inY, int inSize);
	void DefCmd(int inAngle,int inDist);
	void SetRefBall(int inBallx,int inBally);
	void MoveCmd(int inTargetx,int inTargety, int inTargetAng, int inDist, bool inbField);
	void CtrlCmd(int inCtrl,int inReBx,int inReBy, int inAng, int inDist, bool inbField);
	void SetID(int inID);
	void CtrlMidPassCmd( int ctrl, int mid_pass_x, int mid_pass_y, int mid_pass_believe,int mid_pass_jiao );
	CToAgent();
	virtual ~CToAgent();

protected:
	void m_SendWithTail(BYTE* inBuf,int inTail);
	unsigned char m_calsum(unsigned char *inBuf, int inlen);
	void m_Split2Bytes(unsigned char *inTarg, unsigned int inSrc);
	int m_Piece2int(unsigned char *inTarg);
	
	unsigned char m_sendbuf[1024];

	//’œ∞≠ŒÔ
	obstacle m_cObstToSend[TO_AG_OB_NUM];

	bool m_bInitPos;
	int m_nInit_x;
	int m_nInit_y;
	int m_nInit_angle;
};

#endif // !defined(AFX_TOAGENT_H__A14C67D1_131F_4902_BA0A_F5DBC730260B__INCLUDED_)
