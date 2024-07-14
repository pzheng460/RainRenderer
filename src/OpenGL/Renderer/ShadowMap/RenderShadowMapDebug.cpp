#include "../../Renderer.h"

void Renderer::renderShadowMapDebug() {
    float near_plane = 1.0f, far_plane = 100.0f;
    frameBufferDefault->bind();
    frameBufferDefault->reset();
        shaderShadowMapDebug->use();
        shaderShadowMapDebug->setFloat("near_plane", near_plane);
        shaderShadowMapDebug->setFloat("far_plane", far_plane);
        shaderShadowMapDebug->setTexture("depthMap", frameBufferShadowMaps[0]->textureDepthBuffer.get());

        screenQuad->draw();
    frameBufferDefault->unbind();
}