#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

#include "Shader.h"
#include "Texture.h"
#include "Geometry.h"

class Mesh {
public:
    // mesh Data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::shared_ptr<Texture>> textures;
    unsigned int VAO = 0;

    // constructor
    Mesh() = default;

    explicit Mesh(std::vector<Vertex>& vertices,
                  std::vector<unsigned int>& indices,
                  std::vector<std::shared_ptr<Texture>>& textures) :
        vertices(vertices), indices(indices), textures(textures) {
        auto geometryPtr = GeometryFactory::createGeometry(GeometryType::MODEL_MESH);
        geometry = std::shared_ptr<Geometry>(geometryPtr);
        std::dynamic_pointer_cast<ModelMesh>(geometry)->setVerticesAndIndices(vertices, indices);
        geometry->createVAO();
    }

    explicit Mesh(GeometryType geometryType) {
        auto geometryPtr = GeometryFactory::createGeometry(geometryType);
        geometry = std::shared_ptr<Geometry>(geometryPtr);
        geometry->createVAO();
    }

    // render the mesh
    void draw(Shader &shader, bool setTexture = false);

private:
    std::shared_ptr<Geometry> geometry;
};

#endif // MESH_H
