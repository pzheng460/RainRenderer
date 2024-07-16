#include "../../Renderer.h"

void Renderer::renderPostProcessing(FrameBuffer* frameBuffer) {
    frameBuffer->bind();
    frameBuffer->reset();
        shaderPostProcessing->use();
        shaderPostProcessing->setTexture("hdrBuffer", frameBufferIntermediate->textureColorBuffers[0].get());
        shaderPostProcessing->setBool("HDRActive", gui.HDRActive);
        shaderPostProcessing->setFloat("exposure", 10.0f);

        shaderPostProcessing->setTexture("bloomBlur", frameBufferBlooms[!horizontal]->textureColorBuffers[0].get());
        shaderPostProcessing->setBool("bloomActive", gui.bloomActive);

        screenQuad->draw();
    frameBuffer->unbind();
}
