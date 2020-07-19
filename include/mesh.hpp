#ifndef _MESH_HPP_
#define _MESH_HPP_

#include <string>
#include <vector>

#include "hittable.hpp"
#include "vertex.hpp"

class Model : public Hittable {
public:
    Model() = default;
    Model(const std::string& path);

    virtual bool hit(const Ray& ray, double t_min, double t_max,
                     HitRecord& record) const;

    virtual bool bounding_box(double t_min, double t_max,
                              Box& output_box) const;

    std::shared_ptr<Material> material;
    Box box;

private:
    std::vector<Vertex> vertices;
    std::vector<int> indices;
    std::vector<Vec3> face_normals;

    void load_model(const std::string& path);

    void preprocess();

    bool hit_triangle(const Ray& ray, double t_min, double t_max,
                      HitRecord& record, const Vertex& p0, const Vertex& p1,
                      const Vertex& p2, const Vec3& normal) const;
};

#endif