#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <cmath>
#include <opencv2\opencv.hpp>
#include "vec3f.h"
#include "scene.h"


class camera{
public:
	camera(int w,int h,int d,int f);		//初始化
	void move(vec3f m);						//按照向量平移
	void x_rotate(double alpha);			//左右方向上的旋转,向左转为正
	void y_rotate(double alpha);			//上下方向上的旋转,向上转为正
	void draw(scene &s);					//绘制画布
	void show();							//显示画布
	int width,height,dis;					//画布的长和高,视点距离画布的距离
	int focus;								//景深交点
	double **depth;							//各物体距离
	colour **buffer;						//颜色缓存
	vec3f view,stpoint;						//视点,画布的左上顶点
	vec3f xline,yline;						//画布横纵坐标,用基本单位向量表示
	cv::Mat img;							//画布
};

#endif