#ifndef VOLUME_SPHERE_H
#define VOLUME_SPHERE_H

#include "../vec3.h"
#include "../ray.h"
class VolumeSphere
{
public:
    vec3 color;
    double sigma_a;
    double sigma_s;
    VolumeSphere(vec3 c, double r, vec3 clr, double sigma_aa,double sigma_ss)
        :color(clr),radius(r),sigma_a(sigma_aa),center(c),sigma_s(sigma_ss){}
    bool hit(const Ray&r, double &t0, double& t1)const
    {
        auto oc = center - r.origin;
        auto d = r.direction;
        auto a = dot(d, d);
        auto b = -2 * dot(d, oc);
        auto c = dot(oc, oc) - radius * radius;
        // the delta is a known number
        auto delta = b * b - 4 * a * c;
        if (delta < 0)
        {
            return false;
        }
        t0 = (-b - sqrt(delta)) / (2 * a);
        t1=(-b + sqrt(delta)) / (2 * a);
        if(t0>t1){
            std::swap(t0,t1);
        }
        if(t1<0){
            return false;
        }
        if(t0<0){
            t0=0;
        }
        return true;
    }

private:
    vec3 center;
    double radius;


};

#endif