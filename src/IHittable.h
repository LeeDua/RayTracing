#ifndef IHITTABLE_H
#define IHITTABLE_H

#include<memory>

#include "Common.h"
#include "Vec3.h"


namespace RayTracing{
    class Ray;
    class IMaterial;
      
    struct HitRecord{
        Pt3 p;
        //n should be mannually normalized when set!
        Vec3 n;
        dtype t;
        //if normal and ray is in the same dir, ray is inside obj and normal direction should be inverted, the hit should be marked as normAgainstRay=false
        bool normAgainstRay=true;
        std::shared_ptr<IMaterial> material=nullptr;
    };

    class IHittable{
        public:
        virtual bool hit(const Ray& ray, HitRecord& record, dtype minDist, dtype maxDist) const = 0;
    };
}



#endif