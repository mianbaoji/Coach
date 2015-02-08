// CoachUDP.cpp: implementation of the CCoachUDP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MSLCoach.h"
#include "CoachUDP.h"
#include "MSL_Protocol.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCoachUDP::CCoachUDP()
{
	m_nRecvIndex = 0;
	m_lastRecv = 0;
	m_bFrameStart = 0; 
	m_nFrameLength = 15;

	//cnt
	for (int i=0;i<20;i++)
	{
		m_unknownCnt[i].ID = -1;
		m_unknownCnt[i].reccnt = 0;
	}
	m_nCntNum = 0;
	m_pCntList = NULL;
	m_curID = 0;

	m_bFoundBall = false;
	m_bHoldBall= false;

	m_sendbuf[0] = 0x55;
	m_sendbuf[1] = 0xaa;

	m_bEnIPFilter = false;
	unsigned char defIP[4] = {127,0,0,1};
	for (i=0;i<5;i++)
	{
		memcpy(m_creditCnt[i].ip,defIP,4);
	}

	for(i=0;i<(EACH_AG_OBST_NUM*AG_NUM+1);i++)
	{
		m_cObstBuf[i].valid = false;
		
	}
}

CCoachUDP::~CCoachUDP()
{

}

void CCoachUDP::Received(void *inbuf, int inlen, SOCKADDR *inpSock)
{
	unsigned char* pch;
	//////////////////////////////////////////////////////////////////////////
	info.Format("rec %d bytes from ",inlen);
	pch = (unsigned char*)inpSock->sa_data;
	WORD iPort=ntohs(*(WORD*)pch);
	CString ip;
	ip.Format("%d.%d.%d.%d: %d",pch[2],pch[3],pch[4],pch[5],iPort);
	info += ip;
	printf(info);

	info.Format("");
	CString strbyte;
	pch = (unsigned char*)inbuf;
	for (int j=0;j<inlen;j++)
	{
		strbyte.Format("%.2x ",pch[j]);
		info+=strbyte;
	}
	printf(info);
	//////////////////////////////////////////////////////////////////////////

	//保留一下数据来源ip
	pch = (unsigned char*)inpSock->sa_data;
	memcpy(m_curIP,pch+2,4);

	//当启用IP过滤时，检查来源IP是否可信
	if (m_bEnIPFilter == true)
	{
		if (m_CheckCredit(m_curIP) == false)
		{
			return;
		}
	}

	//逐个解析数据
	pbuf = (unsigned char*)inbuf;
	for (int i=0;i<inlen;i++)
	{
		m_ParseByte(pbuf[i]);
	}
}

void CCoachUDP::m_ParseByte(unsigned char inChar)
{
	m_ParseBuf[m_nRecvIndex] = inChar;

	if (m_lastRecv == 0x55 && inChar == 0xAA && m_bFrameStart == 0)
	{
		m_bFrameStart = 1;
		m_ParseBuf[0] = m_lastRecv;
		m_ParseBuf[1] = inChar;
		m_nRecvIndex = 2;
		m_lastRecv=0x00;
		return;
	}

	if (m_bFrameStart)
	{
		//指令长度
		if (m_nRecvIndex == 2)
		{
			m_nFrameLength = inChar;
		}

		//put received data into buffer
		m_ParseBuf[m_nRecvIndex]=inChar;
		m_nRecvIndex++;

		//receive one frame, invoke ParseFrame to parse
		if (m_nRecvIndex==m_nFrameLength)
		{ 
			//m_DispRecvCmd();
			m_ParseFrame(m_ParseBuf, m_nFrameLength);
			m_bFrameStart = 0;
		}

		//receive buffer overflow
		if (m_nRecvIndex>=UPUDPBUFLEN) 
		{
			//m_ResetRcvBuf();
			m_bFrameStart = 0;
		}
	}
	else
		m_lastRecv=inChar;
}

