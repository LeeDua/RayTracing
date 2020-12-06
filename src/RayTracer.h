#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include "HittableVec.h"
#include "Ray.h"
#include "Material.h"

namespace RayTracing{
    class RayTracer{
    public:
        RayTracer(){};
        MatColor trace(Ray& ray, const HittableVec& scene){
            HitRecord hit_record;
            MatColor color(1.0,1.0,1.0);
            bool hit = recursiveTrace(ray, hit_record, color, scene);
            dtype mix = (1.0 + ray.direction()[1])*0.5;
            MatColor mixed_color = (1.0-mix)*MatColor(1.0,1.0,1.0) + mix * MatColor(0.5,0.7,1.0);
            return color * mixed_color;
        }

    private:
        bool recursiveTrace(Ray& ray, HitRecord& hit_record, MatColor& color, const HittableVec& scene);


    };
}

#endif