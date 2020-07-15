#ifndef _MATERIAL_HPP_
#define _MATERIAL_HPP_

#include <memory>

#include "hittable.hpp"
#include "ray.hpp"
#include "texture.hpp"

class Material {
public:
    virtual bool scatter(const Ray& ray_in, const HitRecord& record,
                         Vec3& attenuation, Ray& scattered) const = 0;

    virtual Vec3 emitted(const Vec3& position, const Vec2& texcoord) const {
        return Vec3(0, 0, 0);
    }
};

class Lambertian : public Material {
public:
    Lambertian(std::shared_ptr<Texture> albedo) : albedo(albedo) {}

    virtual bool scatter(const Ray& ray_in, const HitRecord& record,
                         Vec3& attenuation, Ray& scattered) const;

private:
    std::shared_ptr<Texture> albedo;
};

class Metal : public Material {
public:
    Metal(std::shared_ptr<Texture> albedo, double fuzz)
        : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    virtual bool scatter(const Ray& ray_in, const HitRecord& record,
                         Vec3& attenuation, Ray& scattered) const;

private:
    std::shared_ptr<Texture> albedo;
    double fuzz;
};

double schlick(double cosine, double ref_idx);

class Dielectric : public Material {
public:
    Dielectric(double ref_idx) : ref_idx(ref_idx) {}

    virtual bool scatter(const Ray& ray_in, const HitRecord& record,
                         Vec3& attenuation, Ray& scattered) const;

private:
    double ref_idx;
};

class DiffuseLight : public Material {
public:
    DiffuseLight(std::shared_ptr<Texture> a) : emit(a) {}

    virtual bool scatter(const Ray& r_in, const HitRecord& rec,
                         Vec3& attenuation, Ray& scattered) const {
        return false;
    }

    virtual Vec3 emitted(const Vec3& position, const Vec2& texcoord) const {
        return emit->value(position, texcoord);
    }

public:
    std::shared_ptr<Texture> emit;
};

#endif