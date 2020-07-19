#include "vec/matrix4.hpp"

#include "util.hpp"

Matrix4::Matrix4(double e0, double e1, double e2, double e3, double e4,
                 double e5, double e6, double e7, double e8, double e9,
                 double e10, double e11, double e12, double e13, double e14,
                 double e15) {
    e[0] = e0;
    e[1] = e1;
    e[2] = e2;
    e[3] = e3;
    e[4] = e4;
    e[5] = e5;
    e[6] = e6;
    e[7] = e7;
    e[8] = e8;
    e[9] = e9;
    e[10] = e10;
    e[11] = e11;
    e[12] = e12;
    e[13] = e13;
    e[14] = e14;
    e[15] = e15;
}

void Matrix4::load_zero() {
    for (int i = 0; i < 16; i++) {
        e[i] = 0;
    }
}

void Matrix4::load_identity() {
    load_zero();

    e[0] = 1;
    e[5] = 1;
    e[10] = 1;
    e[15] = 1;
}

void Matrix4::set_translation(const Vec3& translation) {
    load_identity();
    e[12] = translation.x();
    e[13] = translation.y();
    e[14] = translation.z();
}

Matrix4 Matrix4::inverse_transpose() const {
    Matrix4 inv;
    double det;

    inv[0] = e[5] * e[10] * e[15] - e[5] * e[11] * e[14] - e[9] * e[6] * e[15] +
             e[9] * e[7] * e[14] + e[13] * e[6] * e[11] - e[13] * e[7] * e[10];

    inv[4] = -e[4] * e[10] * e[15] + e[4] * e[11] * e[14] +
             e[8] * e[6] * e[15] - e[8] * e[7] * e[14] - e[12] * e[6] * e[11] +
             e[12] * e[7] * e[10];

    inv[8] = e[4] * e[9] * e[15] - e[4] * e[11] * e[13] - e[8] * e[5] * e[15] +
             e[8] * e[7] * e[13] + e[12] * e[5] * e[11] - e[12] * e[7] * e[9];

    inv[12] = -e[4] * e[9] * e[14] + e[4] * e[10] * e[13] +
              e[8] * e[5] * e[14] - e[8] * e[6] * e[13] - e[12] * e[5] * e[10] +
              e[12] * e[6] * e[9];

    inv[1] = -e[1] * e[10] * e[15] + e[1] * e[11] * e[14] +
             e[9] * e[2] * e[15] - e[9] * e[3] * e[14] - e[13] * e[2] * e[11] +
             e[13] * e[3] * e[10];

    inv[5] = e[0] * e[10] * e[15] - e[0] * e[11] * e[14] - e[8] * e[2] * e[15] +
             e[8] * e[3] * e[14] + e[12] * e[2] * e[11] - e[12] * e[3] * e[10];

    inv[9] = -e[0] * e[9] * e[15] + e[0] * e[11] * e[13] + e[8] * e[1] * e[15] -
             e[8] * e[3] * e[13] - e[12] * e[1] * e[11] + e[12] * e[3] * e[9];

    inv[13] = e[0] * e[9] * e[14] - e[0] * e[10] * e[13] - e[8] * e[1] * e[14] +
              e[8] * e[2] * e[13] + e[12] * e[1] * e[10] - e[12] * e[2] * e[9];

    inv[2] = e[1] * e[6] * e[15] - e[1] * e[7] * e[14] - e[5] * e[2] * e[15] +
             e[5] * e[3] * e[14] + e[13] * e[2] * e[7] - e[13] * e[3] * e[6];

    inv[6] = -e[0] * e[6] * e[15] + e[0] * e[7] * e[14] + e[4] * e[2] * e[15] -
             e[4] * e[3] * e[14] - e[12] * e[2] * e[7] + e[12] * e[3] * e[6];

    inv[10] = e[0] * e[5] * e[15] - e[0] * e[7] * e[13] - e[4] * e[1] * e[15] +
              e[4] * e[3] * e[13] + e[12] * e[1] * e[7] - e[12] * e[3] * e[5];

    inv[14] = -e[0] * e[5] * e[14] + e[0] * e[6] * e[13] + e[4] * e[1] * e[14] -
              e[4] * e[2] * e[13] - e[12] * e[1] * e[6] + e[12] * e[2] * e[5];

    inv[3] = -e[1] * e[6] * e[11] + e[1] * e[7] * e[10] + e[5] * e[2] * e[11] -
             e[5] * e[3] * e[10] - e[9] * e[2] * e[7] + e[9] * e[3] * e[6];

    inv[7] = e[0] * e[6] * e[11] - e[0] * e[7] * e[10] - e[4] * e[2] * e[11] +
             e[4] * e[3] * e[10] + e[8] * e[2] * e[7] - e[8] * e[3] * e[6];

    inv[11] = -e[0] * e[5] * e[11] + e[0] * e[7] * e[9] + e[4] * e[1] * e[11] -
              e[4] * e[3] * e[9] - e[8] * e[1] * e[7] + e[8] * e[3] * e[5];

    inv[15] = e[0] * e[5] * e[10] - e[0] * e[6] * e[9] - e[4] * e[1] * e[10] +
              e[4] * e[2] * e[9] + e[8] * e[1] * e[6] - e[8] * e[2] * e[5];

    det = e[0] * inv[0] + e[1] * inv[1] + e[2] * inv[2] + e[3] * inv[3];

    if (equal_f(det, 0)) {
        return Matrix4();
    }

    return inv / det;
}

Matrix4 Matrix4::operator*(const float t) const {
    return Matrix4(e[0] * t, e[1] * t, e[2] * t, e[3] * t, e[4] * t, e[5] * t,
                   e[6] * t, e[7] * t, e[8] * t, e[9] * t, e[10] * t, e[11] * t,
                   e[12] * t, e[13] * t, e[14] * t, e[15] * t);
}

Matrix4 Matrix4::operator/(const float t) const {
    if (equal_f(t, 0.0f) || equal_f(t, 1.0f)) {
        return (*this);
    }

    float temp = 1 / t;
    return (*this) * temp;
}

Vec4 Matrix4::operator*(const Vec4& other) const {
    return Vec4(e[0] * other.x() + e[4] * other.y() + e[8] * other.z() +
                    e[12] * other.w(),
                e[1] * other.x() + e[5] * other.y() + e[9] * other.z() +
                    e[13] * other.w(),
                e[2] * other.x() + e[6] * other.y() + e[10] * other.z() +
                    e[14] * other.w(),
                e[3] * other.x() + e[7] * other.y() + e[11] * other.z() +
                    e[15] * other.w());
}