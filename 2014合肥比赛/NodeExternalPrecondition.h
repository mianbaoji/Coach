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

#pragma once

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

