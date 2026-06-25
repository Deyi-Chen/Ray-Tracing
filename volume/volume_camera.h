#ifndef VOLUME_CAMERA_H
#define VOLUME_CAMERA_H
#include "vec3.h"
#include "ray.h"
#include "color.h"
#include "rtweekend.h"
#include "volume_sphere.h"
#include <vector>
#include <atomic>
#include <cmath>
#include "perlin.h"

class volume_camera
{
public:
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int samples_per_pixel = 5;
    int max_depth = 50;
    double vfov = 90;
    void render(const VolumeSphere &world);
    vec3 lookat = vec3(0, 0, -1);
    vec3 lookfrom = vec3(0, 0, 0);
    vec3 vup = vec3(0, 1, 0);
    double defocus_angle = 0;
    double focus_dist = 10;

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
    vec3 u, v, w;
    vec3 defocus_disk_u;
    vec3 defocus_disk_v;
    void initialize();
    color ray_color(const Ray &r, const VolumeSphere &world);
    vec3 sample_square() const;
    vec3 disk_sample() const;
    Ray get_ray(int i, int j) const;
};

vec3 volume_camera::sample_square() const
{
    // return offset by [-0.5, 0.5] × [-0.5, 0.5]
    double offset1 = (random_double(0, 2) - 1) * 0.5;
    double offset2 = (random_double(0, 2) - 1) * 0.5;
    return vec3(offset1, offset2, 0);
}
inline double phase_hg(double g, double cos_theta)
{
    double a = 1.0 + g * g - 2.0 * g * cos_theta;
    double result = (1.0 / (4 * pi)) * ((1.0 - g * g) / (a * std::sqrt(a)));
    return result;
}
inline double fbm(double x, double y, double z)
{
    double sum = 0, amp = 1.0, freq = 1.0;
    for (int o = 0; o < 5; o++)
    {
        sum += amp * perlin_noise(x * freq, y * freq, z * freq);
        freq *= 2.0;
        amp *= 0.5;
    }
    return sum;
}
inline double real_density(vec3 pos)
{
    double frequency = 7;
    vec3 sp = pos * frequency + vec3(13.7, 7.3, 2.9); // random offset
    auto density = fbm(sp.x(), sp.y(), sp.z());
    density = std::max(0.0, density);
    // density=0.5*(density+1);
    density = density * 5;
    return density;
}
inline double shadow_T_light(double sigma_t, const Ray& r, double leave_t){
    double T_light=1.0;
    double step_size=0.05;
    int ns = (int)std::ceil((leave_t) / step_size);
    ns = std::max(1, ns);
    step_size = leave_t / ns;
    for(int i=0;i<ns;i++){
        double t=step_size * (i + random_double(0, 1));
        auto pos=r.at(t);
        double density=real_density(pos);
        double Att=std::exp(-sigma_t*step_size*density);
        T_light=T_light*Att;
        if(T_light<0.001){
            break;
        }        
    }
    
    return T_light;
}
Ray volume_camera::get_ray(int i, int j) const
{
    //(i, j) pixel+ random offset→ shoot a ray from camera
    vec3 offset = sample_square();
    vec3 pixel_loc = pixel00_loc + (i + offset.x()) * delta_u + (j + offset.y()) * delta_v;
    vec3 origin;
    if (defocus_angle <= 0)
    {
        origin = camera_o;
    }
    else
    {
        origin = disk_sample();
    }
    vec3 direction = unit_vector(pixel_loc - origin); //ensure the direction has length 1
    return Ray(origin, direction);
}

vec3 volume_camera::disk_sample() const
{
    auto p = random_in_unit_disk();
    return camera_o + p.x() * defocus_disk_u + p.y() * defocus_disk_v;
}

