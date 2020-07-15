#include "mesh.hpp"

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <cassert>
#include <iostream>

Model::Model(const std::string& path) {
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

    for (int i = 0; i < mesh->mNumVertices; i++) {
        const aiVector3D* position = &(mesh->mVertices[i]);
        const aiVector3D* normal = &(mesh->mNormals[i]);
        const aiVector3D* texcoord = &(mesh->mTextureCoords[0][i]);

        vertices.emplace_back(Vec3(position->x, position->y, position->z),
                              Vec3(normal->x, normal->y, normal->z),
                              Vec2(texcoord->x, texcoord->y));
    }

    indices.reserve(3 * mesh->mNumVertices);

    for (int i = 0; i < mesh->mNumFaces; i++) {
        const aiFace& face = mesh->mFaces[i];

        assert(face.mNumIndices == 3);

        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }
}

bool Model::hit(const Ray& ray, double t_min, double t_max,
                HitRecord& record) const {
    return true;
}