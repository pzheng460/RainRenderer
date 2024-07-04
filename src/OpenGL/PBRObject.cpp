#include "PBRObject.h"
#include <iostream>

PBRObject::PBRObject(const AssimpModel::Model& model)
        : Object(model) {}

PBRObject::PBRObject(GeometryType geometryType) : Object(geometryType, false) {}

void PBRObject::draw(Shader& shader, bool setTexture) {
    shader.use();
    // MVP matrices
    shader.setMat4("model", modelMatrix);
    shader.setMat4("view", viewMatrix);
    shader.setMat4("projection", projectionMatrix);
    shader.setMat3("normalMatrix", glm::transpose(glm::inverse(modelMatrix)));

    if (modelVariant.index() == 0) {
        std::get<AssimpModel::Model>(modelVariant).Draw(shader, setTexture);
    } else {
        if (setTexture) {
            for (int i = 0; i < std::get<GeometryModel>(modelVariant).textures.size(); ++i) {
                auto texture = std::get<GeometryModel>(modelVariant).textures[i];
                shader.setTexture(texture.type, texture.id);
            }
        }
        std::get<GeometryModel>(modelVariant).geometry->draw();
    }
}