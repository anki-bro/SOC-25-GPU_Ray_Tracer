#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
public:
    ray() {}
    ray(const point3& o, const Vec3& d) : origin(o), direction(d) {}
    const point3& getOrigin() const {
        return origin;
    }
    const Vec3& getDirection() const {
        return direction;
    }
    point3 at(float t) const {
        return origin + direction * t;
    }
private:
    point3 origin;  // Starting point of the ray
    Vec3 direction; // Direction of the ray
};

#endif // RAY_H