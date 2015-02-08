// RefClient.cpp: implementation of the CRefClient class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MSLCoach.h"
#include "RefClient.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRefClient::CRefClient()
{
	pRec = NULL;
	m_lastflag = '0';
	pfield = NULL;
	Self = CYAN;
	pGoalCombo = NULL;

	m_nLenofStr = 0;
	m_eGameFlow = BeforeGame;
}

CRefClient::~CRefClient()
{

}

void CRefClient::Received(void *inbuf, int inlen)
{
	pRec = (char*) inbuf;
	if (inlen < 2)
	{
		Parse(*pRec);
	}
	else
	{
		for (int i=0;i<inlen;i++)
		{
			Parse(pRec[i]);
		}
	}
// 	m_strMessage.Format("rec %s ,%d",pRec,inlen);
// 	printf(m_strMessage);
}

void CRefClient::Parse(char inchar)
{	
	//message
	if (m_nLenofStr > 0)
	{
		CString strTmp;
		strTmp.Format("%c",inchar);
		m_strMessage += strTmp;
		m_nLenofStr--;
		if (m_nLenofStr == 0)
		{
			m_strMessage = "[MSG]:" + m_strMessage;
			printf(m_strMessage);
		}
		return;
	}

	switch(inchar)
	{
	case COMM_START:
	//	printf("COMM_START");
		Ref_Start();
		break;
	case COMM_STOP:
		printf("COMM_STOP");
		pfield->REF_Stop();
		break;
		
	case COMM_HALT:
		printf("COMM_HALT");
		pfield->REF_Stop();
		break;

	case COMM_READY:
		printf("COMM_READY");
		break;

	case COMM_PARKING:
		printf("COMM_PARKING");
		if (m_eGameFlow != EndGame)
		{
			pfield->Enter();
		}
		break;
		
// game flow commands
	case COMM_FIRST_HALF:
		printf("COMM_FIRST_HALF");
		m_eGameFlow = FirstHalf;
		break;
	case COMM_HALF_TIME:
		printf("COMM_HALF_TIME");
		m_eGameFlow = HalfTime;
		break;
	case COMM_SECOND_HALF:
		printf("COMM_SECOND_HALF");
		m_eGameFlow = SecondHalf;
		break;
	case COMM_END_GAME:
		printf("COMM_END_GAME");
		m_eGameFlow = EndGame;
		break;
	case COMM_CANCEL:
		printf("COMM_CANCEL");
		break;

// goal status
	case COMM_GOAL_MAGENTA:
		printf("COMM_GOAL_MAGENTA");
		switch(Self)
		{
		case CYAN:
			break;
		case MAGENTA:
			break;
		}
		break;
	case COMM_GOAL_CYAN:
		printf("COMM_GOAL_CYAN");
		switch(Self)
		{
		case CYAN:
			break;
		case MAGENTA:
			break;
		}
		break;
	case COMM_SUBGOAL_MAGENTA:
		printf("COMM_SUBGOAL_MAGENTA");
		switch(Self)
		{
		case CYAN:
			break;
		case MAGENTA:
			break;
		}
		break;
	case COMM_SUBGOAL_CYAN:
		printf("COMM_SUBGOAL_CYAN");
		switch(Self)
		{
		case CYAN:
			break;
		case MAGENTA:
			break;
		}
		break;

// game flow commands 
	case COMM_RESTART:
		printf("COMM_RESTART");
		break;

	case COMM_KICKOFF_MAGENTA:
		printf("COMM_KICKOFF_MAGENTA");
		switch(Self)
		{
		case CYAN:
			pfield->REF_Anti_Kickoff();
			break;
		case MAGENTA:
			pfield->REF_KickOff();
			break;
		}
		m_lastflag = inchar;
		break;

	case COMM_KICKOFF_CYAN:
		printf("COMM_KICKOFF_CYAN");
		switch(Self)
		{
		case CYAN:
			pfield->REF_KickOff();
			break;
		case MAGENTA:
			pfield->REF_Anti_Kickoff();
			break;
		}
		m_lastflag = inchar;
		break;

	case COMM_FREEKICK_MAGENTA:
		printf("COMM_FREEKICK_MAGENTA");
		switch(Self)
		{
		case CYAN:
			pfield->REF_Anti_Freekick();
			break;
		case MAGENTA:
			pfield->REF_Freekick();
			break;
		}
		m_lastflag = inchar;
		break;

	case COMM_FREEKICK_CYAN:
		printf("COMM_FREEKICK_CYAN");
		switch(Self)
		{
		case CYAN:
			pfield->REF_Freekick();
			break;
		case MAGENTA:
			pfield->REF_Anti_Freekick();
			break;
		}
		m_lastflag = inchar;
		break;

	case COMM_GOALKICK_MAGENTA:
		printf("COMM_GOALKICK_MAGENTA");
		switch(Self)
		{
		case CYAN:
			pfield->REF_Anti_Goalkick();
			break;
		case MAGENTA:
			pfield->REF_Goalkick();
			break;
		}
		m_lastflag = inchar;
		break;

	case COMM_GOALKICK_CYAN:
		printf("COMM_GOALKICK_CYAN");
		switch(Self)
		{
		case CYAN:
			pfield->REF_Goalkick();
			break;
		case MAGENTA:
			pfield->REF_Anti_Goalkick();
			break;
		}
		m_lastflag = inchar;
		break;

	case COMM_THROWIN_MAGENTA:
		printf("COMM_THROWIN_MAGENTA");
		switch(Self)
		{
		case CYAN:
			pfield->REF_Anti_Throwin();
			break;
		case MAGENTA:
			pfield->REF_Throwin();
			break;
		}
		m_lastflag = inchar;
		break;

	case COMM_THROWIN_CYAN:
		printf("COMM_THROWIN_CYAN");
		switch(Self)
		{
		case CYAN:
			pfield->REF_Throwin();
			break;
		case MAGENTA:
			pfield->REF_Anti_Throwin();
			break;
		}
		m_lastflag = inchar;
		break;

	case COMM_CORNER_MAGENTA:
		printf("COMM_CORNER_MAGENTA");
		switch(Self)
		{
		case CYAN:
			pfield->REF_Anti_CornerKick();
			break;
		case MAGENTA:
			pfield->REF_CornerKick();
			break;
		}
		m_lastflag = inchar;
		break;

	case COMM_CORNER_CYAN:
		printf("COMM_CORNER_CYAN");
		switch(Self)
		{
		case CYAN:
			pfield->REF_CornerKick();
			break;
		case MAGENTA:
			pfield->REF_Anti_CornerKick();
			break;
		}
		m_lastflag = inchar;
		break;

	case COMM_PENALTY_MAGENTA:
		printf("COMM_PENALTY_MAGENTA");
		switch(Self)
		{
		case CYAN:
			pfield->REF_Anti_Penalty();
			break;
		case MAGENTA:
			pfield->REF_Penalty();
			break;
		}
		m_lastflag = inchar;
		break;

	case COMM_PENALTY_CYAN:
		printf("COMM_PENALTY_CYAN");
		switch(Self)
		{
		case CYAN:
			pfield->REF_Penalty();
			break;
		case MAGENTA:
			pfield->REF_Anti_Penalty();
			break;
		}
		m_lastflag = inchar;
		break;

	case COMM_DROPPED_BALL:
		printf("COMM_DROPPED_BALL");
		pfield->REF_DroppedBall();
		m_lastflag = inchar;
		break;
	case COMM_DUMMY:
		printf("COMM_DUMMY");
		break;

	//other
	case 'W':
		m_strMessage.Format("W");
		m_nLenofStr = 8;
		break;

	case 'R':
		m_strMessage.Format("R");
		m_nLenofStr = 8;
		break;

	default:	
		static CString str;
		str.Format("[unknown] %c",inchar);
		printf(str);
		break;
	}
	
	pfield->Invalidate();
}

