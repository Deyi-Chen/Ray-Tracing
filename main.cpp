#include <iostream>
#include "vec3.h"
#include "ray.h"
#include "color.h"
#include "sphere.h"
#include "rtweekend.h"
#include "hittable_list.h"
#include "camera.h"

int main()
{
    //world
    hittable_list world;
    world.add(make_shared<Sphere>(vec3(0,0,-3), 0.8));
    world.add(make_shared<Sphere>(vec3(0,-100.8,-3), 100));

    //camera
    camera cam;
    cam.image_width=400;
    cam.aspect_ratio=16.0/9.0;
    cam.samples_per_pixel=5;
    cam.render(world);
    
}
