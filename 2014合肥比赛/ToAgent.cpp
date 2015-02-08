// ToAgent.cpp: implementation of the CToAgent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MSLCoach.h"
#include "ToAgent.h"
#include "MSL_Protocol.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToAgent::CToAgent()
{
	m_sendbuf[0] = 0x55;
	m_sendbuf[1] = 0xaa;

	for (int i=0;i<TO_AG_OB_NUM;i++)
	{
		m_cObstToSend[i].valid = false;
	}

	m_bInitPos = false;

	m_nInit_x = 900;
	m_nInit_y = 0;
	m_nInit_angle = 0;
}

CToAgent::~CToAgent()
{

}

void CToAgent::SetID(int inID)
{
	m_sendbuf[3] = (unsigned char)inID;
}

void CToAgent::CtrlCmd(int inCtrl, int inReBx, int inReBy, int inAng, int inDist, bool inbField)
{
	m_sendbuf[2] = 15;
	m_sendbuf[4] = MSL_CMD;
	m_Split2Bytes(&(m_sendbuf[5]),inReBx);		//rebx
	m_Split2Bytes(&(m_sendbuf[7]),inReBy);		//reby	
	m_sendbuf[9] = (unsigned char)inCtrl;		//ctrl
	m_Split2Bytes(&(m_sendbuf[10]),inAng);		//angle
	m_Split2Bytes(&(m_sendbuf[12]),inDist);		// dist

	m_sendbuf[14] = m_calsum(m_sendbuf,14);		//sum
	//Send((char*)m_sendbuf,15);
	SendWithInitPos(m_sendbuf,15);
}

int CToAgent::m_Piece2int(unsigned char *inTarg)
{
	short ret;
	ret = inTarg[0];
	ret <<= 8;
	ret &= 0xff00;
	ret |= ((int)inTarg[1]&0x00ff);
	return ret;
}

void CToAgent::m_Split2Bytes(unsigned char *inTarg, unsigned int inSrc)
{
	if (inTarg == NULL)
	{
		return;
	}

	unsigned short temp = (unsigned short)0x0000ffff&inSrc;
	inTarg[1] = (unsigned char)temp&0x00ff;

	temp >>= 8;

	inTarg[0] = (unsigned char)temp&0x00ff;
}

unsigned char CToAgent::m_calsum(unsigned char *inBuf, int inlen)
{
	unsigned char sum = 0;
	for (int i=0;i<inlen;i++)
	{
		sum += inBuf[i];
	}
	return sum;
}

void CToAgent::MoveCmd(int inTargetx, int inTargety, int inTargetAng, int inDist, bool inbField)
{
	//将角度限制在0～360里
	while (inTargetAng < 0)
	{
		inTargetAng += 360;
	}

	while (inTargetAng > 360)
	{
		inTargetAng -= 360;
	}

	if (inDist<0)
	{
		inDist = -inDist;
	}

	m_sendbuf[2] = 18;								//len
	m_sendbuf[4] = MSL_CMD;
	m_sendbuf[9] = CTRL_MOVETO;						//ctrl
	m_Split2Bytes(&(m_sendbuf[10]),inTargetx);		//x
	m_Split2Bytes(&(m_sendbuf[12]),inTargety);		//y
	m_Split2Bytes(&(m_sendbuf[14]),inTargetAng);	//angle
	m_sendbuf[16] = 100;

	m_sendbuf[17] = m_calsum(m_sendbuf,17);			//sum
	Send((char*)m_sendbuf,18);
	
//	m_SendWithTail(m_sendbuf,18);
}

void CToAgent::SetRefBall(int inBallx, int inBally)
{
	m_Split2Bytes(&(m_sendbuf[5]),inBallx);			//x
	m_Split2Bytes(&(m_sendbuf[7]),inBally);			//y
}

void CToAgent::DefCmd(int inAngle, int inDist)
{
	//将角度限制在0～360里
	while (inAngle < 0)
	{
		inAngle += 360;
	}
	
	while (inAngle > 360)
	{
		inAngle -= 360;
	}
	
	if (inDist<0)
	{
		inDist = -inDist;
	}
	
	m_sendbuf[2] = 15;								//len
	m_sendbuf[4] = MSL_CMD;
	m_sendbuf[9] = CTRL_DEFENCE;					//ctrl
	m_Split2Bytes(&(m_sendbuf[10]),inAngle);		//ang
	m_Split2Bytes(&(m_sendbuf[12]),inDist);			//dist
	
	m_sendbuf[14] = m_calsum(m_sendbuf,14);			//sum
	Send((char*)m_sendbuf,15);
	//m_SendWithTail(m_sendbuf,15);
}

