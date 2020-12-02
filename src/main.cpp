
#include"SpecifiedVec3.h"
#include"Image.h"

using namespace RayTracing;


int main(){

    Color3<double> color(2.0,1.0,3.0);
    Image img(256,256);
    for(int j=img.height()-1;j>=0;j--){
        for(int i=0; i<img.width(); i++){
            img.setColor(i,img.height()-1-j,(int)((double)i/(img.width() - 1)*255.0), (int)(double(j)/(img.height() - 1)*255.0), (int)0.25*255.0);
        }
    }
    img.dumpJPEG();
    img.dumpPPM();
    
    return 0;
}