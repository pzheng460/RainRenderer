#include "../../Renderer.h"

void Renderer::renderGeometry(FrameBuffer* frameBuffer) {
    frameBuffer->bind();
    frameBuffer->reset();
        shaderGeometry->use();

        shaderGeometry->setBool("invertNormals", false);

        shaderGeometry->setMat4("view", scene.camera->getViewMatrix());
        shaderGeometry->setMat4("projection", scene.camera->getProjectionMatrix(width, height));

        if (gui.floorActive) {
            shaderGeometry->setMat4("model", scene.floor->getModelMatrix());

            scene.floor->draw(shaderGeometry.get());
        }

        for (auto& object : scene.objects) {
            shaderGeometry->setMat4("model", object->getModelMatrix());

            object->draw(shaderGeometry.get());
        }
    frameBuffer->unbind();
}
