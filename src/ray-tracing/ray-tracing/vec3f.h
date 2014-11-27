#ifndef __VEC3F_H__
#define __VEC3F_H__

#include <cmath>

//三维向量
class vec3f{
public:
	vec3f(double xx=0,double yy=0,double zz=0):x(xx),y(yy),z(zz){}
	void set(double xx,double yy,double zz){
		x=xx,y=yy,z=zz;
	}
	vec3f operator+(vec3f p){return vec3f(x+p.x,y+p.y,z+p.z);}
	vec3f operator-(vec3f p){return vec3f(x-p.x,y-p.y,z-p.z);}
	vec3f operator-(){return vec3f(-x,-y,-z);}
	vec3f operator*(double p){return vec3f(x*p,y*p,z*p);}
	vec3f operator*(int p){return vec3f(x*p,y*p,z*p);}
	double operator*(vec3f p){return x*p.x+y*p.y+z*p.z;}
	vec3f operator/(double p){return vec3f(x/p,y/p,z/p);}
	vec3f operator/(int p){return vec3f(x/p,y/p,z/p);}
	double square(){return x*x+y*y+z*z;}
	double size(){return sqrt(this->square());}
	vec3f stdize(){return (*this)/(this->size());}
	vec3f cross(vec3f p){return vec3f(y*p.z-z*p.y,z*p.x-x*p.z,x*p.y-y*p.x);}
	double x,y,z;//三个点的坐标
};

#endif