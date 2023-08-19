#pragma once
#include "Vec3.h"

class ray {
public:
    ray() = default;
    ray(const point3 &origin, const vec3 &direction) noexcept : orig(origin), dir(direction) {}

    point3 origin() const noexcept { return orig; }
    vec3 direction() const noexcept { return dir; }

    vec3 at(double t) const { return orig + t * dir; }

private:
    point3 orig;
    vec3 dir;
};
