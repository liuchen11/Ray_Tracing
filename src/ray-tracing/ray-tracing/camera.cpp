#include "camera.h"
#include <stdlib.h>
#include <iostream>

//�ڻ����ϻ���һ����
void drawpoint(int x,int y,colour c,cv::Mat &img){
	img.ptr<uchar>(x)[3*y]=c.blue;
	img.ptr<uchar>(x)[3*y+1]=c.green;
	img.ptr<uchar>(x)[3*y+2]=c.red;
}

//ȡ�����ϵ�һ��
colour getcolour(int x,int y,cv::Mat &img){
	int blue=img.ptr<uchar>(x)[3*y];
	int green=img.ptr<uchar>(x)[3*y+1];
	int red=img.ptr<uchar>(x)[3*y+2];
	return colour(red,green,blue);
}

//colour=������ϵ��*������ɫ+��1-������ϵ����*��������*����������+�߹⣩+������*����������⣩+������*����������⣩��
//���߸���
//���䳡�� ���䷽�� ������ ����ǿ�� ����Я����ɫ
colour track(scene &s,line l,double rate,double strength,double &deep){
	deep=-1;
	if(strength<0.05)
		return colour(0,0,0);
	colour see(0,0,0);
	double dis=-1;
	vec3f fi(0,0,0);
	obj* crossobj=NULL;
	for(int i=0;i<s.object.size();i++){
		std::pair<double,vec3f> tmp=s.object[i]->cross(l);
		if(tmp.first>1e-6&&(dis<-1e-6||tmp.first<dis)){
			dis=tmp.first;
			crossobj=s.object[i];
			fi=tmp.second;
		}
	}
	if(dis<1e-6)
		return colour(0,0,0);
	vec3f crosspoint=l.getpoint(dis);
	//������Ϣ
	colour paint=crossobj->g->mapping(crosspoint);			//��ɫ
	double overflect=s.overreflect;							//������ϵ��
	double through=crossobj->lp.rate;						//������
	double throughrate=crossobj->lp.throughrate;			//����ϵ��
	double reflectrate=crossobj->lp.reflectrate;			//����ϵ��
	double mirror=crossobj->lp.mirrorrate;					//����ϵ��
	double highlight=crossobj->lp.highlightrate;			//�߹�ϵ��
	double absorbrate=1-throughrate-reflectrate;			//����ϵ��
	int power=crossobj->lp.s;								//�߹���
	//������ ������� ������� �������
	see=see+paint*overflect;
	//���������߹����
	//l.v����� fi���� out����� in��Դ�����
	l.v=l.v.stdize(),fi=fi.stdize();
	double proj=(-l.v)*fi;
	//printf("%lf\n",proj);
	vec3f out=fi*2*proj+l.v;
	out=out.stdize();
	double total=0;
	for(int i=0;i<s.weight.size();i++)
		total+=s.weight[i];
	for(int i=0;i<s.light.size();i++){
		double w=s.weight[i]/total;
		vec3f in=s.light[i]-crosspoint;
		double len=in.size();				//�ཻ����Դ�ĳ���
		in=in.stdize();
		line _in(crosspoint,in);
		//Ѱ���ǲ�����Ӱ
		int shade=0;
		for(int j=0;j<s.object.size();j++){
			//printf("%d\n",s.object.size());
			if(s.object[j]==crossobj)
				continue;
			std::pair<double,vec3f> cross=s.object[j]->cross(_in);
			if(cross.first>0.01&&cross.first<len){
				shade=1;
				break;
			}
		}
		if(shade==0){
			see=see+paint*(1-overflect)*absorbrate*(mirror*(in*fi)+highlight*pow(in*out,power))*w;
		}
	}
	//����
	if(reflectrate>0.05){
		line rline(crosspoint,out);
		colour add=track(s,rline,rate,strength*reflectrate,deep)*(1-overflect);
		if(add.red==0&&add.green==0&&add.blue==0){
			//printf("\n");
		}
		//printf("%d %d %d\n",add.red,add.green,add.blue);
		see=see+add;
	}
	//����
	if(throughrate>0.05){
		if(rate>1.01){		//���������
			double cs=(-l.v)*fi;
			double si=sqrt(1-cs*cs);
			double _si=si*rate;
			if(_si<0.99){
				double _ta=_si/sqrt(1-_si*_si);
				vec3f ver=(l.v+out).stdize();
				vec3f t=(-fi)+ver*_ta;
				t=t.stdize();
				line tline(crosspoint,t);
				see=see+track(s,tline,1,strength*throughrate,deep)*(1-overflect);
			}
		}
		else{			//���������
			double cs=(-l.v)*fi;
			double si=sqrt(1-cs*cs);
			double _si=si/through;
			double _ta=_si/sqrt(1-_si*_si);
			vec3f ver=(l.v+out).stdize();
			vec3f t=(-fi)+ver*_ta;
			t=t.stdize();
			line tline(crosspoint,t);
			see=see+track(s,tline,1,strength*throughrate,deep)*(1-overflect);
		}
	}
	deep=dis;
	return see*strength;
}

