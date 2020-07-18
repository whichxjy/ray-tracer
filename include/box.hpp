#ifndef _BOX_HPP_
#define _BOX_HPP_

#include "ray.hpp"
#include "vec/vec3.hpp"

class Box {
public:
    Box(const Vec3& min, const Vec3& max) : min(min), max(max) {}

    bool hit(const Ray& ray, double t_min, double t_max) const;

private:
    Vec3 min;
    Vec3 max;
};

#endif