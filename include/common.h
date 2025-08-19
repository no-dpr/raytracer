#ifndef COMMON_H
#define COMMON_H

#include <cmath>
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

#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif