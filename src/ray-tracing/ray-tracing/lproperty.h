#ifndef __LPROPERTY_H__
#define __LPROPERTY_H__

//光学属性
class lproperty{
public:
	lproperty(double r,double tr,double rr,double mr,double hr,int sr):rate(r),throughrate(tr),reflectrate(rr),mirrorrate(mr),highlightrate(hr),s(sr){}
	double rate;								//折射率
	double throughrate,reflectrate;				//折射系数和反射系数
	double mirrorrate,highlightrate;			//镜面反射系数和高光系数
	int s;										//高光系数
};

#endif