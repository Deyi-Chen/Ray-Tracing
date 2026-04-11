#ifndef HITTABLE_H
#define HITTABLE_H
#include "ray.h"
#include "vec3.h"

class hit_record{
    //return the point of hitting the object, the t in ray, and normal vector
    public:
        vec3 p;
        double t;
        vec3 normal;
        hit_record(){
            p=vec3();
            t=0.0;
            normal=vec3();
        }
};

class hittable{
    public:
        virtual ~hittable(){};
        //pure virtual function
        //i need to know how many objects are hit by a particular ray
        //t_min to prevent t=0, and t_max to record the real value of t(closet object); hit_record to record all the information of one trail
        virtual bool hit(const Ray&r,double t_min, double t_max, hit_record &rec)const=0;
};


#endif 