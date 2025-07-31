#ifndef COLOR_H
#define COLOR_H

#include "config.h"

void write_color(std::ostream &out, const color3 &pixel_color) {

    float r = pixel_color.x();
    float g = pixel_color.y() ;
    float b = pixel_color.z() ;

    // Clamp the color values to the range [0, 1]
    int rbyte = int(255.999 * r);
    int gbyte = int(255.999 * g);   
    int bbyte = int(255.999 * b);

    out << rbyte << ' '
        << gbyte << ' '
        << bbyte << '\n';
}

#endif // COLOR_H