#ifndef GEOMETRYMODEL_H
#define GEOMETRYMODEL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include "Common.h"

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

class Geometry {
public:
    virtual ~Geometry() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    virtual void createVAO() = 0;
    virtual void draw() = 0;

    unsigned int VAO = 0;
    unsigned int VBO = 0;
};

class Sphere final : public Geometry {
public:
    virtual ~Sphere() {
        glDeleteBuffers(1, &EBO);
    }
    void createVAO() override;
    void draw() override;
private:
    unsigned int EBO = 0;
    int indexCount = 0;
};

class Cube final : public Geometry {
public:
    void createVAO() override;
    void draw() override;
};

class Quad final : public Geometry {
public:
    void createVAO() override;
    void draw() override;
};

class Plane final : public Geometry {
public:
    void createVAO() override;
    void draw() override;
};

class ModelMesh final : public Geometry {
public:
    void setVerticesAndIndices(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) {
        this->vertices = vertices;
        this->indices = indices;
    }
    void createVAO() override;
    void draw() override;
private:
    unsigned int EBO = 0;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};

class GeometryFactory {
public:
    static Geometry* createGeometry(GeometryType geometryType);
};
#endif // GEOMETRYMODEL_H
