#include <iostream>
#include "vec3.h"
#include "ray.h"
#include "color.h"
#include "sphere.h"
#include "rtweekend.h"
#include "hittable_list.h"
#include "camera.h"
#include "material.h"
#include "bvh.h"
#include "triangle.h"

int main()
{

    hittable_list world;

    auto tri_mat =make_shared<lambertian>(color(0.8, 0.3, 0.3));
    world.add(make_shared<triangle>(
            vec3(-1, 0, -3),
            vec3(1, 0, -3),
            vec3(0, 1, -3),
            tri_mat));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;
    cam.samples_per_pixel = 50;

    cam.lookfrom = vec3(0, 0, 0);
    cam.lookat = vec3(0, 0, -1);

    cam.render(world);
}

/*

*/