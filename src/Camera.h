#ifndef CAMERA_H
#define CAMERA_H

#include"Common.h"
#include"Ray.h"
#include"Vec3.h"

namespace RayTracing{
    class Camera
    {
    public:
        Camera(){} 
        Camera(Pt3 ori, Dir3 look_at, Dir3 vup, dtype aspect_ratio, dtype fovy, dtype focal):origin(ori),lookAt(look_at),up(vup.normalized()),focal(focal){
            dtype height = tan(degree_to_radius(fovy)/2) * focal * 2.0;
            dtype width = height * aspect_ratio;
            vertical = up*height;
            horizontal = cross(lookAt, vertical).normalized() * width;
            lookAt.normalize();
        };
        void getRay(dtype u, dtype v, Ray& ray) const{
            ray.set(origin,horizontal*(-0.5 + u) + vertical*(-0.5 + v) + lookAt*focal);
        }
        
    private:
        Pt3 origin;
        Dir3 lookAt;
        Dir3 up;
        dtype focal;
        Dir3 horizontal, vertical;
    };
}



#endif