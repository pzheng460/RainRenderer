#ifndef GEOMETRYMODEL_H
#define GEOMETRYMODEL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

enum GeometryType {
    SPHERE,
    CUBE,
    QUAD,
    PLANE
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
    std::vector<unsigned int> textures;
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

class GeometryFactory {
public:
    static std::unique_ptr<Geometry> createGeometry(GeometryType geometryType);
};
#endif // GEOMETRYMODEL_H
