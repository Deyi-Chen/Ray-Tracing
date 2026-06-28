#ifndef VOXEL_GRID_H
#define VOXEL_GRID_H

#include <vector>
#include "vec3.h"

class VoxelGrid{
    public:
        int nx, ny,nz;
        std::vector<double> value;
        vec3 bound_min, bound_max;
        vec3 voxel_size;

        VoxelGrid(int nx,int ny,int nz,const vec3 &bound_min, const vec3 &bound_max):
            nx(nx),ny(ny),nz(nz),bound_min(bound_min),bound_max(bound_max){
                double voxel_size_x=(bound_max.x()-bound_min.x())/nx;
                double voxel_size_y=(bound_max.y()-bound_min.y())/ny;
                double voxel_size_z=(bound_max.z()-bound_min.z())/nz;
                voxel_size=vec3{voxel_size_x,voxel_size_y,voxel_size_z};
                value = std::vector<double>(nx * ny * nz, 0.0);
            }
        void set_value(int i,int j,int k, double val){
            int num=index(i,j,k);
            value[num]=val;
        }
        double get_value(int i,int j,int k)const{
            int num=index(i,j,k);
            return value[num];
        }
        vec3 voxel_center(int i,int j,int k)const{
            //add 0.5 offset to get the center of the voxel
            auto p_x=bound_min.x()+(i+0.5)*voxel_size.x();
            auto p_y=bound_min.y()+(j+0.5)*voxel_size.y();
            auto p_z=bound_min.z()+(k+0.5)*voxel_size.z();
            return vec3{p_x,p_y,p_z};
        }
        vec3 world_to_grid(const vec3& p)const{
            auto res_x=(p.x()-bound_min.x()-0.5*voxel_size.x())/voxel_size.x();
            auto res_y=(p.y()-bound_min.y()-0.5*voxel_size.y())/voxel_size.y();
            auto res_z=(p.z()-bound_min.z()-0.5*voxel_size.z())/voxel_size.z();
            return vec3{res_x,res_y,res_z};
        }
    private:
        int index(int i,int j,int k)const{
            return nx*ny*k+nx*j+i;
        }
            

};

#endif