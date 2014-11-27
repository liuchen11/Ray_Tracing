#ifndef __TRI_H__
#define __TRI_H__

#include "obj.h"

//ax+by+cz+d=0
//三角面片类
class tri{
public:
	tri(vec3f pp1,vec3f pp2,vec3f pp3):p1(pp1),p2(pp2),p3(pp3){
		vec3f v1=p2-p1,v2=p3-p1;
		vec3f fi=v1.cross(v2);
		a=fi.x,b=fi.y,c=fi.z;
		d=-(a*p1.x+b*p1.y+c*p1.z);
	}
	//判断某一点是否在三角形面片中
	int contain(vec3f check){
		double tmp=check.x*a+check.y*b+check.z*c+d;
		if(tmp<-1e-3||tmp>1e-3){
			return 0;
		}
		vec3f v1=p2-p1,v2=p3-p1,vc=check-p1;
		double xy=v1.x*v2.y-v1.y*v2.x,xz=v1.x*v2.z-v1.z*v2.x,yz=v1.y*v2.z-v1.z*v2.y;
		double w1,w2;
		if(xy>1e-3||xy<-1e-3){
			double a=v2.y/xy,b=-v2.x/xy,c=-v1.y/xy,d=v1.x/xy;
			w1=vc.x*a+vc.y*b;
			w2=vc.x*c+vc.y*d;
		}
		else if(xz>1e-3||xz<-1e-3){
			double a=v2.z/xz,b=-v2.x/xz,c=-v1.z/xz,d=v1.x/xz;
			w1=vc.x*a+vc.z*b;
			w2=vc.x*c+vc.z*d;
		}
		else{
			double a=v2.z/yz,b=-v2.y/yz,c=-v1.z/yz,d=v1.y/yz;
			w1=vc.y*a+vc.z*b;
			w2=vc.y*c+vc.z*d;
		}
		if(w1<0||w2<0)
			return 0;
		if(w1+w2>1)
			return 0;
		return 1;
	}
	//与直线求交
	std::pair<double,vec3f> cross(line &l){
		double tmp1=a*l.p.x+b*l.p.y+c*l.p.z+d;
		double tmp2=a*l.v.x+b*l.v.y+c*l.v.z;
		if(tmp2<1e-3&&tmp2>-1e-3){
			vec3f no(0,0,0);
			return std::pair<double,vec3f>(-1,no);
		}
		else{
			double dis=-tmp1/tmp2;
			vec3f pt=l.p+l.v*dis;
			if(contain(pt)){
				vec3f fi(a,b,c);
				if(fi*l.v>0)
					fi=-fi;
				return std::pair<double,vec3f>(dis,fi);
			}
			else{
				vec3f no(0,0,0);
				return std::pair<double,vec3f>(-1,no);
			}
		}
	}
	vec3f p1,p2,p3;
	double a,b,c,d;
};

#endif

#ifndef __TRI_NUM_H__
#define __TRI_NUM_H__

//三角形面片索引
class tri_num{
public:
	tri_num(int rr1,int rr2,int rr3):r1(rr1),r2(rr2),r3(rr3){}
	int r1,r2,r3;
};

#endif

#ifndef __POLY_H__
#define __POLY_H__

#include "obj.h"
#include <fstream>
#include <iostream>
#include <string>

