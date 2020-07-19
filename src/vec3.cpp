#include "vec/vec3.hpp"

#include "vec/vec4.hpp"

Vec3::Vec3(const Vec4& other) {
    e[0] = other[0];
    e[1] = other[1];
    e[2] = other[2];
}