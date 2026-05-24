#ifndef BUNNY_SCENE_H
#define BUNNY_SCENE_H

#include "../hittable_list.h"
#include "../obj_loader.h"
#include "../bvh.h"
#include "../rtweekend.h"

inline hittable_list bunny_scene(){
    hittable_list world;
    auto bunny_mat = make_shared<lambertian>(color(0.8, 0.8, 0.8));
    world.add(load_obj("assets/bunny.obj.txt", bunny_mat));
    world = hittable_list(make_shared<bvh_node>(world));
    return world;
}

#endif