/************************************************************************/
/*                            REF_START                                 */
/************************************************************************/
void CRefClient::Ref_Start()
{
	switch(m_lastflag)
	{
		case COMM_KICKOFF_MAGENTA:
		printf("START_KICKOFF_MAGENTA");
		switch(Self)
		{
		case CYAN:
			pfield->START_Anti_Kickoff();
			break;
		case MAGENTA:
			pfield->START_Kickoff();
			break;
		}
		break;

	case COMM_KICKOFF_CYAN:
		printf("START_KICKOFF_CYAN");
		switch(Self)
		{
		case CYAN:
			pfield->START_Kickoff();
			break;
		case MAGENTA:
			pfield->START_Anti_Kickoff();
			break;
		}
		break;

	case COMM_FREEKICK_MAGENTA:
		printf("START_FREEKICK_MAGENTA");
		switch(Self)
		{
		case CYAN:
			pfield->START_Anti_Freekick();
			break;
		case MAGENTA:
			pfield->START_Freekick();
			break;
		}
		break;

	case COMM_FREEKICK_CYAN:
		printf("START_FREEKICK_CYAN");
		switch(Self)
		{
		case CYAN:
			pfield->START_Freekick();
			break;
		case MAGENTA:
			pfield->START_Anti_Freekick();
			break;
		}
		break;
	case COMM_GOALKICK_MAGENTA:
		printf("START_GOALKICK_MAGENTA");
		switch(Self)
		{
		case CYAN:
			pfield->START_Anti_Goalkick();
			break;
		case MAGENTA:
			pfield->START_Goalkick();
			break;
		}
		break;

	case COMM_GOALKICK_CYAN:
		printf("START_GOALKICK_CYAN");
		switch(Self)
		{
		case CYAN:
			pfield->START_Goalkick();
			break;
		case MAGENTA:
			pfield->START_Anti_Goalkick();
			break;
		}
		break;

	case COMM_THROWIN_MAGENTA:
		printf("START_THROWIN_MAGENTA");
		switch(Self)
		{
		case CYAN:
			pfield->START_Anti_Throwin();
			break;
		case MAGENTA:
			pfield->START_Throwin();
			break;
		}
		break;

	case COMM_THROWIN_CYAN:
		printf("START_THROWIN_CYAN");
		switch(Self)
		{
		case CYAN:
			pfield->START_Throwin();
			break;
		case MAGENTA:
			pfield->START_Anti_Throwin();
			break;
		}
		break;

	case COMM_CORNER_MAGENTA:
		printf("START_CORNER_MAGENTA");
		switch(Self)
		{
		case CYAN:
			pfield->START_Anti_CornerKick();
			break;
		case MAGENTA:
			pfield->START_CornerKick();
			break;
		}
		break;

	case COMM_CORNER_CYAN:
		printf("START_CORNER_CYAN");
		switch(Self)
		{
		case CYAN:
			pfield->START_CornerKick();
			break;
		case MAGENTA:
			pfield->START_Anti_CornerKick();
			break;
		}
		break;

	case COMM_PENALTY_MAGENTA:
		printf("START_PENALTY_MAGENTA");
		switch(Self)
		{
		case CYAN:
			pfield->START_Anti_Penalty();
			break;
		case MAGENTA:
			pfield->START_Penalty();
			break;
		}
		break;

	case COMM_PENALTY_CYAN:
		printf("START_PENALTY_CYAN");
		switch(Self)
		{
		case CYAN:
			pfield->START_Penalty();
			break;
		case MAGENTA:
			pfield->START_Anti_Penalty();
			break;
		}
		break;

	case COMM_DROPPED_BALL:
		printf("START_DROPPED_BALL");
		pfield->START_DroppedBall();
		break;

	default:
		break;
	}
	pfield->Invalidate();
}

