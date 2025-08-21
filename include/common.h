#ifndef COMMON_H
#define COMMON_H

#include <cmath>
#include <random>
#include <iostream>
#include <limits>
#include <memory>

using std::make_shared;
using std::shared_ptr;

const double INF = std::numeric_limits<double>::infinity();
const double PI = 3.14159265358932385;

inline double degrees_to_radians(double degrees) {
    return degrees * PI / 180.0;
}

inline double random_double() {
    // Returns a random real in [0, 1)
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double(double min, double max) {
    // Returns a random real in [min, max)
    return min + (max - min) * random_double();
}

inline int random_int(int min, int max) {
    // Returns a random integer in [min, max]
    return std::floor(random_double(min, max + 1));
}

#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif