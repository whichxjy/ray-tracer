#ifndef _VEC2_HPP_
#define _VEC2_HPP_

#include <cmath>
#include <cstdlib>
#include <iostream>

#include "util.hpp"

class Vec2 {
public:
    Vec2() {}
    Vec2(double e0, double e1) {
        e[0] = e0;
        e[1] = e1;
    }

    inline double x() const { return e[0]; }
    inline double y() const { return e[1]; }

    inline double operator[](int i) const { return e[i]; }
    inline double& operator[](int i) { return e[i]; }

    double e[2];
};

inline Vec2 operator+(const Vec2& v1, const Vec2& v2) {
    return Vec2(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1]);
}

inline Vec2 operator*(const Vec2& v, double t) {
    return Vec2(t * v.e[0], t * v.e[1]);
}

#endif