#ifndef CONFIG_H
#define CONFIG_H

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>

using std::shared_ptr;
using std::make_shared;

const float infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline float degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}
inline double random_double() {
    return std::rand() / (RAND_MAX + 1.0);
}
inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

#include "vec3.h"
#include "ray.h"
#include "color.h"
#include "interval.h"

#endif // CONFIG_H