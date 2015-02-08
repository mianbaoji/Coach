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
#include "MSLCoachDlg.h"

//#define SendToAgent pToAgent /**< The variables saved messages to send  to the players */
//#define ThePlayersInformation m_agents /**< The variables saved messages to send  to the players */


class NodeInput
{
public:
	NodeInput(CToAgent* m_psendToAgent, uagent* m_thePlayersInformation, 
		uagent* m_tempThePlayersInformation);
	~NodeInput();

protected:
	CToAgent* m_pSendToAgent[7]; /**< The array saved messages to send  to the players */
	uagent* m_ThePlayersInformation[7];/**< The array saved messages for  decision-making */
	uagent* m_TempThePlayersInformation[7]; /**< The temp array saved messages for  decision-making*/

	static const int PLAYER_NUMBER = 7;
};
