// ConctDomain.cpp: implementation of the CConctDomain class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConctDomain.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConctDomain::CConctDomain()
{
	_CurDomain.x = 0;
	_CurDomain.y = 0;
	_CurDomain.sum = 0;
	_Range = 4;

	m_Nextx = 0;
	m_Nexty = 0;
	result = NULL;
	map = NULL;
	m_nNumofResult = 0;

	toscan = NULL;

	//debug
	debug_cnt = 0;
}

CConctDomain::~CConctDomain()
{
	DelResults();
	
	if (map != NULL)
	{
		delete[] map;
	}
}

BOOL CConctDomain::_Expend(int x, int y)	//ɨ��õ���Χ�Ŀյ㣬���ڿյ���Χɨ��ǿյ�
{
	//��Ϊ�Ǿ����ɰ�ģ�����Ҫ�Ա�Ե����������⴦��

	//Ϊ���Ч�ʣ����÷ֶ��жϵķ�ʽ��Ŀ���Ǽ��ٳ˷�����
	//��ͼɨ������
	flag = false;	//�������ָ���Ƿ���Ч
	if (y-1 > -1)	//>=0
	{
		for (i=x-1;i<x+1;i++)
		{
			if (i>0 && i<m_nWidth )
			{
				if (flag == false)
				{
					flag = true;
					temp = (y-1)*m_nWidth+i;
					m_cursrcp = srcmap + temp;
					m_curmapp = map + temp;
				}
			}
			else
			{
				flag = false;
			}
			if (flag == true)
			{
				if (*m_cursrcp != _IDVal)
				{
					//��Χɨ��һ����Ŀ����ɫ�ģ�����������Χ���޿���չ��
					if (m_checkpoint(i,y-1) == true)
					{
						return TRUE;
					}
				}
			}
		}
	}

	//ɨ������
	flag = false;
	{
		for (i=x-1;i<x+1;i++)
		{
			if (i>0 && i<m_nWidth )
			{
				if (flag == false)
				{
					flag = true;
					temp = y*m_nWidth+i;
					m_cursrcp = srcmap + temp;
					m_curmapp = map + temp;
				}
			}
			else
			{
				flag = false;
			}
			if (flag == true)
			{
				if (*m_cursrcp != _IDVal)
				{
					//��Χɨ��һ����Ŀ����ɫ�ģ�����������Χ���޿���չ��
					if (m_checkpoint(i,y) == true)
					{
						return TRUE;
					}
				}
			}
		}
	}
	//ɨ������
	flag = false;
	if (y+1 < m_nHeight)
	{
		for (i=x-1;i<x+1;i++)
		{
			if (i>0 && i<m_nWidth )
			{
				if (flag == false)
				{
					flag = true;
					temp = (y+1)*m_nWidth+i;
					m_cursrcp = srcmap + temp;
					m_curmapp = map + temp;
				}
			}
			else
			{
				flag = false;
			}
			if (flag == true)
			{
				if (*m_cursrcp != _IDVal)
				{
					//��Χɨ��һ����Ŀ����ɫ�ģ�����������Χ���޿���չ��
					if (m_checkpoint(i,y+1) == true)
					{
						return TRUE;
					}
				}
			}
		}
	}
	return FALSE;
}

