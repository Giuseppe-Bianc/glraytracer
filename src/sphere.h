#pragma once

#include "Vec3.h"
#include "hittable.h"

class sphere : public hittable {
public:
    sphere(point3 _center, double _radius, shared_ptr<material> _material) noexcept
        : center(_center), radius(_radius), mat(_material), radsq(_radius * _radius) {}

    bool hit(const ray &r, interval ray_t, hit_record &rec) const override {
        const vec3 oc = r.origin() - center;
        const auto a = r.direction().length_squared();
        const auto half_b = dot(oc, r.direction());
        const auto c = oc.length_squared() - radsq;

        const auto discriminant = half_b * half_b - a * c;
        if(discriminant < 0)
            return false;
        const auto sqrtd = std::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (-half_b - sqrtd) / a;
        if(!ray_t.surrounds(root)) {
            root = (-half_b + sqrtd) / a;
            if(!ray_t.surrounds(root))
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        const vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;

        return true;
    }

private:
    shared_ptr<material> mat;
    point3 center{};
    double radius;
    double radsq;
};
