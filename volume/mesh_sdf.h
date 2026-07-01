#ifndef MESH_SDF_H
#define MESH_SDF_H

#include "volume_mesh.h"
#include "voxel_grid.h"
#include "geometry_utils.h"
#include "rtweekend.h"
#include "ray.h"
#include <vector>
#include <omp.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>

inline int intersection_times(const vec3 &p, const Volume_mesh &mesh, const vec3 &direction);
inline bool is_inside(int count);

inline void build_signed_distance_grid(const Volume_mesh &mesh, VoxelGrid &grid)
{
    int nx = grid.nx;
    int ny = grid.ny;
    int nz = grid.nz;
    double min_distance = std::numeric_limits<double>::infinity();
    double max_distance = -std::numeric_limits<double>::infinity();
    long long finite_count = 0;
    long long nan_count = 0;
    long long infinity_count = 0;
    long long negative_count = 0;
    long long positive_count = 0;
    const vec3 direction = unit_vector(vec3(1.0, 0.237, 0.123));
#pragma omp parallel for collapse(3) schedule(static)      \
    reduction(min : min_distance)                          \
    reduction(max : max_distance)                          \
    reduction(+ : finite_count, nan_count, infinity_count) \
    reduction(+ : negative_count, positive_count)
    for (int k = 0; k < nz; k++)
    {
        for (int j = 0; j < ny; j++)
        {
            for (int i = 0; i < nx; i++)
            {
                auto p = grid.voxel_center(i, j, k);
                auto d = point_to_mesh_unsigned_d(p, mesh);
                int count = intersection_times(p, mesh, direction);
                if (is_inside(count))
                {
                    d = -d; // inside is negative
                }
                grid.set_value(i, j, k, d);
                if (std::isnan(d))
                {
                    ++nan_count;
                }
                else if (std::isinf(d))
                {
                    ++infinity_count;
                }
                else
                {
                    ++finite_count;

                    min_distance =
                        std::min(min_distance, d);

                    max_distance =
                        std::max(max_distance, d);
                    if (d < 0.0)
                    {
                        ++negative_count;
                    }
                    else
                    {
                        ++positive_count;
                    }
                }
            }
        }
    }
    std::cerr
        << "\nSigned distance grid completed"
        << "\nResolution: "
        << nx << " x " << ny << " x " << nz
        << "\nFinite values: " << finite_count
        << "\nNaN values: " << nan_count
        << "\nInfinity values: " << infinity_count
        << "\nNegative values: " << negative_count
        << "\nPositive values: " << positive_count
        << "\nMinimum SDF: " << min_distance
        << "\nMaximum SDF: " << max_distance
        << '\n';
}

inline int intersection_times(const vec3 &p, const Volume_mesh &mesh, const vec3 &direction)
{
    Ray ray{p, direction};
    int count = 0;
    for (const std::vector<int> &face : mesh.faces)
    {
        const vec3 &a = mesh.vertices[face[0]];
        const vec3 &b = mesh.vertices[face[1]];
        const vec3 &c = mesh.vertices[face[2]];
        if (ray_hit_triangle(ray, a, b, c))
        {
            count++;
        }
    }
    return count;
}

inline bool is_inside(int count)
{
    if (count % 2 == 0)
    {
        return false;
    }
    return true;
}

inline void print_slice(const VoxelGrid &grid,int number)
{
    for (int j = grid.ny - 1; j >= 0; --j)
    {
        for (int i = 0; i < grid.nx; ++i)
        {
            double d = grid.get_value(i, j, number);

            if (d < 0.0)
            {
                std::cerr << '#';
            }
            else
            {
                std::cerr << '.';
            }
        }
        std::cerr << '\n';
    }
}

#endif