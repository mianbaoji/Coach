// TAC_Kickoff.cpp: implementation of the TAC_Kickoff class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MSLCoach.h"
#include "TAC_Kickoff.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TAC_Kickoff::TAC_Kickoff()
{
	m_ctrl[1].ctrl = CTRL_GOALKEEP;

	m_ctrl[2].ctrl = CTRL_KICKOFF_PRIM;

	m_ctrl[3].ctrl = CTRL_KICKOFF_SLAVE;

	//ko_ctrl[]
}

TAC_Kickoff::~TAC_Kickoff()
{

}

void TAC_Kickoff::Ready(UINT inOwnGoalColor,CPoint inBall)
{
	//开球跑位
	m_ctrl[1].ctrl = CTRL_GOALKEEP;
	m_ctrl[2].ctrl = CTRL_KICKOFF_PRIM_READY;
	m_ctrl[3].ctrl = CTRL_KICKOFF_SLAVE_READY;

	//其他队员跑动到指定点
	for (int i=4;i<7;i++)
	{
		m_ctrl[i].ctrl = CTRL_MOVETO;	
	}

	//设置指定目标
	OwnGoalColor = inOwnGoalColor;

	for (i=1;i<7;i++)
	{
		if (inOwnGoalColor == 1)
		{
			m_ctrl[i].targetGoal = 0;
		}
		else
		{
			m_ctrl[i].targetGoal = 1;
		}
	}
	//计算跑动位置
}

void TAC_Kickoff::Go(UINT inOwnGoalColor, CPoint inBall)
{
	m_ctrl[1].ctrl = CTRL_GOALKEEP;

	m_ctrl[2].ctrl = CTRL_KICKOFF_PRIM;

	m_ctrl[3].ctrl = CTRL_KICKOFF_SLAVE;

	//其他队员跑位到指定点
	for (int i=4;i<7;i++)
	{
		m_ctrl[i].ctrl = CTRL_MOVETO;	
	}
	OwnGoalColor = inOwnGoalColor;
	for (i=1;i<7;i++)
	{
		if (inOwnGoalColor == 1)
		{
			m_ctrl[i].targetGoal = 0;
		}
		else
		{
			m_ctrl[i].targetGoal = 1;
		}
	}
}
