// MSG_Control.h: MSG_Control struct.
// 
//////////////////////////////////////////////////////////////////////

/*************************************************
  http://robot.up-tech.com
  Author: 张万杰      Version: 1.3       Date: 2009/06/09
  Email:  zwj_uptech@126.com
  Description:    // 策略角色信息结构体
  Others:         // 
*************************************************/

#if !defined(AFX_MSG_CONTROL_H__F41F9D4C_4AB4_4C2A_9B68_F3C05F91D61E__INCLUDED_)
#define AFX_MSG_CONTROL_H__F41F9D4C_4AB4_4C2A_9B68_F3C05F91D61E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct MSG_Control 
{
	UINT ctrl;

	bool targetGoal;
	int target_x;
	int target_y;
	int target_angle;

	int from_x;
	int from_y;
	
	int def_dist;
	int def_angle;

	int re_ballx;
	int re_bally;
};

#endif // !defined(AFX_MSG_CONTROL_H__F41F9D4C_4AB4_4C2A_9B68_F3C05F91D61E__INCLUDED_)
