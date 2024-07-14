#include "../../Renderer.h"

void Renderer::renderShadowMap() {
    for (int i = 0; i < scene.lights.size(); ++i) {
        frameBufferShadowMaps[i]->bind();
        frameBufferShadowMaps[i]->reset();
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
        frameBufferShadowMaps[i]->unbind();
    }
}