void CToAgent::AddObst(int inX, int inY, int inSize)
{
	for(int i=0;i<TO_AG_OB_NUM;i++)
	{
		if (m_cObstToSend[i].valid == false)
		{
			m_cObstToSend[i].ID = m_sendbuf[3];
			m_cObstToSend[i].x = inX;
			m_cObstToSend[i].y = inY;
			m_cObstToSend[i].size = inSize;
			m_cObstToSend[i].valid = true;
			break;
		}
	}
}

void CToAgent::Clear_Obst()
{
	for (int i=0;i<TO_AG_OB_NUM;i++)
	{
		m_cObstToSend[i].valid = false;	
	}
}

void CToAgent::Send_Obst()
{
	m_sendbuf[4] = MSL_OBST;
	
	//统计障碍物信息
	int nNumObst = 0;
	for (int i=0;i<TO_AG_OB_NUM;i++)
	{
		if (m_cObstToSend[i].valid == true)
		{
			m_Split2Bytes(&m_sendbuf[6+5*nNumObst],m_cObstToSend[i].x);
			m_Split2Bytes(&m_sendbuf[6+5*nNumObst+2],m_cObstToSend[i].y);
			m_sendbuf[6+5*nNumObst+4] = m_cObstToSend[i].size;
			
			nNumObst ++;
		}
	}
// 	if (nNumObst <= 0)
// 	{
// 		return;
// 	}
	m_sendbuf[2] = 6+5*nNumObst+1;
	m_sendbuf[5] = nNumObst;
	
	m_sendbuf[6+5*nNumObst] = m_calsum(m_sendbuf,6+5*nNumObst);	//校验和
	
	Send((char*)m_sendbuf,6+5*nNumObst+1);
}

void CToAgent::MoveTo(int inTargetx, int inTargety, int inTargetAng, int inDist, bool inbField, int inSpd)
{
	//将角度限制在0～360里
	while (inTargetAng < 0)
	{
		inTargetAng += 360;
	}
	
	while (inTargetAng > 360)
	{
		inTargetAng -= 360;
	}
	
	if (inDist<0)
	{
		inDist = -inDist;
	}
	
	m_sendbuf[2] = 18;								//len
	m_sendbuf[4] = MSL_CMD;
	m_sendbuf[9] = CTRL_MOVETO;						//ctrl
	m_Split2Bytes(&(m_sendbuf[10]),inTargetx);		//x
	m_Split2Bytes(&(m_sendbuf[12]),inTargety);		//y
	m_Split2Bytes(&(m_sendbuf[14]),inTargetAng);	//angle
	m_sendbuf[16] = (UCHAR)inSpd;
	
	m_sendbuf[17] = m_calsum(m_sendbuf,17);			//sum
	Send((char*)m_sendbuf,18);
	
	//m_SendWithTail(m_sendbuf,18);
}

void CToAgent::Send_InfoOfTM(teammate *inAgent, int inNum)
{
	m_sendbuf[4] = MSL_TEAMMATE;
	
	//统计障碍物信息
	int nNumTM = 0;
	for (int i=0;i<inNum;i++)
	{
		if (inAgent[i].valid == true && inAgent[i].ID != m_sendbuf[3])
		{
			m_Split2Bytes(&m_sendbuf[6+7*nNumTM],inAgent[i].x);
			m_Split2Bytes(&m_sendbuf[6+7*nNumTM+2],inAgent[i].y);
			m_Split2Bytes(&m_sendbuf[6+7*nNumTM+4],inAgent[i].V_angle);
			m_sendbuf[6+7*nNumTM+6] = (unsigned char)inAgent[i].V;
			
			nNumTM ++;
		}
	}
// 	if (nNumTM <= 0)
// 	{
// 		return;
// 	}
	m_sendbuf[2] = 6+7*nNumTM+1;
	m_sendbuf[5] = nNumTM;

	m_sendbuf[6+7*nNumTM] = m_calsum(m_sendbuf,6+7*nNumTM);	//校验和
	
	Send((char*)m_sendbuf,m_sendbuf[2]);
} 

