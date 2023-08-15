#pragma once
#include "Vec3.h"

class ray {
public:
    constexpr ray() = default;
    constexpr ray(const vec3 &origin, const vec3 &direction) : orig(origin), dir(direction) {}

    constexpr vec3 origin() const { return orig; }
    constexpr vec3 direction() const { return dir; }

    constexpr vec3 at(double t) const { return orig + t * dir; }

private:
    point3 orig;
    vec3 dir;
};
