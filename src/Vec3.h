#ifndef SPECIFIC_VEC
#define SPECIFIC_VEC

#define INCLUDE_RAW_VEC
#include"_vec3.h"

namespace RayTracing{
    
        template<typename T>
        class Point3: public Vector3<T> {
            public:
            using Vector3<T>::Vector3;
            Point3(const Vector3<T>& v):Vector3<T>::Vector3(v){};
            // Point3(const Point3<T>& v):Vector3<T>::Vector3(static_cast<const Vector3<T>&>(v)){};

            Point3& operator+=(const Point3& v){
                Vector3<T>::e[0] += v[0];
                Vector3<T>::e[1] += v[1];
                Vector3<T>::e[2] += v[2];
                return *this;
            }
            Point3& operator-=(const Point3& v){
                return *this+=(-v);
            }
        };
        template<typename T>
        class Color3: public Vector3<T> {
            public:
            using Vector3<T>::Vector3;
            Color3(const Vector3<T>& v):Color3<T>::Vector3(v){};
            Color3& operator+=(const Color3& v){
                Vector3<T>::e[0] += v[0];
                Vector3<T>::e[1] += v[1];
                Vector3<T>::e[2] += v[2];
                return *this;
            }
            Color3& operator-=(const Color3& v){
                return *this+=(-v);
            }
        };

        typedef Point3<dtype> Dir3;
        typedef Point3<dtype> Pt3;
        typedef Point3<dtype> Vec3;
        typedef Color3<img_dtype> RGB_t;
    
}


#endif