#ifndef _MATRIX4_HPP_
#define _MATRIX4_HPP_

#include "vec/vec3.hpp"
#include "vec/vec4.hpp"

class Matrix4 {
public:
    Matrix4() {}
    Matrix4(double e0, double e1, double e2, double e3, double e4, double e5,
            double e6, double e7, double e8, double e9, double e10, double e11,
            double e12, double e13, double e14, double e15);

    inline double operator[](int i) const { return e[i]; }
    inline double& operator[](int i) { return e[i]; }

    void load_zero();
    void load_identity();

    void set_translation(const Vec3& translation);

    Matrix4 inverse_transpose() const;

    Matrix4 operator*(const float t) const;
    Matrix4 operator/(const float t) const;

    Vec4 operator*(const Vec4& other) const;

    double e[16];
};

#endif