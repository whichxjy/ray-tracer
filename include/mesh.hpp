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

    std::shared_ptr<Material> material;

private:
    std::vector<Vertex> vertices;
    std::vector<int> indices;
    std::vector<Vec3> face_normals;

    bool hitTriangle(const Ray& ray, double t_min, double t_max,
                     HitRecord& record, const Vertex& p0, const Vertex& p1,
                     const Vertex& p2, const Vec3& normal) const;
};

#endif