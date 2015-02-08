/*
 **********************************************************************
 *	Copyright (C) 2015.
 *	All Rights Reserved.
 **********************************************************************
 *
 *	@file		NodeInput.h
 *	@brief		The class of behavior tree's input.
 *
 *	Date        Name        Description
 *	06/02/15	ssj			Creation.
 *********************************************************************/

#pragma once

#include "UPFullField.h"


class NodeInput
{
public:
	NodeInput(CToAgent** m_psendToAgent, uagent* m_thePlayersInformation, 
		uagent* m_tempThePlayersInformation);
	~NodeInput();

protected:
	

	CToAgent* m_pSendToAgent[PLAYER_NUMBER]; /**< The array saved messages to send  to the players */
	uagent* m_ThePlayersInformation[PLAYER_NUMBER];/**< The array saved messages for  decision-making */
	uagent* m_TempThePlayersInformation[PLAYER_NUMBER]; /**< The temp array saved messages for  decision-making*/

};
