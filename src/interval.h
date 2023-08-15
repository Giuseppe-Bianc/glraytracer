#pragma once

#include "headers.h"

class interval {
public:
    double min;
    double max;

    constexpr interval() : min(+infinity), max(-infinity) {}  // Default interval is empty

    constexpr interval(double _min, double _max) : min(_min), max(_max) {}

    bool contains(double x) const { return min <= x && x <= max; }

    bool surrounds(double x) const { return min < x && x < max; }
    double clamp(double x) const {
        if(x < min)
            return min;
        if(x > max)
            return max;
        return x;
    }
};

static inline constexpr interval empty(+infinity, -infinity);
static inline constexpr interval universe(-infinity, +infinity);
