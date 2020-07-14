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
            record.p = ray.at(record.t);

            double u;
            double v;
            get_sphere_uv(record.p, u, v);
            record.u = u;
            record.v = v;

            Vec3 outward_normal = (record.p - center) / radius;
            record.set_face_normal(ray, outward_normal);
            record.material = material;
            return true;
        }

        temp = (-half_b + root) / a;

        if (temp < t_max && temp > t_min) {
            record.t = temp;
            record.p = ray.at(record.t);

            double u;
            double v;
            get_sphere_uv(record.p, u, v);
            record.u = u;
            record.v = v;

            Vec3 outward_normal = (record.p - center) / radius;
            record.set_face_normal(ray, outward_normal);
            record.material = material;
            return true;
        }
    }

    return false;
}

void get_sphere_uv(const Vec3& p, double& u, double& v) {
    auto phi = atan2(p.z(), p.x());
    auto theta = asin(p.y());
    u = 1 - (phi + pi) / (2 * pi);
    v = (theta + pi / 2) / pi;
}