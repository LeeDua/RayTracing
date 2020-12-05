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
            if(hit)
                return color;
            else
                return MatColor(0.0,0.0,0.0);
        }

    private:
        bool recursiveTrace(Ray& ray, HitRecord& hit_record, MatColor& color, const HittableVec& scene);


    };
}

#endif