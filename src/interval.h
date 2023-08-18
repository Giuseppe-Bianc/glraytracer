#pragma once

#include "headers.h"

class interval {
public:
    double min;
    double max;

    constexpr interval() noexcept : min(+infinity), max(-infinity) {}  // Default interval is empty

    constexpr interval(double _min, double _max) : min(_min), max(_max) {}

    double size() const noexcept { return max - min; }

    interval expand(double delta) const {
        const auto padding = delta / 2;
        return interval(min - padding, max + padding);
    }

    bool contains(double x) const noexcept { return min <= x && x <= max; }

    bool surrounds(double x) const noexcept { return min < x && x < max; }
    double clamp(double x) const { return std::clamp(x, min, max); }
};

static inline constexpr interval empty(+infinity, -infinity);
static inline constexpr interval universe(-infinity, +infinity);
