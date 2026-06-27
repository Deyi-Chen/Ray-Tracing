#ifndef VOLUME_OBJ_LOADER_H
#define VOLUME_OBJ_LOADER_H

#include "volume_mesh.h"
#include "vec3.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

inline Volume_mesh load_volume_obj(const std::string &filename)
{
    std::ifstream file(filename);

    if (!file)
    {
        std::cerr
            << "Error: could not open OBJ file "
            << filename << '\n';

        return Volume_mesh();
    }

    double min_x = std::numeric_limits<double>::infinity();
    double min_y = std::numeric_limits<double>::infinity();
    double min_z = std::numeric_limits<double>::infinity();
    double max_x = -std::numeric_limits<double>::infinity();
    double max_y = -std::numeric_limits<double>::infinity();
    double max_z = -std::numeric_limits<double>::infinity();

    std::vector<vec3> vertices;
    std::vector<std::vector<int>> faces;
    std::string line;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string type;
        ss >> type;
        if (type == "v")
        {
            double x, y, z;
            if (!(ss >> x >> y >> z))
            {
                std::cerr
                    << "Warning: invalid vertex line in "
                    << filename << '\n';
                continue;
            }
            vertices.push_back(vec3{x, y, z});
            min_x = std::min(min_x, x);
            min_y = std::min(min_y, y);
            min_z = std::min(min_z, z);
            max_x = std::max(max_x, x);
            max_y = std::max(max_y, y);
            max_z = std::max(max_z, z);
        }
        else if (type == "f")
        {
            int a, b, c;
            ss >> a >> b >> c;
            a--;
            b--;
            c--;
            faces.push_back({a, b, c});
        }
    }
    const vec3 center((min_x + max_x) / 2.0, (min_y + max_y) / 2.0, (min_z + max_z) / 2.0);
    const vec3 size(max_x - min_x, max_y - min_y, max_z - min_z);
    const double max_size = std::max({size.x(), size.y(), size.z()});
    const double scale = 2.0 / max_size;

    double normalized_min_x = std::numeric_limits<double>::infinity();
    double normalized_min_y = std::numeric_limits<double>::infinity();
    double normalized_min_z = std::numeric_limits<double>::infinity();

    double normalized_max_x = -std::numeric_limits<double>::infinity();
    double normalized_max_y = -std::numeric_limits<double>::infinity();
    double normalized_max_z = -std::numeric_limits<double>::infinity();

    for (vec3 &vertex : vertices)
    {
        vertex = (vertex - center) * scale;

        normalized_min_x = std::min(normalized_min_x, vertex.x());
        normalized_min_y = std::min(normalized_min_y, vertex.y());
        normalized_min_z = std::min(normalized_min_z, vertex.z());

        normalized_max_x = std::max(normalized_max_x, vertex.x());
        normalized_max_y = std::max(normalized_max_y, vertex.y());
        normalized_max_z = std::max(normalized_max_z, vertex.z());
    }
    const vec3 normalized_center(
        (normalized_min_x + normalized_max_x) / 2.0,
        (normalized_min_y + normalized_max_y) / 2.0,
        (normalized_min_z + normalized_max_z) / 2.0);

    
    const vec3 normalized_size(
        normalized_max_x - normalized_min_x,
        normalized_max_y - normalized_min_y,
        normalized_max_z - normalized_min_z);

    
    std::cerr
        << "\nNormalized bounds min: ("
        << normalized_min_x << ", "
        << normalized_min_y << ", "
        << normalized_min_z << ')'

        << "\nNormalized bounds max: ("
        << normalized_max_x << ", "
        << normalized_max_y << ", "
        << normalized_max_z << ')'

        << "\nNormalized center: ("
        << normalized_center.x() << ", "
        << normalized_center.y() << ", "
        << normalized_center.z() << ')'

        << "\nNormalized size: ("
        << normalized_size.x() << ", "
        << normalized_size.y() << ", "
        << normalized_size.z() << ")\n";
    Volume_mesh mesh{vertices,faces,vec3{normalized_min_x, normalized_min_y,normalized_min_z},vec3{normalized_max_x,normalized_max_y,normalized_max_z},normalized_size,scale};
    return mesh;
}


#endif