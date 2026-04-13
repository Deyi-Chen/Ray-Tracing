#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "color.h"
#include "ray.h"
#include "vec3.h"
#include "rtweekend.h"

class camera{
    public:
        double aspect_ratio=16.0/9.0;
        int image_width=400;
        void render(const hittable& world);
    private:
        int image_height;
        double port_width;
        double port_height;
        vec3 port_top_left;
        vec3 delta_w;
        vec3 delta_h;
        vec3 camera_o;
        vec3 pixel00_loc;

        void initialize();
        color ray_color(const Ray&r, const hittable&world)const;
};

color camera::ray_color(const Ray&r, const hittable & world)const{
    hit_record rec;
    if(world.hit(r,0.001,infinity,rec)){
        return 0.5 * (rec.normal + color(1,1,1));
    }
    vec3 unit_dir=unit_vector(r.direction);
    double a = 0.5 * (unit_dir.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0)+ a * color(0.5, 0.7, 1.0);
}

void camera::render(const hittable& world){
    initialize();
    std::cout << "P3\n"
          << image_width << ' ' << image_height << "\n"
          << "255\n";
          
    for(int i=0;i<image_height;i++){
        std::clog << "\rScanlines remaining: " << (image_height - i) << ' ' << std::flush;
        for(int j=0;j<image_width;j++){
            vec3 pos=pixel00_loc+j*delta_w+i*delta_h;
            vec3 dir=pos-camera_o;
            Ray r=Ray(camera_o,dir);
            color pixel_color=ray_color(r,world);
            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\rDone.                 \n";
}
void camera::initialize(){
    image_height=int(image_width/aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;
    
    //viewport
    port_width=2.0;
    port_height=port_width*((double)image_height/image_width);
    port_top_left=vec3(-port_width/2,port_height/2,-1);
    delta_w=vec3(port_width/image_width,0,0);
    delta_h=vec3(0,-port_height/image_height,0);
    pixel00_loc=port_top_left+0.5*(delta_w+delta_h);
    //camera origin
    camera_o=vec3(0.0,0.0,0.0);
}
#endif
