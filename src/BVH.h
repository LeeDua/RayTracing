#ifndef BVH_H
#define BVH_H

#include <variant>
#include "HittableVec.h"
#include "AABB.h"

namespace RayTracing{

    class BVHLeaf;
    class BVHNode: public IHittable{
        public:
        BVHNode(const HittableVec& objs){
            // box = objs.box;
            HittableVec left_objs;
            HittableVec right_objs;
            split(objs, left_objs, right_objs);
            if(left_objs.size() > max_objs)
                left = std::make_shared<BVHNode>(left_objs);
            else
                left = std::make_shared<BVHLeaf>(std::move(left_objs));
            if(right_objs.size() > max_objs)
                right = std::make_shared<BVHNode>(right_objs);
            else
                right = std::make_shared<BVHLeaf>(std::move(right_objs));
        }

        bool hit(const Ray& ray, HitRecord& record, dtype minDist, dtype maxDist) const override {

            return false;
        }
        

        private:                
        void split(const HittableVec& original_list, HittableVec& left_list, HittableVec& right_list){
            
        }

        private:
        typedef std::variant<std::shared_ptr<BVHNode>, std::shared_ptr<BVHLeaf>> child_ptr;
        AABB box;
        child_ptr left,right;
        int max_objs = 1;  

    };

    class BVHLeaf: public IHittable{
        public:
        BVHLeaf(HittableVec&& objs):objs(objs){}
        bool hit(const Ray& ray, HitRecord& record, dtype minDist, dtype maxDist) const override {

            return false;
        }
        
        private:
        HittableVec objs;
    };

}



#endif