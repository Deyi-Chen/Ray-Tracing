#ifndef COLOR_H
#define COLOR_H
#include <iostream>
#include "vec3.h"
#include "rtweekend.h"
#include <algorithm>
using color = vec3;
double linear_to_gamma(double c){
    if(c>0){
        return sqrt(c);
    }
    return 0;
    
}

void write_color(std::ostream &out, const color &pixel_color)
{
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    r=linear_to_gamma(r);
    g=linear_to_gamma(g);
    b=linear_to_gamma(b);

    r = std::clamp(r, 0.0, 0.999);
    g = std::clamp(g, 0.0, 0.999);
    b = std::clamp(b, 0.0, 0.999);

    int ir = int(255.999 * r);
    int ig = int(255.999 *g);
    int ib = int(255.999 * b);

    out << ir << ' ' << ig << ' ' << ib << '\n';
}

#endif