
#include"Vec3.h"
#include"Image.h"
#include"Geometry.h"
#include"HittableVec.h"
#include"Camera.h"
#include"RayTracer.h"

#include"omp.h"
#include<chrono>

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
    auto center = std::make_shared<Sphere>(Sphere(Pt3(0.0,0.0,-1.0), 0.5));
    auto ground = std::make_shared<Sphere>(Sphere(Pt3(0.0,-100.5,-1.0), 100.0));
    auto left = std::make_shared<Sphere>(Sphere(Pt3(-1.0,0.0,-1.0), 0.5));
    auto right = std::make_shared<Sphere>(Sphere(Pt3(1.0,0.0,-1.0), 0.5));
    

    mat_ptr diffust_mat = std::make_shared<DiffuseMat>(DiffuseMat(MatColor(0.5,0.5,0.5)));
    mat_ptr ground_mat = std::make_shared<DiffuseMat>(DiffuseMat(MatColor(0.5,0.5,0.5)));
    mat_ptr left_reflect_mat = std::make_shared<ReflectMat>(ReflectMat(MatColor(0.8,0.8,0.8)));
    mat_ptr right_reflect_mat = std::make_shared<ReflectMat>(ReflectMat(MatColor(0.8,0.6,0.2)));
    mat_ptr left_fuzzy_mat = std::make_shared<FuzzyReflectMat>(FuzzyReflectMat(MatColor(0.8,0.8,0.8),0.3));
    mat_ptr right_fuzzy_mat = std::make_shared<FuzzyReflectMat>(FuzzyReflectMat(MatColor(0.8,0.6,0.2),1.0));
    mat_ptr refraction_mat = std::make_shared<RefractionMat>(RefractionMat(1.5));

    center->material=diffust_mat;
    ground->material=ground_mat;
    left->material=refraction_mat;
    right->material=right_reflect_mat;

    world.push(center);
    world.push(ground);
    world.push(left);
    world.push(right);
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
    const int img_width = 400;
    const int img_height = (double)img_width / aspect_ratio;
    Image<RGB_t> img(img_width, img_height);    
    Image<MatColor> buffer(img_width, img_height);
    RayTracer ray_tracer;
    #pragma omp parallel
    for(int j = img.height()-1; j >= 0;j--){
        #pragma omp for nowait
        for(int i=0; i<img.width(); i++){
            for(int sampling=0;sampling<SAMPLE_PER_PIXEL; sampling++){
                dtype du = rand_double();
                dtype dv = rand_double();
                dtype u = (double(i) + du) / (img_width-1);
                dtype v = (double(j) + dv) / (img_height - 1);
                Ray ray;
                cam.getRay(u, v, ray);                    
                buffer.at(i,img.height()-1-j) += ray_tracer.trace(ray, world);
            }
        }
    }
    
    #pragma omp parrallel for collapse(2)
    for(int j=0;j<img.height();j++){
        for(int i=0;i<img.width();i++){
            buffer.at(i,j) /= SAMPLE_PER_PIXEL;
            const MatColor& c = buffer.at(i,j).adjust();
            img.setColor(i,j,c[0],c[1],c[2]);
        }
    }
    // img.dumpPPM("naive_rt.ppm");
    img.dumpJPEG("naive_rt.jpeg");
}

int main(){
    auto start = std::chrono::steady_clock::now();
    // img_test();
    // ray_test();
    ray_tracer_test();

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "Time cost: " << elapsed_seconds.count() << std::endl;

  
    
    return 0;
}