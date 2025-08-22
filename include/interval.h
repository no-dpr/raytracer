#ifndef INTERVAL_H
#define INTERVAL_H

#include "common.h"

class interval {
    public:
        double min, max;

        interval() : min(+INF), max(-INF) {}

        interval(double min, double max) : min(min), max(max) {}

        interval(interval a, interval b) {
            min = (a.min <= b.min) ? a.min : b.min;
            max = (a.max >= b.max) ? a.max : b.max;
        }

        double size() const {
            return max - min;
        }

        bool contains(double x) {
            return x >= min && x <= max;
        }

        bool surrounds(double x) {
            return x > min && x < max;
        }

        double clamp(double x) const {
            if (x < min) return min;
            if (x > max) return max;
            return x;
        }

        interval expand(double delta) const {
            auto padding = delta / 2;
            return interval(min - padding, max + padding);
        }

        static const interval empty, universe;
};

const interval interval::empty  = interval(+INF, -INF);
const interval interval::universe  = interval(-INF, +INF);

interval operator+(const interval& ival, double displacement) {
    return interval(ival.min + displacement, ival.max + displacement);
}

interval operator+(double displacement, const interval& ival) {
    return ival + displacement;
}

#endif