#ifndef HITTABLEVEC_H
#define HITTABLEVEC_H

#include<vector>
#include "IHittable.h"
#include "AABB.h"
#include "Geometry.h"

namespace RayTracing{
    typedef std::shared_ptr<IGeometry> obj_ptr;
    class HittableVec:public IHittable, public virtual IHasBox    
    {    
    public:
        std::vector<obj_ptr> objs;
        HittableVec(){};
        void push(obj_ptr ptr){objs.push_back(ptr);}
        void clear(){ objs.clear(); }
        int size() const { return objs.size(); }

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

        //Be careful! Init has to be called manually after the vec is setup so that bbox can be initialized
        void init(){
            constructBox();
        }

    private:
        void constructBox() override{
            Pt3 minPt(DINF,DINF,DINF);
            Pt3 maxPt(-DINF,-DINF,-DINF);
            for(const obj_ptr& optr: objs){
                for(int i=0;i<3;i++){
                    if(optr->box.p1[i] < minPt[i])
                        minPt[i] = optr->box.p1[i];
                    if(optr->box.p2[i] < minPt[i])
                        minPt[i] = optr->box.p2[i];
                    if(optr->box.p1[i] > maxPt[i])
                        maxPt[i] = optr->box.p1[i];
                    if(optr->box.p2[i] > maxPt[i])
                        maxPt[i] = optr->box.p2[i];
                }
            }
            box = AABB(minPt, maxPt);
        }

    };
}

#endif