#ifndef VOLUME_MESH_H
#define VOLUME_MESH_H

#include <vector>
#include "vec3.h"
class Volume_mesh{
    public:
        Volume_mesh(){}
        Volume_mesh(const std::vector<vec3> &vertices, const std::vector<std::vector<int>> &faces,const vec3 &bound_min, const vec3&bound_max,const vec3& size,double scale):
            vertices(vertices),faces(faces),bound_min(bound_min),bound_max(bound_max),size(size),scale(scale)
        {}
        std::vector<vec3> vertices;
        std::vector<std::vector<int>> faces;
        vec3 bound_min;
        vec3 bound_max;
        vec3 size;
        double scale=1.0;
};


#endif