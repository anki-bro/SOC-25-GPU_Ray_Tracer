#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vec3.h"

inline double linear_to_gamma(double value) {
    if(value > 0){
        return std::pow(value, 1.0 / 2.2);
    } 
    return 0;
}

void write_color(std::ostream &out, const color3 &pixel_color) {

    float r = pixel_color.x();
    float g = pixel_color.y() ;
    float b = pixel_color.z() ;

    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // Clamp the color values to the range [0, 1]
    static const interval color_range(0.000f, 0.999f);
    int rbyte = int(255.999 * color_range._clamp(r));
    int gbyte = int(255.999 * color_range._clamp(g));   
    int bbyte = int(255.999 * color_range._clamp(b));

    out << rbyte << ' '
        << gbyte << ' '
        << bbyte << '\n';
}

#endif // COLOR_H