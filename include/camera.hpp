#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include "ray.hpp"
#include "util.hpp"
#include "vec3.hpp"

class Camera {
public:
    Camera(Vec3 lookfrom, Vec3 lookat, Vec3 vup, double vfov,
           double aspect_ratio, double aperture, double focus_dist) {
        double theta = degrees_to_radians(vfov);
        double h = tan(theta / 2);
        double viewport_height = 2.0 * h;
        double viewport_width = aspect_ratio * viewport_height;

        auto w = unit_vector(lookfrom - lookat);
        auto u = unit_vector(cross(vup, w));
        auto v = cross(w, u);

        origin = lookfrom;
        horizontal = viewport_width * u;
        vertical = viewport_height * v;
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - w;
    }

    Ray get_ray(double s, double t) const {
        return Ray(origin,
                   lower_left_corner + s * horizontal + t * vertical - origin);
    }

private:
    Vec3 origin;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 lower_left_corner;

    Vec3 u;
    Vec3 v;
    Vec3 w;
    double lens_radius;
};

#endif