#include "../../Renderer.h"

void Renderer::renderEnvironmentMapping(FrameBuffer* frameBuffer) {
    frameBuffer->bind();
    frameBuffer->reset();
        shaderEnvironmentMapping->use();

        shaderEnvironmentMapping->setTexture("skybox", scene.skybox->cubeMap.get());
        shaderEnvironmentMapping->setVec3("cameraPos", scene.camera->Position);

        shaderEnvironmentMapping->setMat4("view", scene.camera->getViewMatrix());
        shaderEnvironmentMapping->setMat4("projection", scene.camera->getProjectionMatrix(width, height));

        for (auto& object : scene.objects) {
            shaderEnvironmentMapping->setMat4("model", object->getModelMatrix());

            object->draw();
        }
    frameBuffer->unbind();
}