void CCoachUDP::m_ParseFrame(unsigned char *inBuf, int inLen)
{
	//////////////////////////////////////////////////////////////////////////
	//校验和检查
	unsigned char chk_sum = m_calsum(inBuf,inLen-1);
	unsigned char rec_sum = inBuf[inLen-1];
	if (chk_sum != rec_sum)
	{
		printf("校验和错误！");
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	bool bNewCnt = true;
	m_curID = inBuf[3];

	//参数检测
	if (m_curID > 6)
	{
		return;
	}

	//数据来源检查
	for (int i=0;i<m_nCntNum;i++)
	{
		//检查是否是已经接收过的ip
		if (memcmp(m_unknownCnt[i].ip,m_curIP,4) == 0)
		{
			//IP一样但id不一样的，也作为新连接处理显示出来
			if (m_curID != m_unknownCnt[i].ID)
			{
				continue;
			}
			m_unknownCnt[i].reccnt++;
			bNewCnt = false;
			//////////////////////////////////////////////////////////////////////////
			/*/显示接收数据包数量
			CString strT;
			strT.Format("%d",m_unknownCnt[i].reccnt);		
			m_pCntList->SetItemText(i,2,strT);
			/////////////////////////////////////////////////////////////////////////*/
			break;
		}
	}

	//新连接，增加统计项
	if (bNewCnt == true)
	{
		if (m_nCntNum >= CNTMAX)
		{
			return;
		}
		m_unknownCnt[m_nCntNum].ID = inBuf[3];
		memcpy(m_unknownCnt[m_nCntNum].ip,m_curIP,4);
		m_unknownCnt[m_nCntNum].reccnt = 1;

		//////////////////////////////////////////////////////////////////////////
		//在列表控件里显示
		CString strT;
		strT.Format("%d.%d.%d.%d",m_unknownCnt[m_nCntNum].ip[0],m_unknownCnt[m_nCntNum].ip[1],m_unknownCnt[m_nCntNum].ip[2],m_unknownCnt[m_nCntNum].ip[3]);
		if (m_pCntList != NULL)
		{
			m_pCntList->InsertItem(m_nCntNum,strT);
			m_pCntList->SetItemText(m_nCntNum,0,strT);
			strT.Format("%d",m_unknownCnt[m_nCntNum].ID);
			m_pCntList->SetItemText(m_nCntNum,1,strT);
// 			strT.Format("%d",m_unknownCnt[m_nCntNum].reccnt);		
// 			m_pCntList->SetItemText(m_nCntNum,2,strT);
		}
		//////////////////////////////////////////////////////////////////////////
		m_nCntNum++;	//连接的数量+1
	}

	//////////////////////////////////////////////////////////////////////////
	//若对应id的Agent还未被注册，则注册该id
	if (m_ToAgent[m_curID].bConnected == false)
	{
		m_ToAgent[m_curID].SetID(m_curID);
		CString ip;
		ip.Format("%d.%d.%d.%d",m_curIP[0],m_curIP[1],m_curIP[2],m_curIP[3]);
		//设置新队员IP和端口
		m_ToAgent[m_curID].SetTarget(ip,(20090+m_curID));
		
	}
	//////////////////////////////////////////////////////////////////////////

	//检查是否与策略控件连接
	if (pfield == NULL)
	{
		return;
	}

	//辨别接收到的信息类型
	int hball ;
	int PlayerX;
		int PlayerY;
		int TargetX;
		int TargetY;
	switch(inBuf[4])
	{
		//preballs
	case MSL_PREDICTBALL:

		if(m_bFoundBall==true){
		pfield->SetPreball(inBuf[3],m_Piece2int(&inBuf[5]),m_Piece2int(&inBuf[7]));
		}
		break;
		//
	case MSL_DEBUG:
		hball = m_Piece2int(&inBuf[5]);
		PlayerY = m_Piece2int(&inBuf[7]);
		TargetX = m_Piece2int(&inBuf[9]);
		TargetY = m_Piece2int(&inBuf[11]);
		pfield->Attention_DEBUG(inBuf[3],hball,PlayerY,TargetX,TargetY);
		break;
	case MSL_INFO:	//队员信息
		//检测该队员是否找到球
		if ((0x80 & inBuf[11]) > 0)
		{
			m_bFoundBall = true;
		} 
		else
		{
			m_bFoundBall = false;
		}
		
		
		//更新队员信息
		pfield->SetAgent(
			inBuf[3],				//id
			m_Piece2int(&inBuf[5]),	//x
			m_Piece2int(&inBuf[7]),	//y
			m_Piece2int(&inBuf[9]),	//ang
			(int)inBuf[18],			//v
			m_Piece2int(&inBuf[16]),//v_angle
			(0x7f & inBuf[11]),	//status
			m_bFoundBall,     //foundb
			(int)inBuf[19]);	//holdball  [wzc] & [cyg] 
		
		//更新球信息
		if (m_bFoundBall == true)	//foundball
		{
			pfield->SetBall(inBuf[3],m_Piece2int(&inBuf[12]),m_Piece2int(&inBuf[14]));
		}

		break;
	/*case 99 :
		pfield->SetHoldBallInfo(4,1);*/

	case MSL_OBST:	//障碍物信息
		AClearObstBuf(m_curID);
		int nNumofObst;
		nNumofObst = (inLen-7)/5;
		int baseIndex;
		for (i=0;i<nNumofObst;i++)
		{
			baseIndex = 6+i*5;
			AAddObstToBuf(m_curID,m_Piece2int(&inBuf[baseIndex]),m_Piece2int(&inBuf[baseIndex+2]),inBuf[baseIndex+4]);
		}
		pfield->SetObstacle(m_curID,&(m_cObstBuf[m_curID*EACH_AG_OBST_NUM]));
		//辨别敌我
		pfield->JudgeObstacle(m_curID,&(m_cObstBuf[m_curID*EACH_AG_OBST_NUM]));

		//
		break;

	case MSL_KICK:	//有队员起脚了！紧急反应
		 PlayerX = m_Piece2int(&inBuf[5]);
		PlayerY = m_Piece2int(&inBuf[7]);
		 TargetX = m_Piece2int(&inBuf[9]);
		 TargetY = m_Piece2int(&inBuf[11]);
		pfield->Attention_SomeOneKick(inBuf[3],PlayerX,PlayerY,TargetX,TargetY);
		break;
	}

	
}

int CCoachUDP::m_Piece2int(unsigned char *inTarg)
{
	short ret;
	ret = inTarg[0];
	ret <<= 8;
	ret &= 0xff00;
	ret |= ((int)inTarg[1]&0x00ff);
	return ret;
}

void CCoachUDP::m_ag_Scan()
{
	//获取所处的IP段
	char szHostName[128];
	struct hostent * pHost; //定义结构体 hostent
	if(gethostname(szHostName,128)==0)
	{
		pHost = gethostbyname(szHostName); 
		for(int i=0;pHost!=NULL&&pHost->h_addr_list[i]!=NULL;i++) 
		{
			LPCSTR IPAddress=inet_ntoa (*(struct in_addr *)pHost->h_addr_list[i]);
			CString ip(IPAddress);
			//获取ip前三个数字
			int cnt;
			int ret = ip.Find(".",0);
			while (1)
			{
				ret = ip.Find(".",ret+1);
				if (ret <0)
				{
					break;
				}
				cnt = ret;
			}
			ip = ip.Left(cnt+1);
			ip += "255";
			//AfxMessageBox(ip);
		  
			//在本地的每一个网卡上都扫描一遍agents
			for (int j=1;j<6;j++)
			{
				m_sendbuf[2] = j;	//ID
				m_sendbuf[3] = ROBST_PROBE;
				m_sendbuf[11] = m_calsum(m_sendbuf,11);	//sum

				Send(ip.GetBuffer(ip.GetLength()),20090+j,(char*)m_sendbuf,12);
			}
		}
	}
}

unsigned char CCoachUDP::m_calsum(unsigned char *inBuf, int inlen)
{
	unsigned char sum = 0;
	for (int i=0;i<inlen;i++)
	{
		sum += inBuf[i];
	}
	return sum;
}

void CCoachUDP::UpdateCntList()
{
	if (m_pCntList != NULL)
	{
		for (int i=0;i<m_nCntNum;i++)
		{
			CString strT;
			strT.Format("%d.%d.%d.%d",m_unknownCnt[i].ip[0],m_unknownCnt[i].ip[1],m_unknownCnt[i].ip[2],m_unknownCnt[i].ip[3]);
			
			//m_pCntList->InsertItem(i,strT);
			m_pCntList->SetItemText(i,0,strT);
			strT.Format("%d",m_unknownCnt[i].ID);
			m_pCntList->SetItemText(i,1,strT);
			strT.Format("%d",m_unknownCnt[i].reccnt);		
			m_pCntList->SetItemText(i,2,strT);
		}
	}
}

bool CCoachUDP::m_CheckCredit(unsigned char *inIP)
{
	if (m_bEnIPFilter == false)
	{
		return true;
	}

	int i;
	for (i=0;i<5;i++)
	{
		if (memcmp(inIP,m_creditCnt[i].ip,4) == 0)
		{
			//在信任ip里存在该ip
			return true;
		}
	}
	return false;
}

void CCoachUDP::AllCredit()
{
	int n = m_nCntNum;
	if (n > 5)
	{
		n = 5;
	}

	for (int i=0;i<n;i++)
	{
		m_creditCnt[i] = m_unknownCnt[i];
	}
}

void CCoachUDP::ClearCredit(int inIndex)
{
	if (inIndex >= 5 || inIndex < 0)
	{
		return;
	}

	m_creditCnt[inIndex].ip[0] = 0;
	m_creditCnt[inIndex].ip[1] = 0;
	m_creditCnt[inIndex].ip[2] = 0;
	m_creditCnt[inIndex].ip[3] = 0;
}

void CCoachUDP::AddCredit(int inUnknown, int inCredit)
{
	if (inUnknown >= m_nCntNum || inCredit >=5 || inUnknown<0 || inCredit < 0)
	{
		return;
	}
	m_creditCnt[inCredit] = m_unknownCnt[inUnknown];
}

void CCoachUDP::AClearObstBuf(int inID)
{
	if (inID >= AG_NUM)
	{
		return;
	}
	for (int i=0;i<EACH_AG_OBST_NUM;i++)
	{
		m_cObstBuf[inID*EACH_AG_OBST_NUM+i].valid = false;
		
		
	}
}

void CCoachUDP::AAddObstToBuf(int inID, int inX, int inY, int inSize)
{
	if (inID >= AG_NUM)
	{
		return;
	}

	static int ob_index;
	
	for(int i=0;i<EACH_AG_OBST_NUM;i++)
	{
		ob_index = inID*EACH_AG_OBST_NUM+i;
		if(m_cObstBuf[ob_index].valid == false)
		{
			m_cObstBuf[ob_index].ID = inID;
			m_cObstBuf[ob_index].x = inX;
			m_cObstBuf[ob_index].y = inY;
			m_cObstBuf[ob_index].size = inSize;
			m_cObstBuf[ob_index].valid = true;
			break;
		}
	}

	
}
