#ifndef __OBJ_H__
#define __OBJ_H__

#include "grain.h"
#include "line.h"
#include "lproperty.h"
#include <utility>

//物体抽象类
class obj{
public:
	obj(lproperty llp,grain* gg):lp(llp),g(gg){}
	virtual std::pair<double,vec3f> cross(line& l)=0;		//返回值为交点距射线顶点的长度、交点处的单位法向向量
	lproperty lp;											//光学属性
	grain* g;												//纹理
};

#endif