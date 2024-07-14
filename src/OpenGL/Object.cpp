#include "Object.h"

const glm::mat4 Object::getModelMatrix() const {
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));
    return modelMatrix;
}

void Object::draw(Shader* shader) {
    model.draw(shader);
}