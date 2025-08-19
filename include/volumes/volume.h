#ifndef VOLUME_H
#define VOLUME_H

#include "ray.h"

class HitRecord {
    public:
        point3 p;
        vec3 normal;
        double t;
};

class Volume {
    public:
        virtual ~Volume() = default;

        virtual bool hit(const Ray& r, double ray_tmin, double ray_tmax, HitRecord rec) const = 0;
};

#endif