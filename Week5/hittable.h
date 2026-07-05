
#ifndef HITTABLE_H
#define HITTABLE_H

class material; // Forward declaration

class hit_record {
public:
    point3 p;        
    Vec3 normal;    
    float t; 
    double u; // Texture coordinate u
    double v; // Texture coordinate v
    shared_ptr<material> mat_ptr; // Pointer to the material of the hit object
    bool front_face;  
    void set_face_normal(const ray& r, const Vec3& outward_normal) {
        front_face = r.getDirection().dot(outward_normal) < 0;
        if(front_face) {
            normal = outward_normal;
        } else {
            normal = -outward_normal; 
        }
    }     
};

class hittable {
public:
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
    virtual ~hittable() = default;
};
#endif // HITTABLE_H