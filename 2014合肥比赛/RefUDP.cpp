// RefUDP.cpp: implementation of the CRefUDP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MSLCoach.h"
#include "RefUDP.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRefUDP::CRefUDP()
{

}

CRefUDP::~CRefUDP()
{

}

void CRefUDP::Received(void *inbuf, int inlen, SOCKADDR *inpSock)
{
	if (inlen > 0)
	{
	}
}
