#ifndef HITTABLEVEC_H
#define HITTABLEVEC_H

#include<vector>
#include<memory>
#include "IHittable.h"

namespace RayTracing{
    typedef std::shared_ptr<IHittable> obj_ptr;
    // class HittableVec:public IHittable, public virtual IHasBox
    class HittableVec:public IHittable
    {    
    public:
        std::vector<obj_ptr> objs;
        HittableVec(){};
        void push(obj_ptr ptr){objs.push_back(ptr);}
        void clear(){ objs.clear(); }
        int size() { return objs.size(); }

        //ToDO: Should ret vector record ?
        bool hit(const Ray& ray, HitRecord& record, dtype minDist=0.0001, dtype maxDist=DINF) const override{
            bool hit_once = false;
            for(const obj_ptr& optr: objs){
                if(optr->hit(ray, record, minDist, maxDist)){
                    hit_once = true;
                    maxDist = record.t;
                }
            }
            return hit_once;
        };


        // void constructBox() override{
        //     dtype minx = -DINF, miny=-DINF, minz=DINF, maxx=DINF, maxy=DINF, maxz=DINF;
        //     for(const obj_ptr& optr: objs){
        //         // if(obj_ptr->box)
        //     }
        // }
    };
}

#endif