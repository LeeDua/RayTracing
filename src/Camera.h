#ifndef CAMERA_H
#define CAMERA_H

#include"Common.h"
#include"Ray.h"
#include"Vec3.h"

namespace RayTracing{
    class Camera
    {
    public:
        explicit Camera(Pt3 ori, int viewport_width, int viewport_height, int focal_length):origin(ori){
            horizontal = Vec3(viewport_width, 0, 0);
            vertical = Vec3(0, viewport_height, 0);
            lower_left_corner = origin - horizontal/2 - vertical/2 - Vec3(0,0,focal_length);
        };
        void getRay(dtype u, dtype v, Ray& ray) const{
            ray.set(origin, lower_left_corner + u * horizontal + v * vertical - origin);
        }
        
    private:
        Pt3 origin;
        Vec3 lower_left_corner;
        Vec3 horizontal;
        Vec3 vertical;

    };
}



#endif