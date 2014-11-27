#ifndef __OBJ_H__
#define __OBJ_H__

#include "grain.h"
#include "line.h"
#include "lproperty.h"
#include <utility>

//���������
class obj{
public:
	obj(lproperty llp,grain* gg):lp(llp),g(gg){}
	virtual std::pair<double,vec3f> cross(line& l)=0;		//����ֵΪ��������߶���ĳ��ȡ����㴦�ĵ�λ��������
	lproperty lp;											//��ѧ����
	grain* g;												//����
};

#endif