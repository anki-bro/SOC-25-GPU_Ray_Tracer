#ifndef CONFIG_H
#define CONFIG_H

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>
#include <random>

using std::shared_ptr;
using std::make_shared;

const float infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline float degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}
// single thread-local RNG for the translation unit
static thread_local std::mt19937 rng_generator(1);

inline double random_double() {
    static thread_local std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(rng_generator);
}
inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

inline int random_int(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng_generator);
}

#include "vec3.h"
#include "ray.h"
#include "color.h"
#include "interval.h"

#endif // CONFIG_H
