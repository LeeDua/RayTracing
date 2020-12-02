#ifndef VEC_H
#define VEC_H

#pragma once
#include <math.h>

#include <cstring>
#include <fstream>
#include <iostream>

#include "Common.h"

namespace RayTracing {

    template <typename T>
    class Vec3 {
    public:
        Vec3() { memset(e, 0, sizeof(T) * 3); }
        Vec3(T e0, T e1, T e2) : e{e0, e1, e2} {};
        T x() const { return e[0]; }
        T y() const { return e[1]; }
        T z() const { return e[2]; }
        T operator[](int i) const { return e[i]; }
        T& operator[](int i) { return e[i]; }
        Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
        Vec3 &operator+=(const Vec3 &v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }
        Vec3 &operator-=(const Vec3 &v) { return *this += (-v); }
        T dot(const Vec3 &v) { return e[0] * v[0] + e[1] * v[1] + e[2] * v[2]; }
        Vec3 &operator*=(const multi_type t) {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }
        Vec3 &operator/=(const multi_type t) { return (*this *= 1.0 / t); }
        T square_sum() const { return dot(*this); }
        T length() const { return sqrt(square_sum()); }
        void normalize() { e /= length(); }
        Vec3 normalized() {
            T len = length();
            return Vec3(e[0] / len, e[1] / len, e[2] / len);
        }

    protected:
        T e[3];
    };
        template <typename T>
        inline Vec3<T> operator+(const Vec3<T> &v, T t) {
            return Vec3<T>(v[0] + t, v[1] + t, v[2] + t);
        }
        template <typename T>
        inline Vec3<T> operator-(const Vec3<T> &v, T t) {
            return v + (-t);
        }
        template <typename T>
        inline Vec3<T> operator*(const Vec3<T> &v, multi_type t) {
            return Vec3<T>(v[0] * t, v[1] * t, v[2] * t);
        }
        template <typename T>
        inline Vec3<T> operator*(multi_type t, const Vec3<T> &v) {
            return v * t;
        }
        template <typename T>
        inline Vec3<T> operator/(const Vec3<T> &v, multi_type t) {
            return Vec3<T>(v[0] / t, v[1] / t, v[2] / t);
        }
        template <typename T>
        inline Vec3<T> operator/(multi_type t, const Vec3<T> &v) {
            return v / t;
        }

        template <typename T>
        inline Vec3<T> operator*(const Vec3<T> &a, const Vec3<T> &b) {
            return Vec3<T>(a[0] * b[0] + a[1] * b[1] + a[2] * b[2]);
        }

        template <typename T>
        inline Vec3<T> dot(const Vec3<T> &a, const Vec3<T> &b) {
            return a.dot(b);
        }
        template <typename T>
        inline Vec3<T> cross(const Vec3<T> &a, const Vec3<T> &b) {
            return Vec3<T>(a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2],
                        a[0] * b[1] - a[1] * b[0]);
        }
    
}  // namespace RayTracing

#endif