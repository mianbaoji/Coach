// PositionCycBuf.h: interface for the CPositionCycBuf class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POSITIONCYCBUF_H__82E9130E_3E6C_4C33_B153_8796530C2D46__INCLUDED_)
#define AFX_POSITIONCYCBUF_H__82E9130E_3E6C_4C33_B153_8796530C2D46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct MSL_Position
{
	int x;
	int y;
	int size;
	
}MSL_Position;

class CPositionCycBuf  
{
public:
	void AddPos(int inx,int iny);
	int GetData(MSL_Position* inBuf,int inLen);
	void AddPos(MSL_Position* inPos);
	void SetSize(int inSize);
	void ReleaseData();
	CPositionCycBuf();
	virtual ~CPositionCycBuf();

	//数据
	MSL_Position *m_pData;
	

	int m_nNumOfData;
	int m_nLenOfBuf;
	int m_nSizeOfData;

	//指针
	int m_nCurPos;
	int m_nFirstPos;

	//临时变量
	MSL_Position m_tmpPos;
protected:
	void m_IndexGrow(int* inIndex);
};

#endif // !defined(AFX_POSITIONCYCBUF_H__82E9130E_3E6C_4C33_B153_8796530C2D46__INCLUDED_)
