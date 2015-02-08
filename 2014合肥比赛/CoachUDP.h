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

#define CNTMAX 20   //���������

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
	
	AgCnt m_unknownCnt[CNTMAX];		//δ֪������
	int	m_nCntNum;				//δ֪��������
	CListCtrl* m_pCntList;		//��ʾδ֪����
	
	AgCnt m_creditCnt[5];			//��������
	bool m_bEnIPFilter;				//�Ƿ�����ip����

	unsigned char m_curIP[4];	//��ǰ���ݵ�Դip
	int m_curID;				//��ǰ���ڽ�����������ԱID

	CToAgent m_ToAgent[7];		//��agents��������
	unsigned char m_sendbuf[UPUDPBUFLEN];//���ͻ���

	CUPFullField* pfield;		//������ʾ�ؼ�

	bool m_bFoundBall;
	bool m_bHoldBall;//��ʱ����
	unsigned char* pbuf;		//��ʱ����

	unsigned char m_ParseBuf[UPUDPBUFLEN];
	int m_nRecvIndex;			//��������
	unsigned char m_lastRecv;	//��һ���ַ�
	int m_bFrameStart;			//֡������ʼ
	int m_nFrameLength;			//֡����

	//�ϰ�����Ϣ����
	//obstacle m_cObstBuf[EACH_AG_OBST_NUM*(AG_NUM+1)];
	obstacle m_cObstBuf[EACH_AG_OBST_NUM*(AG_NUM+1)];
protected:
	int i;
	bool m_CheckCredit(unsigned char* inIP);
	unsigned char m_calsum(unsigned char *inBuf, int inlen);
};

#endif // !defined(AFX_COACHUDP_H__2CB0229F_841F_4740_9F86_33E8577CABBA__INCLUDED_)
