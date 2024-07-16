#include "../../Renderer.h"

void Renderer::renderShadowMap(FrameBuffer* frameBuffer, int i) {
    frameBuffer->bind();
    frameBuffer->reset();
    shaderShadowMap->use();
    shaderShadowMap->setMat4("lightSpaceMatrix", scene.lights[i]->getLightSpaceMatrix());
    if (gui.floorActive) {
        shaderShadowMap->setMat4("model", scene.floor->getModelMatrix());

        scene.floor->draw();
    }
    for (auto& object : scene.objects) {
        shaderShadowMap->setMat4("model", object->getModelMatrix());

        object->draw();
    }
    frameBuffer->unbind();
}
