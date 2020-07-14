#include "hittable.hpp"

void HitRecord::set_face_normal(const Ray& ray, const Vec3& outward_normal) {
    front_face = dot(ray.direction, outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
}

bool HittableList::hit(const Ray& ray, double t_min, double t_max,
                       HitRecord& record) const {
    HitRecord temp_record;
    bool hit_anything = false;
    double closest_so_far = t_max;

    for (const auto& object : objects) {
        if (object->hit(ray, t_min, closest_so_far, temp_record)) {
            hit_anything = true;
            closest_so_far = temp_record.t;
            record = temp_record;
        }
    }

    return hit_anything;
}
