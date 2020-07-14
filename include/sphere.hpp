#ifndef _SPHERE_HPP_
#define _SPHERE_HPP_

#include "hittable.hpp"
#include "vec3.hpp"

class Sphere : public Hittable {
public:
    Sphere() {}
    Sphere(Vec3 center, double radius, std::shared_ptr<Material> material)
        : center(center), radius(radius), material(material){};

    virtual bool hit(const Ray& ray, double tmin, double tmax,
                     HitRecord& record) const;

public:
    Vec3 center;
    double radius;
    std::shared_ptr<Material> material;
};

#endif