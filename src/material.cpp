#include "material.hpp"

bool Lambertian::scatter(const Ray& ray_in, const HitRecord& record,
                         Vec3& attenuation, Ray& scattered) const {
    Vec3 scatter_direction = record.normal + random_unit_vector();
    scattered = Ray(record.position, scatter_direction);
    attenuation = albedo->value(record.position, record.texcoord);
    return true;
}

bool Metal::scatter(const Ray& ray_in, const HitRecord& record,
                    Vec3& attenuation, Ray& scattered) const {
    Vec3 reflected = reflect(unit_vector(ray_in.direction), record.normal);
    scattered =
        Ray(record.position, reflected + fuzz * random_in_unit_sphere());
    attenuation = albedo->value(record.position, record.texcoord);
    return (dot(scattered.direction, record.normal) > 0);
}

bool Dielectric::scatter(const Ray& ray_in, const HitRecord& record,
                         Vec3& attenuation, Ray& scattered) const {
    attenuation = Vec3(1.0, 1.0, 1.0);

    double etai_over_etat;

    if (record.front_face) {
        etai_over_etat = 1.0 / ref_idx;
    } else {
        etai_over_etat = ref_idx;
    }

    Vec3 unit_direction = unit_vector(ray_in.direction);
    double cos_theta = fmin(dot(-unit_direction, record.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    if (etai_over_etat * sin_theta > 1.0) {
        Vec3 reflected = reflect(unit_direction, record.normal);
        scattered = Ray(record.position, reflected);
        return true;
    }

    double reflect_prob = schlick(cos_theta, etai_over_etat);
    if (random_double() < reflect_prob) {
        Vec3 reflected = reflect(unit_direction, record.normal);
        scattered = Ray(record.position, reflected);
        return true;
    }

    Vec3 refracted = refract(unit_direction, record.normal, etai_over_etat);
    scattered = Ray(record.position, refracted);
    return true;
}

double schlick(double cosine, double ref_idx) {
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}