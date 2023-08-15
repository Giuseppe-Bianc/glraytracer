#pragma once
#include "headers.h"
class vec3 {
private:
    std::array<double, 3> e;

public:
    constexpr vec3() = default;
    constexpr vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    constexpr double x() const { return e[0]; }
    constexpr double y() const { return e[1]; }
    constexpr double z() const { return e[2]; }

    constexpr vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    constexpr double operator[](int i) const { return e[i]; }
    constexpr double &operator[](int i) { return e[i]; }

    constexpr vec3 &operator+=(const vec3 &v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    constexpr vec3 &operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    constexpr vec3 &operator/=(double t) { return *this *= 1 / t; }

    constexpr double length() const { return sqrtcx(length_squared()); }

    constexpr double length_squared() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }

    constexpr std::string toString() const {
        return "(" + std::to_string(e[0]) + ", " + std::to_string(e[1]) + ", " + std::to_string(e[2]) + ")";  // NOSONAR
    }
    static constexpr vec3 random() { return vec3(random_double(), random_double(), random_double()); }

    static constexpr vec3 random(double min, double max) {
        return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }

private:
    constexpr double sqrtNewtonRaphson(double x, double curr, double prev) const {
        return curr == prev ? curr : sqrtNewtonRaphson(x, 0.5 * (curr + x / curr), curr);
    }

    constexpr double sqrtcx(double x) const {
        return x >= 0 && x < std::numeric_limits<double>::infinity() ? sqrtNewtonRaphson(x, x, 0)
                                                                     : std::numeric_limits<double>::quiet_NaN();
    }
};

// point3 is just an alias for vec3, but useful for geometric clarity in the code.
using point3 = vec3;
using Color = vec3;
constexpr vec3 operator+(const vec3 &u, const vec3 &v) { return vec3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z()); }
constexpr vec3 operator-(const vec3 &u, const vec3 &v) { return vec3(u.x() - v.x(), u.y() - v.y(), u.z() - v.z()); }
constexpr vec3 operator*(const vec3 &u, const vec3 &v) { return vec3(u.x() * v.x(), u.y() * v.y(), u.z() * v.z()); }
constexpr vec3 operator*(double t, const vec3 &v) { return vec3(t * v.x(), t * v.y(), t * v.z()); }
constexpr vec3 operator*(const vec3 &v, double t) { return t * v; }
constexpr vec3 operator/(const vec3 &v, double t) { return (1 / t) * v; }
constexpr double dot(const vec3 &u, const vec3 &v) { return u.x() * v.x() + u.y() * v.y() + u.z() * v.z(); }
constexpr vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.y() * v.z() - u.z() * v.y(), u.z() * v.x() - u.x() * v.z(), u.x() * v.y() - u.y() * v.x());
}

constexpr vec3 unit_vector(vec3 v) { return v / v.length(); }
