// RefUDP.h: interface for the CRefUDP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REFUDP_H__BF0243CC_110A_4135_9CD1_D4B23484C2A6__INCLUDED_)
#define AFX_REFUDP_H__BF0243CC_110A_4135_9CD1_D4B23484C2A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UDPServer.h"

class CRefUDP : public CUDPServer  
{
public:
	void Received(void *inbuf, int inlen, SOCKADDR *inpSock);
	CRefUDP();
	virtual ~CRefUDP();

};

#endif // !defined(AFX_REFUDP_H__BF0243CC_110A_4135_9CD1_D4B23484C2A6__INCLUDED_)
