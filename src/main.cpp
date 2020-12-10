
#include"omp.h"
#include<chrono>
#include<iomanip>

#include"Vec3.h"
#include"Image.h"
#include"Geometry.h"
#include"HittableVec.h"
#include"Camera.h"
#include"RayTracer.h"
#include"BVH.h"

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
    dtype focal_length = 1.0;    
    
    dtype fovy = 90.0;
    Pt3 cam_origin(0.0,0.0,0.0);;
    Dir3 look_at(0.0,0.0,-1.0);
    Dir3 cam_up(0.0, 1.0, 0.0);

    cam = Camera(cam_origin, look_at, cam_up, ASPECT_RATIO, fovy, focal_length);
    auto center = std::make_shared<Sphere>(Pt3(0.0,0.0,-1.0), 0.5);
    auto ground = std::make_shared<Sphere>(Pt3(0.0,-100.5,-1.0), 100.0);
    auto left = std::make_shared<Sphere>(Pt3(-1.0,0.0,-1.0), 0.5);
    auto left_bubble = std::make_shared<Sphere>(Pt3(-1.0,0.0,-1.0), 0.45);
    auto right = std::make_shared<Sphere>(Pt3(1.0,0.0,-1.0), 0.5);
    

    // mat_ptr diffuse_mat = std::make_shared<DiffuseMat>(MatColor(0.5,0.5,0.5));
    // mat_ptr ground_mat = std::make_shared<DiffuseMat>(MatColor(0.5,0.5,0.5));
    mat_ptr diffuse_mat = std::make_shared<DiffuseMat>(MatColor(0.7,0.3,0.3));
    // mat_ptr diffuse_mat = std::make_shared<DiffuseMat>(MatColor(0.1,0.2,0.5));
    mat_ptr ground_mat = std::make_shared<DiffuseMat>(MatColor(0.8,0.8,0.0));
    mat_ptr left_reflect_mat = std::make_shared<ReflectMat>(MatColor(0.8,0.8,0.8));
    mat_ptr right_reflect_mat = std::make_shared<ReflectMat>(MatColor(0.8,0.6,0.2));
    mat_ptr left_fuzzy_mat = std::make_shared<FuzzyReflectMat>(MatColor(0.8,0.8,0.8),0.3);
    mat_ptr right_fuzzy_mat = std::make_shared<FuzzyReflectMat>(MatColor(0.8,0.6,0.2),1.0);
    mat_ptr refraction_mat = std::make_shared<RefractionMat>(1.5);
    mat_ptr bubble_refract_mat = std::make_shared<RefractionMat>(1.0/1.5);


    ground->material=ground_mat;
    center->material=refraction_mat;
    left->material=left_reflect_mat;
    // center->material=refraction_mat;
    right->material = diffuse_mat;
    // left->material=right_reflect_mat;
    // left_bubble->material=bubble_refract_mat;

    world.push(center);
    world.push(ground);
    world.push(left);
    world.push(right);
    // world.push(left_bubble);
}

void random_scene_setup(Camera& cam, HittableVec& world){

    dtype focal_length = 1.0;    
    
    dtype fovy = 20.0;
    Pt3 cam_origin(13.0,2.0,3.0);;
    Dir3 look_at = -cam_origin;
    Dir3 cam_up(0.0, 1.0, 0.0);


    cam = Camera(cam_origin, look_at, cam_up, ASPECT_RATIO, fovy, focal_length);

    auto ground = std::make_shared<Sphere>(Sphere(Pt3(0.0,-1000.0,-0.0), 1000.0));
    mat_ptr ground_mat = std::make_shared<DiffuseMat>(DiffuseMat(MatColor(0.5,0.5,0.5)));
    ground->material=ground_mat;
    world.push(ground);

    const int obj_count = 11;
    const int scale = 1;
    for(int a = -obj_count; a < obj_count; a++){
        for(int b = -obj_count; b < obj_count; b++){
            for(int i=0;i<scale;i++){
                dtype mat_rand = rand_double();
                Pt3 center(a + 0.9*rand_double(), 0.2, b+0.9*rand_double());
                auto sphere = std::make_shared<Sphere>(Sphere(center, 0.2));
                if( (center-Pt3(4.0,0.2,0.0)).length() > 0.9 ){
                    mat_ptr shpere_mat;
                    if(mat_rand < 0.8){
                        shpere_mat = std::make_shared<DiffuseMat>(rand_double_vec3());
                    }
                    else if(mat_rand < 0.95){
                        shpere_mat = std::make_shared<FuzzyReflectMat>(rand_double_vec3(0.5,1.0), rand_double(0.0,0.5));
                    }
                    else{
                        shpere_mat = std::make_shared<RefractionMat>(1.5);
                    }
                    sphere->material = shpere_mat;
                    world.push(sphere);
                }
            }
        }
    }

    mat_ptr glass_mat = std::make_shared<RefractionMat>(1.5);
    auto glass_sphere = std::make_shared<Sphere>(Pt3(0.0,1.0,0.0), 1.0);
    glass_sphere->material = glass_mat;
    world.push(glass_sphere);

    mat_ptr diffuse_mat = std::make_shared<DiffuseMat>(MatColor(0.4,0.2,0.1));
    auto diffuse_sphere = std::make_shared<Sphere>(Pt3(-4.0,1.0,0.0), 1.0);
    diffuse_sphere->material = diffuse_mat;
    world.push(diffuse_sphere);

    mat_ptr reflect_mat = std::make_shared<ReflectMat>(MatColor(0.7,0.6,0.5));
    auto reflect_sphere = std::make_shared<Sphere>(Pt3(4.0,1.0,0.0), 1.0);
    reflect_sphere->material = reflect_mat;
    world.push(reflect_sphere);
}

