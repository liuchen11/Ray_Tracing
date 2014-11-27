#ifndef __PLAIN_H__
#define __PLAIN_H__

#include "obj.h"

//平面类ax+by+cz+d=0
class plain:public obj{
public:
	plain(lproperty llp,grain* gg,double aa=0,double bb=0,double cc=0,double dd=0):obj(llp,gg){
		a=aa,b=bb,c=cc,d=dd;
	}
	//用正交的方法构成平面
	plain(lproperty llp,grain* gg,vec3f pt,vec3f v1,vec3f v2):obj(llp,gg){
		vec3f fi=v1.cross(v2);
		a=fi.x,b=fi.y,c=fi.z;
		d=-(a*pt.x+b*pt.y+c*pt.z);
	}
	void set(double aa,double bb,double cc,double dd){
		a=aa,b=bb,c=cc,d=dd;
	}
	double dis(vec3f p){return (a*p.x+b*p.y+c*p.z+d)/(sqrt(a*a+b*b+c*c));}
	virtual int contain(vec3f p){
		double t=a*p.x+b*p.y+c*p.z+d;
		if(t>-1e-6&&t<1e-6)
			return 1;
		else
			return 0;
	}
	virtual std::pair<double,vec3f> cross(line& l){
		double tmp=a*l.v.x+b*l.v.y+c*l.v.z;
		if(tmp<1e-6&&tmp>-1e-6){
			vec3f no(0,0,0);
			return std::pair<double,vec3f>(-1,no);
		}
		else{
			double dis=-(a*l.p.x+b*l.p.y+c*l.p.z+d)/tmp;
			vec3f yes(a,b,c);
			yes=yes.stdize();
			if(l.v*yes>0)
				yes=-yes;
			return std::pair<double,vec3f>(dis,yes);
		}
	}
	double a,b,c,d;
};

#endif