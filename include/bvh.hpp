#ifndef _BVH_HPP_
#define _BVH_HPP_

#include <memory>

#include "box.hpp"
#include "hittable.hpp"

class BVHNode : public Hittable {
public:
    BVHNode(std::vector<std::shared_ptr<Hittable>>& objects, double t_min,
            double t_max)
        : BVHNode(objects, 0, objects.size() - 1, t_min, t_max) {}

    BVHNode(std::vector<std::shared_ptr<Hittable>>& objects, size_t left,
            size_t right, double t_min, double t_max);

    virtual bool hit(const Ray& ray, double t_min, double t_max,
                     HitRecord& record) const;

    virtual bool bounding_box(double t_min, double t_max,
                              Box& output_box) const;

    std::shared_ptr<Hittable> left_node;
    std::shared_ptr<Hittable> right_node;
    Box box;
};

bool box_compare(const std::shared_ptr<Hittable> lhs,
                 const std::shared_ptr<Hittable> rhs, int axis);

bool box_x_compare(const std::shared_ptr<Hittable> lhs,
                   const std::shared_ptr<Hittable> rhs);

bool box_y_compare(const std::shared_ptr<Hittable> lhs,
                   const std::shared_ptr<Hittable> rhs);

bool box_z_compare(const std::shared_ptr<Hittable> lhs,
                   const std::shared_ptr<Hittable> rhs);

#endif