void CRefClient::isDisconnected()
{
	if (pfield != NULL)
	{
		pfield->bCntRefbox = false;
		pfield->Invalidate();
	}
}

void CRefClient::ConnectSucceed()
{
	if (pfield != NULL)
	{
		pfield->bCntRefbox = true;
		pfield->Invalidate();
	}
}

void CRefClient::PlayerOut(CString inTeam, int inID, int inSec)
{
	if (inTeam == "Cyan")
	{
		if (Self == CYAN)
		{
			pfield->PR_Out(inID);
		}
	} 
	else
	{
		if (Self == MAGENTA)
		{
			pfield->PR_Out(inID);
		}
	}
}

void CRefClient::PlayerIn(CString inTeam, int inID)
{
	if (inTeam == "Cyan")
	{
		if (Self == CYAN)
		{
			pfield->PR_Ready(inID);
		}
	} 
	else
	{
		if (Self == MAGENTA)
		{
			pfield->PR_Ready(inID);
		}
	}
}

void CRefClient::PlayerSubstitution(CString inTeam, int inIn, int inOut)
{
	if (inTeam == "Cyan")
	{
		if (Self == CYAN)
		{
			pfield->PR_Ready(inIn);
			pfield->PR_Out(inOut);
		}
	} 
	else
	{
		if (Self == MAGENTA)
		{
			pfield->PR_Ready(inIn);
			pfield->PR_Out(inOut);
		}
	}
}
