#ifndef MATERIAL_H
#define MATERIAL_H

#include "Ray.h"


namespace RayTracing{

    inline Vec3 rand_double_vec3(dtype min, dtype max){
        return Vec3(rand_double(min,max), rand_double(min, max), rand_double(min,max));
    }

    inline Vec3 rand_double_in_unit_sphere(){
        while(true){
            Vec3 v = rand_double_vec3(-1.0,1.0);
            if(v.length()>=1)continue;
            return v;
        }        
    }

    class IMaterial {
        public:
            virtual void interactWithLight(Ray& ray, const HitRecord& hit_record, MatColor& color) const = 0;

    };
    
    class SolidColorMat: public IMaterial{
        public:
            explicit SolidColorMat(MatColor&& c):solidColor(c){};
            void interactWithLight(Ray& ray, const HitRecord& hit_record, MatColor& color) const override{
                color = color * solidColor;
                ray.die();
            }
        private:
            MatColor solidColor;
    };

    class DiffuseMat: public IMaterial{
        public:
            explicit DiffuseMat(MatColor&& c):attenuation(c){};
            void interactWithLight(Ray& ray, const HitRecord& hit_record, MatColor& color) const override{
                Dir3 dir = hit_record.n + rand_double_in_unit_sphere();
                if(dir.length() < 1e-8)
                    ray.set(hit_record.p, hit_record.n);
                else
                color *= attenuation;
                    ray.set(hit_record.p, dir);
                ray.updateDepthCounter();
            }
        private:
            MatColor attenuation;
    };

    class ReflectMat: public IMaterial{
        public:
            explicit ReflectMat(MatColor&& c):attenuation(c){};
            void interactWithLight(Ray& ray, const HitRecord& hit_record, MatColor& color) const override{
                ray.set(hit_record.p, reflect(ray.direction(), hit_record.n));
                color *= attenuation;
                ray.updateDepthCounter();
            }
        
        private:
            MatColor attenuation;
            //vec vin "ends" at hit point; n is assumed to be unit vector
            inline Vec3 reflect(const Vec3& vin, const Vec3& n) const{
                return vin - 2*vin.dot(n)*n;
            }
    };

}

#endif