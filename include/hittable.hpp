#ifndef _HITTABLE_HPP_
#define _HITTABLE_HPP_

#include <memory>
#include <vector>

#include "ray.hpp"

class Material;

class HitRecord {
public:
    Vec3 p;
    double t;
    Vec3 normal;
    bool front_face;
    std::shared_ptr<Material> material;

    void set_face_normal(const Ray& ray, const Vec3& outward_normal);
};

class Hittable {
public:
    virtual bool hit(const Ray& ray, double t_min, double t_max,
                     HitRecord& record) const = 0;
};

class HittableList : public Hittable {
public:
    HittableList() {}
    HittableList(std::shared_ptr<Hittable> object) { add(object); }

    void clear() { objects.clear(); }
    void add(std::shared_ptr<Hittable> object) { objects.push_back(object); }

    virtual bool hit(const Ray& ray, double tmin, double tmax,
                     HitRecord& record) const;

public:
    std::vector<std::shared_ptr<Hittable>> objects;
};

#endif