
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

        bool hit(const Ray& ray, HitRecord& record, dtype minDist=0.0, dtype maxDist=DINF) const override {
            #ifdef DEBUG
            sphere_count ++;
            #endif
            Vec3 oToC = ray.origin() - center;
            dtype a = ray.direction().square_sum();
            dtype b = 2.0 * dot(oToC, ray.direction());
            dtype c = oToC.square_sum() - radius * radius;
            dtype discriminant = b * b - 4 * a * c;
            if (discriminant > 0) {
                record.t = (-b - sqrt(discriminant)) / (2.0 * a);
                if ( record.t < minDist){
                    record.t = (-b + sqrt(discriminant)) / (2.0 * a);
                }
                if( ! (record.t < maxDist && record.t > minDist) ){
                    return false;
                }
                record.p = ray.origin() + record.t * ray.direction();
                ASSERT(abs((record.p-center).square_sum()-radius*radius)<0.01);
                record.n = (record.p - center).normalized();
                if(record.n.dot(ray.direction())>0){
                    record.normAgainstRay = false;
                    record.n *= -1.0;
                }
                ASSERT(material != nullptr);
                record.material = material;
                record.uv = getUV(record.p);
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
        ParticipatingMedia(bound_ptr bound, dtype density):bound(bound), inverse_density(-1.0/density){
            constructBox();
        }
        bool hit(const Ray& ray, HitRecord& record, dtype minDist=0.0, dtype maxDist=DINF) const override {
            bound->hit(ray, record, minDist, maxDist);
            dtype dist = inverse_density * log(rand_double());
            // ASSUMING DIST IS SMALL AND WILL NOT CAUSE THE RAY HIT THE BOUND MORE THAN ONCE
            // fail to travel through, scatter in all dir
            if(record.t > dist){
                record.t = dist;
                record.p = ray.origin() + dist * ray.direction();
                ASSERT( material != nullptr );
                record.material = material;
                // Normal dir does not matter
            }
            //simply travel through to the bound hit point
            return true;
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
        explicit AxisAlignedRect(Pt3 p1, Pt3 p2, int axis, dtype thick=1e-3):axis(axis){
            if( abs(p1[axis] - p2[axis]) > EPSILON){
                std::cerr << "Illegal usage of AxisAlignedRect axis constructor, p1 and p2 should match on the set axis" << std::endl;
                abort();
            }
            p1[axis] -= thick;
            p2[axis] += thick;
            box = AABB(p1, p2);
        }
        bool hit(const Ray& ray, HitRecord& record, dtype minDist=0.0, dtype maxDist=DINF) const override {
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
            record.p = record.t * ray.direction() + ray.origin();
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