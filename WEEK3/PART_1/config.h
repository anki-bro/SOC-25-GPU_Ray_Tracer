#ifndef CONFIG_H
#define CONFIG_H

#include <cmath>
#include <limits>
#include <memory>
#include <iostream>

using std::shared_ptr;
using std::make_shared;

const float infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline float degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

#include "vec3.h"
#include "ray.h"
#include "color.h"
#include "interval.h"

#endif // CONFIG_H