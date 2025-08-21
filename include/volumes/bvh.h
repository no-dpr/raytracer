#ifndef BVH_H
#define BVH_H

#include "common.h"
#include "volume.h"
#include "volume_list.h"

#include <algorithm>

class bvh_node : public volume {
    public:
        bvh_node(volume_list list) : bvh_node(list.objects, 0, list.objects.size()) {}

        bvh_node(std::vector<shared_ptr<volume>>& objects, size_t start, size_t end) {
            bbox = aabb::empty;
            for (size_t i = start; i < end; i++)
                bbox = aabb(bbox, objects[i]->bounding_box());
            
            int axis = bbox.longest_axis();

            auto comparator = (axis == 0) ? box_compare_x
                            : (axis == 1) ? box_compare_y
                            : box_compare_z;

            size_t object_span = end - start;

            if (object_span == 1) 
                left = right = objects[start];
            else if (object_span == 2) {
                left = objects[start];
                right = objects[start + 1];
            } else {
                std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);

                auto mid = start + object_span / 2;
                left = make_shared<bvh_node>(objects, start, mid);
                right = make_shared<bvh_node>(objects, mid, end);
            }
        }

        aabb bounding_box() const override { return bbox; }

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            if (!bbox.hit(r, ray_t))
                return false;

            bool hit_left = left->hit(r, ray_t, rec);
            bool hit_right = right->hit(r, interval(ray_t.min, hit_left ? rec.t : ray_t.max), rec);

            return hit_left || hit_right;
        }

        static bool box_compare(const shared_ptr<volume> a, const shared_ptr<volume> b, int axis) {
            return a->bounding_box().axis_interval(axis).min < b->bounding_box().axis_interval(axis).min;
        }

        static bool box_compare_x(const shared_ptr<volume> a, const shared_ptr<volume> b) {
            return box_compare(a, b, 0);
        }

        static bool box_compare_y(const shared_ptr<volume> a, const shared_ptr<volume> b) {
            return box_compare(a, b, 1);
        }

        static bool box_compare_z(const shared_ptr<volume> a, const shared_ptr<volume> b) {
            return box_compare(a, b, 2);
        }

    
    private:
        shared_ptr<volume> left;
        shared_ptr<volume> right;
        aabb bbox;
};

#endif