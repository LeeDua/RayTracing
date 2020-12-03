#ifndef HITTABLEVEC_H
#define HITTABLEVEC_H

#include<vector>
#include<memory>
#include "IHittable.h"

namespace RayTracing{
    class HittableVec:public IHittable
    {    
    typedef std::shared_ptr<IHittable> obj_ptr;
    public:
        std::vector<obj_ptr> objs;
        HittableVec(){};
        void push(obj_ptr ptr){objs.push_back(ptr);}
        void clear(){ objs.clear(); }

        //ToDO: Should ret vector record ?
        virtual bool hit(const Ray& ray, HitRecord& record) const override{
            bool hit_once = false;
            for(const obj_ptr& optr: objs){
                if(optr->hit(ray, record)){
                    hit_once = true;
                }
            }
            return hit_once;
        };        
    };
}

#endif