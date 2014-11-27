#ifndef __SCENE_H__
#define __SCENE_H__

#include "obj.h"
#include "vec3f.h"
#include <vector>

//场景类
class scene{
public:
	scene(double reflect){
		overreflect=reflect;
		light.clear();
		object.clear();
	}
	void addlight(vec3f pos,double w){		//点光源
		if(w<0.01)
			return;
		light.push_back(pos);
		weight.push_back(w);
	}
	void addrectlight(vec3f pt,vec3f xline,vec3f yline,double w){
		if(w<0.1)
			return;
		int number=4;
		for(int i=0;i<number;i++)
			for(int j=0;j<number;j++){
				vec3f toadd=pt+xline*(2*i+1)/(2*number)+yline*(2*j+1)/(2*number);
				light.push_back(toadd);
				weight.push_back(w/(number*number));
			}
	}
	void addobj(obj* add){
		object.push_back(add);
	}
	std::vector<vec3f> light;				//各光源
	std::vector<obj*> object;				//场景中的物体
	std::vector<double> weight;				//各光源的强度
	double overreflect;						//漫反射率
};

#endif