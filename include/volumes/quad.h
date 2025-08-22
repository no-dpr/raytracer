#ifndef QUAD_H
#define QUAD_H

#include "volumes/volume.h"
#include "common.h"

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