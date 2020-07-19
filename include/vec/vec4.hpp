#ifndef _VEC4_HPP_
#define _VEC4_HPP_

#include "vec/vec3.hpp"

class Vec4 {
public:
    Vec4() {}
    Vec4(double e0, double e1, double e2, double e4) {
        e[0] = e0;
        e[1] = e1;
        e[2] = e2;
        e[3] = e4;
    }
    Vec4(const Vec3& other) : Vec4(other[0], other[1], other[2], 1) {}

    inline double x() const { return e[0]; }
    inline double y() const { return e[1]; }
    inline double z() const { return e[2]; }
    inline double w() const { return e[3]; }

    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    double e[4];
};

#endif