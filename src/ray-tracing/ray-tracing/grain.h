#ifndef __GRAIN_H__
#define __GRAIN_H__

#include "vec3f.h"
#include "colour.h"
#include <opencv2\opencv.hpp>
#include <vector>

//规律纹理
class grain{
public:
	virtual colour mapping(vec3f p)=0;
};

//单一色调纹理
class single_grain:public grain{
public:
	single_grain(std::vector<colour> p):paint(p){ }
	colour mapping(vec3f p){
		if(paint.size()==0)
			return colour(0,0,0);
		else
			return paint[0];
	}
	std::vector<colour> paint;
};

//双色调条状纹理
class line_grain:public grain{
public:
	line_grain(int i,std::vector<colour> p):interval(i),paint(p){ }
	colour mapping(vec3f p){
		if(paint.size()==0)
			return colour(0,0,0);
		else if(paint.size()==1)
			return paint[0];
		int index=p.x+p.y+p.z;
		if(index<0)
			index=interval-index;
		int tmp=index%interval;
		if(tmp>interval/2)
			return paint[0];
		else
			return paint[1];
	}
	int interval;
	std::vector<colour> paint;
};

//嵌入图片的纹理
class pic_grain:public grain{
public:
	pic_grain(std::string name,int d):filename(name),direction(d){
		img=cv::imread(filename,CV_LOAD_IMAGE_COLOR);
		if(!img.data){
			printf("fileopenfailed!\n");
			exit(-1);
		}
	}
	colour mapping(vec3f p){
		int cx=img.cols,cy=img.rows;
		int x,y;
		if(direction==0){
			x=p.x>0?p.x+1e-6:p.x-1e-6;
			y=p.y>0?p.y+1e-6:p.y-1e-6;
		}
		else if(direction==1){
			x=p.y>0?p.y+1e-6:p.y-1e-6;
			y=p.z>0?p.z+1e-6:p.z-1e-6;
		}
		else{
			x=p.z>0?p.z+1e-6:p.z-1e-6;
			y=p.x>0?p.x+1e-6:p.x-1e-6;
		}
		if(x>=img.cols){
			while(x>=img.cols)
				x-=img.cols;
		}
		else if(x<0){
			while(x<0)
				x+=img.cols;
		}
		if(y>=img.rows){
			while(y>=img.rows)
				y-=img.rows;
		}
		else if(y<0){
			while(y<0)
				y+=img.rows;
		}
		int red=img.ptr<uchar>(y)[3*x+2];
		int green=img.ptr<uchar>(y)[3*x+1];
		int blue=img.ptr<uchar>(y)[3*x];
		return colour(red,green,blue);
	}
	std::string filename;
	cv::Mat img;
	int direction;
};

//格子纹理
class cube_grain:public grain{
public:
	cube_grain(int i,std::vector<colour> p):interval(i),paint(p){ }
	colour mapping(vec3f p){
		if(paint.size()==0)
			return colour(0,0,0);
		else if(paint.size()==1)
			return paint[0];
		int i1,i2,i3;
		int x=p.x>0?p.x+1e-6:p.x-1e-6;
		int y=p.y>0?p.y+1e-6:p.y-1e-6;
		int z=p.z>0?p.z+1e-6:p.z-1e-6;
		//printf("%d %d %d\n",x,y,z);
		while(x<0)
			x+=interval;
		while(x>=interval)
			x-=interval;
		while(y<0)
			y+=interval;
		while(y>=interval)
			y-=interval;
		while(z<0)
			z+=interval;
		while(z>=interval)
			z-=interval;
		if(x<interval/2)
			i1=0;
		else
			i1=1;
		if(y<interval/2)
			i2=0;
		else
			i2=1;
		if(z<interval/2)
			i3=0;
		else
			i3=1;
		//printf("%d %d %d\n",i1,i2,i3);
		if((i1+i2+i3)%2==1){
			//printf("0\n");
			return paint[0];
		}
		else{
			//printf("1\n");
			return paint[1];
		}
	}
	std::vector<colour> paint;
	int interval;
};

//三维纹理
class high_grain:public grain{
public:
	high_grain(std::string name,double *m):filename(name){
		matrix=new double[6];
		for(int i=0;i<6;i++)
			matrix[i]=m[i];
		img=cv::imread(filename,CV_LOAD_IMAGE_COLOR);
		if(!img.data){
			printf("fileloadfailed!\n");
			exit(-1);
		}
	}
	colour mapping(vec3f p){
		int cx=img.cols,cy=img.rows;
		double _x=p.x*matrix[0]+p.y*matrix[1]+p.z*matrix[2];
		double _y=p.x*matrix[3]+p.y*matrix[4]+p.z*matrix[5];
		int x=_x>0?_x+1e-6:_x-1e-6,y=_y>0?_y+1e-6:_y-1e-6;
		while(x<0)
			x+=cx;
		while(x>=cx)
			x-=cx;
		while(y<0)
			y+=cy;
		while(y>=cy)
			y-=cy;
		int red=img.ptr<uchar>(y)[3*x+2];
		int green=img.ptr<uchar>(y)[3*x+1];
		int blue=img.ptr<uchar>(y)[3*x];
		return colour(red,green,blue);
	}
	std::string filename;
	cv::Mat img;
	double* matrix;
};

#endif