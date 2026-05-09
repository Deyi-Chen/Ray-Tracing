#ifndef BVH_H
#define BVH_H

#include "aabb.h"
#include "hittable.h"
#include "hittable_list.h"
#include "rtweekend.h"

class bvh_node : public hittable
{

public:
    bvh_node(hittable_list list)
        : bvh_node(list.objects, 0, list.objects.size())
    {
    }

    bvh_node(
        std::vector<shared_ptr<hittable>> &objects,
        size_t start,
        size_t end)
    {
        int axis = random_int(0, 2);
        auto comparator = box_x_compare;

        if (axis == 1)
        {
            comparator = box_y_compare;
        }
        else if (axis == 2)
        {
            comparator = box_z_compare;
        }

        size_t object_span = end - start;

        if (object_span == 1)
        {
            left = right = objects[start];
        }
        else if (object_span == 2)
        {
            left = objects[start];
            right = objects[start + 1];
        }
        else
        {
            std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);

            auto mid = start + object_span / 2;
            left = make_shared<bvh_node>(objects, start, mid);
            right = make_shared<bvh_node>(objects, mid, end);
        }

        bbox = aabb(left->bounding_box(), right->bounding_box());
    }

    bool hit(
        const Ray &r,
        double t_min,
        double t_max,
        hit_record &rec) const override
    {
        if (!bbox.hit(r, t_min, t_max))
            return false;

        bool hit_left =
            left->hit(r, t_min, t_max, rec);

        bool hit_right =
            right->hit(
                r,
                t_min,
                hit_left ? rec.t : t_max,
                rec);

        return hit_left || hit_right;
    }

    aabb bounding_box() const override
    {
        return bbox;
    }

private:
    shared_ptr<hittable> left;
    shared_ptr<hittable> right;

    aabb bbox;

    static bool box_compare(
        const shared_ptr<hittable> a,
        const shared_ptr<hittable> b,
        int axis)
    {
        return a->bounding_box().minimum[axis] < b->bounding_box().minimum[axis];
    }

    static bool box_x_compare(
        const shared_ptr<hittable> a,
        const shared_ptr<hittable> b)
    {
        return box_compare(a, b, 0);
    }

    static bool box_y_compare(
        const shared_ptr<hittable> a,
        const shared_ptr<hittable> b)
    {
        return box_compare(a, b, 1);
    }

    static bool box_z_compare(
        const shared_ptr<hittable> a,
        const shared_ptr<hittable> b)
    {
        return box_compare(a, b, 2);
    }
};

#endif