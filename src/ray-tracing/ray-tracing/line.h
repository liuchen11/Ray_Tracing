#ifndef __LINE_H__
#define __LINE_H__

#include "vec3f.h"

//射线类
class line{
public:
	line(vec3f pp,vec3f vv):p(pp){
		v=vv.stdize();
	}
	vec3f getpoint(double dis){return p+v*dis;}		//返回距离起始点距离为dis点的点的坐标
	vec3f p;										//起始点
	vec3f v;										//方向上的单位向量
};

#endif