BOOL CConctDomain::Process(int inMode)
{
	DelResults();

	int x,y;
	//////////////////////////////////////////////////////////////////////////
	//��һ�׶Σ��ҳ�ɫ���Ե��

	UCHAR* lCurmapp = map;
	UCHAR* lCursrcp = srcmap;
	//���һ�����������⴦��
	for (x=0;x<m_nWidth;x++)
	{
		if (*lCursrcp == _IDVal)
		{
			*(lCurmapp+m_nWidth) = _IDVal;
		}
		lCursrcp ++;
		lCurmapp ++;
	}

	//ɨ��ڶ��е������ڶ���
	for (y=1;y<m_nHeight-1;y++)
	{
		temp = y*m_nWidth;
		lCurmapp = map + temp;
		lCursrcp = srcmap + temp;
		for (x=0;x<m_nWidth;x++)
		{
			if (*lCursrcp == _IDVal)
			{
				//��ѯ�˵��Ƿ��Ե
				if (x == 0 || x == m_nWidth-1)
				{
					if (x == 0)
					{
						*(lCurmapp+1) = _IDVal;
					}

					if (x == m_nWidth-1)
					{
						*(lCurmapp-1) = _IDVal;
					}
				}
				else
				{
					//����Χ�Ƿ��зǿյ㣩
					//��ɨ��
					UCHAR* tempp = lCursrcp-1;
					if (*tempp != _IDVal)
					{
						*lCurmapp = _IDVal;
					}
					else
					{
						//��
						tempp ++;
						tempp ++;
						if (*tempp != _IDVal)
						{
							*lCurmapp = _IDVal;
						}
					}
					//��������
					if (*lCurmapp != _IDVal)
					{
						tempp = lCursrcp + m_nWidth-1;
						if (*tempp != _IDVal)
						{
							*lCurmapp = _IDVal;
						}
						else
						{
							tempp ++;
							if (*tempp != _IDVal)
							{
								*lCurmapp = _IDVal;
							} 
							else
							{
								tempp ++;
								if (*tempp != _IDVal)
								{
									*lCurmapp = _IDVal;
								}
							}
						}
					}

					//��������
					if (*lCurmapp != _IDVal)
					{
						tempp = lCursrcp - m_nWidth - 1;
						if (*tempp != _IDVal)
						{
							*lCurmapp = _IDVal;
						}
						else
						{
							tempp ++;
							if (*tempp != _IDVal)
							{
								*lCurmapp = _IDVal;
							} 
							else
							{
								tempp ++;
								if (*tempp != _IDVal)
								{
									*lCurmapp = _IDVal;
								}
							}
						}
					}
				}
			}
			lCursrcp ++;
			lCurmapp ++;
		}
	}

	//ɨ������һ��(ָ���Ѿ��Ƶ��ǣ������ٶ�ָ����д���)
	for (x=0;x<m_nWidth;x++)
	{
		if (*lCursrcp == _IDVal)
		{
			*(lCurmapp-m_nWidth) = _IDVal;
		}
		lCursrcp ++;
		lCurmapp ++;
	}
	//////////////////////////////////////////////////////////////////////////
	//�ڶ��׶Σ��ڱ�Ե�����ҳ�������
	//memcpy(map,srcmap,m_nWidth*m_nHeight);
	lCurmapp = map;
	DomainNode* tp = result;
	m_nNumofResult = 0;
	bool useup = false;		//�ж�result�����Ƿ�ľ�
	bool found = false;		//�ж�һ����ɢ�Ƿ���������
	DomainNode* tmpnode;	//Ϊ����toscan��������һ����ʱָ��
	DomainNode* scannode;	//��ǰ��ɨ��Ľڵ�
	UCHAR* scanp;			//�����ŵ���map��λ��
	UCHAR* expp;			//scanp����ʱʹ�õ�λ��ָ��
	
	for (y=0;y<m_nHeight;y++)
	{
		for (x=0;x<m_nWidth;x++)
		{
			if (*lCurmapp == _IDVal)	//ɨ�赽һ�������㣬�ɴ�չ������
			{
				//����ɨ���������һ���ڵ�
				toscan = new DomainNode();
				toscan->x = x;
				toscan->y = y;
				toscan->next = NULL;
				//Ϊ����ͨ������һ�����
				if (useup == true || tp == NULL)
				{
					tp = new DomainNode();
					tp->next = result;
					result = tp;
				}
				else
				{
					tp = tp->next;
				}
				m_nNumofResult ++;
				//�µĽ���ڵ�������ˣ���ʼ��һ�£�֮��tp��Ϊ��ǰ����ڵ��������ͳ��
				tp->left = tp->right = toscan->x;
				tp->bottom = tp->top = toscan->y;
				tp->x = tp->y = 0;
				tp->sum = 0;

				//��ʼ��ɢ
				while (toscan != NULL)
				{
					//��toscan�������һ���������ɢ
					scannode = toscan;
					toscan = toscan->next;
					//���ýڵ���Ϣ������ͨ��ͳ��
					tp->x += scannode->x;
					tp->y += scannode->y;
					tp->sum ++;

					//�жϴ˵�Խ����Χ�ߴ�Ĺ���
					if (scannode->x < tp->left)
					{
						tp->left = scannode->x;
					}
					if (scannode->x > tp->right)
					{
						tp->right = scannode->x;
					}
					if (scannode->y < tp->bottom)
					{
						tp->bottom = scannode->y;
					}
					if (scannode->y > tp->top)
					{
						tp->top = scannode->y;
					}
					
					//ֻ�ԷǱ߽�����ؽ���ͳ��
					if ((scannode->x > 0) && (scannode->x < m_nWidth-1) && (scannode->y > 0) && (scannode->y < m_nHeight-1))
					{
						//����ɨ����Ӧ��mapλ��ָ��
						scanp = map + ((scannode->y)*m_nWidth + scannode->x);

						//ɨ���ϲ�������
						expp = scanp + m_nWidth - 1;
						if (*expp == _IDVal)
						{
							//����ɢ����ӵ�toscan��
							tmpnode = new DomainNode();
							tmpnode->x = scannode->x - 1;
							tmpnode->y = scannode->y + 1;
							tmpnode->next = toscan;
							toscan = tmpnode;
							//Ĩ����ǰ���
							*expp = 0;
						}
						expp ++;
						if (*expp == _IDVal)
						{
							//����ɢ����ӵ�toscan��
							tmpnode = new DomainNode();
							tmpnode->x = scannode->x;
							tmpnode->y = scannode->y + 1;
							tmpnode->next = toscan;
							toscan = tmpnode;
							//Ĩ����ǰ���
							*expp = 0;
						}
						expp ++;
						if (*expp == _IDVal)
						{
							//����ɢ����ӵ�toscan��
							tmpnode = new DomainNode();
							tmpnode->x = scannode->x + 1;
							tmpnode->y = scannode->y + 1;
							tmpnode->next = toscan;
							toscan = tmpnode;
							//Ĩ����ǰ���
							*expp = 0;
						}

						//�м�����ĵ�
						expp = scanp - 1;
						if (*expp == _IDVal)
						{
							//����ɢ����ӵ�toscan��
							tmpnode = new DomainNode();
							tmpnode->x = scannode->x - 1;
							tmpnode->y = scannode->y;
							tmpnode->next = toscan;
							toscan = tmpnode;
							//Ĩ����ǰ���
							*expp = 0;
						}
						expp += 2;
						if (*expp == _IDVal)
						{
							//����ɢ����ӵ�toscan��
							tmpnode = new DomainNode();
							tmpnode->x = scannode->x + 1;
							tmpnode->y = scannode->y;
							tmpnode->next = toscan;
							toscan = tmpnode;
							//Ĩ����ǰ���
							*expp = 0;
						}

						//�²�������
						expp = scanp - m_nWidth - 1;
						if (*expp == _IDVal)
						{
							//����ɢ����ӵ�toscan��
							tmpnode = new DomainNode();
							tmpnode->x = scannode->x - 1;
							tmpnode->y = scannode->y - 1;
							tmpnode->next = toscan;
							toscan = tmpnode;
							//Ĩ����ǰ���
							*expp = 0;
						}
						expp ++;
						if (*expp == _IDVal)
						{
							//����ɢ����ӵ�toscan��
							tmpnode = new DomainNode();
							tmpnode->x = scannode->x;
							tmpnode->y = scannode->y - 1;
							tmpnode->next = toscan;
							toscan = tmpnode;
							//Ĩ����ǰ���
							*expp = 0;
						}
						expp ++;
						if (*expp == _IDVal)
						{
							//����ɢ����ӵ�toscan��
							tmpnode = new DomainNode();
							tmpnode->x = scannode->x + 1;
							tmpnode->y = scannode->y - 1;
							tmpnode->next = toscan;
							toscan = tmpnode;
							//Ĩ����ǰ���
							*expp = 0;
						}
					}	//end if ((scannode->x > 0) && (scannode->x < m_nWidth-1) ����

					//��չ������ڵ㣬ɾ��
					delete scannode;
				}	//end while (toscan != NULL)

				//һ����ͨ����ɢ��������ʼ�Խ�����ݽ���ͳ��
				tp->x = tp->x / tp->sum;
				tp->y = tp->y / tp->sum;

				
				if (tp->next == NULL)
				{
					useup = true;
				}

			}	//end (*lCurmapp == _IDVal)	//ɨ�赽һ�������㣬�ɴ�չ������
			lCurmapp++;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	m_SortRes();
	return TRUE;
}

int CConctDomain::GetCount()
{
	return m_nNumofResult;
}

void CConctDomain::Mark(BYTE *rgbBuf)
{
	/*RGBTRIPLE clr;
	clr.rgbtRed = color.rgbtBlue;
	clr.rgbtGreen = color.rgbtGreen;
	clr.rgbtBlue = color.rgbtRed;

	//int s = 1000000;
	//int t = 0;

	DomainNode node;
	POSITION pos;
	pos = _result.GetHeadPosition();
	while (pos != NULL)
	{	
		node = _result.GetNext(pos);
		DrawFocus(rgbBuf,node.x,node.y,&clr);
	}*/
}

void CConctDomain::SetRange(int inRge)
{
	_Range = inRge;
}

DomainNode CConctDomain::GetDomainAt(int index)
{
	DomainNode ret;
	DomainNode* p = result;
	int sum = 0;
	for (int i=0;i<m_nNumofResult;i++)
	{	
		if (p->sum > sum)
		{
			sum = p->sum;
			ret = *p;
		}
		p=p->next;
	}

	return ret;
}

DomainNode CConctDomain::GetDomainNear(int x, int y)
{
	DomainNode ret;
	DomainNode* p = result;
	int dist_long = 0;
	int dist;
	for (int i=0;i<m_nNumofResult;i++)
	{
		dist = sqrt(static_cast<float>((x - m_nWidth/2)^2+(y - m_nHeight/2)^2));
		if (dist>dist_long)
		{
			dist_long = dist;
			ret = *p;
		}
		p=p->next;
	}
	
	return ret;
}

bool CConctDomain::RadScan(int orgx, int orgy, int drcx, int drcy, CPoint* inpnt)
{
	/*int m_rad = 5;
	int cnt = 0;
	int dx = abs(orgx - drcx);
 	int dy = abs(orgy - drcy);
	if (dx == 0 && dy == 0)
	{
		return false;
	}

 	if (dx > dy)
 	{
 		float slope = (float)(drcy - orgy)/(drcx - orgx);
 		if (drcx < orgx)
 		{
 			//��������ɨ��
			int j;
 			for (int i=orgx;i>0;i--)
 			{
 				j = orgy +(i-orgx)*slope;
 				if (GetPoint(i,j,map) == _IDVal)
 				{
					if (inpnt !=NULL && cnt == 0)
					{
						inpnt->x = i;
						inpnt->y = j;
					}
					cnt ++;
					//�����߷��򳬹�m_rad�����ز��϶��ǽ����һ����
					if (cnt > m_rad)
					{
						return true;
					}
 				}
 			}
 		}
		else
		{
			//��������ɨ��
 			int j;
 			for (int i=orgx;i<=320;i++)
 			{
 				j = orgy +(i-orgx)*slope;
 				if (GetPoint(i,j,map) == _IDVal)
 				{
					if (inpnt !=NULL && cnt == 0)
					{
						inpnt->x = i;
						inpnt->y = j;
					}
					cnt ++;
					//�����߷��򳬹�m_rad�����ز��϶��ǽ����һ����
					if (cnt > m_rad)
					{
						return true;
					}
 				}
 			}
		}
 	}
 	else
 	{
 		float slope = (float)(drcx - orgx)/(drcy - orgy);
 		if (drcy < orgy)
 		{
			//��������ɨ��
 			int i;
 			for (int j=orgy;j>0;j--)
 			{
 				i = orgx + (j-orgy)*slope;
 				if (GetPoint(i,j,map) == _IDVal)
 				{
					if (inpnt !=NULL && cnt == 0)
					{
						inpnt->x = i;
						inpnt->y = j;
					}
					cnt ++;
					//�����߷��򳬹�m_rad�����ز��϶��ǽ����һ����
					if (cnt > m_rad)
					{
						return true;
					}
 				}
 			}
 		}
		else
		{
			//��������ɨ��
 			int i;
 			for (int j=orgy;j<=240;j++)
 			{
 				i = orgx + (j-orgy)*slope;
 				if (GetPoint(i,j,map) == _IDVal)
 				{
					if (inpnt !=NULL && cnt == 0)
					{
						inpnt->x = i;
						inpnt->y = j;
					}
					cnt ++;
					//�����߷��򳬹�m_rad�����ز��϶��ǽ����һ����
					if (cnt > m_rad)
					{
						return true;
					}
 				}
 			}
		}
 	}*/
 	return false;
}

void CConctDomain::InitMap(UCHAR *inMap, int inWidth, int inHeight, UCHAR IDVal)
{
	//�������Ϸ���
	if (inMap == NULL || inWidth <0 || inHeight<0)
	{
		return;
	}

	srcmap = inMap;

	if (inWidth != m_nWidth || inHeight != m_nHeight)
	{
		delete[] map;
		map = new UCHAR[inWidth * inHeight];
		m_nWidth = inWidth;
		m_nHeight = inHeight;
	}

	//memcpy(map,srcmap,m_nWidth*m_nHeight);
	//��ձ����
	memset(map,0,m_nWidth*m_nHeight);

	_IDVal = IDVal;
}

bool CConctDomain::m_checkpoint(int inx, int iny)	//ɨ����map��û�б���ǹ�����Ч��
{	
	m_Nextx = m_Nexty = -1;
	//��ɨ������
	bool ch_flag = false;
	if (iny - 1 >-1)	//>=0
	{
		for (ch_i=inx-1;ch_i<inx+1;ch_i++)
		{
			if (ch_i >=0 && ch_i <m_nWidth )
			{
				if (ch_flag == false)
				{
					ch_temp = (iny-1)*m_nWidth+i;
					ch_cursrcp = srcmap+ch_temp;
					ch_curmapp = map+ch_temp;
					ch_flag = true;
				}
			}
			else
			{
				//�����ʱ���Ǳ���
				ch_flag = false;
			}
			if (ch_flag == true)
			{
				//�Ϸ����꣬�ж��Ƿ���δ����ǹ�����Ч��
				if (*ch_cursrcp == _IDVal)
				{
					//�ж��Ƿ񱻱�ǹ�
					if (*ch_curmapp == 0)
					{
						//���ַ��������ĵ㣡����귵�أ�
						m_Nextx = ch_i;
						m_Nexty = iny - 1;
						*ch_curmapp = 1;
						return true;
					}
				}
				ch_cursrcp ++;
				ch_curmapp ++;
			}
		}
	}

	//ɨ������
	ch_flag = false;
	{
		for (ch_i=inx-1;ch_i<inx+1;ch_i++)
		{
			if (ch_i >=0 && ch_i <m_nWidth )
			{
				if (ch_flag == false)
				{
					ch_temp = (iny)*m_nWidth+i;
					ch_cursrcp = srcmap+ch_temp;
					ch_curmapp = map+ch_temp;
					ch_flag = true;
				}
			}
			else
			{
				//�����ʱ���Ǳ���
				ch_flag = false;
			}
			if (ch_flag == true)
			{
				//�Ϸ����꣬�ж��Ƿ���δ����ǹ�����Ч��
				if (*ch_cursrcp == _IDVal)
				{
					//�ж��Ƿ񱻱�ǹ�
					if (*ch_curmapp == 0)
					{
						//���ַ��������ĵ㣡����귵�أ�
						m_Nextx = ch_i;
						m_Nexty = iny;
						*ch_curmapp = 1;
						return true;
					}
				}
				ch_cursrcp ++;
				ch_curmapp ++;
			}
		}
	}

	//ɨ������
	ch_flag = false;
	if (iny+1 < m_nHeight)
	{
		for (ch_i=inx-1;ch_i<inx+1;ch_i++)
		{
			if (ch_i >=0 && ch_i <m_nWidth )
			{
				if (ch_flag == false)
				{
					ch_temp = (iny+1)*m_nWidth+i;
					ch_cursrcp = srcmap+ch_temp;
					ch_curmapp = map+ch_temp;
					ch_flag = true;
				}
			}
			else
			{
				//�����ʱ���Ǳ���
				ch_flag = false;
			}
			if (ch_flag == true)
			{
				//�Ϸ����꣬�ж��Ƿ���δ����ǹ�����Ч��
				if (*ch_cursrcp == _IDVal)
				{
					//�ж��Ƿ񱻱�ǹ�
					if (*ch_curmapp == 0)
					{
						//���ַ��������ĵ㣡����귵�أ�
						m_Nextx = ch_i;
						m_Nexty = iny + 1;
						*ch_curmapp = 1;
						return true;
					}
				}
				ch_cursrcp ++;
				ch_curmapp ++;
			}
		}
	}

	//ɨ�����˻�û�з��ֿ���չ�㣬����false
	return false;
}

bool CConctDomain::m_checkpoint(UCHAR *inpData)
{
	if (*inpData == _IDVal)
	{
		//�����Χ�Ƿ�
	}
	return false;
}

BOOL CConctDomain::ShowMap(BYTE *rgbBuf, int inWidth, int inHeight)
{
	if (rgbBuf == NULL || inWidth != m_nWidth || inHeight != m_nHeight)
	{
		return FALSE;
	}
	UCHAR* lCurmapp = map;
	UCHAR* lCursrcp = rgbBuf;
	for (int i=0;i<m_nWidth*m_nHeight;i++)
	{
		if (*lCurmapp == _IDVal)
		{
			*lCursrcp = 0xff;
			*(lCursrcp+1) = 0xff;
			*(lCursrcp+2) = 0x0;
		}
		lCurmapp ++;
		lCursrcp +=3;
	}
	return TRUE;
}

void CConctDomain::Dilation(int inTimes)
{
	bool flag = false; //true Ϊ����ɫ��  false Ϊ�뿪ɫ��
	UCHAR* tempp;
	//ɨ��ڶ��е������ڶ���
	for (int y=1;y<m_nHeight-1;y++)
	{
		temp = y*m_nWidth;
		lCursrcp = srcmap + temp;
		flag = false;
		for (int x=1;x<m_nWidth-1;x++)
		{
			if (*lCursrcp == _IDVal)
			{
				if (flag == false)
				{
					//����ɫ���ˣ�������һ��
					tempp = lCursrcp-1;
					*(tempp) = _IDVal;
					flag = true;
				}
			}
			else
			{
				if (flag == true)
				{
					//�Ѿ��뿪ɫ�򣬴˵���Ϊ�ұ�Ե�����
					*lCursrcp = _IDVal;
					flag = false;
				}
			}
			if (x == m_nWidth-2)
			{
				//�ұߵ����ڶ��㣬���⴦��
				if (*lCursrcp == _IDVal)
				{
					//�ұ���һ��
					tempp = lCursrcp+1;
					*(tempp) = _IDVal;
					flag = false;
				}
			}
			lCursrcp ++;
		}
	}

	//���������
	for (int x=0;x<m_nWidth-1;x++)
	{
		flag = false;
		lCursrcp = srcmap + x;
		for (y=1;y<m_nHeight-1;y++)
		{
			if (*lCursrcp == _IDVal)
			{
				if (flag == false)
				{
					//��ɫ�򣬽��·�����
					tempp = lCursrcp - m_nWidth;
					*tempp = _IDVal;
					flag = true;
				}
			}
			else
			{
				if (flag == true)
				{
					//�뿪ɫ�򣬴˵���Ϊ�ϱ�Ե�����
					*lCursrcp = _IDVal;
					flag = false;
				}
			}
			if (y == m_nHeight-2)
			{
				if (*lCursrcp == _IDVal)
				{
					//�뿪ɫ�򣬴˵���Ϊ�ϱ�Ե�����
					*tempp = _IDVal;
				}
			}
			lCursrcp += m_nWidth;
		}
	}
}

void CConctDomain::Erosion(int inMode)
{
	bool flag = false; //true Ϊ����ɫ��  false Ϊ�뿪ɫ��
	UCHAR* tempp;
	//ɨ��ڶ��е������ڶ���
	for (int y=1;y<m_nHeight-1;y++)
	{
		temp = y*m_nWidth;
		lCursrcp = srcmap + temp;
		flag = false;
		for (int x=1;x<m_nWidth;x++)
		{
			if (*lCursrcp == _IDVal)
			{
				if (flag == false)
				{
					//����ɫ���ˣ���������
					tempp = lCursrcp;
					*(tempp) = 0;
					flag = true;
				}
			}
			else
			{
				if (flag == true)
				{
					//�Ѿ��뿪ɫ�򣬴˵���Ϊ�ұ�Ե�����
					tempp = lCursrcp-1;
					*(tempp) = 0;
					flag = false;
				}
			}
			if (x == m_nWidth-1)
			{
				//�ұߵ�����һ�㣬���⴦��
				if (*lCursrcp == _IDVal)
				{
					//�ٵ�
					tempp = lCursrcp;
					*(tempp) = 0;
					flag = false;
				}
			}
			lCursrcp ++;
		}
	}

	//����ĸ�ʴ
	for (int x=0;x<m_nWidth-1;x++)
	{
		flag = false;
		lCursrcp = srcmap + x;
		for (y=1;y<m_nHeight-1;y++)
		{
			if (*lCursrcp == _IDVal)
			{
				if (flag == false)
				{
					//��ɫ�򣬿۵��õ�
					tempp = lCursrcp;
					*tempp = 0;
					flag = true;
				}
			}
			else
			{
				if (flag == true)
				{
					//�뿪ɫ�򣬴˵���Ϊ�ϱ�Ե�����
					tempp = lCursrcp - m_nWidth;
					*tempp = 0;
					flag = false;
				}
			}
			if (y == m_nHeight-1)
			{
				if (*lCursrcp == _IDVal)
				{
					//�뿪ɫ�򣬴˵���Ϊ�ϱ�Ե�����
					*tempp = 0;
				}
			}
			lCursrcp += m_nWidth;
		}
	}
}

void CConctDomain::DrawFocus(BYTE *rgbBuf, long x, long y, RGBTRIPLE *prgb)
{
	if (x<0 || x>m_nWidth-1 || y<0 || y>m_nHeight-1)
	{
		return;
	}
	//������
	int range = 3;

	int minx = x-range+1;
	if(minx < 0)
	{
		minx = 0;

	}
	int maxx = x+range;
	if (maxx>m_nWidth-1)
	{
		maxx = m_nWidth-1;
	}

	BYTE* currgb = rgbBuf + y*m_nWidth*3 + (minx)*3;
	for(int z=minx;z<maxx;z++)
	{
		*currgb = prgb->rgbtBlue;
		currgb++;
		*currgb = prgb->rgbtGreen;
		currgb++;
		*currgb = prgb->rgbtRed;
		currgb++;
	}

	//������
	int miny = y-range+1;
	if (miny <0)
	{
		miny = 0;
	}
	int maxy = y+range;
	if (maxy > m_nHeight-1)
	{
		maxy = m_nHeight-1;
	}

	currgb = rgbBuf + (miny)*m_nWidth*3 + x*3;
	for( z=miny;z<maxy;z++)
	{
		*currgb = prgb->rgbtBlue;
		currgb++;
		*currgb = prgb->rgbtGreen;
		currgb++;
		*currgb = prgb->rgbtRed;
		currgb++;
		currgb+= m_nWidth*3-3;
	}
}

void CConctDomain::Combo(UCHAR *inMap, int inWidth, int inHeight, UCHAR IDVal)
{
	InitMap(inMap,inWidth,inHeight,IDVal);
	Process(0);
}

void CConctDomain::DelResults()
{
	DomainNode* p;
	while (result != NULL)
	{
		p = result;
		result = p->next;
		delete p;
	}
}

void CConctDomain::m_SortRes()
{
	DomainNode backup;
	DomainNode* pop = result;	//��ǰ�����ݵ�
	DomainNode* bigest;			//�������������ͨ��ڵ�
	DomainNode* pnode;			//�����ڵ�
	while (pop != NULL)
	{
		bigest = pop;
		pnode = bigest;
		while (pnode != NULL )
		{
			if (pnode->sum > bigest->sum)
			{
				bigest = pnode;	//������
			}
			pnode = pnode->next;
		}

		if (bigest != pop)	//���Լ����������ģ�bigest<=>pop
		{
			backup = *pop;	//����һ��pop
			*pop = *bigest;			//bigest=>pop
			pop->next = backup.next;//�ָ�pop->next
			backup.next = bigest->next;
			*bigest = backup;		//pop => bigest
		}

		pop = pop->next;
	}
}

DomainNode* CConctDomain::GetDomainIndex(int index)
{
	DomainNode* ret;
	DomainNode* p = result;
	for (int i=0;i<m_nNumofResult;i++)
	{	
		if (i == index)
		{
			ret = p;
			break;
		}
		p=p->next;
	}
	
	return ret;
}
