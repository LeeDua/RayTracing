
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <memory>

#include "IHittable.h"
#include "Vec3.h"
#include "Ray.h"

namespace RayTracing {

class IMaterial;
typedef std::shared_ptr<IMaterial> mat_ptr;

class Geometry: public IHittable{
    public:
        mat_ptr material=nullptr;
};

class Sphere : public Geometry {
   public:
    Pt3 center;
    dtype radius;

    explicit Sphere(Pt3 c, dtype r):center(c),radius(r){};

    bool hit(const Ray& ray, HitRecord& record, dtype minDist=0.0, dtype maxDist=std::numeric_limits<dtype>::infinity()) const override {
        Vec3 oToC = ray.origin() - center;
        dtype a = ray.direction().square_sum();
        dtype b = 2.0 * dot(oToC, ray.direction());
        // dtype half_b = dot(oToC, ray.direction());
        dtype c = oToC.square_sum() - radius * radius;
        dtype discriminant = b * b - 4 * a * c;
        // dtype discriminant = half_b * half_b - a * c;
        if (discriminant > 0) {
            record.t = (-b - sqrt(discriminant)) / (2.0 * a);
            // record.t = ( (-half_b - sqrt(discriminant)) / a);
            if ( record.t < minDist){
                record.t = (-b + sqrt(discriminant)) / (2.0 * a);
            }
            if( ! (record.t < maxDist && record.t > minDist) ){
                // if(record.t < minDist){
                //     std::cout << "DIST TOO SMALL !!!" << std::endl;
                // }
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
            return true;
        }
        return false;
    }
    
};

}  // namespace RayTracing


#endif