
#include "RayTracer.h"

namespace RayTracing
{
    bool RayTracer::recursiveTrace(Ray& ray, HitRecord& hit_record, MatColor& color, const HittableVec& scene){
        bool hit_once = false;
        if(scene.hit(ray, hit_record)){
            hit_once = true;
            hit_record.material->interactWithLight(ray, hit_record, color);
            if(ray.isAlive()){
                return (recursiveTrace(ray, hit_record, color, scene) || hit_once);
            }
        }
        return hit_once;
    }
} // namespace RayTracing
