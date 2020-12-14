#ifndef MATERIAL_H
#define MATERIAL_H

#include "Ray.h"


namespace RayTracing{
    
    inline Vec3 rand_double_vec3(dtype min=0.0, dtype max=1.0){
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
        private:
        
    };
    
    class DiffuseMat: public IMaterial{
        public:
            explicit DiffuseMat(MatColor&& c):attenuation(c){};
            void interactWithLight(Ray& ray, const HitRecord& hit_record, MatColor& color) const override{
                Dir3 dir = hit_record.n + rand_double_in_unit_sphere();
                if(dir.square_sum() < EPSILON)
                    ray.set(hit_record.p, hit_record.n);
                else
                    ray.set(hit_record.p, dir);
                color *= attenuation;
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
            }        
        private:
            MatColor attenuation;
            dtype fuzziness;
            
    };   
    
    class RefractionMat: public IMaterial{
        public:
        explicit RefractionMat(dtype reflect_ratio):reflect_ratio(reflect_ratio){}
        void interactWithLight(Ray& ray, const HitRecord& hit_record, MatColor& color) const override{
            dtype cos_in = -(ray.direction().normalized().dot(hit_record.n));
            dtype directional_reflect_ratio = hit_record.normAgainstRay ?  1.0/reflect_ratio : reflect_ratio;
            ASSERT(cos_in >= 0 && cos_in <= 1.0);
            dtype sin_in = sqrt(1 - cos_in * cos_in);
            dtype sin_out = directional_reflect_ratio * sin_in;
            if(sin_out > 1.0 || reflectance(cos_in, directional_reflect_ratio)>rand_double()){
                ray.set(hit_record.p, reflect(ray.direction(), hit_record.n));
            }
            else{
                dtype cos_out = sqrt(1 - sin_out * sin_out);
                Dir3 refract_dir = (ray.direction() + cos_in * hit_record.n).normalized() * sin_out - hit_record.n * cos_out;
                ray.set(hit_record.p + refract_dir.normalized()*EPSILON_MORE, refract_dir);
            }
        }
        private:
        dtype reflect_ratio;
        inline dtype reflectance(dtype cos_in, dtype directional_reflect_ratio) const{
            dtype r0 = (1-directional_reflect_ratio)/(directional_reflect_ratio+1);
            r0*=r0;
            return r0+(1-r0)*pow((1.0-cos_in),5.0);
        }
    };

    class EmissiveMat: public IMaterial{
        public:
        EmissiveMat(MatColor&& c):lightColor(c){};
        void interactWithLight(Ray& ray, const HitRecord& hit_record, MatColor& color) const override{
            color += lightColor;
            ray.die();
        }
        private:
        MatColor lightColor;
    };

    class CheckerMat: private DiffuseMat{
        public:
        CheckerMat(MatColor&& c1, MatColor&& c2):c1(c1),c2(c2),DiffuseMat(MatColor(1.0,1.0,1.0)){}  
        void interactWithLight(Ray& ray, const HitRecord& hit_record, MatColor& color) const override{
            const Pt3& hit_pt = hit_record.p;
            dtype scale = 10.0;
            dtype sines = std::sin(hit_pt[0]*scale) * std::sin(hit_pt[1]*scale) * std::sin(hit_pt[2]*scale);
            DiffuseMat::interactWithLight(ray, hit_record, color);
            if(sines>0)color*= c1;
            else color*= c2;
        }
        private:
        MatColor c1,c2;
    };

    class RandomScatterMat: public IMaterial{
        public:
        RandomScatterMat(){};
        virtual void interactWithLight(Ray& ray, const HitRecord& hit_record, MatColor& color) const{
            //TODO Check how many times this mat is triggered in DEBUG MODE
            ray.set( hit_record.p, rand_double_in_unit_sphere());
        }
    };

    template<class BaseMat, class Texture>
    class TexturedMat: public IMaterial{
        typedef std::shared_ptr<BaseMat> mat_ptr;
        typedef std::shared_ptr<Texture> tex_ptr;
        public:
        explicit TexturedMat(mat_ptr mp, tex_ptr tp):mat_pointer(mp),tex_pointer(tp){};
        void interactWithLight(Ray& ray, const HitRecord& hit_record, MatColor& color) const override{
            color *= tex_pointer->at(hit_record.uv.u, hit_record.uv.v);
            mat_pointer->interactWithLight(ray, hit_record, color);
        }
        private:
        mat_ptr mat_pointer;
        tex_ptr tex_pointer;
    };

    typedef TexturedMat<RefractionMat, SolidColorTexture> ColoredRefractionMat;
    typedef TexturedMat<DiffuseMat, ImgTexture> ImgTextureDiffuseMat;
    //TODO: Will this cause wierd (black) color if ray is scattered many times in side the bound ?
    typedef TexturedMat<RandomScatterMat, SolidColorTexture> ColoredRandomScatterMat;

}

#endif