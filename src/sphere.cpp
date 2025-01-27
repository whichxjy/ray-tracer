#include "sphere.hpp"

bool Sphere::hit(const Ray& ray, double t_min, double t_max,
                 HitRecord& record) const {
    Vec3 oc = ray.origin - center;
    double a = ray.direction.squared_length();
    double half_b = dot(oc, ray.direction);
    double c = oc.squared_length() - radius * radius;
    double discriminant = half_b * half_b - a * c;

    if (discriminant > 0) {
        double root = sqrt(discriminant);
        double temp = (-half_b - root) / a;

        if (temp < t_max && temp > t_min) {
            record.t = temp;
            record.position = ray.at(record.t);
            get_sphere_texcoord((record.position - center) / radius,
                                record.texcoord);
            Vec3 outward_normal = (record.position - center) / radius;
            record.set_face_normal(ray, outward_normal);
            record.material = material;
            return true;
        }

        temp = (-half_b + root) / a;

        if (temp < t_max && temp > t_min) {
            record.t = temp;
            record.position = ray.at(record.t);
            get_sphere_texcoord((record.position - center) / radius,
                                record.texcoord);
            Vec3 outward_normal = (record.position - center) / radius;
            record.set_face_normal(ray, outward_normal);
            record.material = material;
            return true;
        }
    }

    return false;
}

bool Sphere::bounding_box(double t_min, double t_max, Box& output_box) const {
    output_box = Box(center - Vec3(radius, radius, radius),
                     center + Vec3(radius, radius, radius));
    return true;
}

void get_sphere_texcoord(const Vec3& position, Vec2& texcoord) {
    double phi = atan2(position.z(), position.x());
    double theta = asin(position.y());
    texcoord[0] = 1 - (phi + pi) / (2 * pi);
    texcoord[1] = (theta + pi / 2) / pi;
}