#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <iostream>

#include "vec3.h"
#include "hittable_list.h"
#include "triangle.h"
#include "material.h"
#include "bvh.h"

shared_ptr<hittable> load_obj(const std::string&filename,shared_ptr<material>mat){
    std::ifstream file(filename);
    if(!file){
        std::cerr<<"Error, could not open OBJ file "<<filename<<"\n";
        return make_shared<hittable_list>();
    }
    std::vector<vec3> vertices; //store vertices
    hittable_list mesh;
    std::string line;
    while(std::getline(file,line)){
        std::stringstream ss(line);
        std::string type;
        ss>>type;
        if(type=="v"){
            double x,y,z;
            ss >> x >> y>> z;
            vertices.push_back(vec3(x,y,z));
        }
        else if(type=="f"){
            int a,b,c;
            ss>>a>>b>>c;

            a--;
            b--;
            c--;
            if (
                a < 0 || b < 0 || c < 0 ||
                a >= vertices.size() ||
                b >= vertices.size() ||
                c >= vertices.size()
            ) {
                std::cerr << "Warning: invalid face index in " << filename << "\n";
                continue;
            }  
            mesh.add(make_shared<triangle>(vertices[a],vertices[b],vertices[c],mat));
        }
    }
    std::cerr<<"Load OBJ: "<<filename <<"-vertices: "<<vertices.size()<<"\n";
    return make_shared<bvh_node>(mesh);
}

#endif