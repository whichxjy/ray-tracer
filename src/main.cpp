#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "camera.hpp"
#include "color.hpp"
#include "hittable.hpp"
#include "image/writer.hpp"
#include "material.hpp"
#include "mesh.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "texture.hpp"
#include "util.hpp"

HittableList get_sphere_scene() {
    HittableList world;

    auto light = ([]() {
        auto texture = std::make_shared<SolidColor>(10, 10, 10);
        auto material = std::make_shared<DiffuseLight>(texture);
        return std::make_shared<Sphere>(Vec3(0, 5, 0), 1, material);
    })();
    world.add(light);

    auto ground = ([]() {
        auto texture = std::make_shared<CheckerTexture>(
            std::make_shared<SolidColor>(0.2, 0.3, 0.1),
            std::make_shared<SolidColor>(0.9, 0.9, 0.9));
        auto material = std::make_shared<Lambertian>(texture);
        return std::make_shared<Sphere>(Vec3(0, -1000, 0), 1000, material);
    })();
    world.add(ground);

    auto sphere1 = ([]() {
        auto material = std::make_shared<Dielectric>(1.5);
        return std::make_shared<Sphere>(Vec3(-2.5, 1, 0), 1.0, material);
    })();
    world.add(sphere1);

    auto sphere2 = ([]() {
        auto texture = std::make_shared<ImageTexture>("assets/earth.jpg");
        auto material = std::make_shared<Lambertian>(texture);
        return std::make_shared<Sphere>(Vec3(0, 1, 0), 1.0, material);
    })();
    world.add(sphere2);

    auto sphere3 = ([]() {
        auto texture = std::make_shared<SolidColor>(0.7, 0.6, 0.5);
        auto material = std::make_shared<Metal>(texture, 0.0);
        return std::make_shared<Sphere>(Vec3(2.5, 1, 0), 1.0, material);
    })();
    world.add(sphere3);

    return world;
}

HittableList get_model_scene() {
    HittableList world;

    auto light = ([]() {
        auto texture = std::make_shared<SolidColor>(5, 5, 5);
        auto material = std::make_shared<DiffuseLight>(texture);
        return std::make_shared<Sphere>(Vec3(0, 5, 2), 1, material);
    })();
    world.add(light);

    auto ground = ([]() {
        auto texture = std::make_shared<CheckerTexture>(
            std::make_shared<SolidColor>(0.2, 0.3, 0.1),
            std::make_shared<SolidColor>(0.9, 0.9, 0.9));
        auto material = std::make_shared<Lambertian>(texture);
        return std::make_shared<Sphere>(Vec3(0, -1000, 0), 1000, material);
    })();
    world.add(ground);

    auto sphere1 = ([]() {
        auto texture = std::make_shared<SolidColor>(0.8, 0.6, 0.2);
        auto material = std::make_shared<Metal>(texture, 0.0);
        return std::make_shared<Sphere>(Vec3(-3, 1, 0), 1, material);
    })();
    world.add(sphere1);

    auto tree1 = ([]() {
        auto texture = std::make_shared<ImageTexture>("assets/tree.png");
        auto material = std::make_shared<Lambertian>(texture);
        return std::make_shared<Model>("assets/tree.obj", Vec3(-0.7, 1, 0),
                                       material);
    })();
    world.add(tree1);

    auto tree2 = ([]() {
        auto texture = std::make_shared<ImageTexture>("assets/tree.png");
        auto material = std::make_shared<Lambertian>(texture);
        return std::make_shared<Model>("assets/tree.obj", Vec3(0.7, 1, 0),
                                       material);
    })();
    world.add(tree2);

    auto sphere2 = ([]() {
        auto material = std::make_shared<Dielectric>(1.5);
        return std::make_shared<Sphere>(Vec3(3, 1, 0), 1, material);
    })();
    world.add(sphere2);

    auto sphere3 = ([]() {
        auto texture = std::make_shared<ImageTexture>("assets/earth.jpg");
        auto material = std::make_shared<Lambertian>(texture);
        return std::make_shared<Sphere>(Vec3(-1.5, 0.5, 5), 0.5, material);
    })();
    world.add(sphere3);

    auto sphere4 = ([]() {
        auto texture = std::make_shared<SolidColor>(0.4, 0.7, 0.9);
        auto material = std::make_shared<Lambertian>(texture);
        return std::make_shared<Sphere>(Vec3(1.5, 0.5, 5), 0.5, material);
    })();
    world.add(sphere4);

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

    Vec3 lookfrom(0, 2, 14);
    Vec3 lookat(0, 0.5, -1);
    Vec3 vup(0, 1, 0);
    double vfov = 20;
    double dist_to_focus = (lookfrom - lookat).length();
    double aperture = 2.0;

    Camera camera(lookfrom, lookat, vup, vfov, aspect_ratio, aperture,
                  dist_to_focus);

    int image_width = 600;
    int image_height = static_cast<int>(image_width / aspect_ratio);
    int samples_per_pixel = 10;
    int max_depth = 50;

    HittableList world = get_model_scene();
    world.build();

    Vec3 background_color(0.5, 0.5, 0.5);

    std::vector<std::thread> threads;

    const int channel = 4;

    uint8_t* pixels = new uint8_t[image_width * image_height * channel];

    for (int row = 0; row < image_height - 1; row++) {
        for (int col = 0; col < image_width - 1; col++) {
            threads.emplace_back([=, &camera, &background_color, &world,
                                  &pixels]() {
                Vec3 pixel_color(0, 0, 0);

                for (int s = 0; s < samples_per_pixel; ++s) {
                    double u = (col + random_double()) / (image_width - 1);
                    double v = (row + random_double()) / (image_height - 1);
                    Ray ray = camera.get_ray(u, v);
                    pixel_color +=
                        ray_color(ray, background_color, world, max_depth);
                }

                Vec3 color = get_color(pixel_color, samples_per_pixel);

                size_t index =
                    ((image_height - row - 1) * image_width + col) * channel;

                pixels[index + 0] = color.x();
                pixels[index + 1] = color.y();
                pixels[index + 2] = color.z();
                pixels[index + 3] = 255;
            });
        }
    }

    for (std::thread& thread : threads) {
        thread.join();
    }

    stbi_write_png("result.png", image_width, image_height, channel, pixels,
                   image_width * channel);

    delete[] pixels;
}