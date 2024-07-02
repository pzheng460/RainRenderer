#include "GeometryGenerator.h"
#include <glm/glm.hpp>
#include <vector>

AssimpModel::Model GeometryGenerator::generateSphere() {
    std::vector<AssimpModel::Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<AssimpModel::Texture> textures;

    const unsigned int X_SEGMENTS = 64;
    const unsigned int Y_SEGMENTS = 64;
    const float PI = 3.14159265359f;

    for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
        for (unsigned int y = 0; y <= Y_SEGMENTS; ++y) {
            float xSegment = (float)x / (float)X_SEGMENTS;
            float ySegment = (float)y / (float)Y_SEGMENTS;
            float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            float yPos = std::cos(ySegment * PI);
            float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

            AssimpModel::Vertex vertex;
            vertex.Position = glm::vec3(xPos, yPos, zPos);
            vertex.Normal = glm::vec3(xPos, yPos, zPos);
            vertex.TexCoords = glm::vec2(xSegment, ySegment);
            vertices.push_back(vertex);
        }
    }

    bool oddRow = false;
    for (unsigned int y = 0; y < Y_SEGMENTS; ++y) {
        if (!oddRow) {
            for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
                indices.push_back(y * (X_SEGMENTS + 1) + x);
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
            }
        } else {
            for (int x = X_SEGMENTS; x >= 0; --x) {
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                indices.push_back(y * (X_SEGMENTS + 1) + x);
            }
        }
        oddRow = !oddRow;
    }

    std::vector<AssimpModel::Mesh> meshes = { AssimpModel::Mesh(vertices, indices, textures) };

    return AssimpModel::Model(meshes);
}

AssimpModel::Model GeometryGenerator::generateCube() {
    std::vector<AssimpModel::Vertex> vertices(24);
    std::vector<unsigned int> indices(36);
    std::vector<AssimpModel::Texture> textures;

    float verticesData[] = {
            // positions          // normals           // texcoords
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
            1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
            1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
            1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
            1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
            1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
            1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
            1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
            1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
            1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    unsigned int indicesData[] = {
            0,  1,  2,  0,  3,  1, // Back face
            4,  5,  6,  7,  8,  9, // Front face
            10, 11, 12, 13, 14, 15, // Left face
            16, 17, 18, 19, 20, 21, // Right face
            22, 23, 24, 25, 26, 27, // Top face
            28, 29, 30, 31, 32, 33, // Bottom face
    };

    for (unsigned int i = 0; i < 36; ++i) {
        AssimpModel::Vertex vertex;
        vertex.Position = glm::vec3(verticesData[i * 8], verticesData[i * 8 + 1], verticesData[i * 8 + 2]);
        vertex.Normal = glm::vec3(verticesData[i * 8 + 3], verticesData[i * 8 + 4], verticesData[i * 8 + 5]);
        vertex.TexCoords = glm::vec2(verticesData[i * 8 + 6], verticesData[i * 8 + 7]);
        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < 36; ++i) {
        indices.push_back(indicesData[i]);
    }

    std::vector<AssimpModel::Mesh> meshes = { AssimpModel::Mesh(vertices, indices, textures) };

    return AssimpModel::Model(meshes);
}

AssimpModel::Model GeometryGenerator::generateQuad() {
    std::vector<AssimpModel::Vertex> vertices(4);
    std::vector<unsigned int> indices(6);
    std::vector<AssimpModel::Texture> textures;

    float verticesData[] = {
            // positions        // normals           // texcoords
            -1.0f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
            1.0f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
            1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
    };
    unsigned int indicesData[] = {
            0, 1, 2, 1, 3, 2,
    };

    for (unsigned int i = 0; i < 4; ++i) {
        AssimpModel::Vertex vertex;
        vertex.Position = glm::vec3(verticesData[i * 8], verticesData[i * 8 + 1], verticesData[i * 8 + 2]);
        vertex.Normal = glm::vec3(verticesData[i * 8 + 3], verticesData[i * 8 + 4], verticesData[i * 8 + 5]);
        vertex.TexCoords = glm::vec2(verticesData[i * 8 + 6], verticesData[i * 8 + 7]);
        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < 6; ++i) {
        indices.push_back(indicesData[i]);
    }

    vector<AssimpModel::Mesh> meshes = { AssimpModel::Mesh(vertices, indices, textures) };

    return AssimpModel::Model(meshes);
}

// renderPlane()
// -----------------------------------------
AssimpModel::Model GeometryGenerator::generatePlane() {
    std::vector<AssimpModel::Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<AssimpModel::Texture> textures;

    float planeVertices[] = {
            // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
            50.0f, 0.0f, 50.0f, 2.0f, 0.0f,
            -50.0f, 0.0f, 50.0f, 0.0f, 0.0f,
            -50.0f, 0.0f, -50.0f, 0.0f, 2.0f,

            50.0f, 0.0f, 50.0f, 2.0f, 0.0f,
            -50.0f, 0.0f, -50.0f, 0.0f, 2.0f,
            50.0f, 0.0f, -50.0f, 2.0f, 2.0f
    };
    unsigned int indicesData[] = {0, 2, 1, 3, 5, 4};

    for (unsigned int i = 0; i < 6; ++i) {
        AssimpModel::Vertex vertex;
        vertex.Position = glm::vec3(planeVertices[i * 5], planeVertices[i * 5 + 1], planeVertices[i * 5 + 2]);
        vertex.TexCoords = glm::vec2(planeVertices[i * 5 + 3], planeVertices[i * 5 + 4]);
        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < 6; ++i) {
        indices.push_back(indicesData[i]);
    }

    std::vector<AssimpModel::Mesh> meshes = { AssimpModel::Mesh(vertices, indices, textures) };

    return AssimpModel::Model(meshes);
}