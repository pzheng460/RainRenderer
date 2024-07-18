#include "../../Renderer.h"

void Renderer::renderShadowMapDebug(FrameBuffer* frameBuffer, int i) {
    float near_plane = 0.1f, far_plane = 100.0f;
    frameBuffer->bind();
    frameBuffer->reset();
        shaderShadowMapDebug->use();
        shaderShadowMapDebug->setFloat("near_plane", near_plane);
        shaderShadowMapDebug->setFloat("far_plane", far_plane);
        shaderShadowMapDebug->setTexture("depthMap", frameBufferShadowMaps[i]->textureDepthBuffer.get());

        screenQuad->draw();
    frameBuffer->unbind();
}