void CToAgent::PassCmd(int inTargX, int inTargY)
{
	m_sendbuf[2] = 15;							//len
	m_sendbuf[4] = MSL_CMD;
	m_sendbuf[9] = CTRL_PASS;					//ctrl
	m_Split2Bytes(&(m_sendbuf[10]),inTargX);	//tx
	m_Split2Bytes(&(m_sendbuf[12]),inTargY);	//ty
	
	m_sendbuf[14] = m_calsum(m_sendbuf,14);		//sum
	Send((char*)m_sendbuf,15);
}
void CToAgent::PassCmd_Normal(int inTargX, int inTargY)
{
	m_sendbuf[2] = 15;							//len
	m_sendbuf[4] = MSL_CMD;
	m_sendbuf[9] = CTRL_3M_PASS;					//ctrl
	m_Split2Bytes(&(m_sendbuf[10]),inTargX);	//tx
	m_Split2Bytes(&(m_sendbuf[12]),inTargY);	//ty
	
	m_sendbuf[14] = m_calsum(m_sendbuf,14);		//sum
	Send((char*)m_sendbuf,15);
}
void CToAgent::CatchCmd(int inFromX, int inFromY)
{
	m_sendbuf[2] = 15;							//len
	m_sendbuf[4] = MSL_CMD;
	m_sendbuf[9] = CTRL_CATCH;					//ctrl
	m_Split2Bytes(&(m_sendbuf[10]),inFromX);	//fx
	m_Split2Bytes(&(m_sendbuf[12]),inFromY);	//fy
	
	m_sendbuf[14] = m_calsum(m_sendbuf,14);		//sum
	Send((char*)m_sendbuf,15);
}
void CToAgent::CatchCmd_Normal(int inFromX, int inFromY)
{
	m_sendbuf[2] = 15;							//len
	m_sendbuf[4] = MSL_CMD;
	m_sendbuf[9] = CTRL_3M_CATCH;					//ctrl
	m_Split2Bytes(&(m_sendbuf[10]),inFromX);	//fx
	m_Split2Bytes(&(m_sendbuf[12]),inFromY);	//fy
	
	m_sendbuf[14] = m_calsum(m_sendbuf,14);		//sum
	Send((char*)m_sendbuf,15);
}

void CToAgent::SendInitPos(int inX, int inY, int inAng)
{
	unsigned char Sendbuf[13];
	Sendbuf[0] = 0x55;
	Sendbuf[1] = 0xaa;
	Sendbuf[2] = 13;
	Sendbuf[3] = m_sendbuf[3];
	Sendbuf[4] = MSL_INITPOS;
	Sendbuf[5] = 1;
	m_Split2Bytes(&(Sendbuf[6]),inX);
	m_Split2Bytes(&(Sendbuf[8]),inY);
	m_Split2Bytes(&(Sendbuf[10]),inAng);
	Sendbuf[12] = m_calsum(Sendbuf,12);		//sum
	Send((char*)Sendbuf,13);
}

void CToAgent::m_SendWithTail(BYTE *inBuf, int inTail)
{
	//obst
	BYTE* pObstBuf = &(inBuf[inTail]);
	pObstBuf[0] = m_sendbuf[0];
	pObstBuf[1] = m_sendbuf[1];
	pObstBuf[3] = m_sendbuf[3];
	pObstBuf[4] = MSL_OBST;
	
	//统计障碍物信息
	int nNumObst = 0;
	for (int i=0;i<TO_AG_OB_NUM;i++)
	{
		if (m_cObstToSend[i].valid == true)
		{
			m_Split2Bytes(&pObstBuf[6+5*nNumObst],m_cObstToSend[i].x);
			m_Split2Bytes(&pObstBuf[6+5*nNumObst+2],m_cObstToSend[i].y);
			pObstBuf[6+5*nNumObst+4] = m_cObstToSend[i].size;
			
			nNumObst ++;
		}
	}
	pObstBuf[2] = 6+5*nNumObst+1;
	pObstBuf[5] = nNumObst;
	
	pObstBuf[6+5*nNumObst] = m_calsum(pObstBuf,6+5*nNumObst);	//校验和
	
	//Send((char*)m_sendbuf,6+5*nNumObst+1);
	//////////////////////////////////////////////////////////////////////////

	//teammate
	// 	BYTE* pTMBuf = &(pObstBuf[6+5*nNumObst+1]);
//	pTMBuf[0] = m_sendbuf[0];
//	pTMBuf[1] = m_sendbuf[1];
// 	pTMBuf[4] = MSL_TEAMMATE;
// 	
// 	//统计队员信息
// 	int nNumTM = 0;
// 	for (i=0;i<7;i++)
// 	{
// 		if (inAgent[i].valid == true && inAgent[i].ID != pTMBuf[3])
// 		{
// 			m_Split2Bytes(&pTMBuf[6+7*nNumTM],inAgent[i].x);
// 			m_Split2Bytes(&pTMBuf[6+7*nNumTM+2],inAgent[i].y);
// 			m_Split2Bytes(&pTMBuf[6+7*nNumTM+4],inAgent[i].V_angle);
// 			pTMBuf[6+7*nNumTM+6] = (unsigned char)inAgent[i].V;
// 			
// 			nNumTM ++;
// 		}
// 	}
// 	pTMBuf[2] = 6+7*nNumTM+1;
// 	pTMBuf[5] = nNumTM;
// 	
// 	pTMBuf[6+7*nNumTM] = m_calsum(pTMBuf,6+7*nNumTM);	//校验和
	
	//Send((char*)m_sendbuf,m_sendbuf[2]);
	//////////////////////////////////////////////////////////////////////////
	Send((char*)inBuf,inBuf[2] + pObstBuf[2]);
}

