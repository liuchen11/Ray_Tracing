#include "vec3f.h"
#include "line.h"
#include "colour.h"
#include "lproperty.h"
#include "grain.h"
#include "obj.h"
#include "ball.h"
#include "poly.h"
#include "plain.h"
#include "scene.h"
#include "camera.h"
#include <iostream>
#include <string>

int main(){
	std::vector<colour> empty,red,green,blue,yellow,open_blue,orange,blackwhite;
	empty.push_back(colour(255,255,255));					//纯透明
	red.push_back(colour(255,0,0));							//红色
	green.push_back(colour(0,255,0));						//绿色
	blue.push_back(colour(0,0,255));						//蓝色
	yellow.push_back(colour(255,255,0));					//黄色
	orange.push_back(colour(255,128,0));					//橙色
	open_blue.push_back(colour(100,100,255));				//透明蓝色
	blackwhite.push_back(colour(0,0,0));					//黑白色
	blackwhite.push_back(colour(255,255,255));
	vec3f v1(500,100,500),v2(600,-100,300),v3(600,-100,700),v4(300,-100,500);		//四面体的顶点
	tri_num n1(0,1,2),n2(0,1,3),n3(0,2,3),n4(1,2,3);								//面片
	std::vector<vec3f> pt;
	std::vector<tri_num> index;
	pt.push_back(v1),pt.push_back(v2),pt.push_back(v3),pt.push_back(v4);
	index.push_back(n1),index.push_back(n2),index.push_back(n3),index.push_back(n4);
	lproperty hard(1.3,0,0,0.7,0.3,20);				//实心
	lproperty reflect(1.3,0.3,0.7,0.5,0.5,20);		//反射
	lproperty reflect2(1.3,0,0.3,0.5,0.5,20);		//反射2
	lproperty through(1.4,0.7,0,0.5,0.5,20);		//折射
	double m[6]={0.3,0,0.3,0,0.3,0};
	int choice;
	std::cout<<"使用哪一种壁纸  阿狸(0)/树木(1)纹理"<<std::endl;
	std::cin>>choice;
	std::string filename;
	if(choice%2==0)
		filename="ali.jpg";
	else
		filename="shumu.jpg";
	grain *g_empty=new single_grain(empty);
	grain *g_red=new single_grain(red);
	grain *g_green=new single_grain(green);
	grain *g_blue=new single_grain(blue);
	grain *g_yellow=new single_grain(yellow);
	grain *g_open=new single_grain(open_blue);
	grain *g_orange=new single_grain(orange);
	grain *g_cube=new cube_grain(200,blackwhite);
	grain *g_pic=new high_grain(filename,m);
	grain *g_wenli=new high_grain("wenli.jpg",m);
	grain *g_shuijing=new high_grain("shuijing.jpg",m);
	ball b1(reflect,g_green,vec3f(500,200,2000),300);
	std::cout<<"是(1)否(0)使用三维纹理"<<std::endl;
	std::cin>>choice;
	if(choice%2==1){
		b1.g=g_wenli;
		b1.lp=reflect2;
	}
	ball b2(through,g_yellow,vec3f(400,900,7000),400);
	ball b3(through,g_shuijing,vec3f(1000,2000,8000),1500);
	ball b4(hard,g_orange,vec3f(550,600,1500),100);
	plain back(hard,g_pic,0,0,1,-8000);
	plain front(hard,g_pic,0,0,1,1000);
	plain down(hard,g_pic,1,0,0,-1000);
	poly poly1(through,g_red,"poly.obj");
	poly poly2(through,g_open,"cube.obj");
	poly poly3(hard,g_open,"out.obj");
	scene s(0.2);
	std::cout<<"是(1)否(0)加入面光源"<<std::endl;
	std::cin>>choice;
	if(choice%2==1)
		s.addrectlight(vec3f(-300,600,0),vec3f(0,400,0),vec3f(400,0,0),8);
	std::cout<<"是(1)否(0)加入点光源"<<std::endl;
	std::cin>>choice;
	if(choice%2==1){
		s.addlight(vec3f(0,0,0),1);
		s.addlight(vec3f(0,500,0),1);
	}
	s.object.clear();
	s.addobj(&b1);
	//s.addobj(&b2);
	s.addobj(&b3);
	s.addobj(&b4);
	s.addobj(&down),s.addobj(&back),s.addobj(&front);
	std::cout<<"是(1)否(0)加入正四面体"<<std::endl;
	std::cin>>choice;
	if(choice%2==1)
		s.addobj(&poly1);
	std::cout<<"是(1)否(0)加入立方体"<<std::endl;
	std::cin>>choice;
	if(choice%2==1)
		s.addobj(&poly2);
	std::cout<<"是(1)否(0)加入较复杂obj物体兔子"<<std::endl;
	std::cin>>choice;
	if(choice%2==1)
		s.addobj(&poly3);
	camera c(1080,720,1500,800);
	std::cout<<"是(1)否(0)旋转"<<std::endl;
	std::cin>>choice;
	if(choice%2==1)
		c.y_rotate(15);
	c.draw(s);
	return 0;
}