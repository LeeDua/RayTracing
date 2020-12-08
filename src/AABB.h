#ifndef AABB_H
#define AABB_H

#include "Vec3.h"
#include "Ray.h"

namespace RayTracing{
    class AABB: IHittable{
        public:
        Pt3 p1,p2;        
        AABB(){}
        AABB(Pt3 p1, Pt3 p2):p1(p1),p2(p2){}
        bool hit(const Ray& ray, HitRecord& record, dtype minDist, dtype maxDist) const override {
            dtype max_tmin = DINF, min_tmax = -DINF;
            for(int i=0; i<3; i++){
                dtype t1 = (p1[i]-ray.origin()[i])/ray.direction()[i];
                dtype t2 = (p2[i]-ray.origin()[i])/ray.direction()[i];
                dtype tmin = std::min(t1, t2);
                dtype tmax = std::max(t1, t2);
                if(tmin > max_tmin)
                    max_tmin = tmin;
                if(tmax < min_tmax)
                    min_tmax = tmax;
            }
            if(min_tmax<max_tmin && max_tmin>minDist && min_tmax<maxDist)
                return true;
            return false;
        }
    };

    AABB join(const AABB& b1, const AABB& b2){
        return AABB(
            Pt3(
                std::min(std::min(b1.p1[0],b1.p2[0]) , std::min(b2.p1[0],b2.p2[0])),
                std::min(std::min(b1.p1[1],b1.p2[1]) , std::min(b2.p1[1],b2.p2[1])),
                std::min(std::min(b1.p1[2],b1.p2[2]) , std::min(b2.p1[2],b2.p2[2]))                
            ),
            Pt3(
                std::max(std::max(b1.p1[0],b1.p2[0]) , std::max(b2.p1[0],b2.p2[0])),
                std::max(std::max(b1.p1[1],b1.p2[1]) , std::max(b2.p1[1],b2.p2[1])),
                std::max(std::max(b1.p1[2],b1.p2[2]) , std::max(b2.p1[2],b2.p2[2]))
            )
        );
    }

    class IHasBox{
        public:
        AABB box;
        private:
        virtual void constructBox() = 0;
    };

}

#endif