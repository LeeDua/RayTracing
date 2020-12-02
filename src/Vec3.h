#ifndef VEC_H
#define VEC_H

#pragma once
#include<iostream>
#include<fstream>
#include<math.h>
#include"Common.h"

namespace vector{
    template<class Self, typename T>
    class Vec3{
        public:
            Vec3(){ memset(e, 0, sizeof(T)*3); }
            Vec3(T e0, e1, e2):e{e0, e1, e2} {};
            T  x() const { return e[0]; }
            T  y() const { return e[1]; }
            T  z() const { return e[2]; }
            Vec3 operator-() const { return vec3(-e[0], -e[1], -e[2])};
            T operator[](int i) const { return e[i]; };
            T& operator[](int i){ return e[i] };
            Self& operator+=(const Self& v){
                e[0] += v.e[0];
                e[1] += v.e[1];
                e[2] += v.e[2];
                return *this;
            }
            Self& operator-=(const Self& v){
                return *this+=(-v);
            }
            T dot(const Vec3& v){
                return e[0]*v[0] + e[1]*v[1] + e[2]*v[2];
            }
            vec3& operator*=(const multi_type t){
                e[0] *= t;
                e[1] *= t;
                e[2] *= t;
                return *this;
            }
            vec3& operator/=(const multi_type t){
                return (*this *= 1.0/t);
            }
            T square_sum() const {
                return dot(*this);
            }
            T length() const {
                return sqrt(square_sum());
            }
            
        private:
            T e[3];
    };


}

#endif