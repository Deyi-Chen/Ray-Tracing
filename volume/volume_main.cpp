#include <iostream>
#include "vec3.h"
#include "ray.h"
#include "color.h"
#include "rtweekend.h"
#include "volume/volume_camera.h"

int main(){
    VolumeSphere sphere{vec3(0, 0, -2), 0.8, vec3(1.0, 0.6, 0.7), 0.1,0.2};
    volume_camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;
    cam.lookfrom = vec3(0, 0, 0);
    cam.lookat = vec3(0, 0, -1);
    cam.vfov = 60;
    cam.render(sphere);
}

