#include "../../Renderer.h"

void Renderer::renderBasic(FrameBuffer* frameBuffer) {
    frameBuffer->bind();
    frameBuffer->reset();
        shaderBasic->use();

        shaderBasic->setMat4("view", scene.camera->getViewMatrix());
        shaderBasic->setMat4("projection", scene.camera->getProjectionMatrix(width, height));

        if (gui.floorActive) {
            shaderBasic->setMat4("model", scene.floor->getModelMatrix());
            scene.floor->draw(shaderBasic.get());
        }

        for (auto& object : scene.objects) {
            shaderBasic->setMat4("model", object->getModelMatrix());
            object->draw(shaderBasic.get());
        }
    frameBuffer->unbind();
}
