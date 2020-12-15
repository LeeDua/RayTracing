#ifndef VEC3_H
#define VEC3_H

#define INCLUDE_RAW_VEC
#include"_vec3.h"


namespace RayTracing{

    template<class T> using Point3 = Vector3<T>;
    template<class T> using Color3 = Vector3<T>;

    typedef Point3<dtype> Dir3;
    typedef Point3<dtype> Pt3;
    typedef Point3<dtype> Vec3;
    typedef Color3<img_dtype> RGB_t;
    typedef Color3<dtype> MatColor;

    void adjustColor(MatColor& c){
        c[0] = sqrt(std::min(c[0], 1.0));
        c[1] = sqrt(std::min(c[1], 1.0));
        c[2] = sqrt(std::min(c[2], 1.0));
    }

    RGB_t toRGB(const MatColor& c){
        return RGB_t{
            img_dtype(255.99*c[0]),
            img_dtype(255.99*c[1]),
            img_dtype(255.99*c[2])
        };
    }

    MatColor toDouble(const RGB_t& c){
        return MatColor{
            1.0/255.99*c[0],
            1.0/255.99*c[1],
            1.0/255.99*c[2]
        };
    }

}






#endif