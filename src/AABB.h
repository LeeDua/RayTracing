#ifndef AABB_H
#define AABB_H

#include "Vec3.h"

namespace RayTracing{
    class AABB{
        public:
        Pt3 p1,p2;        
        AABB(){}
        AABB(Pt3 p1, Pt3 p2):p1(p1),p2(p2){}
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
        virtual void constructBox() = 0;
    };

}

#endif