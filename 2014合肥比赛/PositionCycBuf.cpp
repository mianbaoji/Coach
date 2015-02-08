// PositionCycBuf.cpp: implementation of the CPositionCycBuf class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PositionCycBuf.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPositionCycBuf::CPositionCycBuf()
{
	
	m_pData = NULL;
	m_nNumOfData = 0;
	m_nCurPos = 0;
	m_nFirstPos = 0;

	m_tmpPos.x = 0;
	m_tmpPos.y = 0;
	m_tmpPos.size = 1;
	
	m_nSizeOfData = sizeof(*m_pData);
}

CPositionCycBuf::~CPositionCycBuf()
{
	ReleaseData();
}

void CPositionCycBuf::ReleaseData()
{
	if (m_pData != NULL)
	{
		delete[] m_pData;
		m_pData = NULL;
	}
	m_nCurPos = 0;
	m_nFirstPos = 0;
	m_nNumOfData = 0;
	m_nLenOfBuf= 0;
}

void CPositionCycBuf::SetSize(int inSize)
{
	ReleaseData();
	m_pData = new MSL_Position[inSize];
	m_nLenOfBuf = inSize;
	m_nCurPos = 0;
	m_nFirstPos = 0;
}

void CPositionCycBuf::AddPos(MSL_Position *inPos)
{
	if (m_nLenOfBuf == 0 )
	{
		return;
	}

	if (m_nNumOfData == 0)
	{
		//�տ�ʼ����һ������
		memcpy(&m_pData[m_nCurPos],inPos,sizeof(MSL_Position));
		m_nNumOfData = 1;
		return;
	}
	
	m_IndexGrow(&m_nCurPos);
	memcpy(&m_pData[m_nCurPos],inPos,sizeof(MSL_Position));

	if (m_nCurPos == m_nFirstPos)
	{
		//��β������
		m_IndexGrow(&m_nFirstPos);
		//�������ݸ���
		m_nNumOfData = m_nLenOfBuf;
	}
	else
	{
		m_nNumOfData ++;
	}
}

int CPositionCycBuf::GetData(MSL_Position *inBuf, int inLen)
{
	//��ȷ�����ص����ݸ���
	int nGet;
	if (inLen < m_nNumOfData)
	{
		nGet = inLen;
	} 
	else
	{
		nGet = m_nNumOfData;
	}

	if (m_nCurPos >= m_nFirstPos)
	{
		//���������ֱ��ȡ��
		memcpy(inBuf,&m_pData[m_nFirstPos],nGet*m_nSizeOfData);
	} 
	else
	{
		//��������Խ��β���ޣ���Ҫ�ж�ȡ����Χ
		int LenToEnd = m_nLenOfBuf - m_nFirstPos;
		if (nGet <= LenToEnd)
		{
			//Ҫȡ������ûԽ�磬ֱ��ȡ��
			memcpy(inBuf,&m_pData[m_nFirstPos],nGet*m_nSizeOfData);
		} 
		else
		{
			memcpy(inBuf,&m_pData[m_nFirstPos],LenToEnd*m_nSizeOfData);
			memcpy(&inBuf[LenToEnd],m_pData,(nGet - LenToEnd)*m_nSizeOfData);
		}
	}

	return nGet;
}

void CPositionCycBuf::AddPos(int inx, int iny)
{
	m_tmpPos.x = inx;
	m_tmpPos.y = iny;
	AddPos(&m_tmpPos);
}

void CPositionCycBuf::m_IndexGrow(int* inIndex)
{
	(*inIndex) ++;
	if ((*inIndex) >= m_nLenOfBuf)
	{
		(*inIndex) = 0;
	}
}
