// CoachUDP.h: interface for the CCoachUDP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COACHUDP_H__2CB0229F_841F_4740_9F86_33E8577CABBA__INCLUDED_)
#define AFX_COACHUDP_H__2CB0229F_841F_4740_9F86_33E8577CABBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UDPServer.h"
#include "UPFullField.h"
#include "ToAgent.h"

#define CNTMAX 20   //最大连接数

typedef struct  
{
	unsigned char ip[4];
	int ID;
	int reccnt;
}AgCnt;

class CCoachUDP : public CUDPServer  
{
public:
	void AAddObstToBuf(int inID, int inX, int inY, int inSize);
	void AClearObstBuf(int inID);
	void AddCredit(int inUnknown,int inCredit);
	void ClearCredit(int inIndex);
	void AllCredit();
	void UpdateCntList();
	void m_ag_Scan();
	int m_Piece2int(unsigned char* inTarg);
	void m_ParseFrame(unsigned char *inBuf, int inLen);
	void m_ParseByte(unsigned char inChar);
	void Received(void *inbuf, int inlen, SOCKADDR* inpSock);
	CCoachUDP();
	virtual ~CCoachUDP();
	
	AgCnt m_unknownCnt[CNTMAX];		//未知的连接
	int	m_nCntNum;				//未知连接数量
	CListCtrl* m_pCntList;		//显示未知连接
	
	AgCnt m_creditCnt[5];			//信任连接
	bool m_bEnIPFilter;				//是否启用ip过滤

	unsigned char m_curIP[4];	//当前数据的源ip
	int m_curID;				//当前正在解析的数据球员ID

	CToAgent m_ToAgent[7];		//向agents发送数据
	unsigned char m_sendbuf[UPUDPBUFLEN];//发送缓冲

	CUPFullField* pfield;		//策略显示控件

	bool m_bFoundBall;
	bool m_bHoldBall;//临时变量
	unsigned char* pbuf;		//临时变量

	unsigned char m_ParseBuf[UPUDPBUFLEN];
	int m_nRecvIndex;			//接收索引
	unsigned char m_lastRecv;	//上一个字符
	int m_bFrameStart;			//帧解析开始
	int m_nFrameLength;			//帧长度

	//障碍物信息缓冲
	//obstacle m_cObstBuf[EACH_AG_OBST_NUM*(AG_NUM+1)];
	obstacle m_cObstBuf[EACH_AG_OBST_NUM*(AG_NUM+1)];
protected:
	int i;
	bool m_CheckCredit(unsigned char* inIP);
	unsigned char m_calsum(unsigned char *inBuf, int inlen);
};

#endif // !defined(AFX_COACHUDP_H__2CB0229F_841F_4740_9F86_33E8577CABBA__INCLUDED_)
