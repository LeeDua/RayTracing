
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
#include"Texture.h"

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

void createBox(Pt3 p1, Pt3 p2, HittableVec& world, mat_ptr mat){
    typedef std::shared_ptr<AxisAlignedRect> rect_ptr;    
    std::vector<rect_ptr> recs;
    recs.push_back(std::make_shared<AxisAlignedRect>(Pt3(p1[0],p1[1],p1[2]),Pt3(p1[0],p2[1],p2[2]),0));
    recs.push_back(std::make_shared<AxisAlignedRect>(Pt3(p2[0],p1[1],p1[2]),Pt3(p2[0],p2[1],p2[2]),0));
    recs.push_back(std::make_shared<AxisAlignedRect>(Pt3(p1[0],p1[1],p1[2]),Pt3(p2[0],p1[1],p2[2]),1));
    recs.push_back(std::make_shared<AxisAlignedRect>(Pt3(p1[0],p2[1],p1[2]),Pt3(p2[0],p2[1],p2[2]),1));
    recs.push_back(std::make_shared<AxisAlignedRect>(Pt3(p1[0],p1[1],p1[2]),Pt3(p2[0],p2[1],p1[2]),2));
    recs.push_back(std::make_shared<AxisAlignedRect>(Pt3(p1[0],p1[1],p2[2]),Pt3(p2[0],p2[1],p2[2]),2));
    for(auto rect: recs){
        rect->material = mat;
        world.push(rect);
    }
}

