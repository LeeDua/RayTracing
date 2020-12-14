#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include "HittableVec.h"
#include "Ray.h"
#include "Material.h"
#include "BVH.h"

namespace RayTracing{
    
    class RayTracerBase{
        public:
        virtual MatColor trace(Ray& ray) const = 0;
        private:
        virtual void recursiveTrace(Ray& ray, HitRecord& hit_record, MatColor& color) const = 0;
    };

    class RayTracer: public virtual RayTracerBase{
    public:
        RayTracer(HittableVec& scene):scene(scene){};
        MatColor trace(Ray& ray) const override{
            HitRecord hit_record;
            MatColor color(1.0,1.0,1.0);
            recursiveTrace(ray, hit_record, color);
            // dtype mix = (1.0 + ray.direction()[1])*0.5;
            // MatColor mixed_color = (1.0-mix)*MatColor(1.0,1.0,1.0) + mix * MatColor(0.5,0.7,1.0);
            // return color * mixed_color;
            if(ray.hit_once)
                return color;
            return MatColor();
        }

    private:
        void recursiveTrace(Ray& ray, HitRecord& hit_record, MatColor& color) const override{
            if(scene.hit(ray, hit_record)){
                hit_record.material->interactWithLight(ray, hit_record, color);
                ray.updateDepthCounter();
                if(ray.isAlive()){
                    return recursiveTrace(ray, hit_record, color);
                }
            }
        }
        HittableVec scene;
    };

    class BVHRayTracer: public RayTracer{
        public:
        //has to do post init for bbox, so the scene is not marked const
        BVHRayTracer(HittableVec& scene):RayTracer(scene){
            scene.init();
            tree = std::make_shared<BVHTree>(scene);
        }
        private:
        void recursiveTrace(Ray& ray, HitRecord& hit_record, MatColor& color) const override{
            if(tree->hit(ray, hit_record)){
                hit_record.material->interactWithLight(ray, hit_record, color);
                ray.updateDepthCounter();
                if(ray.isAlive()){
                    recursiveTrace(ray, hit_record, color);
                }             
            }
        }

        private:
        std::shared_ptr<BVHTree> tree;
    };
}

#endif