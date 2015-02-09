#include "stdafx.h"
#include "NodeInput.h"



NodeInput::NodeInput(CToAgent** m_psendToAgent,
	uagent* m_thePlayersInformation,
	uagent* m_tempThePlayersInformation,
	MSG_Control* m_dicisionalPlayers)
{
	for (int i = 0; i < PLAYER_NUMBER; i++)
	{
		m_pSendToAgent[i] = m_psendToAgent[i];
		m_ThePlayersInformation[i] = &(m_thePlayersInformation[i]);
		m_TempThePlayersInformation[i] = &(m_tempThePlayersInformation[i]);
		m_DicisionalPlayers[i] = &(m_dicisionalPlayers[i]);
	}
}