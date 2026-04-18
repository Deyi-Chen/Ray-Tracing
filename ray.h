#ifndef RAY_H
#define RAY_H
#include "vec3.h"

class Ray{
    public:
        vec3 origin;
        vec3 direction;
        Ray(){}
        Ray(const vec3 &origin, const vec3 & direction):
            origin(origin),direction(direction)
        {}
        vec3 at(double t)const{
            return origin+t*direction;                                                                      
        }                                           
};

#endif