#ifndef __LINE_H__
#define __LINE_H__

#include "vec3f.h"

//������
class line{
public:
	line(vec3f pp,vec3f vv):p(pp){
		v=vv.stdize();
	}
	vec3f getpoint(double dis){return p+v*dis;}		//���ؾ�����ʼ�����Ϊdis��ĵ������
	vec3f p;										//��ʼ��
	vec3f v;										//�����ϵĵ�λ����
};

#endif