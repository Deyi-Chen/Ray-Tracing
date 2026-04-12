#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include<memory>
#include<vector>

using std::make_shared;
using std::shared_ptr;

class hittable_list:public hittable{
    public:
        std::vector<shared_ptr<hittable>> objects;
        hittable_list(){};
        hittable_list(shared_ptr<hittable>object){add(object);} //pointer for polymorphism
        void add(shared_ptr<hittable>object){
            objects.push_back(object);
        }
        bool hit(const Ray&r,double t_min, double t_max, hit_record &rec)const override{
            hit_record temp;
            bool hit_anything=false;
            double closest_t=t_max;
            for(const auto & obj:objects){
                if(obj->hit(r,t_min,closest_t,temp)){
                    hit_anything=true;
                    closest_t=temp.t;
                    rec=temp;
                
                }
            }
            return hit_anything;
        }

};




#endif