#pragma once
#include "headers.h"

class vec3 {
private:
    glm::dvec3 e;

public:
    vec3() noexcept : e(0.0, 0.0, 0.0) {}
    vec3(double e0, double e1, double e2) noexcept : e(e0, e1, e2) {}
    explicit vec3(const glm::dvec3 &ev) noexcept : e(ev) {}

#pragma optimize("gt", on)
    double x() const noexcept { return e.x; }
#pragma optimize("gt", on)
    double y() const noexcept { return e.y; }
#pragma optimize("gt", on)
    double z() const noexcept { return e.z; }

#pragma optimize("gt", on)
    glm::dvec3 getE() const noexcept { return e; }
#pragma optimize("gt", on)
    vec3 operator-() const noexcept { return vec3(-e.x, -e.y, -e.z); }
#pragma optimize("gt", on)
    double operator[](int i) const { return e[i]; }
#pragma optimize("gt", on)
    double &operator[](int i) { return e[i]; }
#pragma optimize("gt", on)
    vec3 &operator+=(const vec3 &v) {
        e += v.e;
        return *this;
    }
#pragma optimize("gt", on)
    vec3 &operator*=(double t) {
        e *= t;
        return *this;
    }
#pragma optimize("gt", on)
    vec3 &operator/=(double t) { return *this *= 1.0 / t; }
#pragma optimize("gt", on)
    double length() const { return glm::length(e); }
#pragma optimize("gt", on)
    double length_squared() const { return glm::dot(e, e); }
#pragma optimize("gt", on)
    bool near_zero() const noexcept {
        constexpr double s = 1e-8;
        return (std::fabs(e.x) < s) && (std::fabs(e.y) < s) && (std::fabs(e.z) < s);
    }
#pragma optimize("gt", on)
    static vec3 random() { return vec3(glm::linearRand(0.0, 1.0), glm::linearRand(0.0, 1.0), glm::linearRand(0.0, 1.0)); }
#pragma optimize("gt", on)
    static vec3 random(double min, double max) {
        return vec3(glm::linearRand(min, max), glm::linearRand(min, max), glm::linearRand(min, max));
    }
#pragma optimize("gt", on)
    inline std::string toString() const { return glm::to_string(e); }
};

using point3 = vec3;
using color = vec3;
#pragma optimize("gt", on)
inline vec3 operator+(const vec3 &u, const vec3 &v) { return vec3(u.getE() + v.getE()); }
#pragma optimize("gt", on)
inline vec3 operator-(const vec3 &u, const vec3 &v) { return vec3(u.getE() - v.getE()); }
#pragma optimize("gt", on)
inline vec3 operator*(const vec3 &u, const vec3 &v) { return vec3(u.getE() * v.getE()); }
#pragma optimize("gt", on)
inline vec3 operator*(double t, const vec3 &v) { return vec3(t * v.getE()); }
#pragma optimize("gt", on)
inline vec3 operator*(const vec3 &v, double t) { return t * v; }
#pragma optimize("gt", on)
inline vec3 operator/(vec3 v, double t) { return (1.0 / t) * v; }
#pragma optimize("gt", on)
inline double dot(const vec3 &u, const vec3 &v) { return glm::dot(u.getE(), v.getE()); }
#pragma optimize("gt", on)
inline vec3 cross(const vec3 &u, const vec3 &v) { return vec3(glm::cross(u.getE(), v.getE())); }
#pragma optimize("gt", on)
inline vec3 unit_vector(vec3 v) { return vec3(glm::normalize(v.getE())); }
#pragma optimize("gt", on)
inline vec3 random_in_unit_disk() {
    while(true) {
        auto p = vec3(glm::linearRand(-1.0, 1.0), glm::linearRand(-1.0, 1.0), 0.0);
        if(glm::length2(p.getE()) < 1.0)
            return p;
    }
}
#pragma optimize("gt", on)
inline vec3 random_in_unit_sphere() {
    while(true) {
        auto p = vec3::random(-1.0, 1.0);
        if(glm::length2(p.getE()) < 1.0)
            return p;
    }
}
#pragma optimize("gt", on)
inline vec3 random_unit_vector() { return unit_vector(random_in_unit_sphere()); }
#pragma optimize("gt", on)
inline vec3 random_on_hemisphere(const vec3 &normal) {
    vec3 on_unit_sphere = random_unit_vector();
    if(glm::dot(on_unit_sphere.getE(), normal.getE()) > 0.0)
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}
#pragma optimize("gt", on)
inline vec3 reflect(const vec3 &v, const vec3 &n) { return v - 2.0 * dot(v, n) * n; }
#pragma optimize("gt", on)
inline vec3 refract(const vec3 &uv, const vec3 &n, double etai_over_etat) {
    auto cos_theta = glm::min(glm::dot(-uv.getE(), n.getE()), 1.0);
    const vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    const vec3 r_out_parallel = -std::sqrt(glm::abs(1.0 - glm::length2(r_out_perp.getE()))) * n;
    return r_out_perp + r_out_parallel;
}
