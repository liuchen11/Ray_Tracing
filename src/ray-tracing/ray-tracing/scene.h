#ifndef __SCENE_H__
#define __SCENE_H__

#include "obj.h"
#include "vec3f.h"
#include <vector>

//������
class scene{
public:
	scene(double reflect){
		overreflect=reflect;
		light.clear();
		object.clear();
	}
	void addlight(vec3f pos,double w){		//���Դ
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
	std::vector<vec3f> light;				//����Դ
	std::vector<obj*> object;				//�����е�����
	std::vector<double> weight;				//����Դ��ǿ��
	double overreflect;						//��������
};

#endif