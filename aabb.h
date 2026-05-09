#ifndef AABB_H
#define AABB_H

#include "vec3.h"
#include "ray.h"

#include <algorithm>

class aabb
{
public:
    vec3 maximum;
    vec3 minimum;

    aabb() {}

    aabb(const vec3 &a, const vec3 &b)
    {
        minimum = a;
        maximum = b;
    }
    aabb(const aabb &box0,
         const aabb &box1)
    {
        minimum = vec3(

            std::min(box0.minimum.x(),
                     box1.minimum.x()),

            std::min(box0.minimum.y(),
                     box1.minimum.y()),

            std::min(box0.minimum.z(),
                     box1.minimum.z()));

        maximum = vec3(

            std::max(box0.maximum.x(),
                     box1.maximum.x()),

            std::max(box0.maximum.y(),
                     box1.maximum.y()),

            std::max(box0.maximum.z(),
                     box1.maximum.z()));
    }
    bool hit(const Ray &r, double t_min, double t_max) const
    {

        for (int i = 0; i < 3; i++)
        {

            double dir = r.direction[i];
            double o = r.origin[i];

            double minv = minimum[i];
            double maxv = maximum[i];

            auto t0x = (minv - o) / dir;
            auto t1x = (maxv - o) / dir;

            if (dir < 0)
            {
                std::swap(t0x, t1x);
            }

            t_min = std::max(t0x, t_min);
            t_max = std::min(t1x, t_max);

            if (t_min >= t_max)
            {
                return false;
            }
        }

        return true;
    }
};

#endif