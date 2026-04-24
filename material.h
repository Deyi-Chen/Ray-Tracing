#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "color.h"
#include "ray.h"

class material
{
public:
    virtual ~material() = default;
    virtual bool scatter(const Ray &r, const hit_record &rec, color &attenuation, Ray &scattered)
    {
        return false;
    }
};

class lambertian : public material
{
public:
    lambertian(const color &albedo) : albedo(albedo) {}
    bool scatter(const Ray &r, const hit_record &rec, color &attenuation, Ray &scattered) override
    {
        auto scatter_direction = rec.normal + random_unit_vector();
        if (scatter_direction.near_zero())
        {
            scatter_direction = rec.normal;
        }
        scattered = Ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

private:
    color albedo;
};

class metal : public material
{
public:
    metal(const color &albedo, double fuzz) : albedo(albedo), fuzz(fuzz = (fuzz < 1) ? fuzz : 1) {}
    bool scatter(const Ray &r, const hit_record &rec, color &attenuation, Ray &scattered) override
    {
        auto scatter_direction = unit_vector(reflected(rec.normal, r.direction)) + fuzz * random_unit_vector();
        if (scatter_direction.near_zero())
        {
            scatter_direction = rec.normal;
        }
        scattered = Ray(rec.p, scatter_direction);
        attenuation = albedo;
        return (dot(scatter_direction, rec.normal) > 0);
    }

private:
    color albedo;
    double fuzz;
};

class dielectric : public material
{
public:
    dielectric(double refraction_index) : refraction_index(refraction_index) {}
    bool scatter(const Ray &r, const hit_record &rec, color &attenuation, Ray &scattered) override
    {
        attenuation = color(1, 1, 1);
        double ri;
        if (rec.front_face)
        {
            ri = 1.0 / refraction_index;
        }
        else
        {
            ri = refraction_index;
        }
        vec3 unit_dir = unit_vector(r.direction);
        double cos_theta = std::fmin(std::max(-1 * dot(unit_dir, rec.normal),0.0), 1.0);
        double sin_theta = sqrt(1 - cos_theta * cos_theta);
        bool cannot_refract = (ri * sin_theta) > 1.0;
        vec3 direction;
        if (cannot_refract||reflectance(cos_theta, ri) > random_double())
        {
            direction = reflected(rec.normal, r.direction);
        }
        else
        {
            direction = refract(r.direction, rec.normal, ri);
        }

        scattered = Ray(rec.p, direction);
        return true;
    }

private:
    double refraction_index;
    static double reflectance(double cosine, double refraction_index)
    {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0 * r0;
        return r0 + (1 - r0) * std::pow((1 - cosine), 5);
    }
};

#endif