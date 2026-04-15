#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <cstdlib>  

// Common Headers
#include "color.h"
#include "ray.h"
#include "vec3.h"

// C++ Std Usings

using std::make_shared;
using std::shared_ptr;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    // [0,1) random number
    return std::rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    // [min,max)random number
    return min + (max - min) * random_double();
}

inline vec3 random(double min, double max){
    return vec3(random_double(min,max), random_double(min,max), random_double(min,max));
}

inline vec3 random_in_unit_sphere(){
    vec3 trail=random(-1,1);
    while(trail.length_squared()>1 || trail.length_squared()<1e-160){
        trail=random(-1,1);
    }
    return trail;
}

inline vec3 random_unit_vector(){
    return unit_vector(random_in_unit_sphere()); //vectors are uniformally distributed in the sphere
}

inline vec3 random_in_hemisphere(const vec3& normal){
    vec3 trail=random_unit_vector();
    if(dot(trail,normal)<0){ //<0 means 
        trail=-1*trail;
    }
    return trail;
}
#endif