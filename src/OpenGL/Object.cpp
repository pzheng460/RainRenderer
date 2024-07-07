#include "Object.h"

void Object::setMVP(Camera& camera, float SCR_WIDTH, float SCR_HEIGHT) {
    // MVP matrices
    modelMatrix = glm::translate(glm::mat4(1.0f), position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));
    viewMatrix = camera.GetViewMatrix();
    // 参数：fov视野，aspect宽高比，zNear近平面，zFar远平面
    projectionMatrix = glm::perspective(glm::radians(camera.Zoom), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
}

void Object::draw(Shader& shader, bool setTexture) {
    shader.use();
    shader.setMat4("model", modelMatrix);
    shader.setMat4("view", viewMatrix);
    shader.setMat4("projection", projectionMatrix);

    model.draw(shader, setTexture);
}
