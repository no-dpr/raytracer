#ifndef VOLUME_LIST_H
#define VOLUME_LIST_H

#include "volumes/volume.h"
#include "common.h"

#include <vector>

class volume_list : public volume {
    public:
        std::vector<shared_ptr<volume>> objects;
        
        volume_list() {}
        volume_list(shared_ptr<volume> object) { add(object); }

        void clear() { objects.clear(); }
        
        void add(shared_ptr<volume> object) {
            objects.push_back(object);
            bbox = aabb(bbox, object->bounding_box());
        }

        aabb bounding_box() const override { return bbox; }

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            hit_record temp_rec;
            bool hit_anything = false;
            auto closest_so_far = ray_t.max;

            for (const auto& object : objects) { // O(n)!!! we can do better.
                if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                    hit_anything = true;
                    closest_so_far = temp_rec.t;
                    rec = temp_rec;
                }
            }

            return hit_anything;
        }

    private:
        aabb bbox;
};

#endif