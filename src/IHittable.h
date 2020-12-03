#ifndef IHITTABLE_H
#define IHITTABLE_H

#include "Common.h"
#include "Vec3.h"
#include "Ray.h"

namespace RayTracing{
    struct HitRecord{
        Pt3 p;
        Vec3 n;
        dtype t;
        //if in normal and ray is in the same dir, ray is inside obj and normal direction should be inverted, the hit should be marked as normAgainstRay=false
        bool normAgainstRay=true;
    };

    class IHittable
    {
    public:
        virtual bool hit(const Ray& ray, HitRecord& record) const = 0;
    };
}



#endif