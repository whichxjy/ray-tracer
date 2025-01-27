#ifndef _HITTABLE_HPP_
#define _HITTABLE_HPP_

#include <memory>
#include <vector>

#include "box.hpp"
#include "ray.hpp"
#include "vec/vec2.hpp"

class Material;
class BVHNode;

class HitRecord {
public:
    Vec3 position;
    Vec3 normal;
    Vec2 texcoord;
    double t;
    bool front_face;
    std::shared_ptr<Material> material;

    void set_face_normal(const Ray& ray, const Vec3& outward_normal);
};

class Hittable {
public:
    virtual bool hit(const Ray& ray, double t_min, double t_max,
                     HitRecord& record) const = 0;

    virtual bool bounding_box(double t_min, double t_max,
                              Box& output_box) const = 0;
};

class HittableList : public Hittable {
public:
    HittableList() {}
    HittableList(std::shared_ptr<Hittable> object) { add(object); }

    void clear() { objects.clear(); }
    void add(std::shared_ptr<Hittable> object) { objects.push_back(object); }

    virtual bool hit(const Ray& ray, double t_min, double t_max,
                     HitRecord& record) const;

    virtual bool bounding_box(double t_min, double t_max,
                              Box& output_box) const;

    void build();

public:
    std::vector<std::shared_ptr<Hittable>> objects;
    std::shared_ptr<BVHNode> bvh_tree_root;
};

#endif