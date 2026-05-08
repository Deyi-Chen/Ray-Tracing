#ifndef AABB_H
#define AABB_H

#include "vec3.h"
#include "ray.h"
#include <algorithm>

struct AABB{
    vec3 maximum;
    vec3 minimum;
};

bool hit_aabb(const AABB& box, const Ray &r, double t_min, double t_max){
    for (int i=0;i<3;i++){
        double dir=r.direction[i];
        double o=r.origin[i];
        double minv=box.minimum[i];
        double maxv=box.maximum[i];
        auto t0x=(minv-o)/dir;
        auto t1x=(maxv-o)/dir;
        if(dir<0){
            std::swap(t0x,t1x);
        }
        t_min=std::max(t0x,t_min);
        t_max=std::min(t1x,t_max);
        if(t_min>=t_max){
            return false;
        }
    }
    return true;
}

#endif