//������ж� 
//ballshot.h


//#include "stdafx.h"
#ifndef ballshot_h
#define ballshot_h
#include <math.h>
//#include "Math2.h"
#include <iostream>

#define MK 1.25

class BallShot
{

	protected:
	public:
	float   quanball_h[100][3];
	float   Shot_ball_point_X;//�����ȫ����λ��x
	float   Shot_ball_point_Y;//�����ȫ����λ��y
	float   min_dis_ball_x;//�洢������ж�������ʼ�������������ȫ������ ��û���ʱΪ��ĵ�ǰ��������
	float   min_dis_ball_y;
	int     Shot_ball;//������ж�
	BallShot(void);
	~BallShot(void);

	char Ball_Shot_Determine(float benji_x,float benji_y,float ball_x,float ball_y,float ball_w);
	//����ֵ
	//return(2);//�˵����м���ص�
	//return(1);//�˵��ǿ��з����е�
	//return(0);//��û�����
};
#endif

