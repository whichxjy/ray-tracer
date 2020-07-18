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