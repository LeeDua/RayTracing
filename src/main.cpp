
#include"Vec3.h"
#include"Image.h"
#include"Geometry.h"

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
    const dtype aspect_ratio = 16.0 / 9.0;
    // const dtype aspect_ratio = 1.0;

    const int img_width = 1600;
    const int img_height = img_width / aspect_ratio;

    dtype viewport_height = 2.0;
    dtype viewport_width = aspect_ratio * viewport_height;
    dtype focal_length = 1.0;

    Pt3 origin = Pt3(0,0,0);
    Vec3 horizontal = Vec3(viewport_width, 0, 0);
    Vec3 vertical = Vec3(0, viewport_height, 0);
    Vec3 lower_left_corner = origin - horizontal/2 - vertical/2 - Vec3(0,0,focal_length);

    Sphere sphere(Pt3(0.0,0.0,-1), 0.5);
    HitRecord hit_result;

    Image<RGB_t> img(img_width, img_height);
    for(int j=0;j<img.height();j++){
        for(int i=0; i<img.width(); i++){
            dtype u = double(i) / (img_width-1);
            dtype v = double(j) / (img_height - 1);
            Ray ray(origin, lower_left_corner + u*horizontal + v*vertical);
            sphere.hit(ray, hit_result);
            
            RGB_t color;
            if(hit_result.t>0){
                Vec3 c = (hit_result.n + 1.0) * 0.5* 255;
                color.set((img_dtype)c[0],(img_dtype)c[1],(img_dtype)c[2]);
            }
            else{
                dtype mix = (1.0 + ray.direction().normalized()[1])*0.5;
                color = (1.0-mix)*RGB_t(255,255,255) + mix * RGB_t(255*0.5,255*0.7,255*1.0);
            }
            img.setColor(i, j, color);
        }
    }
    img.dumpJPEG("rbsphere.jpeg");


    
    


}

int main(){
    // img_test();
    ray_test();

  
    
    return 0;
}