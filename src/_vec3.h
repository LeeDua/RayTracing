#ifndef VEC_H
#define VEC_H

#ifdef INCLUDE_RAW_VEC

#include <math.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include "Common.h"

namespace RayTracing {

    template <typename T>
    class Vector3 {
    public:
        Vector3() { memset(e, 0, sizeof(T) * 3); }
        Vector3(T e0, T e1, T e2) : e{e0, e1, e2} {};
        Vector3(const Vector3<T>& v){ memcpy(e, &v, sizeof(T) * 3);}
        T x() const { return e[0]; }
        T y() const { return e[1]; }
        T z() const { return e[2]; }
        const T* operator&() const { return e; }
        T operator[](int i) const { return e[i]; }
        T& operator[](int i) { return e[i]; }
        Vector3 operator-() const { return Vector3(-e[0], -e[1], -e[2]); }
        Vector3 &operator+=(const Vector3 &v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }
        Vector3 &operator-=(const Vector3 &v) { return *this += (-v); }
        T dot(const Vector3 &v) const { return e[0] * v[0] + e[1] * v[1] + e[2] * v[2]; }
        Vector3 &operator*=(const multi_type t) {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }
        Vector3 &operator/=(const multi_type t) { return (*this *= 1.0 / t); }
        void set(T e0, T e1, T e2){ e[0] = e0; e[1] = e1; e[2] = e2; }
        T square_sum() const { return dot(*this); }
        T length() const { return sqrt(square_sum()); }
        void normalize() { e /= length(); }
        Vector3 normalized() {
            T len = length();
            return Vector3(e[0] / len, e[1] / len, e[2] / len);
        }

    protected:
        T e[3];
    };
        template <typename T>
        inline Vector3<T> operator+(const Vector3<T> &v, T t) {
            return Vector3<T>(v[0] + t, v[1] + t, v[2] + t);
        }
        template <typename T>
        inline Vector3<T> operator-(const Vector3<T> &v, T t) {
            return v + (-t);
        }
        template <typename T>
        inline Vector3<T> operator*(const Vector3<T> &v, multi_type t) {
            return Vector3<T>(v[0] * t, v[1] * t, v[2] * t);
        }
        template <typename T>
        inline Vector3<T> operator*(multi_type t, const Vector3<T> &v) {
            return v * t;
        }
        template <typename T>
        inline Vector3<T> operator/(const Vector3<T> &v, multi_type t) {
            return Vector3<T>(v[0] / t, v[1] / t, v[2] / t);
        }
        template <typename T>
        inline Vector3<T> operator/(multi_type t, const Vector3<T> &v) {
            return v / t;
        }

        template <typename T>
        inline Vector3<T> operator*(const Vector3<T> &a, const Vector3<T> &b) {
            return Vector3<T>(a[0] * b[0] + a[1] * b[1] + a[2] * b[2]);
        }

        template <typename T>
        inline Vector3<T> operator+(const Vector3<T> &a, const Vector3<T> &b) {
            return Vector3<T>(a[0] + b[0], a[1] + b[1], a[2] + b[2]);
        }

        template <typename T>
        inline Vector3<T> operator-(const Vector3<T> &a, const Vector3<T> &b) {
            return a+(-b);
        }

        template <typename T>
        inline dtype dot(const Vector3<T> &a, const Vector3<T> &b) {
            return a.dot(b);
        }
        template <typename T>
        inline Vector3<T> cross(const Vector3<T> &a, const Vector3<T> &b) {
            return Vector3<T>(a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2],
                        a[0] * b[1] - a[1] * b[0]);
        }
    
}  // namespace RayTracing

#endif //INCLUDE_RAW_VEC

#endif