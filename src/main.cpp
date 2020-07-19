#include <iostream>
#include <memory>

#define STB_IMAGE_IMPLEMENTATION
#include "camera.hpp"
#include "color.hpp"
#include "hittable.hpp"
#include "material.hpp"
#include "mesh.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "texture.hpp"
#include "util.hpp"

HittableList get_scene() {
    HittableList world;

    auto diffuse_light = std::make_shared<DiffuseLight>(
        std::make_shared<SolidColor>(10, 10, 10));
    world.add(std::make_shared<Sphere>(Vec3(0, 5, 0), 1, diffuse_light));

    auto checker = std::make_shared<CheckerTexture>(
        std::make_shared<SolidColor>(0.2, 0.3, 0.1),
        std::make_shared<SolidColor>(0.9, 0.9, 0.9));
    auto ground_material = std::make_shared<Lambertian>(checker);
    world.add(
        std::make_shared<Sphere>(Vec3(0, -1000, 0), 1000, ground_material));

    auto material1 = std::make_shared<Dielectric>(1.5);
    world.add(std::make_shared<Sphere>(Vec3(-2.5, 1, 0), 1.0, material1));

    auto earth_texture = std::make_shared<ImageTexture>("earth.jpg");
    auto earth_surface = std::make_shared<Lambertian>(earth_texture);
    world.add(std::make_shared<Sphere>(Vec3(0, 1, 0), 1.0, earth_surface));

    auto material3 = std::make_shared<Metal>(
        std::make_shared<SolidColor>(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<Sphere>(Vec3(2.5, 1, 0), 1.0, material3));

    world.build();

    return world;
}

HittableList get_model_scene() {
    HittableList world;

    auto checker = std::make_shared<CheckerTexture>(
        std::make_shared<SolidColor>(0.2, 0.3, 0.1),
        std::make_shared<SolidColor>(0.9, 0.9, 0.9));
    auto ground_material = std::make_shared<Lambertian>(checker);
    world.add(
        std::make_shared<Sphere>(Vec3(0, -1005, 0), 1000, ground_material));

    auto earth_texture = std::make_shared<ImageTexture>("earth.jpg");
    auto earth_surface = std::make_shared<Lambertian>(earth_texture);
    world.add(std::make_shared<Sphere>(Vec3(-2, 2, 0), 1.0, earth_surface));

    auto model = std::make_shared<Model>("tree.obj");
    auto tree_texture = std::make_shared<ImageTexture>("tree.png");
    auto tree_surface = std::make_shared<Lambertian>(tree_texture);
    model->material = tree_surface;
    world.add(model);

    world.build();

    return world;
}

Vec3 ray_color(const Ray& ray, const Vec3& background_color,
               const Hittable& world, int depth) {
    if (depth <= 0) {
        return Vec3(0, 0, 0);
    }

    HitRecord record;

    if (!world.hit(ray, 0.001, infinity, record)) {
        return background_color;
    }

    Ray scattered;
    Vec3 attenuation;
    Vec3 emitted = record.material->emitted(record.position, record.texcoord);

    if (!record.material->scatter(ray, record, attenuation, scattered)) {
        return emitted;
    }

    return emitted + attenuation * ray_color(scattered, background_color, world,
                                             depth - 1);
}

int main() {
    double aspect_ratio = 16.0 / 9.0;

    Vec3 lookfrom(0, 2, 12);
    Vec3 lookat(0, 1, -1);
    Vec3 vup(0, 1, 0);
    double dist_to_focus = (lookfrom - lookat).length();
    double aperture = 2.0;

    Camera camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture,
                  dist_to_focus);

    int image_width = 200;
    int image_height = static_cast<int>(image_width / aspect_ratio);
    int samples_per_pixel = 100;
    int max_depth = 50;

    std::cout << "P3" << std::endl
              << image_width << ' ' << image_height << std::endl
              << "255" << std::endl;

    HittableList world = get_model_scene();
    Vec3 background_color(0.7, 0.7, 0.7);

    std::vector<Vec3> pixel_colors;

    for (int j = image_height - 1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {
            Vec3 pixel_color(0, 0, 0);

            for (int s = 0; s < samples_per_pixel; ++s) {
                double u = (i + random_double()) / (image_width - 1);
                double v = (j + random_double()) / (image_height - 1);
                Ray ray = camera.get_ray(u, v);
                pixel_color +=
                    ray_color(ray, background_color, world, max_depth);
            }

            pixel_colors.emplace_back(pixel_color);
        }
    }

    for (const Vec3& pixel_color : pixel_colors) {
        write_color(std::cout, pixel_color, samples_per_pixel);
    }
}