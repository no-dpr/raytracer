#ifndef PERLIN_H
#define PERLIN_H

#include "common.h"

class perlin {
    public:
        perlin() {
            for (int i = 0; i < point_count; i++) {
                randvec[i] = unit_vector(vec3::random(-1, 1));
            }

            perlin_generate_perm(perm_x);
            perlin_generate_perm(perm_y);
            perlin_generate_perm(perm_z);
        }

        double noise(const point3& p) const {
            // Get fractional parts of coordinates
            auto x_frac = p.x() - std::floor(p.x());
            auto y_frac = p.y() - std::floor(p.y());
            auto z_frac = p.z() - std::floor(p.z());

            // Get integer parts of coordinates
            auto x_int = int(std::floor(p.x()));
            auto y_int = int(std::floor(p.y()));
            auto z_int = int(std::floor(p.z()));

            // Get hash information for trilinear interpolation.
            vec3 c[2][2][2];
            for (int dx = 0; dx < 2; dx++) {
                for (int dy = 0; dy < 2; dy++) {
                    for (int dz = 0; dz < 2; dz++) {
                        c[dx][dy][dz] = randvec[
                            perm_x[(x_int + dx) & 255] ^ 
                            perm_y[(y_int + dy) & 255] ^ 
                            perm_z[(z_int + dz) & 255] 
                        ];
                    }
                }
            }

            return perlin_interp(c, x_frac, y_frac, z_frac);
        }

        double turb(const point3& p, int depth) const {
            auto accum = 0.0;
            auto temp_p = p;
            auto weight = 1.0;

            for(int i = 0; i < depth; i++) {
                accum += weight * noise(temp_p);
                weight *= 0.5;
                temp_p *= 2;
            }

            return std::fabs(accum);
        }

    private:
        static const int point_count = 256;
        vec3 randvec[point_count];
        int perm_x[point_count];
        int perm_y[point_count];
        int perm_z[point_count];

        static void perlin_generate_perm(int* p) {
            for(int i = 0; i < point_count; i++)
                p[i] = i;
            
            permute(p, point_count);
        }

        static void permute(int* p, int n) {
            for (int i = n - 1; i > 0; i--) {
                int target = random_int(0, i);
                int temp = p[i];
                p[i] = p[target];
                p[target] = temp;
            }
        }

        static double trilinear_interp(double c[2][2][2], double x, double y, double z) {
            auto accum = 0.0;
            for (int i=0; i < 2; i++)
                for (int j=0; j < 2; j++)
                    for (int k=0; k < 2; k++)
                        accum += (i*x + (1-i)*(1-x))
                                * (j*y + (1-j)*(1-y))
                                * (k*z + (1-k)*(1-z))
                                * c[i][j][k];
    
            return accum;
        } 

        static double perlin_interp(const vec3 c[2][2][2], double x, double y, double z) {

            // Smooth fractional parts using Hermite cubic
            double xx = x * x * (3-2 * x);
            double yy = y * y * (3-2 * y);
            double zz = z * z * (3-2 * z);

            auto accum = 0.0;
            for (int i=0; i < 2; i++)
                for (int j=0; j < 2; j++)
                    for (int k=0; k < 2; k++) {
                        vec3 weight_v(x-i, y-j, z-k);

                        accum += (i*x + (1-i)*(1-x))
                                * (j*y + (1-j)*(1-y))
                                * (k*z + (1-k)*(1-z))
                                * dot(c[i][j][k], weight_v);
                    }
    
            return accum;
        }

};

#endif