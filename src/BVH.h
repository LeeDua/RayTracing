#ifndef BVH_H
#define BVH_H

#include <variant>
#include "HittableVec.h"
#include "AABB.h"

namespace RayTracing{

    class BVHLeaf;
    class BVHBase: public virtual IHittable{};
    class BVHNode: public BVHBase, public virtual IHasBox{
        public:
        BVHNode(const HittableVec& objs){
            ASSERT( (objs.box.p1- objs.box.p2).length() > 0.0001 );
            box = objs.box;
            HittableVec left_objs;
            HittableVec right_objs;
            split(objs, left_objs, right_objs);
            left_objs.init();
            right_objs.init();
            // std::cout << "Current node size: " << objs.size() << " ; child size: " << left_objs.size() << " " << right_objs.size() << std::endl;
            ASSERT(left_objs.size() > 0 && right_objs.size() > 0);
            if(left_objs.size() > max_objs)
                left = std::static_pointer_cast<BVHBase>(std::make_shared<BVHNode>(left_objs));
            else
                left = std::static_pointer_cast<BVHBase>(std::make_shared<BVHLeaf>(std::move(left_objs)));
            if(right_objs.size() > max_objs)
                right = std::static_pointer_cast<BVHBase>(std::make_shared<BVHNode>(right_objs));
            else
                right = std::static_pointer_cast<BVHBase>(std::make_shared<BVHLeaf>(std::move(right_objs)));
        }

        bool hit(const Ray& ray, HitRecord& record, dtype minDist=0.0001, dtype maxDist=DINF) const override {
            #ifdef DEBUG
            node_count ++;
            #endif
            // ASSERT(left.holds_alternative() && right.holds_alternative());
            if(box.hit(ray,record, minDist, maxDist)){
                //should traverse both left and right to deal with multiple ray-obj collision
                bool hit_left = left->hit(ray, record, minDist, maxDist);
                if(hit_left)
                    maxDist = record.t;                
                bool hit_right = right->hit(ray, record, minDist, maxDist);
                return ( hit_left || hit_right );
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
                }
            }
            mid = std::min(box.p1[axis], box.p2[axis]) + minmax / 2;
            for(const obj_ptr& optr: original_list.objs){
                if( (optr->box.p1[axis]+optr->box.p2[axis])/2 < mid)
                    left_list.push(optr);
                else
                    right_list.push(optr);
            }
            // deal with zero length list because theres one obj containing all the others
            if(left_list.size() == 0 || right_list.size() == 0){
                left_list.clear();
                right_list.clear();
                auto mid_iter = original_list.objs.begin() + original_list.size() / 2;
                left_list.objs.resize(original_list.size()/2);
                right_list.objs.resize(original_list.size() - original_list.size()/2);
                std::copy(original_list.objs.begin(), mid_iter, left_list.objs.begin());
                std::copy(mid_iter, original_list.objs.end(), right_list.objs.begin());
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

    class BVHLeaf: public BVHBase{
        public:
        BVHLeaf(HittableVec&& objs):objs(objs){
            // std::cout << "LEAF NODE: " << objs.size() << std::endl;
        }
        bool hit(const Ray& ray, HitRecord& record, dtype minDist, dtype maxDist) const override {
            #ifdef DEBUG
            leaf_count++;
            #endif
            if(objs.objs.size() == 1)
                return objs.objs[0]->hit(ray, record, minDist, maxDist);
            return objs.hit(ray, record, minDist, maxDist);            
        }
        
        private:
        HittableVec objs;
    };

    typedef BVHNode BVHTree;

}



#endif