color volume_camera::ray_color(const Ray &r, const VolumeSphere &world)
{
    // sky
    vec3 unit_dir = unit_vector(r.direction);
    double a = 0.5 * (unit_dir.y() + 1.0);
    color sky_color = (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    double t0 = 0.0, t1 = 0.0;
    if (!world.hit(r, t0, t1))
    {
        return sky_color;
    }
    vec3 light_pos = vec3{0, 1, 0};
    color light_color = vec3{1.3, 0.3, 0.9};
    double sigma_a = world.sigma_a;
    double sigma_s = world.sigma_s;
    double sigma_t = sigma_a + sigma_s;
    double g = 0.8;

    double step_size = 0.1;
    int ns = (int)std::ceil((t1 - t0) / step_size);
    ns = std::max(1, ns);
    step_size = (t1 - t0) / ns;

    double T = 1.0;
    color in_scatter = vec3{0, 0, 0};
    bool debug_mode = false;
    if (!debug_mode)
    {
        for (int i = 0; i < ns; i++)
        {
            double t = t0 + step_size * (i + random_double(0, 1));
            auto pos = r.at(t);
            double density=real_density(pos);
            double Att = std::exp(-sigma_t * density * step_size);
            // background
            T = T * Att;
            if (T < 0.001)
                break; // optimization
            // in scattering
            vec3 to_light = light_pos - pos;
            double dis_light = to_light.length();
            vec3 to_light_n = unit_vector(to_light);
            Ray light_ray{pos, to_light_n};
            double lt0 = 0.0, lt1 = 0.0;
            world.hit(light_ray, lt0, lt1);
            double T_light = shadow_T_light(sigma_t,light_ray,lt1);
            double cos_theta = -dot(to_light_n, unit_dir);
            double ph = phase_hg(g, cos_theta);
            color in_scatter_step = ph * light_color * T_light * sigma_s * step_size * density * world.color;
            in_scatter += T * in_scatter_step;
        }
    }
    else
    {
        for (int i = 0; i < ns; i++)
        {
            double t = t0 + step_size * (i + random_double(0, 1));
            auto pos = r.at(t);
            double frequency = 7;
            vec3 sp = pos * frequency + vec3(13.7, 7.3, 2.9); // random offset
            auto density = fbm(sp.x(), sp.y(), sp.z());
            density = std::max(0.0, density);
            return color(density, density, density);
        }
    }

    return sky_color * T + in_scatter;
}

void volume_camera::render(const VolumeSphere &world)
{
    initialize();

    std::vector<color> framebuffer(image_width * image_height);
    std::atomic<int> lines_done{0};

#pragma omp parallel for schedule(dynamic, 1)
    for (int j = 0; j < image_height; j++)
    {
        for (int i = 0; i < image_width; i++)
        {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; s++)
            {
                Ray r = get_ray(i, j);
                pixel_color += ray_color(r, world);
            }
            pixel_color *= pixel_samples_scale;
            framebuffer[j * image_width + i] = pixel_color;
        }
        int done = ++lines_done;
#pragma omp critical
        std::clog << "\rScanlines done: " << done << " / " << image_height << std::flush;
    }

    std::cout << "P3\n"
              << image_width << ' ' << image_height << "\n255\n";
    for (int j = 0; j < image_height; j++)
    {
        for (int i = 0; i < image_width; i++)
        {
            write_color(std::cout, framebuffer[j * image_width + i]);
        }
    }
    std::clog << "\rDone.                                       \n";
}

void volume_camera::initialize()
{
    image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;
    // camera origin
    camera_o = lookfrom;
    // antialiasing
    pixel_samples_scale = 1.0 / samples_per_pixel;
    // port_width and height
    // auto focal_length=(lookfrom-lookat).length();
    auto theta = degrees_to_radians(vfov);
    auto h = std::tan(theta / 2);
    port_height = 2 * h * focus_dist;
    port_width = port_height * ((double)image_width / image_height);

    // basis vector
    w = unit_vector(lookfrom - lookat);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);

    vec3 viewport_u = port_width * u;
    vec3 viewport_v = port_height * (-1 * v);

    delta_u = viewport_u / image_width;
    delta_v = viewport_v / image_height;
    port_top_left = camera_o - (focus_dist * w) - (viewport_u) / 2 - (viewport_v / 2);
    pixel00_loc = port_top_left + 0.5 * (delta_u + delta_v);
    auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
    defocus_disk_u = u * defocus_radius;
    defocus_disk_v = v * defocus_radius;
}

#endif