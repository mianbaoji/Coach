//������ж� 
//ballshot.cpp

#include "stdafx.h"

#ifndef ballshot_cpp
#define ballshot_cpp

#include "ballshot.h"

BallShot::BallShot(void)
{
	int i;  
	for(i=0;i<100;i++)
	{	quanball_h[i][0]=0;
		quanball_h[i][1]=0;
		quanball_h[i][2]=0;
	}
	Shot_ball_point_X=0;//�����ȫ����λ��x
	Shot_ball_point_Y=0;//�����ȫ����λ��y
	min_dis_ball_x=0;//�洢������ж�������ʼ�������������ȫ������ ��û���ʱΪ��ĵ�ǰ��������
	min_dis_ball_y=0;
	Shot_ball=0;//������ж�

}
BallShot::~BallShot(void)
{
;
}

char BallShot::Ball_Shot_Determine(float benji_x,float benji_y,float ball_x,float ball_y,float ball_w)
{
	int i;
	//�洢��100�������ȫ��λ����Ϣ
	for(i=100-1 ; i>0 ; i--)
	{
		quanball_h[i][0] =quanball_h[i-1][0];//[0]y,[1]x,[2]w //ȫ������Ϣ��ʷ��¼
		quanball_h[i][1] =quanball_h[i-1][1];
		quanball_h[i][2] =quanball_h[i-1][2];
	}
	quanball_h[0][0]=ball_y;
	quanball_h[0][1]=ball_x;
	quanball_h[0][2]=ball_w;
	//��������Tʱ�����н�·�� 0Ϊ��ǰ֡ 1Ϊ��һ֡ 2����֡ ......
	float ball_distance=0;//�����Ӿ��� �߹��ĳ�����·�� ��λ(����)
	float startx,starty,startT,endT,ball_velocity;
	const int testT=20;//���ü���Ƿ���� ��֡��
	const int seeBall=10;//�ж����������С���ص�����
	startT=-1;//����Ĭ����Ч����
	endT  =-1;
	for(i=0 ; i<testT ; i++)
	{	if (quanball_h[i][2]>seeBall)//�ҵ���ʼλ��
	{
		starty=quanball_h[i][0];
		startx=quanball_h[i][1];
		startT=i;
		break;
	}
	}
	for(i++ ; i<testT ; i++)
	{	if (quanball_h[i][2]>seeBall)
	{
		ball_distance+=sqrt((starty-quanball_h[i][0])*(starty-quanball_h[i][0])+(startx-quanball_h[i][1])*(startx-quanball_h[i][1]))*MK/2.7;
		starty=quanball_h[i][0];
		startx=quanball_h[i][1];
		endT=i;
	}
	}
	//�ж� ��ֹ�� ��Ч������5֡������Ч���ݲ��� ������Ч
	if ((startT>=0)&&(endT>(startT+5)))//�������һֱû���¾� ����ԭ���ж�����
	{
		ball_velocity=ball_distance*45.0/(endT-startT);//45.0ÿ��֡�� ���� ���Ӿ����߹��ĳ�����·�̵�ƽ���ٶ�
		if (ball_velocity>1000)// 10m/s 10 00
		{
			if (Shot_ball==0)
			{	
				Shot_ball_point_Y=quanball_h[int(endT)][0];//�����ȫ����λ��y
				Shot_ball_point_X=quanball_h[int(endT)][1];//�����ȫ����λ��x
				min_dis_ball_y=Shot_ball_point_Y;//�洢������ж�������ʼ�������������ȫ������ ��û���ʱΪ��ĵ�ǰ��������
				min_dis_ball_x=Shot_ball_point_X;
			}
			Shot_ball+=21;//������ж� ��21֡������1֡�����ж�����(ball_velocity>1000)==10m/s
			if (Shot_ball>21)
				Shot_ball=21;
			
		}
		else
		{	Shot_ball--;//������ж�
		if (Shot_ball<0)
			Shot_ball=0;
		if (Shot_ball==0)
		{	Shot_ball_point_X=0;//�����ȫ����λ��x
			Shot_ball_point_Y=0;//�����ȫ����λ��y
		}
		
		}
	}
	if (Shot_ball>21)
		Shot_ball=21;
	if (Shot_ball<0)
		Shot_ball=0;
	
	if (Shot_ball>0)
	{
		//�洢������ж�������ʼ�������������ȫ������ ��û���ʱΪ��ĵ�ǰ��������
		if (
			sqrt((min_dis_ball_y-benji_y)*(min_dis_ball_y-benji_y)+(min_dis_ball_x-benji_x)*(min_dis_ball_x-benji_x))//coordinate_transfer_radius(min_dis_ball_y-benji_y,min_dis_ball_x-benji_x)
		   >sqrt((ball_y        -benji_y)*(ball_y        -benji_y)+(ball_x        -benji_x)*(ball_x        -benji_x))//>coordinate_transfer_radius(ball_y       -benji_y,ball_x        -benji_x)
			)
		{
			min_dis_ball_y=ball_y;
			min_dis_ball_x=ball_x;
			return(2);//�˵����м���ص�
		}
		return(1);//�˵��ǿ��з����е�
	}
	else
	{
		min_dis_ball_x=ball_x;//�洢������ж�������ʼ�������������ȫ������ ��û���ʱΪ��ĵ�ǰ��������
		min_dis_ball_y=ball_y;
		return(0);//��û�����
	}
	
}

#endif