#ifndef AABB_H
#define AABB_H

#include "common.h"

class aabb {
    public:
        interval x, y, z;

        aabb() {}

        aabb(const interval& x, const interval& y, const interval& z) : x(x), y(y), z(z) {}

        aabb(const point3& a, const point3& b) {
            x = (a.x() < b.x()) ? interval(a.x(), b.x()) :  interval(b.x(), a.x());
            y = (a.y() < b.y()) ? interval(a.y(), b.y()) :  interval(b.y(), a.y());
            z = (a.z() < b.z()) ? interval(a.z(), b.z()) :  interval(b.z(), a.z());
        }

        aabb(const aabb& a, const aabb& b) {
            x = interval(a.x, b.x);
            y = interval(a.y, b.y);
            z = interval(a.z, b.z);
        }

        interval axis_interval(int n) const {
            if (n == 0) return x;
            else if (n == 1) return y;
            else return z;
        }

        aabb expand(double delta) const {
            return aabb(x.expand(delta), y.expand(delta), z.expand(delta));
        }

        bool hit(const ray& r, interval ray_t) const {
            const point3& ray_orig = r.origin();
            const vec3&   ray_dir  = r.direction();
    
            for (int axis = 0; axis < 3; axis++) {
                const interval& ax = axis_interval(axis);
                const double adinv = 1.0 / ray_dir[axis];

                auto t0 = (ax.min - ray_orig[axis]) * adinv;
                auto t1 = (ax.max - ray_orig[axis]) * adinv;

                if (t0 < t1) {
                    if (t0 > ray_t.min) ray_t.min = t0;
                    if (t1 < ray_t.max) ray_t.max = t1;
                } else {
                    if (t1 > ray_t.min) ray_t.min = t1;
                    if (t0 < ray_t.max) ray_t.max = t0;
                }

                if (ray_t.max <= ray_t.min) return false;
            }

            return true;
        }

        int longest_axis() const {
            if (x.size() > y.size())
                return x.size() > z.size() ? 0 : 2;
            else
                return y.size() > z.size() ? 1 : 2;
        }

        static const aabb empty;
        static const aabb universe;
};

const aabb aabb::empty = aabb(interval::empty, interval::empty, interval::empty);
const aabb aabb::universe = aabb(interval::universe, interval::universe, interval::universe);

#endif