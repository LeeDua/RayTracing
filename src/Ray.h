#ifndef RAY_H

#define RAY_H

#include "Vec3.h"

namespace RayTracing{
    class Ray{
        public:
        Ray(){};
        Ray(Pt3 ori, Dir3 dir):
            _origin(ori), _direction(dir.normalized()){};
        
        Pt3 origin() const { return _origin;}
        Dir3 direction() const { return _direction;}
        void set(const Pt3& ori, const Dir3& dir){ _origin = ori; _direction = dir.normalized(); }

        Vec3 at(multi_type t) const {
            return _origin + t * _direction;
        }

        int updateDepthCounter(){ 
            ASSERT( recursiveDepth > 0 );
            recursiveDepth -= 1;
            return recursiveDepth; 
        }
        void die(){ recursiveDepth = 0; }
        bool isAlive(){return (recursiveDepth != 0); }
            
        
        private:
        Pt3 _origin;
        Dir3 _direction;
        int recursiveDepth = MAX_RECURSIVE_DEPTH;
    };
    
}

#endif