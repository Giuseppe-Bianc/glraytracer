#pragma once

#include "hittable.h"
#include "material.h"
#include "rtweekend.h"
#include <stb_image_write.h>
#define LOGING_PROGRESS
class camera {
public:
    /* Public Camera Parameters Here */

    void render(const hittable &world) {
        // Create a std::vector to hold the image data
        image_data.resize(C_ST(w) * h * channels);
        initialize();
    }

    int samples_per_pixel = 10;  // Count of random samples for each pixel
    int max_depth = 10;          // Maximum number of ray bounces into scene
    double vfov = 90;            // Vertical view angle (field of view)

private:
    std::size_t channels = 3;  // RGB channels
    std::vector<unsigned char> image_data;
    point3 center{};       // Camera center
    point3 pixel00_loc{};  // Location of pixel 0, 0
    vec3 pixel_delta_u{};  // Offset to pixel to the right
    vec3 pixel_delta_v{};  // Offset to pixel below
    /* Private Camera Variables Here */

#pragma optimize("gt", on)
    void initialize() {
        constexpr double focal_length = 1.0;
        const auto theta = degrees_to_radians(vfov);
        const auto hh = std::tan(theta / 2);
        const auto viewport_height = 2 * h * focal_length;
        const double viewport_width = viewport_height * (C_D(w) / h);
        center = point3();

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        const auto viewport_u = vec3(viewport_width, 0, 0);
        const auto viewport_v = vec3(0, -viewport_height, 0);

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / w;
        pixel_delta_v = viewport_v / h;

        // Calculate the location of the upper left pixel.
        const auto viewport_upper_left = center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

#pragma optimize("gt", on)
    inline double linear_to_gamma(double linear_component) noexcept { return std::sqrt(linear_component); }
#pragma optimize("gt", on)
    Color ray_color(const ray &r, int depth, const hittable &world) const {
        // If we've exceeded the ray bounce limit, no more light is gathered.
        if(depth <= 0)
            return Color(0, 0, 0);

        if(hit_record rec; world.hit(r, interval(0.001, infinity), rec)) {
            ray scattered;
            if(Color attenuation; rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, depth - 1, world);
            return Color(0, 0, 0);
        }

        const vec3 unit_direction = unit_vector(r.direction());
        const auto a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
    }
#pragma optimize("gt", on)
    void write_color(Color pixel_color, int samples_per_pixl, size_t index) {
        auto r = pixel_color.x();
        auto g = pixel_color.y();
        auto b = pixel_color.z();

        // Divide the color by the number of samples.
        const double scal = 1.0 / samples_per_pixl;
        r *= scal;
        g *= scal;
        b *= scal;

        // Apply the linear to gamma transform.
        r = linear_to_gamma(r);
        g = linear_to_gamma(g);
        b = linear_to_gamma(b);

        // Write the translated [0,255] value of each color component.
        static const interval intensity(0.000, 0.999);
        image_data.at(index) = NC_UC(256 * intensity.clamp(r));      // Red channel
        image_data.at(index + 1) = NC_UC(256 * intensity.clamp(g));  // Green channel
        image_data.at(index + 2) = NC_UC(256 * intensity.clamp(b));  // Blue channel
    }
#pragma optimize("gt", on)
    ray get_ray(int i, int j) const {
        // Get a randomly sampled camera ray for the pixel at location i,j.

        const auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
        const auto pixel_sample = pixel_center + pixel_sample_square();

        const auto ray_origin = center;
        const auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

#pragma optimize("gt", on)
    vec3 pixel_sample_square() const {
        // Returns a random point in the square surrounding a pixel at the origin.
        const auto px = -0.5 + random_double();
        const auto py = -0.5 + random_double();
        return (px * pixel_delta_u) + (py * pixel_delta_v);
    }
};
