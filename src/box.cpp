#include "box.hpp"

#include <cmath>

bool Box::hit(const Ray& ray, double t_min, double t_max) const {
    for (int i = 0; i < 3; i++) {
        auto t0 = fmin((min[i] - ray.origin[i]) / ray.direction[i],
                       (max[i] - ray.origin[i]) / ray.direction[i]);
        auto t1 = fmax((min[i] - ray.origin[i]) / ray.direction[i],
                       (max[i] - ray.origin[i]) / ray.direction[i]);

        t_min = fmax(t0, t_min);
        t_max = fmin(t1, t_max);

        if (t_max <= t_min) {
            return false;
        }
    }
    return true;
}

Box surrounding_box(const Box& lhs, const Box& rhs) {
    Vec3 min(fmin(lhs.min.x(), rhs.min.x()), fmin(lhs.min.y(), rhs.min.y()),
             fmin(lhs.min.z(), rhs.min.z()));

    Vec3 max(fmax(lhs.max.x(), rhs.max.x()), fmax(lhs.max.y(), rhs.max.y()),
             fmax(lhs.max.z(), rhs.max.z()));

    return Box(min, max);
}