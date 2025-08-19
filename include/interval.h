#ifndef INTERVAL_H
#define INTERVAL_H

#include "common.h"

class interval {
    public:
        double min, max;

        interval() : min(+INF), max(-INF) {}

        interval(double min, double max) : min(min), max(max) {}

        double size() const {
            return max - min;
        }

        bool contains(double x) {
            return x >= min && x <= max;
        }

        bool surrounds(double x) {
            return x > min && x < max;
        }

        static const interval empty, universe;
};

const interval interval::empty  = interval(+INF, -INF);
const interval interval::universe  = interval(-INF, +INF);

#endif