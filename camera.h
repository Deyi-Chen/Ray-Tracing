#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "color.h"
#include "ray.h"
#include "vec3.h"
#include "rtweekend.h"
#include "material.h"

class camera{
    public:
        double aspect_ratio=16.0/9.0;
        int image_width=400;
        int samples_per_pixel=5; 
        int max_depth=50;
        double vfov=90;
        void render(const hittable& world);
        vec3 lookat=vec3(0,0,-1);
        vec3 lookfrom=vec3(0,0,0);
        vec3 vup=vec3(0,1,0);
    private:
        int image_height;
        double port_width;
        double port_height;
        double pixel_samples_scale;
        vec3 port_top_left;
        vec3 delta_u;
        vec3 delta_v;
        vec3 camera_o;
        vec3 pixel00_loc;
        vec3 u,v,w;
        void initialize();
        color ray_color(const Ray&r, const hittable&world,int depth);
        vec3 sample_square() const;
        Ray get_ray(int i, int j) const;
};

vec3 camera::sample_square() const{
    //return offset by [-0.5, 0.5] × [-0.5, 0.5] 
    double offset1=(random_double(0,2)-1)*0.5;
    double offset2=(random_double(0,2)-1)*0.5;
    return vec3(offset1,offset2,0);
}

Ray camera::get_ray(int i, int j) const{
    //(i, j) pixel+ random offset→ shoot a ray from camera 
    vec3 offset=sample_square();
    vec3 pixel_loc=pixel00_loc+(i+offset.x())*delta_u+(j+offset.y())*delta_v;
    vec3 direction=pixel_loc-camera_o;
    return Ray(camera_o,direction);
}


color camera::ray_color(const Ray&r, const hittable & world,int depth){
    hit_record rec;
    if(depth==0){
        return color(0,0,0);
    }
    if(world.hit(r,0.001,infinity,rec)){
        Ray scattered;
        color attenuation;
        if(rec.mat->scatter(r,rec,attenuation,scattered)){
            return attenuation*ray_color(scattered,world,depth-1);
        }
        return color(0,0,0);
    }
    //sky
    vec3 unit_dir=unit_vector(r.direction);
    double a = 0.5 * (unit_dir.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0)+ a * color(0.5, 0.7, 1.0);
}

void camera::render(const hittable& world){
    initialize();
    std::cout << "P3\n"
          << image_width << ' ' << image_height << "\n"
          << "255\n";
          
    for(int j=0;j<image_height;j++){
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for(int i=0;i<image_width;i++){
            color pixel_color(0,0,0);
            for(int s=0;s<samples_per_pixel;s++){
                Ray r = get_ray(i, j);
                pixel_color += ray_color(r, world,max_depth);
            }
            pixel_color *= pixel_samples_scale;
            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\rDone.                 \n";
}
void camera::initialize(){
    image_height=int(image_width/aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;
    //camera origin
    camera_o=lookfrom;
    //antialiasing
    pixel_samples_scale = 1.0 / samples_per_pixel;
    //port_width and height
    auto focal_length=(lookfrom-lookat).length();
    auto theta=degrees_to_radians(vfov);
    auto h=std::tan(theta/2);
    port_height=2*h*focal_length;
    port_width=port_height*((double)image_width/image_height);

    //basis vector
    w=unit_vector(lookfrom-lookat);
    u=unit_vector(cross(vup,w));
    v=cross(w,u);

    vec3 viewport_u=port_width*u;
    vec3 viewport_v=port_height*(-1*v);

    delta_u=viewport_u/image_width;
    delta_v=viewport_v / image_height;
    port_top_left=camera_o-(focal_length*w)-(viewport_u)/2-(viewport_v/2);
    pixel00_loc=port_top_left+0.5*(delta_u+delta_v);

}
#endif
