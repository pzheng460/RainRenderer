#include "../../Renderer.h"

void Renderer::renderNormalVisualization() {
    frameBufferMSAA->bind();
        shaderNormalVisualization->use();

        shaderNormalVisualization->setMat4("view", scene.camera->getViewMatrix());
        shaderNormalVisualization->setMat4("projection", scene.camera->getProjectionMatrix(width, height));

        if (gui.floorActive) {
            shaderNormalVisualization->setMat4("model", scene.floor->getModelMatrix());

            scene.floor->draw();
        }

        for (auto& object : scene.objects) {
            shaderNormalVisualization->setMat4("model", object->getModelMatrix());

            object->draw();
        }
    frameBufferMSAA->unbind();
}