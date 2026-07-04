#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
public:
    ray() {}

    ray(const point3& o, const Vec3& d, double t) : origin(o), direction(d), tm(t) {}

    ray(const point3& origin, const Vec3& direction)
      : ray(origin, direction, 0) {}

    const point3& getOrigin() const {
        return origin;
    }
    const Vec3& getDirection() const {
        return direction;
    }

    double time() const {
        return tm;
    }
    point3 at(float t) const {
        return origin + direction * t;
    }
private:
    point3 origin;  // Starting point of the ray
    Vec3 direction; // Direction of the ray
    double tm;    // Time associated with the ray
};

#endif // RAY_H