#ifndef _RAY_HPP_
#define _RAY_HPP_

#include "vec3.hpp"

class Ray {
public:
    Ray() {}
    Ray(const Vec3& origin, const Vec3& direction)
        : origin(origin), direction(direction) {}

    Vec3 at(double t) const { return origin + t * direction; }

    Vec3 origin;
    Vec3 direction;
};

#endif