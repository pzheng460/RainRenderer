#include "../../Renderer.h"

void Renderer::renderBRDFLUT(FrameBuffer *frameBuffer) {
    frameBuffer->textureColorBuffers[0] = scene.skybox->brdfLUT;
    frameBuffer->setAllSizes(scene.skybox->width, scene.skybox->height);
    frameBuffer->init();

    frameBuffer->bind();
    frameBuffer->reset();
        shaderBRDFLUT->use();

        screenQuad->draw();
    frameBuffer->unbind();
}
