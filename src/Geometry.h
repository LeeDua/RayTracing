
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>

#include "IHittable.h"
#include "AABB.h"
#include "Vec3.h"
#include "Ray.h"
#include "Texture.h"

namespace RayTracing {

    class IMaterial;
    typedef std::shared_ptr<IMaterial> mat_ptr;
    
    class IGeometry: public virtual IHittable, public virtual IHasBox{  
        public:
            mat_ptr material=nullptr;
            virtual TexCoor getUV(const Pt3& pt) const = 0;
    };

    class Sphere : public IGeometry{
    public:
        Pt3 center;
        dtype radius;

        explicit Sphere(Pt3 c, dtype r):center(c),radius(r){
            ASSERT(radius>0);
            constructBox();
        };

        bool hit(const Ray& ray, HitRecord& record, dtype minDist=MIN_HIT_DIST, dtype maxDist=DINF) const override {
            #ifdef DEBUG
            sphere_count ++;
            #endif
            Vec3 oToC = ray.origin() - center;
            dtype a = ray.direction().square_sum();
            dtype b = 2.0 * dot(oToC, ray.direction());
            dtype c = oToC.square_sum() - radius * radius;
            dtype discriminant = b * b - 4 * a * c;
            if (discriminant > 0) {
                dtype t = (-b - sqrt(discriminant)) / (2.0 * a);
                if ( t < minDist){
                    t = (-b + sqrt(discriminant)) / (2.0 * a);
                }
                // Allow same maxDist for multiple hit check on the same obj (e.g boundary check for inside fog)
                if( ! ( t < maxDist && t > minDist) ){
                    return false;
                }
                // only write record here to avoid dirty write
                record.t = t;
                // record.p = ray.origin() + record.t * ray.direction();
                record.p = ray.at(record.t);
                ASSERT(abs((record.p-center).square_sum()-radius*radius)<0.01);
                record.n = (record.p - center).normalized();
                if(record.n.dot(ray.direction())>0){
                    record.normAgainstRay = false;
                    record.n *= -1.0;
                }
                ASSERT(material != nullptr);
                record.material = material;
                record.uv = getUV(record.n);
                return true;
            }
            return false;
        }

        void constructBox() override{
            box = AABB(
                center - Vec3(radius, radius, radius),
                center + Vec3(radius, radius, radius)
            );
        }

        TexCoor getUV(const Pt3& pt) const override{
            dtype phi = std::atan2(-pt[2],pt[0]) + PI;
            dtype theta = std::acos(-pt[1]);
            return TexCoor{phi/(2.0*PI),theta/PI};
        } 
    };

    class ParticipatingMedia: public IGeometry{
        typedef std::shared_ptr<IGeometry> bound_ptr;
        public:
        ParticipatingMedia(bound_ptr bound, dtype density):bound(bound), inverse_density(-1.0/density){
            constructBox();
        }
        bool hit(const Ray& ray, HitRecord& record, dtype minDist=MIN_HIT_DIST, dtype maxDist=DINF) const override {
            HitRecord boundary_hit;
            // should be inside boundary, NOTE DINF is used as maxDist because we are only testing whether the origin of ray is inside the boundary
            bool inside_boundary = bound->hit(ray, boundary_hit, minDist, DINF);
            if(!inside_boundary) return false;
            dtype dist = inverse_density * log(rand_double());
            // ASSUMING DIST IS SMALL AND WILL NOT CAUSE THE RAY HIT THE BOUND MORE THAN ONCE
            // fail to travel through, scatter in all dir
            // Check dist <= previous nearest hit, allowing equal because the ray may hit the boundary before
            // if other objs are nearer than dist, the following hit check after particapating media will overide hit record, so it's totally fine!
            if( dist < boundary_hit.t && (dist < maxDist || abs(dist-maxDist)<EPSILON_MORE)){
                record.t = dist;    
                // Normal dir does not matter
                ASSERT( material != nullptr );
                record.material = material;
                // record.p = ray.origin() + record.t * ray.direction();
                record.p = ray.at(record.t);
                return true;
            }
            //simply do nothing to the passed in record and let other objs in scene to test hit            
            return false;
        }
        void constructBox() override{
            box = bound->box;
        }
        TexCoor getUV(const Pt3& pt) const override{
            return TexCoor{0.0,0.0};
        }
        private:
        bound_ptr bound;
        dtype inverse_density;
    };

    class AxisAlignedRect: private AABB, public IGeometry{
        public:
        explicit AxisAlignedRect(Pt3 _p1, Pt3 _p2, int axis, dtype thick=1e-3):axis(axis){
            if( abs(_p1[axis] - _p2[axis]) > EPSILON){
                std::cerr << "Illegal usage of AxisAlignedRect axis constructor, p1 and p2 should match on the set axis" << std::endl;
                abort();
            }
            AABB::p1 = _p1;
            AABB::p2 = _p2;
            AABB::p1[axis] -= thick;
            AABB::p2[axis] += thick;
            AxisAlignedRect::AABB(AABB::p1, AABB::p2);
            constructBox();
        }
        bool hit(const Ray& ray, HitRecord& record, dtype minDist=MIN_HIT_DIST, dtype maxDist=DINF) const override {
            dtype max_tmin,min_tmax;
            AABB::checkBoxHit(ray, max_tmin, min_tmax);
            if(min_tmax < max_tmin)
                return false;
            if(min_tmax < minDist || max_tmin > maxDist)
                return false;
            if(max_tmin < minDist)
                record.t = min_tmax;
            else
                record.t = max_tmin;
            // record.p = record.t * ray.direction() + ray.origin();
            record.p = ray.at(record.t);
            ASSERT( material != nullptr );
            record.material = material;
            Pt3 mid = (box.p1 + box.p2) / 2;
            record.n = Dir3();
            record.n[axis] = record.p[axis]-mid[axis];
            record.n.normalize();
            if(record.n.dot(ray.direction())>0){
                record.normAgainstRay = false;
                record.n *= -1.0;
            }
            #ifdef DEBUG
            rect_hit_count ++;
            #endif            
            return true;
        }
        void constructBox() override{
            box = AABB( p1, p2 );
        }
        TexCoor getUV(const Pt3& pt) const override{
            return TexCoor{0.0,0.0};
        }

        private:
        int axis;
    };

}  // namespace RayTracing


#endif