#ifndef SPHERE_H
#define SPHERE_H    
#include "hittable.h"

class sphere : public hittable {
public:
    sphere() {}
    sphere(const point3& c, float r) : center(c), radius(std::fmax(0,r)) {}

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        Vec3 oc = center - r.getOrigin();
        auto a = r.getDirection().length() * r.getDirection().length();
        auto h = oc.dot(r.getDirection());
        auto c = oc.length() * oc.length() - radius * radius;
        auto discriminant = h * h - a * c;

        if (discriminant < 0) {
            return false; // no hit
        } 
        auto sqrt_d = std::sqrt(discriminant);
        auto t1 = (h - sqrt_d) / a;
        if(!ray_t.surrounds(t1)) {
            t1 = (h + sqrt_d) / a; // check the second root
            if(!ray_t.surrounds(t1)) {
                return false; // no hit
            }
        }
        rec.t = t1;
        rec.p = r.at(rec.t);
        Vec3 outward_normal = (rec.p - center) / radius; // Normalized normal vector
        rec.set_face_normal(r, outward_normal);
        return true; // hit
    }
private:
    point3 center; // Center of the sphere
    float radius;  // Radius of the sphere
};

#endif // SPHERE_H

