#include "color.hpp"

#include "util.hpp"

Vec3 get_color(Vec3 pixel_color, int samples_per_pixel) {
    double r = pixel_color.x();
    double g = pixel_color.y();
    double b = pixel_color.z();

    double scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    return Vec3(static_cast<uint8_t>(256 * clamp(r, 0.0, 0.999)),
                static_cast<uint8_t>(256 * clamp(g, 0.0, 0.999)),
                static_cast<uint8_t>(256 * clamp(b, 0.0, 0.999)));
}
