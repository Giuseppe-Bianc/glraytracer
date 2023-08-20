#pragma once

#include "Timer.h"
#include "hittable.h"
#include "material.h"
#include "rtweekend.h"
#include <stb_image_write.h>
#define LOGING_PROGRESS
class camera {
public:
    /* Public Camera Parameters Here */
    int samples_per_pixel = 10;

    void render(const hittable &world) {
        // Create a std::vector to hold the image data
        initialize();
        const double scal = 1.0 / samples_per_pixel;
        Timer t;
        for(int j = 0; j < h; ++j) {
#ifdef LOGING_PROGRESS
            Timer tt;
#endif  // LOGING_PROGRESS
            for(int i = 0; i < w; ++i) {
                color pixel_color(0, 0, 0);
                for(int sample = 0; sample < samples_per_pixel; ++sample) {
                    const ray r = get_ray(i, j);
                    pixel_color += ray_color(r, world);
                }
                const auto index = C_ST(C_ST(j) * w + i) * channels;
                write_color(pixel_color, scal, index);
            }
#ifdef LOGING_PROGRESS
            tt.stop();
            LINFO("Scanlines remaining: {} previus done in {:f} ms", (h - j), tt.elapsedMilliseconds());
#endif  // LOGING_PROGRESS
        }
        t.stop();

        LINFO("tempo creazione data immagine {} eseguito in {:f} s", filename, t.elapsedSeconds());

        // Write the image data to a file
        stbi_write_png(filename.data(), w, h, C_I(channels), image_data.data(), w * C_I(channels));
    }

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
        center = point3(0, 0, 0);

        // Determine viewport dimensions.
        constexpr auto focal_length = 1.0;
        constexpr auto viewport_height = 2.0;
        constexpr auto viewport_width = viewport_height * (C_D(w) / h);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        const auto viewport_u = vec3(viewport_width, 0, 0);
        const auto viewport_v = vec3(0, -viewport_height, 0);

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / w;
        pixel_delta_v = viewport_v / h;

        // Calculate the location of the upper left pixel.
        const auto viewport_upper_left = center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
        image_data = std::vector<unsigned char>(C_ST(w) * h * channels);
    }

#pragma optimize("gt", on)
    color ray_color(const ray &r, const hittable &world) const {
        if(hit_record rec; world.hit(r, interval(0, infinity), rec)) {
            return 0.5 * (rec.normal + color(1, 1, 1));
        }

        const vec3 unit_direction = unit_vector(r.direction());
        const auto a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }

#pragma optimize("gt", on)
    void write_color(const color &pixel_color, const double &scal, size_t index) {
        auto r = pixel_color.x();
        auto g = pixel_color.y();
        auto b = pixel_color.z();
        r *= scal;
        g *= scal;
        b *= scal;
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

#pragma optimize("gt", on)
};
