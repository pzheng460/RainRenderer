#include "../../Renderer.h"

void Renderer::renderSkyboxSphereMapToCubeMap(FrameBuffer *frameBuffer) {
    frameBuffer->textureColorBuffers[0] = scene.skybox->cubeMap;
    frameBuffer->setAllSizes(scene.skybox->width, scene.skybox->height);

    frameBuffer->init();

    shaderSkyboxSphereMapToCubeMap->use();

    shaderSkyboxSphereMapToCubeMap->setTexture("equirectangularMap", scene.skybox->sphereMap.get());
    shaderSkyboxSphereMapToCubeMap->setMat4("projection", scene.skybox->captureProjection);

    frameBuffer->bind();

    for (int i = 0; i < 6; ++i) {
        shaderSkyboxSphereMapToCubeMap->setMat4("view", scene.skybox->captureViews[i]);
        frameBuffer->setColorTextureAttachmentCubeFace(i);
        frameBuffer->reset();
        scene.skybox->draw();
    }

    frameBuffer->unbind();

    scene.skybox->cubeMap->setMipMap();
}