#include "../../Renderer.h"

void Renderer::renderLight(FrameBuffer* frameBuffer) {
    frameBuffer->bind();

    shaderLight->use();

    shaderLight->setMat4("view", scene.camera->getViewMatrix());
    shaderLight->setMat4("projection", scene.camera->getProjectionMatrix(width, height));
    for (auto& light : scene.lights) {
        shaderLight->setVec3("lightColor", light->getColor());
        shaderLight->setMat4("model", light->getModelMatrix());

        light->draw();
    }
    frameBuffer->unbind();
}
