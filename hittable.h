#ifndef HITTABLE_H
#define HITTABLE_H
#include "ray.h"
#include "vec3.h"

class material;

class hit_record{
    //return the point of hitting the object, the t in ray, and normal vector
    public:
        vec3 p;
        double t;
        vec3 normal;
        shared_ptr<material> mat;
        bool front_face;
        hit_record(){
            p=vec3();
            t=0.0;
            normal=vec3();
            front_face=true;
        }
        void set_face_normal(const Ray&r,const vec3& outward_normal){
            //if ray comes outside from the sphere, then the dot is negative, front_face is true
            front_face=dot(r.direction,outward_normal)<0;
            if(!front_face){
                //if it comes inside the sphere, reverse the direction of normal
                normal=-1*outward_normal;
            }
            else{
                normal=outward_normal;
            }
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