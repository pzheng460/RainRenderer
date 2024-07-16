#include "../../Renderer.h"

void Renderer::renderSkyboxHDR(FrameBuffer *frameBuffer) {
    frameBuffer->bind();
    glDepthFunc(GL_LEQUAL);
    shaderSkyboxHDR->use();

    shaderSkyboxHDR->setTexture("environmentMap", scene.skybox->cubeMap.get());
    shaderSkyboxHDR->setMat4("view", scene.camera->getViewMatrix());
    shaderSkyboxHDR->setMat4("projection", scene.camera->getProjectionMatrix(width, height));

    scene.skybox->draw();
    glDepthFunc(GL_LESS);
    frameBuffer->unbind();
}
