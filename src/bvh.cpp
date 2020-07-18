#include "bvh.hpp"

#include <algorithm>

#include "util.hpp"

BVHNode::BVHNode(std::vector<std::shared_ptr<Hittable>>& objects, size_t left,
                 size_t right, double t_min, double t_max) {
    double random_num = random_double(0, 3);

    int axis;

    if (random_num < 1) {
        axis = 0;
    } else if (random_num < 2) {
        axis = 1;
    } else {
        axis = 2;
    }

    auto compare = (axis == 0) ? box_x_compare
                               : (axis == 1) ? box_y_compare : box_z_compare;

    size_t object_span = right - left + 1;

    if (object_span == 1) {
        left_node = objects[left];
        right_node = objects[left];
    } else if (object_span == 2) {
        if (compare(objects[left], objects[left + 1])) {
            left_node = objects[left];
            right_node = objects[left + 1];
        } else {
            left_node = objects[left + 1];
            right_node = objects[left];
        }
    } else {
        std::sort(objects.begin() + left, objects.begin() + right + 1, compare);

        size_t mid = left + object_span / 2;

        left_node = std::make_shared<BVHNode>(objects, left, mid, t_min, t_max);
        right_node =
            std::make_shared<BVHNode>(objects, mid + 1, right, t_min, t_max);
    }

    Box box_left;
    Box box_right;

    if (!left_node->bounding_box(t_min, t_max, box_left) ||
        !right_node->bounding_box(t_min, t_max, box_right)) {
        std::cerr << "No bounding box in BVH node constructor." << std::endl;
    }

    box = surrounding_box(box_left, box_right);
}

bool BVHNode::hit(const Ray& ray, double t_min, double t_max,
                  HitRecord& record) const {
    if (!box.hit(ray, t_min, t_max)) {
        return false;
    }

    bool hit_left = left_node->hit(ray, t_min, t_max, record);
    bool hit_right =
        right_node->hit(ray, t_min, hit_left ? record.t : t_max, record);

    return hit_left || hit_right;
}

bool BVHNode::bounding_box(double t_min, double t_max, Box& output_box) const {
    output_box = box;
    return true;
}

bool box_compare(const std::shared_ptr<Hittable> lhs,
                 const std::shared_ptr<Hittable> rhs, int axis) {
    Box lhs_box;
    Box rhs_box;

    if (!lhs->bounding_box(0, 0, lhs_box) ||
        !rhs->bounding_box(0, 0, rhs_box)) {
        std::cerr << "No bounding box in this node." << std::endl;
    }

    return lhs_box.min.e[axis] < rhs_box.min.e[axis];
}

bool box_x_compare(const std::shared_ptr<Hittable> lhs,
                   const std::shared_ptr<Hittable> rhs) {
    return box_compare(lhs, rhs, 0);
}

bool box_y_compare(const std::shared_ptr<Hittable> lhs,
                   const std::shared_ptr<Hittable> rhs) {
    return box_compare(lhs, rhs, 1);
}

bool box_z_compare(const std::shared_ptr<Hittable> lhs,
                   const std::shared_ptr<Hittable> rhs) {
    return box_compare(lhs, rhs, 2);
}