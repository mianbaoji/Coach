/*
 **********************************************************************
 *	Copyright (C) 2015.
 *	All Rights Reserved.
 **********************************************************************
 *
 *	@file		NodeExternalPrecondition.h
 *	@brief		The class about external precondition of behavior tree's node.
 *
 *	Date        Name        Description
 *	06/02/15	ssj			Creation.
 *********************************************************************/

//#pragma once

#if !defined(AFX_OWNGOALCOMBO_H__AFB535D2_CED2_4F92_B092_F6BAC17451FA)
#define AFX_OWNGOALCOMBO_H__AFB535D2_CED2_4F92_B092_F6BAC17451FA


#include "NodeInput.h"

class NodeExternalPrecondition
{
public:
	/**
	 *	\brief External enaluate.
	 *
	 *  \param	nodeInput	Node's input.
	 *
	 *	\return	The result of whether or not input accord to external precondition.
	 */
	virtual bool Evaluate(const NodeInput& nodeInput)const = 0;
};

#endif



