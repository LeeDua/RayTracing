
#include"Vec3.h"
#include"Image.h"
#include"Geometry.h"
#include"HittableVec.h"
#include"Camera.h"
#include"RayTracer.h"

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

void naive_scene_setup(Camera& cam, HittableVec& world){
    const dtype aspect_ratio = 16.0 / 9.0;
    dtype focal_length = 1.0;
    dtype fovy = 90.0;

    Pt3 cam_origin;
    Dir3 look_at(0.0,0.0,-1.0);
    Dir3 cam_up(0.0, 1.0, 0.0);

    cam = Camera(cam_origin, look_at, cam_up, aspect_ratio, fovy, focal_length);
    auto s1 = std::make_shared<Sphere>(Sphere(Pt3(0.0,0.0,-1.0), 0.5));
    auto s2 = std::make_shared<Sphere>(Sphere(Pt3(0.0,-100.5,-1.0), 100.0));

    mat_ptr mat1 = std::make_shared<DiffuseMat>(DiffuseMat(MatColor(0.5,0.5,0.5)));
    mat_ptr mat2 = std::make_shared<DiffuseMat>(DiffuseMat(MatColor(0.5,0.5,0.5)));

    s1->material = mat1;
    s2->material = mat2;

    world.push(s1);
    world.push(s2);
}

void ray_test(){
    const dtype aspect_ratio = 16.0 / 9.0;
    Camera cam;
    HittableVec world;
    naive_scene_setup(cam, world);
    const int img_width = 1600;
    const int img_height = (double)img_width / aspect_ratio;
    Image<RGB_t> img(img_width, img_height);    
    
    for(int j = img_height-1; j >= 0;j--){
        for(int i=0; i<img.width(); i++){
            HitRecord hit_record;            
            RGB_t color;
            dtype u = double(i) / (img_width-1);
            dtype v = double(j) / (img_height - 1);
            Ray ray;
            cam.getRay(u, v, ray);            
            world.hit(ray, hit_record);

            if(world.hit(ray, hit_record)){
                Vec3 c = (hit_record.n + Vec3(1.0,1.0,1.0)) * 0.5* 255.0;
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

void ray_tracer_test(){
    const dtype aspect_ratio = 16.0 / 9.0;
    Camera cam;
    HittableVec world;
    naive_scene_setup(cam, world);
    const int img_width = 800;
    const int img_height = (double)img_width / aspect_ratio;
    Image<RGB_t> img(img_width, img_height);    
    
    RayTracer ray_tracer;
    
    for(int j = img.height()-1; j >= 0;j--){
        for(int i=0; i<img.width(); i++){
            MatColor pixel_color;
            int hit_count = 0;       
            for(int sampling=0;sampling<SAMPLE_PER_PIXEL; sampling++){
                dtype du = rand_double();
                dtype dv = rand_double();
                dtype u = (double(i) + du) / (img_width-1);
                dtype v = (double(j) + dv) / (img_height - 1);
                Ray ray;
                cam.getRay(u, v, ray);            
                MatColor color = ray_tracer.trace(ray, world);
                pixel_color += color;
                if(color.length()>0.01){
                    hit_count += 1;
                }
            }
            if(hit_count > 1){
                pixel_color /= hit_count;
            }
            pixel_color = pixel_color.adjust();
            img.setColor(i, img.height()-1-j, pixel_color);
        }
    }
    // img.dumpPPM("naive_rt.ppm");
    img.dumpJPEG("naive_rt.jpeg");
}

int main(){
    // img_test();
    // ray_test();
    ray_tracer_test();

  
    
    return 0;
}