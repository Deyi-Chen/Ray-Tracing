#include <iostream>
#include "vec3.h"
#include "ray.h"
#include "color.h"
#include "sphere.h"
#include "rtweekend.h"
#include "hittable_list.h"
#include "camera.h"
#include "material.h"

int main() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(vec3(0,-1000,0), 1000, ground_material));

for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
        auto choose_mat = random_double();
        vec3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

        if ((center - vec3(4, 0.2, 0)).length() > 0.9
            && (center - vec3(-2.0, 0.2, 2.0)).length() > 0.9) {   
            shared_ptr<material> Sphere_material;

            if (choose_mat < 0.8) {
                // diffuse
                auto albedo = color::random() * color::random();
                Sphere_material = make_shared<lambertian>(albedo);
                world.add(make_shared<Sphere>(center, 0.2, Sphere_material));
            } else if (choose_mat < 0.95) {
                // metal
                auto albedo = color::random(0.5, 1);
                auto fuzz = random_double(0, 0.5);
                Sphere_material = make_shared<metal>(albedo, fuzz);
                world.add(make_shared<Sphere>(center, 0.2, Sphere_material));
            } else {
                // glass
                Sphere_material = make_shared<dielectric>(1.5);
                world.add(make_shared<Sphere>(center, 0.2, Sphere_material));
            }
        }
    }
}
    
    world.add(make_shared<Sphere>(vec3(-2.0, 0.2, 2.0), 0.2, make_shared<dielectric>(1.5)));
    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<Sphere>(vec3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(vec3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(vec3(4, 1, 0), 1.0, material3));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1200;
    cam.samples_per_pixel = 500;
    cam.max_depth         = 50;

    cam.vfov     = 20;
    cam.lookfrom = vec3(13,2,3);
    cam.lookat   = vec3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_dist    = 10.0;

    cam.render(world);
}
