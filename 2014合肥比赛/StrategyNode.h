/*
 **********************************************************************
 *	Copyright (C) 2015.
 *	All Rights Reserved.
 **********************************************************************
 *
 *	@file		StrategyNode.h
 *	@brief		The class of decisiton-behavior tree's node.		
 *
 *	Date        Name        Description
 *	07/02/15	ssj			Creation.
 *********************************************************************/

#if !defined(AFX_OWNGOALCOMBO_H__AFB535D2_CED2_4F92_B092)
#define AFX_OWNGOALCOMBO_H__AFB535D2_CED2_4F92_B092

#include"Node.h"

class StageStopNode :NodePrioritySelector
{
public:
	StageStopNode(Node* p_parentNode, const char* p_name,
		NodeExternalPrecondition* p_externalPrecondition);
};

#endif