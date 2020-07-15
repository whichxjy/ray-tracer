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

private:
    std::vector<Vertex> vertices;
    std::vector<int> indices;
};

#endif