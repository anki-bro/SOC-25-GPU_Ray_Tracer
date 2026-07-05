#ifndef SPHERE_H
#define SPHERE_H    
#include "hittable.h"

class sphere : public hittable {
public:
    sphere() {}
    // Stationary Sphere
    sphere(const point3& static_center, double radius, shared_ptr<material> mat)
      : center(static_center, Vec3(0,0,0)), radius(std::fmax(0,radius)), mat(mat) {}

    // Moving Sphere
    sphere(const point3& center1, const point3& center2, double radius,
           shared_ptr<material> mat)
      : center(center1, center2 - center1), radius(std::fmax(0,radius)), mat(mat) {}

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        point3 current_center = center.at(r.time());
        Vec3 oc = current_center - r.getOrigin();
        auto a = r.getDirection().length_squared();
        auto h = oc.dot(r.getDirection());
        auto c = oc.length_squared() - radius * radius;
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
        Vec3 outward_normal = (rec.p - current_center) / radius; // Normalized normal vector
        rec.set_face_normal(r, outward_normal);
        get_sphere_uv(outward_normal, rec.u, rec.v); // compute texture coordinates
        rec.mat_ptr = mat; // Set material pointer to nullptr or assign a material if needed
        return true; // hit
    }
private:
    ray center; // Center of the sphere
    double radius;  // Radius of the sphere
    shared_ptr<material> mat; // Pointer to the material of the sphere

    static void get_sphere_uv(const point3& p, double& u, double& v) {
        auto theta = std::acos(-p.y());
        auto phi = std::atan2(-p.z(), p.x()) + pi;
        u = phi / (2 * pi);
        v = theta / pi;
    }
};

#endif // SPHERE_H