camera::camera(int w,int h,int d,int f):width(w),height(h),dis(d),focus(f),view(height/2,width/2,-dis),stpoint(0,0,0){
	xline.set(1,0,0);yline.set(0,1,0);
	img.create(height,width,CV_8UC3);
	img.setTo(cv::Scalar(255,255,255));
	depth=new double*[width];
	buffer=new colour*[width];
	for(int i=0;i<width;i++){
		depth[i]=new double[height];
		buffer[i]=new colour[height];
	}
}

void camera::move(vec3f m){
	view=view+m;
	stpoint=stpoint+m;
}

void camera::x_rotate(double alpha){		//����תΪ��
	alpha=alpha/180*3.14159265;
	vec3f zline=xline.cross(yline);
	zline=zline.stdize();
	vec3f mid=stpoint+xline*(double)height/2;
	double z=(double)height*sin(alpha)/2,x=(double)height*cos(alpha)/2;
	stpoint=mid-xline*x-zline*z;
	xline=(mid-stpoint).stdize();
	zline=xline.cross(yline);
	zline=zline.stdize();
	vec3f centerpoint=stpoint+xline*(double)height/2+yline*(double)width/2;
	view=centerpoint-zline*dis;
}

void camera::y_rotate(double alpha){		//����תΪ��
	alpha=alpha/180*3.14159265;
	vec3f zline=xline.cross(yline);
	zline=zline.stdize();
	vec3f mid=stpoint+yline*(double)width/2;
	double z=(double)width*sin(alpha)/2,y=(double)width*cos(alpha)/2;
	stpoint=mid-yline*y-zline*z;
	yline=(mid-stpoint).stdize();
	zline=xline.cross(yline);
	zline=zline.stdize();
	vec3f centerpoint=stpoint+xline*(double)height/2+yline*(double)width/2;
	view=centerpoint-zline*dis;
}

void camera::draw(scene &s){
	for(int i=0;i<width;i++){
		printf("%d\n",i);
		for(int j=0;j<height;j++){
			vec3f out=stpoint+yline*i+xline*j;
			line look(out,out-view);
			buffer[i][j]=track(s,look,1,1,depth[i][j]);
			drawpoint(j,i,buffer[i][j],img);
		}
	}
	show();
	std::cout<<"��(1)��(0)���п���ݲ�����"<<std::endl;
	int choice;
	std::cin>>choice;
	if(choice%2==1){
		for(int i=0;i<width;i++){
			printf("%d\n",i);
			for(int j=0;j<height;j++){
				vec3f out=stpoint+yline*i+xline*j+vec3f(0.5,0,0);
				line look(out,out-view);
				buffer[i][j]=track(s,look,1,1,depth[i][j])/2+buffer[i][j]/2;
				//drawpoint(j,i,buffer[i][j],img);
			}
		}
		//show();
		for(int i=0;i<width;i++){
			printf("%d\n",i);
			for(int j=0;j<height;j++){
				vec3f out=stpoint+yline*i+xline*j+vec3f(0,0.5,0);
				line look(out,out-view);
				buffer[i][j]=track(s,look,1,1,depth[i][j])/3+buffer[i][j]/3*2;
				//drawpoint(j,i,buffer[i][j],img);
			}
		}
		//show();
		for(int i=0;i<width;i++){
			printf("%d\n",i);
			for(int j=0;j<height;j++){
				vec3f out=stpoint+yline*i+xline*j+vec3f(0.5,0.5,0);
				line look(out,out-view);
				buffer[i][j]=track(s,look,1,1,depth[i][j])/4+buffer[i][j]/4*3;
				drawpoint(j,i,buffer[i][j],img);
			}
		}
		show();
	}
	for(int i=0;i<width;i++){
		printf("%d\n",i);
		for(int j=0;j<height;j++){
			int x1=depth[i][j]/(double)focus+0.5;
			int x2=(double)focus/depth[i][j]+0.5;
			int x=x1>x2?x1:x2;
			if(x>1&&focus>1){
				int red=0,green=0,blue=0;
				if(x>5)
					x=5;
				int mod=2*x+1;
				for(int t=0,number=0;number<30;t++){
					srand(t*100);
					int c=rand()%mod-x;
					srand(t*10);
					int r=rand()%mod-x;
					if(j+r>=0&&j+r<height&&i+c>=0&&i+c<width){
						int _x1=depth[i+c][j+r]/(double)focus+0.5;
						int _x2=(double)focus/depth[i+c][j+r]+0.5;
						int _x=_x1>_x2?_x1:_x2;
						if(_x>5)
							_x=5;
						if(x-_x>=-1&&x-_x<=1){
							colour tmp=buffer[i+c][j+r];
							red+=tmp.red,green+=tmp.green,blue+=tmp.blue;
							number++;
						}
					}
				}
				red/=30,green/=30,blue/=30;
				drawpoint(j,i,colour(red,green,blue),img);
			}
			else{
				drawpoint(j,i,buffer[i][j],img);
			}
		}
	}
	show();
}

void camera::show(){
	cv::imshow("show",img);
	cv::waitKey(0);
}
