#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <cstdlib>

#include "color.h"
#include "ray.h"
#include "vec3.h"

using std::make_shared;
using std::shared_ptr;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

// 注意: random_double 已移到 vec3.h 顶部
// 全局 random(min, max) 已删除, 改用 vec3::random(min, max)

inline vec3 random_in_unit_sphere() {
    vec3 trail = vec3::random(-1, 1);
    while (trail.length_squared() > 1 || trail.length_squared() < 1e-160) {
        trail = vec3::random(-1, 1);
    }
    return trail;
}

inline vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

inline vec3 random_in_hemisphere(const vec3& normal) {
    vec3 trail = random_unit_vector();
    if (dot(trail, normal) < 0) {
        trail = -1 * trail;
    }
    return trail;
}

inline vec3 random_in_unit_disk() {
    vec3 result = vec3(random_double(-1, 1), random_double(-1, 1), 0);
    while (result.length() > 1) {
        result = vec3(random_double(-1, 1), random_double(-1, 1), 0);
    }
    return result;
}

#endif