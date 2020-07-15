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

private:
    double e[2];
};

#endif