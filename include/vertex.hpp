#ifndef _VERTEX_HPP_
#define _VERTEX_HPP_

#include "vec/vec2.hpp"
#include "vec/vec3.hpp"

struct Vertex {
    Vec3 position;
    Vec3 normal;
    Vec2 texcoord;

    Vertex(const Vec3& position, const Vec3& normal, const Vec2& texcoord)
        : position(position), normal(normal), texcoord(texcoord) {}
};

#endif