#ifndef QUAD_H
#define QUAD_H

#include "common.h"
#include "volumes/volume.h"
#include "volumes/volume_list.h"

class quad : public volume {
    public:
        quad(const point3& Q, const vec3& u, const vec3& v, shared_ptr<material> mat) :
            corner(Q), side_a(u), side_b(v), mat(mat) {
                auto n = cross(u, v);
                normal = unit_vector(n);
                D = dot(normal, Q);
                w = n / dot(n, n);

                set_bounding_box();
            }

        virtual void set_bounding_box() {
            bbox = aabb(aabb(corner, corner + side_a + side_b), aabb(corner + side_a, corner + side_b)).pad();
        }

        aabb bounding_box() const override { return bbox; }


        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            double denom = dot(normal, r.direction());
            if (std::fabs(denom) <= 1e-8) return false;

            auto t = (D - dot(normal, r.origin())) / denom;
            if (!ray_t.contains(t)) return false;

            auto intersection = r.at(t);
            vec3 planar_hitpoint_vec = intersection - corner;
            auto alpha = dot(w, cross(planar_hitpoint_vec, side_b));
            auto beta = dot(w, cross(side_a, planar_hitpoint_vec));

            if (!hit_ab(alpha, beta, rec)) return false;

            rec.t = t;
            rec.p = intersection;
            rec.mat = mat;
            rec.set_face_normal(r, normal);

            return true;
        }

        virtual bool hit_ab(double alpha, double beta, hit_record& rec) const {
            interval unit_interval = interval(0, 1);
            // Given the hit point in plane coordinates, return false if it is outside the
            // primitive, otherwise set the hit record UV coordinates and return true.

            if (!unit_interval.contains(alpha) || !unit_interval.contains(beta))
                return false;

            rec.u = alpha;
            rec.v = beta;
            return true;
        }

    protected:
        point3 corner;
        vec3 side_a, side_b;
        vec3 normal;
        double D;
        vec3 w;
        shared_ptr<material> mat;
        aabb bbox;

};

inline shared_ptr<volume_list> box(const point3& a, const point3& b, shared_ptr<material> mat)
{
    // Returns the 3D box (six sides) that contains the two opposite vertices a & b.

    auto sides = make_shared<volume_list>();

    // Construct the two opposite vertices with the minimum and maximum coordinates.
    auto min = point3(std::fmin(a.x(),b.x()), std::fmin(a.y(),b.y()), std::fmin(a.z(),b.z()));
    auto max = point3(std::fmax(a.x(),b.x()), std::fmax(a.y(),b.y()), std::fmax(a.z(),b.z()));

    auto dx = vec3(max.x() - min.x(), 0, 0);
    auto dy = vec3(0, max.y() - min.y(), 0);
    auto dz = vec3(0, 0, max.z() - min.z());

    sides->add(make_shared<quad>(point3(min.x(), min.y(), max.z()),  dx,  dy, mat)); // front
    sides->add(make_shared<quad>(point3(max.x(), min.y(), max.z()), -dz,  dy, mat)); // right
    sides->add(make_shared<quad>(point3(max.x(), min.y(), min.z()), -dx,  dy, mat)); // back
    sides->add(make_shared<quad>(point3(min.x(), min.y(), min.z()),  dz,  dy, mat)); // left
    sides->add(make_shared<quad>(point3(min.x(), max.y(), max.z()),  dx, -dz, mat)); // top
    sides->add(make_shared<quad>(point3(min.x(), min.y(), min.z()),  dx,  dz, mat)); // bottom

    return sides;
}

class triangle : public quad {
    public:
        triangle(const point3& Q, const vec3& u, const vec3& v, shared_ptr<material> mat) :
            quad(Q, u, v, mat) {}

        void set_bounding_box() override {
            bbox = aabb(aabb(corner, corner + side_a), aabb(corner, corner + side_b)).pad();
        }

        bool hit_ab(double alpha, double beta, hit_record& rec) const override {
            if (alpha < 0 || beta < 0 || alpha + beta > 1)
                return false;

            rec.u = alpha;
            rec.v = beta;
            return true;
        }
    
};

#endif