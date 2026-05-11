# Ray-Tracing

A simple ray tracer implemented while following the **Ray Tracing in One Weekend** tutorial series.

This project renders a basic 3D scene using ray tracing techniques, including rays, spheres, materials, lighting behavior, camera setup, and image output. It also implements a BVH (Bounding Volume Hierarchy) to accelerate ray-object intersection, making it practical to render scenes with hundreds of objects.

## Preview

![Render Preview](./image.png)

## Features

- Basic ray generation
- Sphere intersection
- Surface normals
- Anti-aliasing
- Diffuse (Lambertian) materials
- Metal materials with fuzz
- Dielectric / glass materials
- Positionable camera with depth of field
- BVH acceleration structure
- PPM image output

## Build and Run

This project uses CMake.

### Build

```bash
cmake -S . -B build
cmake --build build
```

This produces an executable at `build/raytracer`.

### Run

```bash
./build/raytracer > image.ppm
```

## Reference

- [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html) by Peter Shirley
