
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "IHittable.h"
#include "Vec3.h"

namespace RayTracing {
class Sphere : public IHittable {
   public:
    Pt3 center;
    dtype radius;

    explicit Sphere(Pt3 c, dtype r):center(c),radius(r){};

    bool hit(const Ray& ray, HitRecord& record) const {
        Vec3 oToC = ray.origin() - center;
        dtype a = ray.direction().square_sum();
        dtype b = 2.0 * dot(oToC, ray.direction());
        dtype c = oToC.square_sum() - radius * radius;
        dtype discriminant = b * b - 4 * a * c;
        if (discriminant > 0) {
            record.t = (-b - sqrt(discriminant)) / (2.0 * a);
            record.p = ray.origin() + record.t * ray.direction();
            ASSERT(abs((record.p-center).square_sum()-radius*radius)<0.01);
            record.n = (record.p - center).normalized();
            if(record.n.dot(ray.direction())<0){
                record.normAgainstRay = false;
                record.n *= -1.0;
            }
            return true;
        }
        record.t = -1.0;
        return false;
    }
    
};

}  // namespace RayTracing


#endif