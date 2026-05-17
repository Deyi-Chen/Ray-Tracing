# Ray-Tracing

A simple ray tracer implemented while following the **Ray Tracing in One Weekend** tutorial series.

This project renders a basic 3D scene using ray tracing techniques, including rays, spheres, materials, lighting behavior, camera setup, and image output. It also implements a BVH (Bounding Volume Hierarchy) to accelerate ray-object intersection, and parallelizes the render loop with OpenMP, making it practical to render scenes with hundreds of objects

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
- OpenMP multi-threaded rendering
- PPM image output

## Build and Run

This project uses CMake and requires a C++17 compiler with OpenMP support (any modern GCC, Clang, or MSVC).

### Build

```bash
cmake -S . -B build
cmake --build build
```

This produces an optimized (-O3) executable at build/raytracer. CMake defaults to a Release build.

### Run

```bash
./build/raytracer > image.ppm
```

## Performance

Final scene (1200×675, 500 samples per pixel, max depth 50) on an Intel Core Ultra 9 275HX (24 cores, WSL2):

| Build               | Time     |
| ------------------- | -------- |
| Single-threaded     | ~4m 25s  |
| OpenMP (24 threads) | ~1m 16s  |

## Reference

- [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html) by Peter Shirley