void ray_test(){
    Camera cam;
    HittableVec world;
    naive_scene_setup(cam, world);
    const int img_width = 400;
    const int img_height = (double)img_width / ASPECT_RATIO;
    Image<RGB_t> img(img_width, img_height);    
    RayTracer ray_tracer(world);
    
    for(int j = img_height-1; j >= 0;j--){
        for(int i=0; i<img.width(); i++){
            HitRecord hit_record;            
            RGB_t color;
            dtype u = double(i) / (img_width-1);
            dtype v = double(j) / (img_height - 1);
            Ray ray;
            cam.getRay(u, v, ray);            
            MatColor c = ray_tracer.trace(ray);
            img.setColor(i,img_height-1-j, c[0], c[1],c[2]);
        }
    }
    img.dumpPPM("ray_test.ppm");
    img.dumpJPEG("ray_test.jpeg");
}

void naive_test(){
    Camera cam;
    HittableVec world;
    // naive_scene_setup(cam, world);
    random_scene_setup(cam, world);
    const int img_width = IMG_WIDTH;
    const int img_height = (double)img_width / ASPECT_RATIO;
    Image<RGB_t> img(img_width, img_height);    
    Image<MatColor> buffer(img_width, img_height);
    RayTracer ray_tracer(world);
    // RayTracer naive_ray_tracer(world);

    Ray ray1, ray2;
    cam.getRay(0.5,0.5,ray1);
    // cam.getRay(0.5,0.5,ray2);
    ray_tracer.trace(ray1);
    // naive_ray_tracer.trace(ray1);
    DEBUG_PRINT("Node hit       : " << node_count);
    DEBUG_PRINT("AABB hit check : " << box_count);
    DEBUG_PRINT("AABB hit true  : " << aabb_hit_count);
    DEBUG_PRINT("Leaf hit       : " << leaf_count);
    DEBUG_PRINT("Sphere hit     : " << sphere_count);
    return;   
}

void ray_tracer_test(){
    auto start = std::chrono::steady_clock::now();
    Camera cam;
    HittableVec world;
    // naive_scene_setup(cam, world);
    random_scene_setup(cam, world);
    const int img_width = IMG_WIDTH;
    const int img_height = (double)img_width / ASPECT_RATIO;
    Image<RGB_t> img(img_width, img_height);    
    Image<MatColor> buffer(img_width, img_height);
    BVHRayTracer ray_tracer(world);
    // RayTracer ray_tracer(world);
    
    auto trace_start = std::chrono::steady_clock::now();
    #pragma omp parallel
    for(int j = img.height()-1; j >= 0;j--){
        #pragma omp master
        {
            dtype remain_percentage = (double)j/img.height();
            auto current = std::chrono::steady_clock::now();
            std::chrono::duration<double> time_used = current-start;
            std::cout << "\rWork remained: " << std::setfill(' ') << std::setw(2) << int(remain_percentage*100.0) << " % " <<  std::setfill(' ') << std::setw(5) <<int(time_used.count()/(1-remain_percentage)*remain_percentage) << " s" << std::flush;
        }
        #pragma omp for nowait
        for(int i=0; i<img.width(); i++){
            for(int sampling=0;sampling<SAMPLE_PER_PIXEL; sampling++){
                dtype du = rand_double();
                dtype dv = rand_double();
                dtype u = (double(i) + du) / (img_width-1);
                dtype v = (double(j) + dv) / (img_height - 1);
                Ray ray;
                cam.getRay(u, v, ray);                    
                buffer.at(i,img.height()-1-j) += ray_tracer.trace(ray);
            }
        }
        
    }
    auto trace_end = std::chrono::steady_clock::now();
    std::chrono::duration<double> trace_time = trace_end - trace_start;
    std::cout << std::endl <<"Trace cost: " << trace_time.count() << std::endl;
    
    #pragma omp parrallel for collapse(2)
    for(int j=0;j<img.height();j++){
        for(int i=0;i<img.width();i++){
            buffer.at(i,j) /= SAMPLE_PER_PIXEL;
            MatColor c = buffer.at(i,j).adjust();
            img.setColor(i,j,c[0],c[1],c[2]);
        }
    }
    // img.dumpPPM("random_scene.ppm");
    img.dumpJPEG("random_scene.jpeg");

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << std::endl <<"Time cost: " << elapsed_seconds.count() << std::endl;

    DEBUG_PRINT("Node hit       : " << node_count);
    DEBUG_PRINT("AABB hit check : " << box_count);
    DEBUG_PRINT("AABB hit true  : " << aabb_hit_count);
    DEBUG_PRINT("Leaf hit       : " << leaf_count);
    DEBUG_PRINT("Sphere hit     : " << sphere_count);
}

int main(){
    // img_test();
    // ray_test();
    ray_tracer_test();
    

  
    
    return 0;
}