//多面体,由三角面片构成
class poly:public obj{
public:
	poly(lproperty llp,grain *gg,std::vector<vec3f> _elem,std::vector<tri_num> _index):obj(llp,gg){
		int firstone=1;
		elem.clear();
		index.clear();
		for(int i=0;i<_elem.size();i++){
			elem.push_back(_elem[i]);
			if(firstone==1||elem[i].x<minx)
				minx=elem[i].x;
			if(firstone==1||elem[i].x>maxx)
				maxx=elem[i].x;
			if(firstone==1||elem[i].y<miny)
				miny=elem[i].y;
			if(firstone==1||elem[i].y>maxy)
				maxy=elem[i].y;
			if(firstone==1||elem[i].z<minz)
				minz=elem[i].z;
			if(firstone==1||elem[i].z>maxz)
				maxz=elem[i].z;
			firstone=0;
		}
		int number=elem.size();
		for(int i=0;i<_index.size();i++){
			if(_index[i].r1<number&&_index[i].r2<number&&_index[i].r3<number&&_index[i].r1>=0&&_index[i].r2>=0&&_index[i].r3>=0)
				index.push_back(_index[i]);
		}
	}
	poly(lproperty llp,grain *gg,std::string filename):obj(llp,gg){
		elem.clear();
		index.clear();
		std::ifstream in(filename);
		char type;
		double x,y,z;
		int index1,index2,index3;
		int firstone=1;
		while(in>>type){
			if(type=='v'){
				in>>x>>y>>z;
				elem.push_back(vec3f(x,y,z));
				if(firstone==1||elem[elem.size()-1].x<minx)
					minx=elem[elem.size()-1].x;
				if(firstone==1||elem[elem.size()-1].x>maxx)
					maxx=elem[elem.size()-1].x;
				if(firstone==1||elem[elem.size()-1].y<miny)
					miny=elem[elem.size()-1].y;
				if(firstone==1||elem[elem.size()-1].y>maxy)
					maxy=elem[elem.size()-1].y;
				if(firstone==1||elem[elem.size()-1].z<minz)
					minz=elem[elem.size()-1].z;
				if(firstone==1||elem[elem.size()-1].z>maxz)
					maxz=elem[elem.size()-1].z;
				firstone=0;
			}
			else if(type=='f'){
				in>>index1>>index2>>index3;
				if(index1<elem.size()&&index2<elem.size()&&index3<elem.size()&&index1>=0&&index2>=0&&index3>=0){
					index.push_back(tri_num(index1,index2,index3));
				}
			}
		}
		//std::cout<<minx<<" "<<maxx<<std::endl;
		//std::cout<<miny<<" "<<maxy<<std::endl;
		//std::cout<<minz<<" "<<maxz<<std::endl;
	}
	std::pair<double,vec3f> cross(line &l){
		vec3f no(0,0,0);
		std::pair<double,vec3f> best(-1,no);
		int through=0;
		double xmin=(minx-l.p.x)/l.v.x;
		double xmax=(maxx-l.p.x)/l.v.x;
		double ymin=(miny-l.p.y)/l.v.y;
		double ymax=(maxy-l.p.y)/l.v.y;
		double zmin=(minz-l.p.z)/l.v.z;
		double zmax=(maxz-l.p.z)/l.v.z;
		if(xmin>1e-6){
			double xy=l.p.y+xmin*l.v.y;
			double xz=l.p.z+xmin*l.v.z;
			if(xy<maxy&&xy>miny&&xz<maxz&&xz>minz){
				through=1;
			}
		}
		if(through==0&&xmax>1e-6){
			double xy=l.p.y+xmax*l.v.y;
			double xz=l.p.z+xmax*l.v.z;
			if(xy<maxy&&xy>miny&&xz<maxz&&xz>minz){
				through=1;
			}
		}
		if(through==0&&ymin>1e-6){
			double yx=l.p.x+ymin*l.v.x;
			double yz=l.p.z+ymin*l.v.z;
			if(yx<maxx&&yx>minx&&yz<maxz&&yz>minz){
				through=1;
			}
		}
		if(through==0&&ymax>1e-6){
			double yx=l.p.x+ymax*l.v.x;
			double yz=l.p.z+ymax*l.v.z;
			if(yx<maxx&&yx>minx&&yz<maxz&&yz>minz){
				through=1;
			}
		}
		if(through==0&&zmin>1e-6){
			double zx=l.p.x+zmin*l.v.x;
			double zy=l.p.y+zmin*l.v.y;
			if(zx<maxx&&zx>minx&&zy<maxy&&zy>miny){
				through=1;
			}
		}
		if(through==0&&zmax>1e-6){
			double zx=l.p.x+zmax*l.v.x;
			double zy=l.p.y+zmax*l.v.y;
			if(zx<maxx&&zx>minx&&zy<maxy&&zy>miny){
				through=1;
			}
		}
		if(through==1){
			for(int i=0;i<index.size();i++){
				tri present(elem[index[i].r1],elem[index[i].r2],elem[index[i].r3]);
				std::pair<double,vec3f> tmp=present.cross(l);
				if(tmp.first>1e-3&&(tmp.first<best.first||best.first<-1e-3))
					best=tmp;
			}
		}
		return best;
	}
	double minx,maxx,miny,maxy,minz,maxz;
	std::vector<vec3f> elem;
	std::vector<tri_num> index;
};

#endif