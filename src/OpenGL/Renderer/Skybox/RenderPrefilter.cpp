#include "../../Renderer.h"

void Renderer::renderPrefilter(FrameBuffer *frameBuffer) {
    frameBuffer->textureColorBuffers[0] = scene.skybox->prefilter;
    frameBuffer->textureColorBuffers[0]->setSize(128, 128);
    dynamic_cast<TextureCubeMap*>(frameBuffer->textureColorBuffers[0].get())->init();

    frameBuffer->bind();
        shaderPrefilter->use();

        shaderPrefilter->setTexture("environmentMap", scene.skybox->cubeMap.get());
        shaderPrefilter->setMat4("projection", scene.skybox->captureProjection);
        int maxMipLevels = 5;
        for (int mip = 0; mip < maxMipLevels; ++mip) {
            int mipWidth = 128 * std::pow(0.5, mip);
            int mipHeight = 128 * std::pow(0.5, mip);

            frameBuffer->setSize(mipWidth, mipHeight);
            frameBuffer->rboDepth->setSize(mipWidth, mipHeight);
            frameBuffer->rboDepth->init();

            frameBuffer->setViewPort();

            float roughness = (float)mip / (float)(maxMipLevels - 1);
            shaderPrefilter->setFloat("roughness", roughness);

            for (int i = 0; i < 6; ++i) {
                shaderPrefilter->setMat4("view", scene.skybox->captureViews[i]);
                frameBuffer->setColorTextureAttachmentCubeFace(i, 0, mip);
                frameBuffer->clear();
                scene.skybox->draw();
            }
        }
    frameBuffer->unbind();
}