// TAC_Kickoff.h: interface for the TAC_Kickoff class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TAC_KICKOFF_H__2C75764C_76EE_487E_906F_4164827A5E24__INCLUDED_)
#define AFX_TAC_KICKOFF_H__2C75764C_76EE_487E_906F_4164827A5E24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "MSG_Control.h"
#include "ITactic.h"

class TAC_Kickoff : public ITactic
{
public:
	void Go(UINT inOwnGoalColor,CPoint inBall);
	void Ready(UINT inOwnGoalColor,CPoint inBall);
	UINT OwnGoalColor;
	TAC_Kickoff();
	virtual ~TAC_Kickoff();

};

#endif // !defined(AFX_TAC_KICKOFF_H__2C75764C_76EE_487E_906F_4164827A5E24__INCLUDED_)
