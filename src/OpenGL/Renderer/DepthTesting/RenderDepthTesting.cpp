#include "../../Renderer.h"

void Renderer::renderDepthTesting() {
    frameBufferMSAA->bind();
    frameBufferMSAA->reset();
        shaderDepthTesting->use();

        shaderDepthTesting->setMat4("view", scene.camera->getViewMatrix());
        shaderDepthTesting->setMat4("projection", scene.camera->getProjectionMatrix(width, height));

        if (gui.floorActive) {
            shaderDepthTesting->setMat4("model", scene.floor->getModelMatrix());

            scene.floor->draw();
        }

        for (auto& object : scene.objects) {
            shaderDepthTesting->setMat4("model", object->getModelMatrix());

            object->draw();
        }
    frameBufferMSAA->unbind();
}
