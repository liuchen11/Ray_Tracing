#ifndef __LPROPERTY_H__
#define __LPROPERTY_H__

//��ѧ����
class lproperty{
public:
	lproperty(double r,double tr,double rr,double mr,double hr,int sr):rate(r),throughrate(tr),reflectrate(rr),mirrorrate(mr),highlightrate(hr),s(sr){}
	double rate;								//������
	double throughrate,reflectrate;				//����ϵ���ͷ���ϵ��
	double mirrorrate,highlightrate;			//���淴��ϵ���͸߹�ϵ��
	int s;										//�߹�ϵ��
};

#endif