#include "../../Renderer.h"

void Renderer::renderIrradiance(FrameBuffer *frameBuffer) {
    frameBuffer->textureColorBuffers[0] = scene.skybox->irradiance;
    frameBuffer->setAllSizes(32, 32);
    frameBuffer->init();

    frameBuffer->bind();
        shaderIrradiance->use();

        shaderIrradiance->setTexture("environmentMap", scene.skybox->cubeMap.get());
        shaderIrradiance->setMat4("projection", scene.skybox->captureProjection);

        frameBuffer->setViewPort();
        for (int i = 0; i < 6; ++i) {
            shaderIrradiance->setMat4("view", scene.skybox->captureViews[i]);
            frameBuffer->setColorTextureAttachmentCubeFace(i);
            frameBuffer->clear();
            scene.skybox->draw();
        }
    frameBuffer->unbind();
}