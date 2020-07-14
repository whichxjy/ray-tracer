#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_

#include "vec3.hpp"

class Texture {
public:
    virtual Vec3 value(double u, double v, const Vec3& p) const = 0;
};

class SolidColor : public Texture {
public:
    SolidColor(Vec3 color) : color(color) {}

    SolidColor(double r, double g, double b) : SolidColor(Vec3(r, g, b)) {}

    virtual Vec3 value(double u, double v, const Vec3& p) const {
        return color;
    }

private:
    Vec3 color;
};

class CheckerTexture : public Texture {
public:
    CheckerTexture() {}
    CheckerTexture(std::shared_ptr<Texture> lhs, std::shared_ptr<Texture> rhs)
        : even(lhs), odd(rhs) {}

    virtual Vec3 value(double u, double v, const Vec3& p) const {
        auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());

        if (sines < 0) {
            return odd->value(u, v, p);
        }

        return even->value(u, v, p);
    }

private:
    std::shared_ptr<Texture> odd;
    std::shared_ptr<Texture> even;
};

#endif