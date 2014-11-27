#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <cmath>
#include <opencv2\opencv.hpp>
#include "vec3f.h"
#include "scene.h"


class camera{
public:
	camera(int w,int h,int d,int f);		//��ʼ��
	void move(vec3f m);						//��������ƽ��
	void x_rotate(double alpha);			//���ҷ����ϵ���ת,����תΪ��
	void y_rotate(double alpha);			//���·����ϵ���ת,����תΪ��
	void draw(scene &s);					//���ƻ���
	void show();							//��ʾ����
	int width,height,dis;					//�����ĳ��͸�,�ӵ���뻭���ľ���
	int focus;								//�����
	double **depth;							//���������
	colour **buffer;						//��ɫ����
	vec3f view,stpoint;						//�ӵ�,���������϶���
	vec3f xline,yline;						//������������,�û�����λ������ʾ
	cv::Mat img;							//����
};

#endif