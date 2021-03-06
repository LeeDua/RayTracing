#ifndef RAY_H
#define RAY_H

#include "Vec.h"

namespace RayTracing{
    class Ray{
        public:
        Ray(){};
        Ray(Pt3 ori, Dir3 dir):
            _origin(ori), _direction(dir.normalized()){};
        
        inline Pt3 origin() const { return _origin;}
        inline Dir3 direction() const { return _direction;}
        void set(const Pt3& ori, const Dir3& dir){ _origin = ori; _direction = dir.normalized(); }

        Vec3 at(multi_type t) const {
            return _origin + t * _direction;
        }

        int updateDepthCounter(){ 
            recursiveDepth -= 1;
            return recursiveDepth; 
        }
        void die(){ recursiveDepth = 0; hit_once = true; }
        bool isAlive(){return (recursiveDepth > 0); }
            
        public:
        bool hit_once = false;

        private:
        Pt3 _origin;
        Dir3 _direction;
        int recursiveDepth = MAX_RECURSIVE_DEPTH;
    };
    
}

#endif