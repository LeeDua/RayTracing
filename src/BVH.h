#ifndef BVH_H
#define BVH_H

#include <variant>
#include "HittableVec.h"
#include "AABB.h"

namespace RayTracing{

    class BVHLeaf;
    class BVHBase: public virtual IHittable{};
    class BVHNode: public virtual BVHBase, public virtual IHasBox{
        public:
        BVHNode(const HittableVec& objs){
            ASSERT( (objs.box.p1- objs.box.p2).length() > 0.0001 );
            box = objs.box;
            HittableVec left_objs;
            HittableVec right_objs;
            split(objs, left_objs, right_objs);
            if(left_objs.size() > max_objs)
                left = std::static_pointer_cast<BVHBase>(std::make_shared<BVHNode>(left_objs));
            else
                left = std::static_pointer_cast<BVHBase>(std::make_shared<BVHLeaf>(std::move(left_objs)));
            if(right_objs.size() > max_objs)
                right = std::static_pointer_cast<BVHBase>(std::make_shared<BVHNode>(right_objs));
            else
                right = std::static_pointer_cast<BVHBase>(std::make_shared<BVHLeaf>(std::move(right_objs)));
        }

        bool hit(const Ray& ray, HitRecord& record, dtype minDist, dtype maxDist) const override {
            // ASSERT(left.holds_alternative() && right.holds_alternative());
            if(box.hit(ray,record, minDist, maxDist)){
                return ( left->hit(ray, record, minDist, maxDist) || right->hit(ray, record, minDist, maxDist));
            }
            return false;
        }

        private:           
        void split(const HittableVec& original_list, HittableVec& left_list, HittableVec& right_list){
            int axis = 0;
            dtype mid;
            dtype minmax = -DINF;
            for(int i=0;i<3;i++){
                dtype axis_minmax = abs(box.p1[i]-box.p2[i]);
                if( axis_minmax > minmax ){
                    minmax = axis_minmax;
                    axis = i;
                    mid = std::min(box.p1[i], box.p2[i]) + minmax / 2;
                }
            }
            for(const obj_ptr& optr: original_list.objs){
                if(optr->mid()[axis] < mid)
                    left_list.push(optr);
                else
                    right_list.push(optr);
            }
        }
        
        // Do not need mannual bbox construct because the box is the same as hittableVec
        void constructBox() override{}

        private:
        typedef std::shared_ptr<BVHBase> child_ptr;
        AABB box;
        child_ptr left,right;
        int max_objs = 1;  

    };

    class BVHLeaf: public virtual BVHBase{
        public:
        BVHLeaf(HittableVec&& objs):objs(objs){}
        bool hit(const Ray& ray, HitRecord& record, dtype minDist, dtype maxDist) const override {
            return objs.hit(ray, record, minDist, maxDist);
        }
        
        private:
        HittableVec objs;
    };

}



#endif