#ifndef _SPHERE_HPP_
#define _SPHERE_HPP_

#include "hittable.hpp"
#include "vec/vec2.hpp"
#include "vec/vec3.hpp"

class Sphere : public Hittable {
public:
    Sphere() {}
    Sphere(Vec3 center, double radius, std::shared_ptr<Material> material)
        : center(center), radius(radius), material(material){};

    virtual bool hit(const Ray& ray, double t_min, double t_max,
                     HitRecord& record) const;

    virtual bool bounding_box(double t_min, double t_max,
                              Box& output_box) const;

public:
    Vec3 center;
    double radius;
    std::shared_ptr<Material> material;
};

void get_sphere_texcoord(const Vec3& position, Vec2& texcoord);

#endif