#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "color.h"
#include "ray.h"

class material{
    public:
        virtual ~material()=default;
        virtual bool scatter(const Ray &r, const hit_record &rec, color &attenuation, Ray&scattered){
            return false;
        }
};

class lambertian:public material{
    public:
        lambertian(const color& albedo):
            albedo(albedo){}
        bool scatter(const Ray &r, const hit_record &rec, color &attenuation, Ray&scattered)override{
            auto scatter_direction=rec.normal+random_unit_vector();
            if(scatter_direction.near_zero()){
                scatter_direction=rec.normal;
            }
            scattered=Ray(rec.p,scatter_direction);
            attenuation=albedo;
            return true;
        }
    private:
        color albedo;
};

class metal:public material{
    public:
        metal(const color& albedo):
            albedo(albedo){}
        bool scatter(const Ray &r, const hit_record &rec, color &attenuation, Ray&scattered)override{
            auto scatter_direction=reflected(rec.normal,r.direction);
            if(scatter_direction.near_zero()){
                scatter_direction=rec.normal;
            }
            scattered=Ray(rec.p,scatter_direction);
            attenuation=albedo;
            return true;
        }
    private:
        color albedo;
};

#endif