#ifndef SPECIFIC_VEC
#define SPECIFIC_VEC

#pragma once
#include"Vec3.h"

namespace RayTracing{
    
        template<typename T>
        class Point3: public Vec3<T> {
            public:
            using Vec3<T>::Vec3;
            Point3& operator+=(const Point3& v){
                Vec3<T>::e[0] += v[0];
                Vec3<T>::e[1] += v[1];
                Vec3<T>::e[2] += v[2];
                return *this;
            }
            Point3& operator-=(const Point3& v){
                return *this+=(-v);
            }
        };
        template<typename T>
        class Color3: public Vec3<T> {
            public:
            using Vec3<T>::Vec3;
            Color3& operator+=(const Color3& v){
                Vec3<T>::e[0] += v[0];
                Vec3<T>::e[1] += v[1];
                Vec3<T>::e[2] += v[2];
                return *this;
            }
            Color3& operator-=(const Color3& v){
                return *this+=(-v);
            }
        };
    
}


#endif