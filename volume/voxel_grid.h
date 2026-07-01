#ifndef VOXEL_GRID_H
#define VOXEL_GRID_H

#include <vector>
#include "vec3.h"
#include "ray.h"
#include <algorithm>
#include <cmath>
class VoxelGrid
{
public:
    int nx, ny, nz;
    std::vector<double> value;
    vec3 bound_min, bound_max;
    vec3 voxel_size;

    VoxelGrid(int nx, int ny, int nz, const vec3 &bound_min, const vec3 &bound_max) : nx(nx), ny(ny), nz(nz), bound_min(bound_min), bound_max(bound_max)
    {
        double voxel_size_x = (bound_max.x() - bound_min.x()) / nx;
        double voxel_size_y = (bound_max.y() - bound_min.y()) / ny;
        double voxel_size_z = (bound_max.z() - bound_min.z()) / nz;
        voxel_size = vec3{voxel_size_x, voxel_size_y, voxel_size_z};
        value = std::vector<double>(nx * ny * nz, 0.0);
    }
    void set_value(int i, int j, int k, double val)
    {
        int num = index(i, j, k);
        value[num] = val;
    }
    double get_value(int i, int j, int k) const
    {
        int num = index(i, j, k);
        return value[num];
    }
    vec3 voxel_center(int i, int j, int k) const
    {
        // add 0.5 offset to get the center of the voxel
        auto p_x = bound_min.x() + (i + 0.5) * voxel_size.x();
        auto p_y = bound_min.y() + (j + 0.5) * voxel_size.y();
        auto p_z = bound_min.z() + (k + 0.5) * voxel_size.z();
        return vec3{p_x, p_y, p_z};
    }
    vec3 world_to_grid(const vec3 &p) const
    {
        auto res_x = (p.x() - bound_min.x() - 0.5 * voxel_size.x()) / voxel_size.x();
        auto res_y = (p.y() - bound_min.y() - 0.5 * voxel_size.y()) / voxel_size.y();
        auto res_z = (p.z() - bound_min.z() - 0.5 * voxel_size.z()) / voxel_size.z();
        return vec3{res_x, res_y, res_z};
    }
    double lerp(double a, double b, double t) const
    {
        return a + t * (b - a);
    }
    double sample_sdf(const vec3 &p) const
    {
        vec3 pos = world_to_grid(p);
        double x = std::clamp(pos.x(), 0.0, (double)(nx - 1));
        double y = std::clamp(pos.y(), 0.0, (double)(ny - 1));
        double z = std::clamp(pos.z(), 0.0, (double)(nz - 1));
        int x0 = (int)(std::floor(x));
        int x1 = std::min(x0 + 1, nx - 1);
        double tx = x - x0;
        int y0 = (int)(std::floor(y));
        int y1 = std::min(y0 + 1, ny - 1);
        double ty = y - y0;
        int z0 = (int)(std::floor(z));
        int z1 = std::min(z0 + 1, nz - 1);
        double tz = z - z0;
        double a000 = get_value(x0, y0, z0);
        double a100 = get_value(x1, y0, z0);
        double a010 = get_value(x0, y1, z0);
        double a110 = get_value(x1, y1, z0);
        double a001 = get_value(x0, y0, z1);
        double a101 = get_value(x1, y0, z1);
        double a011 = get_value(x0, y1, z1);
        double a111 = get_value(x1, y1, z1);
        double x00 = lerp(a000, a100, tx);
        double x01 = lerp(a010, a110, tx);
        double x10 = lerp(a001, a101, tx);
        double x11 = lerp(a011, a111, tx);
        double y00 = lerp(x00, x01, ty);
        double y01 = lerp(x10, x11, ty);
        double res = lerp(y00, y01, tz);
        return res;
    }
     
    bool hit_box(const Ray &r, double &t_min, double &t_max) const
    {
        //wrong
        for (int i = 0; i < 3; ++i)
        {
            double dir = r.direction[i];
            double o = r.origin[i];
            double minv = bound_min[i];
            double maxv = bound_max[i];

            double axis_t0 = (minv - o) / dir;
            double axis_t1 = (maxv - o) / dir;

            if (dir < 0.0)
            {
                std::swap(axis_t0, axis_t1);
            }

            t_min = std::max(t_min, axis_t0);
            t_max = std::min(t_max, axis_t1);

            if (t_min >= t_max)
            {
                return false;
            }
        }

        return true;
    }

private:
    int index(int i, int j, int k) const
    {
        return nx * ny * k + nx * j + i;
    }
};

#endif