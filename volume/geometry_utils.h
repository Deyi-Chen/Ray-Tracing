#ifndef GEOMETRY_UTILS_H
#define GEOMETRY_UTILS_H

#include "vec3.h"
#include "volume_mesh.h"
#include "ray.h"
#include <limits>
#include <algorithm>

inline double point_to_triangle_d(const vec3 &p, const vec3 &A, const vec3 &B, const vec3 &C)
{
    vec3 AB = B - A;
    vec3 AC = C - A;
    vec3 normal = cross(AB, AC);
    double normal_len2 = dot(normal, normal);
    if (normal_len2 < 1e-12)
    {
        return std::numeric_limits<double>::infinity();
    }
    auto t = dot((p - A), normal) / (dot(normal, normal));
    auto Q = p - t * normal; // Q is the projection point of p onto the triangle plane
    vec3 v0 = AB;
    vec3 v1 = AC;
    vec3 v2 = Q - A;
    auto d00 = dot(v0, v0);
    auto d01 = dot(v0, v1);
    auto d11 = dot(v1, v1);
    auto d20 = dot(v2, v0);
    auto d21 = dot(v2, v1);
    auto D = d00 * d11 - d01 * d01;
    auto u = (d11 * d20 - d01 * d21) / D;
    auto v = (d00 * d21 - d01 * d20) / D;
    constexpr double eps = 1e-10;
    if (u >= -eps && v >= -eps && u + v <= 1.0 + eps)
    {
        // if the projection point inside the triangle
        return (p - Q).length();
    }
    double d = std::numeric_limits<double>::infinity();
    // AB
    double t_ab = dot((p - A), AB) / (AB.length_squared());
    t_ab = std::clamp(t_ab, 0.0, 1.0);
    auto r_ab = A + t_ab * AB;
    auto d_ab = (p - r_ab).length();
    // BC
    vec3 BC = C - B;
    double t_bc = dot((p - B), BC) / (BC.length_squared());
    t_bc = std::clamp(t_bc, 0.0, 1.0);
    auto r_bc = B + t_bc * BC;
    auto d_bc = (p - r_bc).length();
    // CA
    vec3 CA = A - C;
    double t_ca = dot((p - C), CA) / (CA.length_squared());
    t_ca = std::clamp(t_ca, 0.0, 1.0);
    auto r_ca = C + t_ca * CA;
    auto d_ca = (p - r_ca).length();

    d = std::min({d_ab, d_bc, d_ca});
    return d;
}

inline double point_to_mesh_unsigned_d(vec3 p, const Volume_mesh &mesh)
{
    double d = std::numeric_limits<double>::infinity();
    for (const std::vector<int> &face : mesh.faces)
    {
        int a = face[0];
        int b = face[1];
        int c = face[2];
        vec3 v1 = mesh.vertices[a];
        vec3 v2 = mesh.vertices[b];
        vec3 v3 = mesh.vertices[c];
        d=std::min(d,point_to_triangle_d(p,v1,v2,v3));
    }
    return d;
}

inline bool ray_hit_triangle(const Ray &r, const vec3 & v0, const vec3 & v1, const vec3& v2) 
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
        if (u < 0.0 || u > 1.0)
        {
            return false;
        }
        auto qvec = cross(t_dis, e1);
        auto v = dot(dir, qvec) / det;
        if (v < 0.0 || u + v > 1.0)
        {
            return false;
        }
        auto t = dot(e2, qvec) / det;
        constexpr double eps = 1e-10;
        if (t < eps)
        {
            return false;
        }
        return true;
}

#endif 