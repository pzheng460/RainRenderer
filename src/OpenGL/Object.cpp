#include "Object.h"

Object::Object(const Model& model, RenderMode renderMode)
        : data(model), renderMode(renderMode) {
}

Object::Object(ImplicitGeometryType geometryType, RenderMode renderMode)
        : data(geometryType), renderMode(renderMode) {
}

void Object::setMVP(Camera& camera, float SCR_WIDTH, float SCR_HEIGHT) {
    // MVP matrices
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f)); // Translate it down so it's at the center of the scene
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f)); // Scale it down
    viewMatrix = camera.GetViewMatrix();
    projectionMatrix = glm::perspective(glm::radians(camera.Zoom), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
}
