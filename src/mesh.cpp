#include "mesh.hpp"

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <cassert>
#include <iostream>

#include "util.hpp"

Model::Model(const std::string& path) {
    load_model(path);
    preprocess();
}

bool Model::hit(const Ray& ray, double t_min, double t_max,
                HitRecord& record) const {
    HitRecord temp_record;
    bool hit_anything = false;
    double closest_so_far = t_max;

    assert(indices.size() % 3 == 0);

    for (int i = 0; i < indices.size(); i += 3) {
        int index1 = indices[i + 0];
        int index2 = indices[i + 1];
        int index3 = indices[i + 2];

        if (hit_triangle(ray, t_min, closest_so_far, temp_record,
                         vertices[index1], vertices[index2], vertices[index3],
                         face_normals[i / 3])) {
            hit_anything = true;
            closest_so_far = temp_record.t;
            record = temp_record;
        }
    }

    return hit_anything;
}

bool Model::bounding_box(double t_min, double t_max, Box& output_box) const {
    output_box = box;
    return true;
}

void Model::load_model(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        path, aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                  aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

    if (!scene) {
        std::cerr << "Fail to load model: " << path << std::endl;
        return;
    }

    assert(scene->mNumMeshes == 1);

    const aiMesh* mesh = scene->mMeshes[0];

    vertices.reserve(mesh->mNumVertices);

    Vec3 min(infinity, infinity, infinity);
    Vec3 max(-infinity, -infinity, -infinity);

    for (int i = 0; i < mesh->mNumVertices; i++) {
        const aiVector3D* position = &(mesh->mVertices[i]);
        const aiVector3D* normal = &(mesh->mNormals[i]);
        const aiVector3D* texcoord = &(mesh->mTextureCoords[0][i]);

        min[0] = fmin(min[0], position->x);
        min[1] = fmin(min[1], position->y);
        min[2] = fmin(min[2], position->z);

        max[0] = fmax(max[0], position->x);
        max[1] = fmax(max[1], position->y);
        max[2] = fmax(max[2], position->z);

        vertices.emplace_back(Vec3(position->x, position->y, position->z),
                              Vec3(normal->x, normal->y, normal->z),
                              Vec2(texcoord->x, texcoord->y));
    }

    box = Box(min, max);

    indices.reserve(3 * mesh->mNumVertices);

    for (int i = 0; i < mesh->mNumFaces; i++) {
        const aiFace& face = mesh->mFaces[i];

        assert(face.mNumIndices == 3);

        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }
}

void Model::preprocess() {
    for (int i = 0; i < indices.size(); i += 3) {
        int index0 = indices[i + 0];
        int index1 = indices[i + 1];
        int index2 = indices[i + 2];

        Vec3 face_normal =
            cross(vertices[index1].position - vertices[index0].position,
                  vertices[index2].position - vertices[index0].position);

        face_normals.push_back(unit_vector(face_normal));
    }
}

bool Model::hit_triangle(const Ray& ray, double t_min, double t_max,
                         HitRecord& record, const Vertex& p0, const Vertex& p1,
                         const Vertex& p2, const Vec3& normal) const {
    double n_dot_dir = dot(normal, ray.direction);

    if (equal_f(n_dot_dir, 0)) {
        return false;
    }

    double d = -dot(normal, p0.position);
    double t = -(dot(normal, ray.origin) + d) / n_dot_dir;

    if (t < t_min || t > t_max) {
        return false;
    }

    record.t = t;
    record.position = ray.at(t);
    record.material = material;

    Vec3 r = record.position - p0.position;
    Vec3 q1 = p1.position - p0.position;
    Vec3 q2 = p2.position - p0.position;

    double q1_squaredLen = q1.squared_length();
    double q2_squaredLen = q2.squared_length();
    double q1_dot_q2 = dot(q1, q2);
    double r_dot_q1 = dot(r, q1);
    double r_dot_q2 = dot(r, q2);

    double determinant =
        1.0 / (q1_squaredLen * q2_squaredLen - q1_dot_q2 * q1_dot_q2);

    double omega1 =
        determinant * (q2_squaredLen * r_dot_q1 - q1_dot_q2 * r_dot_q2);
    double omega2 =
        determinant * (-q1_dot_q2 * r_dot_q1 + q1_squaredLen * r_dot_q2);

    if (omega1 + omega2 > 1 || omega1 < 0 || omega2 < 0) {
        return false;
    }

    record.normal = p0.normal * (1.0 - omega1 - omega2) + p1.normal * omega1 +
                    p2.normal * omega2;
    record.texcoord = p0.texcoord * (1.0 - omega1 - omega2) +
                      p1.texcoord * omega1 + p2.texcoord * omega2;

    if (dot(record.normal, ray.direction) > 0) {
        record.normal = -record.normal;
    }

    return true;
}