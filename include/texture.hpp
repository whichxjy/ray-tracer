#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_

#include "vec3.hpp"

class Texture {
public:
    virtual Vec3 value(double u, double v, const Vec3& p) const = 0;
};

class SolidColor : public Texture {
public:
    SolidColor() {}
    SolidColor(Vec3 color) : color(color) {}

    SolidColor(double r, double g, double b) : SolidColor(Vec3(r, g, b)) {}

    virtual Vec3 value(double u, double v, const Vec3& p) const {
        return color;
    }

private:
    Vec3 color;
};

#endif