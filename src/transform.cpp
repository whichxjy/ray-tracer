#include "transform.hpp"

void Transform::translate(const Vec3& dt) { translation += dt; }

Matrix4 Transform::to_matrix() {
    Matrix4 translation_matrix;
    translation_matrix.set_translation(translation);
    return translation_matrix;
}

Matrix4 Transform::to_inv_matrix() {
    Matrix4 translation_matrix;
    translation_matrix.set_translation(translation);
    return translation_matrix.inverse_transpose();
}