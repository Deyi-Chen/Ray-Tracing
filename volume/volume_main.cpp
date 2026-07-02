#include "volume_obj_loader.h"
#include "volume_mesh.h"
#include "mesh_sdf.h"
#include "geometry_utils.h"
#include "volume_camera.h"
#include <iostream>

int main()
{
    Volume_mesh mesh = load_volume_obj("assets/bunny_closed.obj");
    vec3 padding{0.2, 0.2, 0.2};
    auto voxel_min = mesh.bound_min - padding;
    auto voxel_max = mesh.bound_max + padding;
    VoxelGrid grid{32, 32, 32, voxel_min, voxel_max};
    build_signed_distance_grid(mesh, grid);
    volume_camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;
    cam.samples_per_pixel = 20;
    cam.max_depth = 10;

    const vec3 target(0, 0, 0);
    const double distance = 4.5;

    cam.lookfrom = target + vec3(0, distance, 0);
    cam.lookat = target;
    cam.vup = vec3(0, 0, -1);

    cam.vfov = 30;
    cam.defocus_angle = 0.0;
    cam.focus_dist = distance;
    double sigma_a = 0.02;
    double sigma_s = 0.7;
    color volume_color{1.0, 0.4, 0.55};
    cam.render(grid,sigma_s,sigma_a,volume_color);
}