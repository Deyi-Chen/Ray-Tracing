#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "ray.h"
#include "vec3.h"

class Sphere : public hittable
{
public:
    Sphere(const vec3 &c, double r)
    {
        center = c;
        radius = r;
    }
    virtual bool hit(const Ray &r, double t_min, double t_max, hit_record &rec) const override
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
        auto t = (-b - sqrt(delta)) / (2 * a);
        //if t is too small or there is a smaller t for some closer object
        if (t < t_min || t > t_max)
        {
            t=(-b + sqrt(delta)) / (2 * a);
            if (t < t_min || t > t_max){
                return false;
            }
        }
        rec.p = r.at(t);
        rec.t = t;
        rec.normal = unit_vector(rec.p - center);
        return true;
    }

private:
    vec3 center;
    double radius;
};

#endif