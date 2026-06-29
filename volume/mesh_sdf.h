#ifndef MESH_SDF_H
#define MESH_SDF_H

#include "volume_mesh.h"
#include "voxel_grid.h"
#include "geometry_utils.h"
#include <omp.h>

void build_unsigned_distance_grid(const Volume_mesh& mesh, VoxelGrid& grid){
    int nx=grid.nx;
    int ny=grid.ny;
    int nz=grid.nz;
    #pragma omp parallel for collapse(3)
    for(int k=0;k<nz;k++){
        for(int j=0;j<ny;j++){
            for(int i=0;i<nx;i++){
                auto p=grid.voxel_center(i,j,k); 
                auto d=point_to_mesh_unsigned_d(p,mesh);
                grid.set_value(i,j,k,d);
            }
        }
    }
}

#endif