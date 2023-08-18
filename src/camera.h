#pragma once

#include "Timer.h"
#include "Vec3.h"
#include "headers.h"
#include "hittable.h"
#include "interval.h"
#include <stb_image_write.h>
#define LOGING_PROGRESS
class camera {
public:
    /* Public Camera Parameters Here */

    void render(const hittable &world) {
        // Create a std::vector to hold the image data
        image_data.resize(C_ST(w) * h * channels);
        initialize();
        Timer t;
        for(int j = 0; j < h; ++j) {
#ifdef LOGING_PROGRESS
            Timer tt;
#endif  // LOGING_PROGRESS
            for(int i = 0; i < w; ++i) {
                const auto index = C_ST(C_ST(j) * w + i) * channels;
                Color pixel_color{0, 0, 0};
                for(int sample = 0; sample < samples_per_pixel; ++sample) {
                    const ray r = get_ray(i, j);
                    pixel_color += ray_color(r, world);
                }
                write_color(pixel_color, samples_per_pixel, index);
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

    int samples_per_pixel = 10;

private:
    std::size_t channels = 3;  // RGB channels
    std::vector<unsigned char> image_data;
    point3 center{};       // Camera center
    point3 pixel00_loc{};  // Location of pixel 0, 0
    vec3 pixel_delta_u{};  // Offset to pixel to the right
    vec3 pixel_delta_v{};  // Offset to pixel below
    /* Private Camera Variables Here */

    void initialize() {
        constexpr double focal_length = 1.0;
        constexpr double viewport_height = 2.0;
        constexpr double viewport_width = viewport_height * (C_D(w) / h);
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

    Color ray_color(const ray &r, const hittable &world) const {
        if(hit_record rec; world.hit(r, interval(0, infinity), rec)) {
            return 0.5 * (rec.normal + Color(1, 1, 1));
        }

        const vec3 unit_direction = unit_vector(r.direction());
        const auto a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
    }
    void write_color(Color pixel_color, int samples_per_pixl, size_t index) {
        auto r = pixel_color.x();
        auto g = pixel_color.y();
        auto b = pixel_color.z();

        // Divide the color by the number of samples.
        const double scal = 1.0 / samples_per_pixl;
        r *= scal;
        g *= scal;
        b *= scal;

        // Write the translated [0,255] value of each color component.
        static const interval intensity(0.000, 0.999);
        image_data[index + 0] = C_UC(256 * intensity.clamp(r));  // Red channel
        image_data[index + 1] = C_UC(256 * intensity.clamp(g));  // Green channel
        image_data[index + 2] = C_UC(256 * intensity.clamp(b));  // Blue channel
    }
    ray get_ray(int i, int j) const {
        // Get a randomly sampled camera ray for the pixel at location i,j.

        const auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
        const auto pixel_sample = pixel_center + pixel_sample_square();

        const auto ray_origin = center;
        const auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    vec3 pixel_sample_square() const {
        // Returns a random point in the square surrounding a pixel at the origin.
        const auto px = -0.5 + random_double();
        const auto py = -0.5 + random_double();
        return (px * pixel_delta_u) + (py * pixel_delta_v);
    }
};