void naive_scene_setup(Camera& cam, HittableVec& world){
    dtype focal_length = 1.0;    
    
    dtype fovy = 90.0;
    // dtype fovy = 20.0;
    Pt3 cam_origin(0.0,0.0,0.0);
    // Pt3 cam_origin(13.0,2.0,3.0);
    Dir3 look_at(0.0,0.0,-1.0);
    // Dir3 look_at(-13.0,-2.0,-3.0);
    Dir3 cam_up(0.0, 1.0, 0.0);

    cam = Camera(cam_origin, look_at, cam_up, ASPECT_RATIO, fovy, focal_length);
    auto center = std::make_shared<Sphere>(Pt3(0.0,0.0,-1.0), 0.5);
    // auto center = std::make_shared<Sphere>(Pt3(0.0,0.0,0.0), 2.0);

    auto ground = std::make_shared<Sphere>(Pt3(0.0,-100.5,-1.0), 100.0);
    auto left = std::make_shared<Sphere>(Pt3(-1.0,0.0,-1.0), 0.5);
    auto left_bubble = std::make_shared<Sphere>(Pt3(-1.0,0.0,-1.0), 0.45);
    auto right = std::make_shared<Sphere>(Pt3(1.0,0.0,-1.0), 0.5);
    auto fog_sphere = std::make_shared<ParticipatingMedia>(center,20.0);
    

    // mat_ptr diffuse_mat = std::make_shared<DiffuseMat>(MatColor(0.5,0.5,0.5));
    mat_ptr ground_mat = std::make_shared<DiffuseMat>(MatColor(0.8,0.8,0.8));
    // mat_ptr diffuse_mat = std::make_shared<DiffuseMat>(MatColor(0.7,0.3,0.3));
    // mat_ptr diffuse_mat = std::make_shared<DiffuseMat>(MatColor(0.1,0.2,0.5));
    // mat_ptr ground_mat = std::make_shared<DiffuseMat>(MatColor(0.8,0.8,0.0));
    mat_ptr left_reflect_mat = std::make_shared<ReflectMat>(MatColor(0.8,0.8,0.8));
    mat_ptr right_reflect_mat = std::make_shared<ReflectMat>(MatColor(0.8,0.6,0.2));
    mat_ptr left_fuzzy_mat = std::make_shared<FuzzyReflectMat>(MatColor(0.8,0.8,0.8),0.3);
    mat_ptr right_fuzzy_mat = std::make_shared<FuzzyReflectMat>(MatColor(0.8,0.6,0.2),1.0);
    mat_ptr refraction_mat = std::make_shared<RefractionMat>(1.5);
    mat_ptr bubble_refract_mat = std::make_shared<RefractionMat>(1.0/1.5);
   
    auto random_scatter_mat = std::make_shared<RandomScatterMat>();
    // auto solid_tex = std::make_shared<SolidColorTexture>(MatColor(1.0,1.0,1.0));
    auto solid_tex = std::make_shared<SolidColorTexture>(MatColor(0.2,0.4,0.9));
    mat_ptr fog_mat = std::make_shared<ColoredRandomScatterMat>(random_scatter_mat, solid_tex);

    auto one_diffuse = std::make_shared<DiffuseMat>(MatColor(1.0,1.0,1.0));
    auto earth_img = std::make_shared<ImgTexture>("../earthmap.jpg");
    auto earth_texture = std::make_shared<ImgTextureDiffuseMat>(one_diffuse, earth_img);
    // center->material = earth_texture;



    ground->material=ground_mat;
    center->material=ground_mat;
    fog_sphere->material = fog_mat;
    left->material=right_fuzzy_mat;
    // center->material=refraction_mat;
    right->material = left_fuzzy_mat;
    // left->material=right_reflect_mat;
    // left_bubble->material=bubble_refract_mat;

    // world.push(center);
    world.push(fog_sphere);
    world.push(ground);
    // world.push(left);
    // world.push(right);
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
    mat_ptr ground_mat = std::make_shared<DiffuseMat>(MatColor(0.5,0.5,0.5));
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

void cornel_box_setup(Camera& cam, HittableVec& world){
    dtype focal_length = 1.0;    
    dtype fovy = 40.0;
    Pt3 cam_origin(278.0, 278.0, -800.0);
    Dir3 look_at(0.0, 0.0, 800.0);
    Dir3 cam_up(0.0, 1.0, 0.0);
    cam = Camera(cam_origin, look_at, cam_up, ASPECT_RATIO, fovy, focal_length);

    auto red_diffuse = std::make_shared<DiffuseMat>(MatColor(0.65, 0.05, 0.05));
    auto gray_diffuse = std::make_shared<DiffuseMat>(MatColor(0.73, 0.73, 0.73));
    auto green_diffuse = std::make_shared<DiffuseMat>(MatColor(0.12, 0.45, 0.15));
    auto light_mat = std::make_shared<EmissiveMat>(MatColor(15.0,15.0,15.0));

    auto light = std::make_shared<AxisAlignedRect>(Pt3(213.0,554.0,227.0), Pt3(343.0,554.0,332.0), 1);
    light->material = light_mat;
    world.push(light);

    auto left_wall = std::make_shared<AxisAlignedRect>(Pt3(555.0,0.0,0.0),Pt3(555.0,555.0,555.0), 0);
    left_wall->material = green_diffuse;
    world.push(left_wall);

    auto right_wall = std::make_shared<AxisAlignedRect>(Pt3(0.0,0.0,0.0), Pt3(0.0,555.0,555.0), 0);
    right_wall->material = red_diffuse;
    world.push(right_wall);

    auto back_wall = std::make_shared<AxisAlignedRect>(Pt3(0.0,0.0,555.0), Pt3(555.0,555.0,555.0), 2);
    back_wall->material = gray_diffuse;
    world.push(back_wall);
    
    auto up_wall = std::make_shared<AxisAlignedRect>(Pt3(0.0,555.0,0.0), Pt3(555.0,555.0,555.0), 1);
    up_wall->material = gray_diffuse;
    world.push(up_wall);

    auto bottom_wall = std::make_shared<AxisAlignedRect>(Pt3(0.0,0.0,0.0), Pt3(555.0,0.0,555.0), 1);
    bottom_wall->material = gray_diffuse;
    world.push(bottom_wall);

    auto test_sphere = std::make_shared<Sphere>(Pt3(278.0,278.0,150.0),50.0);
    // test_sphere->material = gray_diffuse;
    test_sphere->material = light_mat;
    // world.push(test_sphere);

    Pt3 front_p1(130.0,0.0,65.0);
    Pt3 front_p2(295.0,165.0, 230.0);
    createBox(front_p1, front_p2, world, gray_diffuse);
    Pt3 back_p1(265.0,0.0,295.0);
    Pt3 back_p2(430.0,330.0,460.0);
    createBox(back_p1, back_p2, world, gray_diffuse);


}

void final_scene_setup(Camera& cam, HittableVec& world){
    dtype focal_length = 1.0;    
    dtype fovy = 40.0;
    Pt3 cam_origin(478, 278, -600);;
    Dir3 look_at(-200,0,600);
    Dir3 cam_up(0.0, 1.0, 0.0);
    cam = Camera(cam_origin, look_at, cam_up, ASPECT_RATIO, fovy, focal_length);

    auto earth_sphere = std::make_shared<Sphere>(Pt3(400.0,200.0,400.0),100.0);
    auto one_diffuse = std::make_shared<DiffuseMat>(MatColor(1.0,1.0,1.0));
    auto earth_img = std::make_shared<ImgTexture>("../earthmap.jpg");
    auto earth_texture = std::make_shared<ImgTextureDiffuseMat>(one_diffuse, earth_img);
    earth_sphere->material = earth_texture;
    world.push(earth_sphere);

    auto diffuse_sphere = std::make_shared<Sphere>(Pt3(400.0,400.0,200.0),50.0);
    auto diffuse_mat = std::make_shared<DiffuseMat>(MatColor(0.7,0.3,0.1));
    diffuse_sphere->material = diffuse_mat;
    world.push(diffuse_sphere);

    auto glass_sphere = std::make_shared<Sphere>(Pt3(260.0,150.0,45.0),50.0);
    auto refraction_mat = std::make_shared<RefractionMat>(1.5);
    glass_sphere->material = refraction_mat;
    world.push(glass_sphere);

    auto blue_glass = std::make_shared<Sphere>(Pt3(360.0,150.0,145.0),70.0);
    blue_glass->material = refraction_mat;
    world.push(blue_glass);

    auto blue_fog = std::make_shared<ParticipatingMedia>(blue_glass, 0.2);
    auto random_scatter_mat = std::make_shared<RandomScatterMat>();
    // auto solid_tex = std::make_shared<SolidColorTexture>(MatColor(1.0,1.0,1.0));
    auto blue_texutre = std::make_shared<SolidColorTexture>(MatColor(0.2,0.4,0.9));
    auto blue_fog_mat = std::make_shared<ColoredRandomScatterMat>(random_scatter_mat, blue_texutre);
    blue_fog->material = blue_fog_mat;
    world.push(blue_fog);


    auto metal_sphere = std::make_shared<Sphere>(Pt3(0.0,150.0,145.0),50.0);
    auto metal_mat = std::make_shared<FuzzyReflectMat>(MatColor(0.8,0.8,0.9),1.0);
    metal_sphere->material = metal_mat;
    world.push(metal_sphere);

    auto noise_sphere = std::make_shared<Sphere>(Pt3(220.0,280.0,300.0),80.0);
    auto noise_mat = std::make_shared<DiffuseMat>(MatColor(0.8,0.8,0.9));
    noise_sphere->material = noise_mat;
    world.push(noise_sphere);

    auto light_mat = std::make_shared<EmissiveMat>(MatColor(7.0,7.0,7.0));
    auto light = std::make_shared<AxisAlignedRect>(Pt3(123.0,554.0,147.0),Pt3(423.0,554.0,412.0),1);
    light->material = light_mat;
    world.push(light);
    

    // auto white_mat = std::make_shared<DiffuseMat>(MatColor(0.73,0.73,0.73));
    // const int dots = 1000;
    // for(int i=0;i<dots;i++){
    //     auto dot = std::make_shared<Sphere>(MatColor(rand_double_vec3(0.0,165.0))+Vec3(-100.0,270.0,395.0),10.0);
    //     dot->material = white_mat;
    //     world.push(dot);
    // }

    auto green_mat = std::make_shared<DiffuseMat>(MatColor(0.48,0.83,0.53));
    // const int boxes = 20;
    // for(int i=0; i<boxes;i++){
    //     for(int j=0;j<boxes;j++){
    //         dtype w = 100.0;
    //         dtype x0 = -1000.0 + i*w;
    //         dtype z0 = -1000.0 + j*w;
    //         dtype y0 = 0.0;
    //         dtype x1 = x0 + w;
    //         dtype y1 = rand_double(1.0,101.0);
    //         dtype z1 = z0 + w;
    //         createBox(Pt3(x0,y0,z0), Pt3(x1,y1,z1), world, green_mat);
    //     }
    // }
    createBox(Pt3(-1000.0,0.0,-1000.0),Pt3(1000.0,20.0,1000.0), world, green_mat);
    // auto ground = std::make_shared<AxisAlignedRect>(Pt3(-1000.0,0.0,-1000.0),Pt3(1000.0,0.0,1000.0),1);
    // ground->material = green_mat;
    // world.push(ground);


    auto boundary = std::make_shared<Sphere>(Pt3(0.0,0.0,0.0),70);
    auto scene_fog = std::make_shared<ParticipatingMedia>(boundary, MIN_HIT_DIST);
    // auto solid_tex = std::make_shared<SolidColorTexture>(MatColor(1.0,1.0,1.0));
    auto scene_fog_tex = std::make_shared<SolidColorTexture>(MatColor(1.0,1.0,1.0));
    auto scene_fog_mat = std::make_shared<ColoredRandomScatterMat>(random_scatter_mat, scene_fog_tex);
    scene_fog->material = scene_fog_mat;
    world.push(scene_fog);

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
    naive_scene_setup(cam, world);
    // random_scene_setup(cam, world);
    // cornel_box_setup(cam, world);
    const int img_width = IMG_WIDTH;
    const int img_height = (double)img_width / ASPECT_RATIO;
    Image<RGB_t> img(img_width, img_height);    
    Image<MatColor> buffer(img_width, img_height);
    // BVHRayTracer ray_tracer(world);
    RayTracer ray_tracer(world);

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
    DEBUG_PRINT("Rect hit       : " << rect_hit_count);
    DEBUG_PRINT("Light hit      : " << light_hit_count);
    return;   
}

void ray_tracer_test(){
    auto start = std::chrono::steady_clock::now();
    Camera cam;
    HittableVec world;
    // naive_scene_setup(cam, world);
    // random_scene_setup(cam, world);
    // cornel_box_setup(cam, world);
    final_scene_setup(cam,world);
    const int img_width = IMG_WIDTH;
    const int img_height = (double)img_width / ASPECT_RATIO;
    Image<RGB_t> img(img_width, img_height);    
    Image<MatColor> buffer(img_width, img_height);
    BVHRayTracer ray_tracer(world);
    // RayTracer ray_tracer(world);
    omp_set_dynamic(0);
    omp_set_num_threads(8);
    auto trace_start = std::chrono::steady_clock::now();
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
                buffer.at(i,img.height()-1-j) += ray_tracer.trace(ray);
            }
        }
        #pragma omp master
        {
            dtype remain_percentage = (double)(j+1)/img.height();
            auto current = std::chrono::steady_clock::now();
            std::chrono::duration<double> time_used = current-start;
            if((img.height() - j) >= 2){
                std::cout << "\rWork remained: " << std::setfill(' ') << std::setw(2) << int(remain_percentage*100.0) << " % " <<  std::setfill(' ') << std::setw(5) <<int(time_used.count()/(1-remain_percentage)*remain_percentage) << " s" << std::setfill(' ') << std::setw(5) << int(time_used.count()) << " s" << std::flush;
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
    DEBUG_PRINT("Rect hit       : " << rect_hit_count);
    DEBUG_PRINT("Light hit      : " << light_hit_count);

}

int main(){
    // img_test();
    // ray_test();
    ray_tracer_test();
    // naive_test();
    

  
    
    return 0;
}