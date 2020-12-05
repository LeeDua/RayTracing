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

    //vec vin "ends" at hit point; n is assumed to be unit vector
    inline Vec3 reflect(const Vec3& vin, const Vec3& n){
        return vin - 2*vin.dot(n)*n;
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
    };

    class FuzzyReflectMat: public IMaterial{
        public:
            explicit FuzzyReflectMat(MatColor&& c, dtype fuzziness=0.0):attenuation(c),fuzziness(std::min(fuzziness,1.0)){};
            void interactWithLight(Ray& ray, const HitRecord& hit_record, MatColor& color) const override{
                ray.set(hit_record.p, fuzziness*rand_double_in_unit_sphere()+reflect(ray.direction(), hit_record.n));
                color *= attenuation;
                ray.updateDepthCounter();
            }        
        private:
            MatColor attenuation;
            dtype fuzziness;
            
    };   
    
    class RefractionMat: public IMaterial{
    public:
        explicit RefractionMat(dtype reflect_ratio):reflect_ratio(reflect_ratio){}
        void interactWithLight(Ray& ray, const HitRecord& hit_record, MatColor& color) const override{
            dtype directional_reflect_ratio = hit_record.normAgainstRay ? reflect_ratio : 1.0/reflect_ratio;
            dtype cos_in = -(ray.direction().dot(hit_record.n));
            ASSERT(cos_in > 0);
            dtype sin_in = sqrt(1 - cos_in * cos_in);
            dtype sin_out = directional_reflect_ratio * sin_in;
            if(sin_out > 1.0 || reflectance(cos_in, directional_reflect_ratio)>rand_double()){
                ray.set(hit_record.p, reflect(ray.direction(), hit_record.n));
            }
            else{
                dtype cos_out = sqrt(1 - sin_out * sin_out);
                Dir3 refract_dir = -(ray.direction() + cos_in * hit_record.n) * sin_out - hit_record.n * cos_out;
                ray.set(hit_record.p, refract_dir);
            }
            ray.updateDepthCounter();
        }
    private:
        dtype reflect_ratio;
        inline dtype reflectance(dtype cos_in, dtype directional_reflect_ratio) const{
            dtype r0 = (directional_reflect_ratio-1)/(directional_reflect_ratio+1);
            r0*=r0;
            return r0+(1-r0)*pow((1-cos_in),5);
        }
    };   


}

#endif