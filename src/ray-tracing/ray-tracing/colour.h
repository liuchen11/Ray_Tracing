#ifndef __COLOUR_H__
#define __COLOUR_H__

class colour{
public:
	colour(int r=0,int g=0,int b=0):red(r),green(g),blue(b){}
	bool operator==(colour p){return p.blue==blue&&p.green==green&&p.red==red;}
	bool operator!=(colour p){return p.blue!=blue||p.green!=green||p.red!=red;}
	colour operator+(colour p){
		int r,g,b;
		r=(red+p.red>255?255:(red+p.red));
		g=(green+p.green>255?255:(green+p.green));
		b=(blue+p.blue>255?255:(blue+p.blue));
		return colour(r,g,b);
	}
	colour operator-(colour p){
		int r,g,b;
		r=(red-p.red<0?0:red-p.red);
		g=(green-p.green<0?0:green-p.green);
		b=(blue-p.blue<0?0:blue-p.blue);
		return colour(r,g,b);
	}
	colour operator*(int p){
		if(p<0)
			return colour(0,0,0);
		int r,g,b;
		r=(red*p>255?255:red*p);
		g=(green*p>255?255:green*p);
		b=(blue*p>255?255:blue*p);
		return colour(r,g,b);
	}
	colour operator*(double p){
		if(p<0)
			return colour(0,0,0);
		int r,g,b;
		r=(red*p>255?255:red*p);
		g=(green*p>255?255:green*p);
		b=(blue*p>255?255:blue*p);
		return colour(r,g,b);
	}
	colour operator/(int p){
		if(p<0)
			return colour(0,0,0);
		int r,g,b;
		r=(red/p>255?255:red/p);
		g=(green/p>255?255:green/p);
		b=(blue/p>255?255:blue/p);
		return colour(r,g,b);
	}
	colour operator/(double p){
		if(p<0)
			return colour(0,0,0);
		int r,g,b;
		r=(red/p>255?255:red/p);
		g=(green/p>255?255:green/p);
		b=(blue/p>255?255:blue/p);
		return colour(r,g,b);
	}
	int red,green,blue;
};

#endif