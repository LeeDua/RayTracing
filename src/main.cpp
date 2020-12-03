
#include"Vec3.h"
#include"Image.h"
#include"Geometry.h"
#include"HittableVec.h"
#include"Camera.h"

using namespace RayTracing;

void img_test(){
    Color3<double> color(2.0,1.0,3.0);
    Image<RGB_t> img(1920,1080);
    for(int j=img.height()-1;j>=0;j--){
        for(int i=0; i<img.width(); i++){
            img.setColor(i, img.height()-1-j, img_dtype((double)i/(img.width() - 1)*255.0), img_dtype(double(j)/(img.height() - 1)*255.0), img_dtype(0.25*255.0));
        }
    }
    img.dumpJPEG();
}

void ray_test(){
    const dtype aspect_ratio = 26.0 / 9.0;
    const int img_width = 1600;
    const int img_height = img_width / aspect_ratio;
    Image<RGB_t> img(img_width, img_height);

    dtype viewport_height = 2.0;
    dtype viewport_width = aspect_ratio * viewport_height;
    dtype focal_length = 1.0;
    Camera cam(Pt3(0,0,0), viewport_width, viewport_height, focal_length);

    HittableVec world;
    world.push(std::make_shared<Sphere>(Pt3(0.0,0.0,-1.0), 0.5));
    // world.push(std::make_shared<Sphere>(Pt3(0.0,-100.5,-1.0), 100.0));
    
    HitRecord hit_result;


    for(int j = img_height-1; j >= 0;j--){
        for(int i=0; i<img.width(); i++){
            RGB_t color;
            dtype u = double(i) / (img_width-1);
            dtype v = double(j) / (img_height - 1);
            Ray ray;
            cam.getRay(u, v, ray);            
            world.hit(ray, hit_result);

            if(hit_result.t>0){
                Vec3 c = (hit_result.n + 1.0) * 0.5* 255;
                color.set((img_dtype)c[0],(img_dtype)c[1],(img_dtype)c[2]);
            }
            else{
                dtype mix = (1.0 + ray.direction().normalized()[1])*0.5;
                color = (1.0-mix)*RGB_t(255,255,255) + mix * RGB_t(255*0.5,255*0.7,255*1.0);
            }
            img.setColor(i, img_height-1-j, color);
        }
    }
    img.dumpJPEG("rbsphere.jpeg");
}

int main(){
    // img_test();
    ray_test();

  
    
    return 0;
}