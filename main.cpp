#include <iostream>
#include "vec3.h"
#include "ray.h"
#include "color.h"

double hit_sphere(const vec3&center,const Ray&r, double radius){
    auto oc=center-r.A;
    auto d=r.B;
    auto a=dot(d,d);
    auto b=-2*dot(d,oc);
    auto c=dot(oc,oc)-radius*radius;
    //the delta is a known number
    auto delta=b*b-4*a*c;
    if(delta<0){
        return -1;
    }
    auto t=(-b-sqrt(delta))/(2*a);
    if(t<0){
        return -1;
    }
    return t;
}

color ray_color(const Ray&r){
    vec3 sphere_center=vec3(0,0,-3);
    double sphere_radius=0.8;
    double t=hit_sphere(sphere_center,r,sphere_radius);
    if(t>=0){
        vec3 P=r.at(t);
        vec3 sphere_normal=unit_vector(P-sphere_center);
        vec3 bias=vec3(1,1,1);
        return (sphere_normal+bias)*0.5;
    }
    vec3 unit_dir=unit_vector(r.B);
    double a = 0.5 * (unit_dir.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0)+ a * color(0.5, 0.7, 1.0);
}

int main()
{
    //image
    int image_width = 400;
    int image_height = 225;

    //viewport
    double port_width=2.0;
    double port_height=port_width*((double)image_height/image_width);
    vec3 port_top_left=vec3(-port_width/2,port_height/2,-1);
    vec3 delta_w=vec3(port_width/image_width,0,0);
    vec3 delta_h=vec3(0,-port_height/image_height,0);

    //camera origin
    vec3 camera_o=vec3(0.0,0.0,0.0);

    std::cout << "P3\n"
          << image_width << ' ' << image_height << "\n"
          << "255\n";
          
    for(int i=0;i<image_height;i++){
        std::clog << "\rScanlines remaining: " << (image_height - i) << ' ' << std::flush;
        for(int j=0;j<image_width;j++){
            vec3 pos=port_top_left+j*delta_w+i*delta_h;
            vec3 dir=pos-camera_o;
            Ray r=Ray(camera_o,dir);
            color pixel_color=ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\rDone.                 \n";
}
