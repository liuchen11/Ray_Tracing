#include <fstream>
#include <iostream>
using namespace std;

int main(int argc,char* argv[]){
    ifstream in(argv[1]);
    ofstream out(argv[2]);
    double ax,bx,ay,by,az,bz;
    cin>>ax>>bx>>ay>>by>>az>>bz;
    char buffer[256];
    while(in>>buffer){
        if(buffer[0]=='v'){
            double x,y,z;
            in>>x>>y>>z;
            x=ax*x+bx;
            y=ay*y+by;
            z=az*z+bz;
            out<<"v "<<x<<" "<<y<<" "<<z<<endl;
        }
        else if(buffer[0]=='f'){
            int index1,index2,index3;
            in>>index1>>index2>>index3;
            out<<"f "<<index1-1<<" "<<index2-1<<" "<<index3-1<<endl;
        }
    }
    return 0;
}
