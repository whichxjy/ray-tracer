#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_

#include "image.hpp"
#include "vec/vec3.hpp"

class Texture {
public:
    virtual Vec3 value(const Vec3& position, const Vec2& texcoord) const = 0;
};

class SolidColor : public Texture {
public:
    SolidColor(Vec3 color) : color(color) {}

    SolidColor(double r, double g, double b) : SolidColor(Vec3(r, g, b)) {}

    virtual Vec3 value(const Vec3& position, const Vec2& texcoord) const {
        return color;
    }

private:
    Vec3 color;
};

class CheckerTexture : public Texture {
public:
    CheckerTexture(std::shared_ptr<Texture> lhs, std::shared_ptr<Texture> rhs)
        : even(lhs), odd(rhs) {}

    virtual Vec3 value(const Vec3& position, const Vec2& texcoord) const {
        double u = texcoord.x();
        double v = texcoord.y();

        auto sines = sin(10 * position.x()) * sin(10 * position.y()) *
                     sin(10 * position.z());

        if (sines < 0) {
            return odd->value(position, texcoord);
        }

        return even->value(position, texcoord);
    }

private:
    std::shared_ptr<Texture> odd;
    std::shared_ptr<Texture> even;
};

class ImageTexture : public Texture {
public:
    ImageTexture(const std::string& path) {
        data = stbi_load(path.c_str(), &width, &height, &channel, 0);
        if (data == nullptr) {
            std::cerr << "Failed to load the image " << path.c_str()
                      << std::endl;
        }
    }
    virtual ~ImageTexture() { free(data); }

    virtual Vec3 value(const Vec3& position, const Vec2& texcoord) const {
        if (data == nullptr) {
            return Vec3(0, 1, 1);
        }

        double u = clamp(texcoord.x(), 0.0, 1.0);
        double v = 1.0 - clamp(texcoord.y(), 0.0, 1.0);

        int i = static_cast<int>(u * width);
        int j = static_cast<int>(v * height);

        if (i >= width) {
            i = width - 1;
        }
        if (j >= height) {
            j = height - 1;
        }

        const double color_scale = 1.0 / 255.0;
        int index = (j * width + i) * channel;

        double r = color_scale * data[index + 0];
        double g = color_scale * data[index + 1];
        double b = color_scale * data[index + 2];

        return Vec3(r, g, b);
    }

private:
    unsigned char* data;
    int width;
    int height;
    int channel;
};

#endif