void CToAgent::SendWithInitPos(BYTE *inBuf, int inTail)
{
	if (m_bInitPos == true)
	{
		BYTE* initbuf = &(inBuf[inTail]);
		initbuf[0] = 0x55;
		initbuf[1] = 0xaa;
		initbuf[2] = 13;
		initbuf[3] = m_sendbuf[3];
		initbuf[4] = MSL_INITPOS;
		initbuf[5] = 1;
		m_Split2Bytes(&(initbuf[6]),m_nInit_x);
		m_Split2Bytes(&(initbuf[8]),m_nInit_y);
		m_Split2Bytes(&(initbuf[10]),m_nInit_angle);
		initbuf[12] = m_calsum(initbuf,12);		//sum
		Send((char*)inBuf,inBuf[2] + initbuf[2]);

		m_bInitPos = false;
	}
	else
	{
		Send((char*)inBuf,inTail);
	}
}	

void CToAgent::SetInitPos(int inx, int iny, int inangle)
{
	m_nInit_x = inx;
	m_nInit_y = iny;
	m_nInit_angle = inangle;
	m_bInitPos = true;
}

void CToAgent::CtrlMidPassCmd( int ctrl, int mid_pass_x, int mid_pass_y, int mid_pass_believe,int mid_pass_jiao )
{
	//throw std::exception("The method or operation is not implemented.");

	m_sendbuf[2] = 19;								//len
	m_sendbuf[4] = MSL_CMD;
	m_sendbuf[9] = ctrl;						//ctrl
	m_Split2Bytes(&(m_sendbuf[10]),mid_pass_x);		//x
	m_Split2Bytes(&(m_sendbuf[12]),mid_pass_y);		//y
	m_Split2Bytes(&(m_sendbuf[14]),mid_pass_believe);	//b
	m_Split2Bytes(&(m_sendbuf[16]),mid_pass_jiao);
	m_sendbuf[18] = m_calsum(m_sendbuf,18);			//sum
	Send((char*)m_sendbuf,19);
}

void CToAgent::Predictball(int inCtrl,int inReBx,int inReBy,float jiao,float chang){
	
	//
	m_sendbuf[2] = 15;
	m_sendbuf[4] = MSL_CMD;
	m_Split2Bytes(&(m_sendbuf[5]),inReBx);		//rebx
	m_Split2Bytes(&(m_sendbuf[7]),inReBy);		//reby	
	m_sendbuf[9] = (unsigned char)inCtrl;		//ctrl
	m_Split2Bytes(&(m_sendbuf[10]),jiao*100);		//angle
	m_Split2Bytes(&(m_sendbuf[12]),chang*100);		// dist

	m_sendbuf[14] = m_calsum(m_sendbuf,14);		//sum
	Send((char*)m_sendbuf,15);
	
}


void CToAgent::FollowPosition(int inCtrl,int goalx,int goaly,int angle,int dist){

	m_sendbuf[2] = 18;
	m_sendbuf[4] = MSL_CMD;

	if(goaly>600){
	m_Split2Bytes(&(m_sendbuf[10]),goalx+50);		//rebx
	m_Split2Bytes(&(m_sendbuf[12]),goaly-50);		//reby	
	m_sendbuf[9] = (unsigned char)CTRL_MOVETO;		//ctrl
    m_Split2Bytes(&(m_sendbuf[14]),0);		//angle
	}
	else{
	m_Split2Bytes(&(m_sendbuf[10]),goalx+50);		//rebx
	m_Split2Bytes(&(m_sendbuf[12]),goaly+50);		//reby	
	m_sendbuf[9] = (unsigned char)CTRL_MOVETO;		//ctrl
    m_Split2Bytes(&(m_sendbuf[14]),0);		//angle
	}
    m_sendbuf[16] = 100;

	m_sendbuf[17] = m_calsum(m_sendbuf,17);			//sum
	Send((char*)m_sendbuf,18);
}
