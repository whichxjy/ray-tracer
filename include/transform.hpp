#ifndef _TRANSFORM_HPP_
#define _TRANSFORM_HPP_

#include "vec/matrix4.hpp"
#include "vec/vec3.hpp"

class Transform {
public:
    Vec3 translation;

    void translate(const Vec3& dt);

    Matrix4 to_matrix();
    Matrix4 to_inv_matrix();
};

#endif