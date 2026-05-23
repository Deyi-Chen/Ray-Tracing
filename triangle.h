#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "vec3.h"
#include "hittable.h"
#include "ray.h"

class triangle : public hittable
{
public:
    vec3 v0, v1, v2;

    shared_ptr<material> mat;

    triangle(
        vec3 a,
        vec3 b,
        vec3 c,
        shared_ptr<material> m) : v0(a), v1(b), v2(c), mat(m) {}

    virtual bool hit(const Ray &r, double t_min, double t_max, hit_record &rec) const
    {
        auto o = r.origin;
        auto dir = r.direction;
        auto e1 = v1 - v0;
        auto e2 = v2 - v0;
        auto t_dis = o - v0;
        auto pvec = cross(dir, e2);
        auto det = dot(e1, pvec);
        if (fabs(det) < 1e-8)
        {
            return false;
        }
        auto u = dot(t_dis, pvec) / det;
        auto qvec = cross(t_dis, e1);
        auto v = dot(dir, qvec) / det;
        if (u < 0.0 || u > 1.0)
        {
            return false;
        }
        if (v < 0.0 || u + v > 1.0)
        {
            return false;
        }
        auto t = dot(e2, qvec) / det;
        if (t < t_min || t > t_max)
        {
            return false;
        }
        rec.t = t;
        rec.p = r.at(t);
        vec3 outward_normal = unit_vector(cross(e1, e2));
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;
        return true;
    }
    virtual aabb bounding_box() const override
    {
        vec3 small(
            std::min(v0.x(), std::min(v1.x(), v2.x())),
            std::min(v0.y(), std::min(v1.y(), v2.y())),
            std::min(v0.z(), std::min(v1.z(), v2.z())));

        vec3 big(
            std::max(v0.x(), std::max(v1.x(), v2.x())),
            std::max(v0.y(), std::max(v1.y(), v2.y())),
            std::max(v0.z(), std::max(v1.z(), v2.z())));

        const double delta = 0.0001;

        small += -1 * (vec3(delta, delta, delta));
        big += vec3(delta, delta, delta);

        return aabb(small, big);
    }
};

#endif