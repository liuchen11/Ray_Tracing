#ifndef __BALL_H__
#define __BALL_H__

#include "obj.h"

//«Ú¿‡
class ball:public obj{
public:
	ball(lproperty llp,grain* gg,vec3f c,int rr=0):obj(llp,gg){
		center=c;
		r=rr;
	}
	std::pair<double,vec3f> cross(line& l){
		double a=l.v.square(),b=2*(l.v*(l.p-center)),c=(l.p-center).square()-r*r;
		if(b*b-4*a*c<0){
			vec3f no(0,0,0);
			return std::pair<double,vec3f>(-1,no);
		}
		else{
			double dis=(-b-sqrt(b*b-4*a*c))/(2*a);
			if(dis<0.001){
				dis=(-b+sqrt(b*b-4*a*c))/(2*a);
			}
			vec3f pt=l.p+l.v*dis;
			vec3f fi=pt-center;
			if(fi*l.v>0)
				fi=-fi;
			fi=fi.stdize();
			return std::pair<double,vec3f>(dis,fi);
		}
	}
	vec3f center;
	int r;
};

#endif